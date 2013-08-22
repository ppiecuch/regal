/*
  Copyright (c) 2011-2012 NVIDIA Corporation
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

/*

 GL_EXT_debug_marker emulation layer
 Nigel Stewart

 */

#ifndef __REGAL_MARKER_H__
#define __REGAL_MARKER_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <list>
#include <string>

#include <GL/Regal.h>

#include "RegalLog.h"
#include "RegalContext.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct Marker {

  void Init(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
    Internal("Regal::Marker::Init","()");
  }

  void InsertEventMarker(RegalContext &ctx, const std::string &marker)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(marker);
    Internal("Regal::Marker::InsertEventMarker","marker=",marker);
    //Info("// ",marker);
  }

  void PushGroupMarker(RegalContext &ctx, const std::string &marker)
  {
    UNUSED_PARAMETER(ctx);
    Internal("Regal::Marker::PushGroupMarker","marker=",marker);
    //Info("// ",marker," ...");
    markerStack.push_back(marker);
  }

  void PopGroupMarker(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
    Internal("Regal::Marker::PopGroupMarker","()");
    std::string marker;
    if (markerStack.size())
      marker.swap(markerStack.back());
    markerStack.pop_back();
    //Info("// ... ",marker);
  }

  void FrameTerminator(RegalContext &ctx);

  std::size_t indent() const { return markerStack.size()*2; }

  public:

    // GL_EXT_debug_marker
    // http://www.khronos.org/registry/gles/extensions/EXT/EXT_debug_marker.txt
    //... If length is 0 then marker is assumed to be null-terminated....

    static inline std::string
    toStringEXT(GLsizei length, const char *marker)
    {
      if (length<=0 || !marker)
        return std::string(marker ? marker : "");
      return std::string(marker,length);
    }

    // GL_KHR_debug
    // http://www.opengl.org/registry/specs/KHR/debug.txt
    // ... If length is negative, it is implied that message contains
    // a null terminated string...

    static inline std::string
    toStringKHR(GLsizei length, const char *marker)
    {
      if (length==0 || !marker)
        return std::string();
      if (length<0)
        return std::string(marker);
      return std::string(marker,length);
    }

    std::list< std::string > markerStack;
};

REGAL_NAMESPACE_END

#endif
