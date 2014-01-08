#!/usr/bin/python -B

formulae = {
    'GetString' : {
        'entries' : [ 'glGetString' ],
        'impl' : '''
// Regal interceptions
RegalAssert(_context->emuInfo);
switch (name)
{
  case GL_VENDOR:     return reinterpret_cast<const GLubyte *>(_context->emuInfo->vendor.c_str());
  case GL_RENDERER:   return reinterpret_cast<const GLubyte *>(_context->emuInfo->renderer.c_str());
  case GL_VERSION:    return reinterpret_cast<const GLubyte *>(_context->emuInfo->version.c_str());
  case GL_EXTENSIONS: return reinterpret_cast<const GLubyte *>(_context->emuInfo->extensions.c_str());
  default:
    break;
}'''
    }
}
