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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <cstring>

#include <string>
using std::string;

#include <algorithm>

using std::max;
using std::min;

#include <boost/print/string_list.hpp>
typedef boost::print::string_list<string> string_list;

#include "RegalXfer.h"
#include "RegalLog.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

namespace {
  
  template <typename T> struct Limits { static const T max = ~0; };
  template <> struct Limits<GLubyte> { static const GLubyte max = 0xff; };
  template <> struct Limits<GLushort> { static const GLushort max = 0xffff; };
  template <> struct Limits<GLuint> { static const GLuint max = 0xffffffff; };
  template <> struct Limits<GLbyte> { static const GLbyte max = 0x7f; };
  template <> struct Limits<GLshort> { static const GLshort max = 0x7fff; };
  template <> struct Limits<GLint> { static const GLint max = 0x7fffffff; };
  
  template <typename T> GLfloat ToFloat( T v ) { return max( -1.0f, GLfloat( v ) / GLfloat( Limits<T>::max ) ); }
  template <typename T> T ToType( GLfloat f ) { return T( max( -1.0f * GLfloat( Limits<T>::max ), min( GLfloat( Limits<T>::max ), f * GLfloat( Limits<T>::max ) ) ) ); }
  
  /*
  GLfloat ToFloat( GLubyte v ) { return GLfloat( v ) / GLfloat( 0xff ); }
  GLfloat ToFloat( GLushort v ) { return GLfloat( v ) / GLfloat( 0xffff ); }
  GLfloat ToFloat( GLuint v ) { return GLfloat( v ) / GLfloat( 0xffffffff ); }

  GLfloat ToFloat( GLbyte v ) { return max( -1.0f, GLfloat( v ) / GLfloat( 0x7f ) ); }
  GLfloat ToFloat( GLshort v ) { return max( -1.0f, GLfloat( v ) / GLfloat( 0x7fff ) ); }
  GLfloat ToFloat( GLint v ) { return max( -1.0f, GLfloat( v ) / GLfloat( 0x7fffffff ) ); }
  */
  
  GLubyte ToUbyte( GLfloat v ) { return ToType<GLubyte>( v ); }
  GLushort ToUshort( GLfloat v ) { return ToType<GLushort>( v ); }
  GLuint ToUint( GLfloat v ) { return ToType<GLuint>( v ); }
  
  GLbyte ToByte( GLfloat v ) { return ToType<GLbyte>( v ); }
  GLshort ToShort( GLfloat v ) { return ToType<GLshort>( v ); }
  GLint ToInt( GLfloat v ) { return ToType<GLint>( v ); }
  
  Float4 ToFloat4( GLenum format, GLenum type, const GLvoid *pixels ) {
    return Float4();
  }
  
  
}

void RegalXfer::PixelStore( RegalContext * ctx, GLenum pname, GLint param )
{

  switch( pname ) {
    case GL_UNPACK_ROW_LENGTH: unpackRowLength = param; break;
    case GL_UNPACK_SKIP_ROWS: unpackSkipRows = param; break;
    case GL_UNPACK_SKIP_PIXELS: unpackSkipPixels = param; break;
    default: break;
  }
  
}

void RegalXfer::TexImage2D( RegalContext * ctx, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
  DispatchTable & tbl = ctx->dispatcher.emulation;
  tbl.glTexImage2D( target, level, internalFormat, width, height, border, format, type, pixels );
}

void RegalXfer::TexSubImage2D( RegalContext * ctx, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels )
{
  DispatchTable & tbl = ctx->dispatcher.emulation;
  tbl.glTexSubImage2D( target, level , xoffset, yoffset, width, height, format, type, pixels );
}


REGAL_NAMESPACE_END

#endif // REGAL_EMULATION










