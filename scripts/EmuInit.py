#!/usr/bin/python -B

formulae = {

  # For all of GL API, check that Regal is initialized either via
  # RegalMakeCurrent,  or via the selective CGL/EGL/GLX/WGL functions
  # in the global scope below.

  'assert' :
  {
    'entries' : [ '.*' ],
    'prefix'  : 'RegalAssert(Init::isInitialized());'
  }

}

#
# Hook into the MakeCurrent and DestroyContext functions in
# order to manage Regal contexts.
#

formulaeGlobal = {

  # Selectively initialize Regal for known initialization calls
  # in addition to Regal API entry points.
  #
  # CGLGetCurrentContext is needed for Mac OS X/GLUT
  # eglGetDisplay is needed for apitrace eglretrace tool.
  # glXGetProcAddress is needed for Linux chromium
  # glXQueryExtension is needed for freeglut X11
  # glXGetProcAddressARB is needed for Linux Minecraft 1.6.1
  # glXChooseVisual is needed for Linux eihort Minecraft world viewer
  # wglCreateContext, wglGetCurrentContext, wglGetProcAddress for a start

  'init' :
  {
    'entries' : [
      'CGLChoosePixelFormat', 'CGLGetCurrentContext',
      'eglGetDisplay', 'eglGetCurrentContext',
      'glXChooseVisual', 'glXGetProcAddress', 'glXQueryExtension', 'glXGetProcAddressARB',
      'wglCreateContext', 'wglGetCurrentContext', 'wglGetProcAddress'
    ],
    'prefix'  : 'Init::init();'
  },

  # WGL

  'wglMakeCurrent' : {
      'entries' : [ 'wglMakeCurrent' ],
      'init' : '''
if (ret)
{
  Init::init();
  Init::makeCurrent(RegalSystemContext(hglrc));
}'''
    },

  'wglDeleteContext' : {
      'entries' : [ 'wglDeleteContext' ],
      'init' : [ 'Init::destroyContext(RegalSystemContext(hglrc));' ]
  },

  # GLX

  'glXMakeCurrent' : {
      'entries' : [ 'glXMakeCurrent' ],
      'init' : [ 'if (ret)',
                 '    Init::makeCurrent(RegalSystemContext(ctx));' ]
  },

  'glXMakeContextCurrent' : {
      'entries' : [ 'glXMakeContextCurrent' ],
      'init' : '''
if (ret)
{
  Init::init();
  Init::makeCurrent(RegalSystemContext(ctx));
}'''
    },

  'glXDestroyContext' : {
      'entries' : [ 'glXDestroyContext' ],
      'init' : [ 'Init::destroyContext(RegalSystemContext(ctx));' ]
  },

    # EGL

  'eglMakeCurrent' : {
      'entries' : [ 'eglMakeCurrent' ],
      'init' : '''
#if !REGAL_SYS_PPAPI
if (ret)
{
  Init::init();
  Init::makeCurrent(ctx);
}
#endif'''
    },

  'eglDestroyContext' : {
      'entries' : [ 'eglDestroyContext' ],
      'init' : [ 'Init::destroyContext(RegalSystemContext(ctx));' ]
  },

  # CGL

  'CGLSetCurrentContext' : {
      'entries' : [ 'CGLSetCurrentContext' ],
      'init' : '''
if (!ret)
{
  Init::init();
  Init::makeCurrent(ctx);
}'''
  },

  'CGLDestroyContext' : {
      'entries' : [ 'CGLDestroyContext' ],
      'init' : [ 'Init::destroyContext(RegalSystemContext(ctx));' ]
  },
}
