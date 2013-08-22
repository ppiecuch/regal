/*

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

*/

#include <waffle.h>

#include <GL/Regal.h>
#include "render.h"

#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef REGAL_SYS_OSX
extern void cocoa_init();
extern void cocoa_finish();
#endif

int main (int argc, char ** argv)
{
#if REGAL_SYS_OSX
    cocoa_init();
#endif

    const int32_t init_attrib_list[] =
    {
      WAFFLE_PLATFORM,
#if   REGAL_SYS_ANDROID
      WAFFLE_PLATFORM_ANDROID,
#elif REGAL_SYS_OSX
      WAFFLE_PLATFORM_CGL,
#elif REGAL_SYS_GLX
      WAFFLE_PLATFORM_GLX,
#else
      WAFFLE_PLATFORM_X11_EGL,
#endif
      WAFFLE_NONE
    };

    bool ok = waffle_init(init_attrib_list);
    if (!ok)
    {
      printf("waffle_init failed.\n");
      exit(1);
    }

    const char *display_name =
#if REGAL_SYS_GLX
      getenv("DISPLAY");
#else
      NULL;
#endif

    struct waffle_display *dpy = waffle_display_connect(display_name);
    if (!dpy)
    {
      printf("waffle_display_connect failed.\n");
      exit(1);
    }

    const int32_t config_attrib_list[] =
    {
      WAFFLE_CONTEXT_API,
#if   REGAL_SYS_ANDROID
      WAFFLE_CONTEXT_OPENGL_ES2,
#elif REGAL_SYS_OSX
      WAFFLE_CONTEXT_OPENGL,
#elif REGAL_SYS_GLX
      WAFFLE_CONTEXT_OPENGL,
#else
      WAFFLE_CONTEXT_OPENGL_ES2,
#endif
      WAFFLE_RED_SIZE,        8,
      WAFFLE_GREEN_SIZE,      8,
      WAFFLE_BLUE_SIZE,       8,
      WAFFLE_DOUBLE_BUFFERED, true,

      WAFFLE_NONE
    };

    struct waffle_config *config = waffle_config_choose(dpy, config_attrib_list);
    if (!config)
    {
      printf("waffle_config_choose failed.\n");
      exit(1);
    }

    struct waffle_context *ctx = waffle_context_create(config, NULL);
    if (!config)
    {
      printf("waffle_context_create failed.\n");
      exit(1);
    }

    struct waffle_window *window = waffle_window_create(config, 500, 500);
    if (!window)
    {
      printf("waffle_window_create failed.\n");
      exit(1);
    }

    ok = waffle_window_show(window);
    if (!ok)
    {
      printf("waffle_window_show failed.\n");
      exit(1);
    }

    ok = waffle_make_current(dpy, window, ctx);
    if (!ok)
    {
      printf("waffle_make_current failed.\n");
      exit(1);
    }

#ifdef __APPLE__
    RegalMakeCurrent(CGLGetCurrentContext());
#endif

    // OpenGL rendering

    dreamTorusReshape(500, 500);
    for(int i = 0; i < 1000; i++ )
    {
       dreamTorusDisplay(true);
       waffle_window_swap_buffers(window);
       usleep( 16000 );
    }

    // Cleanup

    ok = waffle_window_destroy(window);
    if (!ok)
    {
      printf("waffle_make_current failed.\n");
      exit(1);
    }

    ok = waffle_context_destroy(ctx);
    if (!ok)
    {
      printf("waffle_make_current failed.\n");
      exit(1);
    }

    ok = waffle_config_destroy(config);
    if (!ok)
    {
      printf("waffle_make_current failed.\n");
      exit(1);
    }

    ok = waffle_display_disconnect(dpy);
    if (!ok)
    {
      printf("waffle_make_current failed.\n");
      exit(1);
    }

#if REGAL_SYS_OSX
    cocoa_finish();
#endif
}
