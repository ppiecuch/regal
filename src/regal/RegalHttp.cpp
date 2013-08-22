/*
  Copyright (c) 2011 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"
#include "RegalHttp.h"

#if REGAL_NO_HTTP

REGAL_NAMESPACE_BEGIN

namespace Http
{
  void Init()  {}
  void Start() {}
  void Stop()  {}
}

REGAL_NAMESPACE_END

#else

REGAL_GLOBAL_BEGIN

#include "RegalLog.h"
#include "RegalInit.h"
#include "RegalConfig.h"
#include "RegalThread.h"
#include "RegalFavicon.h"
#include "RegalContextInfo.h"

#include <map>

#include <string>
#include <cstdio>
#include <cstdarg>
using namespace std;

#include <boost/print/print_string.hpp>
using boost::print::print_string;

#include "mongoose.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Http
{
  bool enabled = !REGAL_NO_HTTP;  // Enabled by default
  int  port    = REGAL_HTTP_PORT; // HTTP listening port - 8080 by default

  mg_callbacks callbacks;         // Callbacks
  mg_context   *ctx = NULL;       // Mongoose context

  void Init()
  {
    Internal("Http::Init","()");

    // Environment variable HTTP configuration

    #ifndef REGAL_NO_GETENV
    getEnv("REGAL_NO_HTTP", enabled);
    getEnv("REGAL_HTTP_PORT", port);
    #endif

    // Compile-time HTTP configuration

    #ifdef REGAL_HTTP_PORT
    port = REGAL_HTTP_PORT;
    #endif
  }

  const char * const br = "<br/>\n";

  static int log_message(const struct mg_connection *conn, const char *message)
  {
    UNUSED_PARAMETER(conn);
    HTrace(message ? message : "");
    return 1;
  }

  static int begin_request(struct mg_connection *conn)
  {
    const struct mg_request_info *request_info = mg_get_request_info(conn);

    HTrace(request_info->request_method ? request_info->request_method : "", " ",
           request_info->uri            ? request_info->uri            : "",
           request_info->query_string   ?                        "?"   : "",
           request_info->query_string   ? request_info->query_string   : "");

    if (!strcmp("/favicon.ico",request_info->uri))
    {
      string http = print_string(
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: image/x-icon\r\n"
                "Content-Length: ", sizeof(favicon), "\r\n"
                "\r\n");

      mg_write(conn,http.c_str(),http.length());
      mg_write(conn,favicon,sizeof(favicon));
      return 1;
    }

    string body;

    if (!strcmp("/log",request_info->uri))
    {
      Logging::getLogMessagesHTML(body);
    }
    else if (!strcmp("/glEnable",request_info->uri))
    {
           if (!strcmp("GL_LOG_INFO_REGAL",    request_info->query_string)) Logging::enableError    = true;
      else if (!strcmp("GL_LOG_WARNING_REGAL", request_info->query_string)) Logging::enableWarning  = true;
      else if (!strcmp("GL_LOG_ERROR_REGAL",   request_info->query_string)) Logging::enableInfo     = true;
      else if (!strcmp("GL_LOG_APP_REGAL",     request_info->query_string)) Logging::enableApp      = true;
      else if (!strcmp("GL_LOG_DRIVER_REGAL",  request_info->query_string)) Logging::enableDriver   = true;
      else if (!strcmp("GL_LOG_INTERNAL_REGAL",request_info->query_string)) Logging::enableInternal = true;
      else if (!strcmp("GL_LOG_HTTP_REGAL",    request_info->query_string)) Logging::enableHttp     = true;

      else if (!strcmp("REGAL_FRAME_TIME",     request_info->query_string)) Logging::frameTime      = true;

      else if (!strcmp("REGAL_MD5_COLOR",      request_info->query_string)) Config::frameMd5Color    = true;
      else if (!strcmp("REGAL_MD5_STENCIL",    request_info->query_string)) Config::frameMd5Stencil  = true;
      else if (!strcmp("REGAL_MD5_DEPTH",      request_info->query_string)) Config::frameMd5Depth    = true;
      else if (!strcmp("REGAL_SAVE_COLOR",     request_info->query_string)) Config::frameSaveColor   = true;
      else if (!strcmp("REGAL_SAVE_STENCIL",   request_info->query_string)) Config::frameSaveStencil = true;
      else if (!strcmp("REGAL_SAVE_DEPTH",     request_info->query_string)) Config::frameSaveDepth   = true;

      body += print_string("glEnable(", request_info->query_string, ");",br,br);
      body += print_string("<a href=\"/glDisable?",request_info->query_string,"\">toggle</a>");
    }
    else if (!strcmp("/glDisable",request_info->uri))
    {
           if (!strcmp("GL_LOG_INFO_REGAL",    request_info->query_string)) Logging::enableError    = false;
      else if (!strcmp("GL_LOG_WARNING_REGAL", request_info->query_string)) Logging::enableWarning  = false;
      else if (!strcmp("GL_LOG_ERROR_REGAL",   request_info->query_string)) Logging::enableInfo     = false;
      else if (!strcmp("GL_LOG_APP_REGAL",     request_info->query_string)) Logging::enableApp      = false;
      else if (!strcmp("GL_LOG_DRIVER_REGAL",  request_info->query_string)) Logging::enableDriver   = false;
      else if (!strcmp("GL_LOG_INTERNAL_REGAL",request_info->query_string)) Logging::enableInternal = false;
      else if (!strcmp("GL_LOG_HTTP_REGAL",    request_info->query_string)) Logging::enableHttp     = false;

      else if (!strcmp("REGAL_FRAME_TIME",     request_info->query_string)) Logging::frameTime      = false;

      else if (!strcmp("REGAL_MD5_COLOR",      request_info->query_string)) Config::frameMd5Color    = false;
      else if (!strcmp("REGAL_MD5_STENCIL",    request_info->query_string)) Config::frameMd5Stencil  = false;
      else if (!strcmp("REGAL_MD5_DEPTH",      request_info->query_string)) Config::frameMd5Depth    = false;
      else if (!strcmp("REGAL_SAVE_COLOR",     request_info->query_string)) Config::frameSaveColor   = false;
      else if (!strcmp("REGAL_SAVE_STENCIL",   request_info->query_string)) Config::frameSaveStencil = false;
      else if (!strcmp("REGAL_SAVE_DEPTH",     request_info->query_string)) Config::frameSaveDepth   = false;

      body += print_string("glDisable(", request_info->query_string, ");",br,br);
      body += print_string("<a href=\"/glEnable?",request_info->query_string,"\">toggle</a>");
    }
    else
    {
      ::REGAL_NAMESPACE_INTERNAL::Init::getContextListingHTML(body);
    }

    string html = print_string(
      "<html><body>\n",
      body,
      "</body></html>\n");

    string http = print_string(
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Content-Length: ", html.length(), "\r\n"
              "\r\n",
              html);

    mg_write(conn,http.c_str(),http.length());
    return 1;  // Mark as handled for Mongoose
  }

  void Start()
  {
    Internal("Http::Start","()");

    if (enabled && !ctx)
    {
      memset(&callbacks,0,sizeof(callbacks));
      callbacks.log_message   = log_message;
      callbacks.begin_request = begin_request;

      // Try listening on the configured port number (8080 by default)
      // and advance foward until one is available

      for (int i = 0; i<100; ++i)
      {
        string j = print_string(port+i);
        const char *options[3] = { "listening_ports", j.c_str(), NULL};
        ctx = mg_start(&callbacks, NULL, options);
        if (ctx)
          break;
      }

      const char *option = NULL;
      if (ctx)
        option = mg_get_option(ctx,"listening_ports");

      if (option)
      {
        Info("Listening for HTTP connections on port ",option);
      }
      else
      {
        Info("Not listening for HTTP connections.");
      }
    }
  }

  void Stop()
  {
    Internal("Http::Stop","()");

    if (ctx)
    {
      HTrace("Closing HTTP connections.");

      // Currently there is a problem with shutting down mongoose
      // on Windows - so just skip the cleanup for now.

      #if !REGAL_SYS_WGL
      mg_stop(ctx);
      #endif

      ctx = NULL;
    }
  }

}

REGAL_NAMESPACE_END

#endif

