#!/usr/bin/python -B

formulae = {

  # glFinish

  'glFinish' : {
      'entries' : [ 'glFinish' ],
      'post' : '''
// Notify Regal::Frame about the glFinish() event
_context->frame->glFinish(*_context);
'''
  }

#  # Capture images or md5s for other functions too, such as glViewport
#
#  'glFinish' : {
#      'entries' : [ 'glRectf' ],
#      'post' : '''
#if (Config::frameCapture)
#  _context->frame->capture(*_context);'''
#  }

}

formulaeGlobal = {

    # GL_GREMEDY_frame_terminator

    'GL_GREMEDY_frame_terminator' : {
        'entries' : [ 'glFrameTerminatorGREMEDY' ],
        'impl' : '''
#if REGAL_FRAME
// Notify Regal::Frame about the frame terminator event.
if (_context && _context->frame)
  _context->frame->glFrameTerminatorGREMEDY(*_context);
#endif
RegalAssert(_context->info);
// Return to application unless GL_GREMEDY_frame_terminator is supported by the driver.
if (!_context->info->gl_gremedy_frame_terminator) return;
'''
    },

    # wglSwapBuffers

    'wglSwapBuffers' : {
        'entries' : [ 'wglSwapBuffers' ],
        'impl' : '''
#if REGAL_FRAME
RegalContext *_context = REGAL_GET_CONTEXT();
// Notify Regal::Frame about the swap buffers event.
if (_context && _context->frame)
    _context->frame->wglSwapBuffers(*_context);
#endif'''
    },

    # glXSwapBuffers

    'glXSwapBuffers' : {
        'entries' : [ 'glXSwapBuffers' ],
        'impl' : '''
// Keep track of X11 Display and GLXDrawable for logging purposes.
RegalContext *_context = REGAL_GET_CONTEXT();
if (_context)
{
    #if REGAL_SYS_X11
    _context->x11Display  = dpy;
    #endif
    _context->x11Drawable = drawable;
}
#if REGAL_FRAME
// Notify Regal::Frame about the swap buffers event.
if (_context && _context->frame)
    _context->frame->glXSwapBuffers(*_context);
#endif'''
    },

    # glXMakeCurrent

    'glXMakeCurrent' : {
        'entries' : [ 'glXMakeCurrent' ],
        'impl' : '''
// Keep track of X11 Display and GLXDrawable for logging purposes.
RegalContext *_context = REGAL_GET_CONTEXT();
if (_context)
{
    #if REGAL_SYS_X11
    _context->x11Display  = dpy;
    #endif
    _context->x11Drawable = drawable;
}'''
    },

    # eglSwapBuffers

    'eglSwapBuffers' : {
        'entries' : [ 'eglSwapBuffers' ],
        'impl' : '''
#if REGAL_FRAME
RegalContext *_context = REGAL_GET_CONTEXT();
// Notify Regal::Frame about the swap buffers event.
if (_context && _context->frame)
    _context->frame->eglSwapBuffers(*_context);
#endif'''
    },

    # CGLFlushDrawable

    'CGLFlushDrawable' : {
        'entries' : [ 'CGLFlushDrawable' ],
        'impl' : '''
#if REGAL_FRAME
RegalContext *_context = REGAL_GET_CONTEXT();
// Notify Regal::Frame about the flush drawable event.
if (_context && _context->frame)
    _context->frame->CGLFlushDrawable(*_context);
#endif'''
    }
}
