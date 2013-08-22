/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2013 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __REGAL_JSON_H__
#define __REGAL_JSON_H__

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <string>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Json {

enum Object
{
  JSON_ROOT = 0,

  JSON_REGAL,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE_DIRECTORY,
  JSON_REGAL_CONFIG_CACHE_ENABLE,
  JSON_REGAL_CONFIG_CACHE_SHADER,
  JSON_REGAL_CONFIG_CACHE_SHADERREAD,
  JSON_REGAL_CONFIG_CACHE_SHADERWRITE,
  JSON_REGAL_CONFIG_CACHE_TEXTURE,
  JSON_REGAL_CONFIG_CACHE_TEXTUREREAD,
  JSON_REGAL_CONFIG_CACHE_TEXTUREWRITE,
  JSON_REGAL_CONFIG_CONFIGFILE,
  JSON_REGAL_CONFIG_DISPATCH,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_BIN,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_BV,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_DSA,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_FILTER,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_HINT,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_IFF,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_OBJ,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PATH,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PPA,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PPCA,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_RECT,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_SO,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_TEXC,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_TEXSTO,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_VAO,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_XFER,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_BIN,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_BV,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_DSA,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_FILTER,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_HINT,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_IFF,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_OBJ,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PATH,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PPA,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PPCA,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_RECT,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_SO,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_TEXC,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_TEXSTO,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_VAO,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_XFER,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_CODE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_DEBUG,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_DRIVER,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_EMULATION,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_ERROR,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_LOG,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_STATISTICS,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE_TRACE,
  JSON_REGAL_CONFIG_DISPATCH_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_FORCE_EMULATION,
  JSON_REGAL_CONFIG_FORCE,
  JSON_REGAL_CONFIG_FORCE_CORE,
  JSON_REGAL_CONFIG_FORCE_ES1,
  JSON_REGAL_CONFIG_FORCE_ES2,
  JSON_REGAL_CONFIG_FRAME,
  JSON_REGAL_CONFIG_FRAME_LIMIT,
  JSON_REGAL_CONFIG_FRAME_MD5,
  JSON_REGAL_CONFIG_FRAME_MD5_COLOR,
  JSON_REGAL_CONFIG_FRAME_MD5_DEPTH,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK_COLOR,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK_DEPTH,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK_STENCIL,
  JSON_REGAL_CONFIG_FRAME_MD5_STENCIL,
  JSON_REGAL_CONFIG_FRAME_SAVE,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_COLOR,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_DEPTH,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_STENCIL,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_COLOR,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_DEPTH,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_STENCIL,
  JSON_REGAL_CONFIG_SYSTEM,
  JSON_REGAL_CONFIG_SYSTEM_EGL,
  JSON_REGAL_CONFIG_SYSTEM_ES1,
  JSON_REGAL_CONFIG_SYSTEM_ES2,
  JSON_REGAL_CONFIG_SYSTEM_GL,
  JSON_REGAL_CONFIG_SYSTEM_GLX,
  JSON_REGAL_CONFIG_TRACE,
  JSON_REGAL_CONFIG_TRACE_FILE,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING_BUFFERLIMIT,
  JSON_REGAL_LOGGING_CALLBACK,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE_APP,
  JSON_REGAL_LOGGING_ENABLE_DRIVER,
  JSON_REGAL_LOGGING_ENABLE_ERROR,
  JSON_REGAL_LOGGING_ENABLE_HTTP,
  JSON_REGAL_LOGGING_ENABLE_INFO,
  JSON_REGAL_LOGGING_ENABLE_INTERNAL,
  JSON_REGAL_LOGGING_ENABLE_WARNING,
  JSON_REGAL_LOGGING_FILENAME,
  JSON_REGAL_LOGGING_FRAMESTATISTICS,
  JSON_REGAL_LOGGING_FRAMETIME,
  JSON_REGAL_LOGGING_JSON,
  JSON_REGAL_LOGGING_JSONFILE,
  JSON_REGAL_LOGGING_LOG,
  JSON_REGAL_LOGGING_MAXBYTES,
  JSON_REGAL_LOGGING_MAXLINES,
  JSON_REGAL_LOGGING_ONCE,
  JSON_REGAL_LOGGING_POINTERS,
  JSON_REGAL_LOGGING_PROCESS,
  JSON_REGAL_LOGGING_THREAD,

  JSON_UNDEFINED
};

struct Parser
{
  Parser();
  ~Parser();

  void onPush(const std::string &name);
  void onPop();

  void onValue();
  void onValue(const bool         value);
  void onValue(const long         value);
  void onValue(const std::string &value);

  //

  static bool parseFile(const std::string &filename);
  static bool parseString(const char *json);

  //

  std::size_t depth;
  Object      current;
};

}

REGAL_NAMESPACE_END

#endif

