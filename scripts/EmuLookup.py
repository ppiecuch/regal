#!/usr/bin/python -B

formulae = {
    'wglGetProcAddress' : {
        'entries' : [ '(wglGetProcAddress)()' ],
        'impl' : '''
//
// wglGetProcAddress returns NULL for unsupported
// entry points.  Here we need Regal to behave
// similarly depending on what's emulated.
//

// Regal-only and ES 1.0 aliased functions are
// always provided by Regal

// ret = Lookup::regal_Lookup<PROC>(${arg0});
// if (ret)
//  return ret;

// Ask the back-end driver if the entry point
// is available.  If not, return NULL
//
// TODO - we need to query ContextInfo and
//        EmulationInfo here instead.

ret = _next->call(&_next->${m1}${m2})(${arg0});
if (!ret)
  return NULL;

// Return the Regal implementation of the entry
// point, either GL or WGL

ret = Lookup::gl_Lookup<PROC>(${arg0});
if (ret)
  return ret;

ret = Lookup::wgl_Lookup<PROC>(${arg0});
if (ret)
  return ret;

// As a last resort, dispatch the lookup to
// the global dispatch stack...
'''
    },

    'glXGetProcAddress' : {
        'entries' : [ '(glXGetProcAddress)(|ARB)' ],
        'impl' : '''
ret = Lookup::gl_Lookup<__GLXextFuncPtr>(reinterpret_cast<const char *>(${arg0}));
if (ret)
  return ret;
ret = Lookup::glx_Lookup<__GLXextFuncPtr>(reinterpret_cast<const char *>(${arg0}));
if (ret)
  return ret;
'''
    },

    'eglGetProcAddress' : {
        'entries' : [ '(eglGetProcAddress)(|ARB)' ],
        'impl' : '''
ret = Lookup::gl_Lookup<__eglMustCastToProperFunctionPointerType>(reinterpret_cast<const char *>(${arg0}));
if (ret)
  return ret;
ret = Lookup::egl_Lookup<__eglMustCastToProperFunctionPointerType>(reinterpret_cast<const char *>(${arg0}));
if (ret)
  return ret;
'''
    }
}
