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

 Regal Emulation layer constants
 Cass Everitt, Scott Nations

 */

#ifndef __REGAL_EMU_H__
#define __REGAL_EMU_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu
{

// In ES2 mode, 16 texture units only?

// No. of fixed function texture units. 2 minimum.

#ifndef REGAL_EMU_MAX_TEXTURE_UNITS
#define REGAL_EMU_MAX_TEXTURE_UNITS 4
#endif

// No. of texture coordinate sets. 8 minimum.

#ifndef REGAL_EMU_MAX_TEXTURE_COORDS
#define REGAL_EMU_MAX_TEXTURE_COORDS 16
#endif

// No. of active vertex attributes.  16 minimum.

#ifndef REGAL_EMU_MAX_VERTEX_ATTRIBS
#define REGAL_EMU_MAX_VERTEX_ATTRIBS 16
#endif

// Max no. of vertex buffers. 16 minimum

#ifndef REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS
#define REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS 16
#endif

// Total no. of texture units accessible by the GL.  96 minimum

#ifndef REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS
#define REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS 96
#endif

// Max depth of the server attribute stack.  16 minimum.

#ifndef REGAL_EMU_MAX_ATTRIB_STACK_DEPTH
#define REGAL_EMU_MAX_ATTRIB_STACK_DEPTH 16
#endif

// Max depth of the client attribute stack.  16 minimum.

#ifndef REGAL_EMU_MAX_CLIENT_ATTRIB_STACK_DEPTH
#define REGAL_EMU_MAX_CLIENT_ATTRIB_STACK_DEPTH 16
#endif

// Max no. of active draw buffers. 8 minimum

#ifndef REGAL_EMU_MAX_DRAW_BUFFERS
#define REGAL_EMU_MAX_DRAW_BUFFERS 8
#endif

// Max no. of active viewports. 16 minimum

#ifndef REGAL_EMU_MAX_VIEWPORTS
#define REGAL_EMU_MAX_VIEWPORTS 16
#endif

#if REGAL_EMULATION

extern bool validTextureEnum(GLenum texture);
extern bool validTextureUnit(GLuint unit);

#endif // REGAL_EMULATION

};

REGAL_NAMESPACE_END

#endif // ! __REGAL_EMU_H__
