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

#ifndef __REGAL_PRIVATE_H__
#define __REGAL_PRIVATE_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include "RegalUtil.h"
#include "RegalConfig.h"
#include "RegalLookup.h"
#include "RegalDispatch.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct RegalContext;

inline GLfloat  fixedToFloat (const GLfixed  v) { return v / 65536.0f;          }
inline GLdouble fixedToDouble(const GLfixed  v) { return v / 65536.0;           }
inline GLfixed  floatToFixed (const GLfloat  v) { return GLfixed(v * 65536.0f); }
inline GLdouble floatToDouble(const GLfloat  v) { return GLdouble(v);           }
inline GLfixed  doubleToFixed(const GLdouble v) { return GLfixed(v * 65536.0);  }
inline GLfloat  doubleToFloat(const GLdouble v) { return GLfloat(v);            }

inline void fixedToFloat(GLfloat *dest, const GLfixed *src, const std::size_t n)
{
  for (std::size_t i=0; i<n; ++i)
    dest[i] = fixedToFloat(src[i]);
}

inline void fixedToDouble(GLdouble *dest, const GLfixed *src, const std::size_t n)
{
  for (std::size_t i=0; i<n; ++i)
    dest[i] = fixedToFloat(src[i]);
}

inline void floatToFixed(GLfixed *dest, const GLfloat *src, const std::size_t n)
{
  for (std::size_t i=0; i<n; ++i)
    dest[i] = floatToFixed(src[i]);
}

inline void floatToDouble(GLdouble *dest, const GLfloat *src, const std::size_t n)
{
  for (std::size_t i=0; i<n; ++i)
    dest[i] = floatToDouble(src[i]);
}

inline void doubleToFixed(GLfixed *dest, const GLdouble *src, const std::size_t n)
{
  for (std::size_t i=0; i<n; ++i)
    dest[i] = doubleToFixed(src[i]);
}

inline void doubleToFloat(GLfloat *dest, const GLdouble *src, const std::size_t n)
{
  for (std::size_t i=0; i<n; ++i)
    dest[i] = doubleToFloat(src[i]);
}

using ::RegalErrorCallback;

#define REGAL_DECL_EXPORT 1
#define REGAL_SYS_WGL_DECLARE_WGL 1

// attrib mapping

enum RegalFixedFunctionAttrib {
    RFF2A_Vertex = 0,
    RFF2A_Weights = 1,
    RFF2A_Normal = 2,
    RFF2A_Color = 3,
    RFF2A_SecondaryColor = 4,
    RFF2A_FogCoord = 5,
    RFF2A_EdgeFlag = 6,
    RFF2A_TexCoord = 7,
    RFF2A_Invalid = 99
};

const GLenum RFFA2Enum[] = {
    GL_VERTEX_ARRAY,
    GL_WEIGHT_ARRAY_ARB,
    GL_NORMAL_ARRAY,
    GL_COLOR_ARRAY,
    GL_SECONDARY_COLOR_ARRAY,
    GL_FOG_COORDINATE_ARRAY,
    GL_EDGE_FLAG_ARRAY,
    GL_TEXTURE_COORD_ARRAY
};

const GLuint RFF2AMap8[] = {  0, 99, 2, 1, 99, 7, 99, 3 };
const GLuint RFF2AInvMap8[] = {
    RFF2A_Vertex,
    RFF2A_Color,
    RFF2A_Normal,
    RFF2A_TexCoord,
    RFF2A_TexCoord,
    RFF2A_TexCoord,
    RFF2A_SecondaryColor,
    RFF2A_FogCoord
};
const GLuint RFF2ATexBegin8 = 3;
const GLuint RFF2ATexEnd8 = 6;
const GLuint RFF2AMap16[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const GLuint RFF2AInvMap16[] = {
    RFF2A_Vertex,
    RFF2A_Weights,
    RFF2A_Normal,
    RFF2A_Color,
    RFF2A_SecondaryColor,
    RFF2A_FogCoord,
    RFF2A_EdgeFlag,
    RFF2A_Invalid,
    RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord,
    RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord
};
const GLuint RFF2ATexBegin16 = 8;
const GLuint RFF2ATexEnd16 = 16;

REGAL_NAMESPACE_END

#endif // REGAL_PRIVATE_H
