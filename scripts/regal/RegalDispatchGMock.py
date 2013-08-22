#!/usr/bin/python -B

from string import Template
from collections import defaultdict

from ApiUtil import outputCode

from ApiCodeGen import typeCode
from ApiCodeGen import paramsTypeCode
from ApiCodeGen import paramsNameCode
from ApiCodeGen import paramsDefaultCode

###############################################################################

dispatchGMockHeaderTemplate = Template(
    '''${AUTOGENERATED}
${LICENSE}

#ifndef __REGAL_DISPATCH_GMOCK_H__
#define __REGAL_DISPATCH_GMOCK_H__

#include "gmock/gmock.h"

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DispatchTableGL;

struct RegalGMockInterface
{
  RegalGMockInterface();
  virtual ~RegalGMockInterface();
  static RegalGMockInterface* current;

${API_DISPATCH_MOCK_DEFINE}
};

void InitDispatchTable${DISPATCH_NAME}(DispatchTableGL &tbl);

REGAL_NAMESPACE_END

#endif // ! __REGAL_DISPATCH_GMOCK_H__''')

dispatchGMockDefineUnmockableFragmentTemplate = Template(
    '  // ${NAME} has too many arguments for GMock.')

dispatchGMockDefineMockFragmentTemplate = Template(
    '  MOCK_METHOD${PARAM_COUNT}(${NAME}, ${RTYPE}(${PARAM_TYPES}));')


dispatchGMockSourceTemplate = Template(
    '''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "gmock/gmock.h"

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalDispatch.h"
#include "RegalDispatchGMock.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace {

${API_DISPATCH_FUNC_DEFINE}

} // namespace

RegalGMockInterface::RegalGMockInterface()
{
  current = this;
}

RegalGMockInterface::~RegalGMockInterface()
{
  current = NULL;
}

RegalGMockInterface* RegalGMockInterface::current;

void InitDispatchTable${DISPATCH_NAME}(DispatchTableGL &tbl)
{
${API_DISPATCH_FUNC_INIT}
}

REGAL_NAMESPACE_END''')


dispatchGMockDispatchSetFragmentTemplate = Template(
    '  tbl.${NAME} = ${PREFIX}${NAME};')

dispatchGMockDefineFuctionFragmentTemplate = Template(
    '''${RTYPE} REGAL_CALL ${PREFIX}${NAME}(${PARAM_TYPES_NAMES}) {
  return RegalGMockInterface::current->${NAME}(${PARAM_NAMES});
}
''')

dispatchGMockDefineNotMockedFuctionFragmentTemplate = Template(
    '''${RTYPE} REGAL_CALL ${PREFIX}${NAME}(${PARAM_TYPES_NAMES}) {
  // ${NAME} has too many arguments for GMock.
}
''')


def canGenerateGmock(args):
  return args['PARAM_COUNT'] <= 10


def dispatchGMockDefineMockFragment(args):
  if canGenerateGmock(args):
    return dispatchGMockDefineMockFragmentTemplate.substitute(args)
  else:
    return dispatchGMockDefineUnmockableFragmentTemplate.substitute(args)


def dispatchGMockDefineFuctionFragment(args):
  if canGenerateGmock(args):
    return dispatchGMockDefineFuctionFragmentTemplate.substitute(args)
  else:
    return dispatchGMockDefineNotMockedFuctionFragmentTemplate.substitute(args)


functionCategoriesToMock = [
    'GL_VERSION_1_0',
    'GL_VERSION_1_1',
    'GL_VERSION_2_0']


explicitFunctionsToMock = frozenset([
    'glBindBuffer',
    'glBindVertexArray',
    'glBindVertexBuffer',
    'glClientActiveTexture',
    'glClientAttribDefaultEXT',
    'glColorPointer',
    'glDisableClientState',
    'glDisableClientStateiEXT',
    'glDisableVertexAttribArray',
    'glEdgeFlagPointer',
    'glEnableClientState',
    'glEnableVertexAttribArray',
    'glFogCoordPointer',
    'glIndexPointer',
    'glMultiTexCoordPointerEXT',
    'glNormalPointer',
    'glPrimitiveRestartIndex',
    'glSecondaryColorPointer',
    'glVertexAttribBinding',
    'glVertexAttribFormat',
    'glVertexAttribIFormat',
    'glVertexAttribLFormat',
    'glVertexBindingDivisor',
    'glVertexPointer'])


def generateGMockFunctionApi(apis):
  for api in apis:
    if api.name != 'gl':
      continue

    for function in api.functions:
      if not function.needsContext:
        continue
      if getattr(function, 'regalOnly', False):
        continue
      if (function.category not in functionCategoriesToMock and
          function.name not in explicitFunctionsToMock):
        continue

      yield dict(
          PREFIX="gmock_",
          NAME=function.name,
          RTYPE=typeCode(function.ret.type).strip(),
          PARAM_COUNT=len(function.parameters),
          PARAM_TYPES=paramsTypeCode(function.parameters, True),
          PARAM_NAMES=paramsNameCode(function.parameters),
          PARAM_TYPES_NAMES=paramsDefaultCode(function.parameters, True))


def generateGMockHeader(apis, args):
  defineAllMocks = '\n'.join(
      dispatchGMockDefineMockFragment(args)
      for args in generateGMockFunctionApi(apis))

  substitute = defaultdict(
      str,
      AUTOGENERATED=args.generated,
      LICENSE=args.license,
      COPYRIGHT=args.copyright,
      DISPATCH_NAME='GMock',
      API_DISPATCH_MOCK_DEFINE=defineAllMocks)

  outputCode(
      '%s/RegalDispatchGMock.h' % args.testdir,
      dispatchGMockHeaderTemplate.substitute(substitute))


def generateGmockSource(apis, args):
  defineAllFunctions = '\n'.join(
      dispatchGMockDefineFuctionFragment(args)
      for args in generateGMockFunctionApi(apis))

  initAllFunctions = '\n'.join(
      dispatchGMockDispatchSetFragmentTemplate.substitute(args)
      for args in generateGMockFunctionApi(apis))

  substitute = defaultdict(
      str,
      AUTOGENERATED=args.generated,
      LICENSE=args.license,
      COPYRIGHT=args.copyright,
      DISPATCH_NAME='GMock',
      API_DISPATCH_FUNC_DEFINE=defineAllFunctions,
      API_DISPATCH_FUNC_INIT=initAllFunctions)

  outputCode(
      '%s/RegalDispatchGMock.cpp' % args.testdir,
      dispatchGMockSourceTemplate.substitute(substitute))
