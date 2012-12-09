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
  
  template <typename T> struct Limits { static const T min = 0, max = ~0; };
  template <> struct Limits<GLubyte>  { static const GLubyte  min = 0,          max = 0xff;       };
  template <> struct Limits<GLushort> { static const GLushort min = 0,          max = 0xffff;     };
  template <> struct Limits<GLuint>   { static const GLuint   min = 0,          max = 0xffffffff; };
  template <> struct Limits<GLbyte>   { static const GLbyte   min = 0x81,       max = 0x7f;       };
  template <> struct Limits<GLshort>  { static const GLshort  min = 0x8001,     max = 0x7fff;     };
  template <> struct Limits<GLint>    { static const GLint    min = 0x80000001, max = 0x7fffffff; };
  
  template <typename T> GLfloat ToFloat( T v ) {
    return  min( Limits<T>::max, max( Limits<T>::min, v ) ) / GLfloat( Limits<T>::max );
  }
  
  template <typename T> T ToType( GLfloat f ) {
    return T( 0.5f + Limits<T>::max * max( -1.0f , min( 1.0f, f ) ) );
  }
  
  GLubyte ToUbyte( GLfloat v ) { return ToType<GLubyte>( v ); }
  GLushort ToUshort( GLfloat v ) { return ToType<GLushort>( v ); }
  GLuint ToUint( GLfloat v ) { return ToType<GLuint>( v ); }
  
  GLbyte ToByte( GLfloat v ) { return ToType<GLbyte>( v ); }
  GLshort ToShort( GLfloat v ) { return ToType<GLshort>( v ); }
  GLint ToInt( GLfloat v ) { return ToType<GLint>( v ); }
  
  int PixelSize( GLenum format, GLenum type ) {
    int cmpsz = 0;
    switch( type ) {
      case GL_UNSIGNED_SHORT_5_5_5_1:
      case GL_UNSIGNED_SHORT_1_5_5_5_REV:
      case GL_UNSIGNED_SHORT_5_6_5:
      case GL_UNSIGNED_SHORT_5_6_5_REV:
      case GL_UNSIGNED_SHORT_4_4_4_4:
      case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        return 2;
      case GL_UNSIGNED_INT_10_10_10_2:
      case GL_UNSIGNED_INT_2_10_10_10_REV:
      case GL_UNSIGNED_INT_8_8_8_8:
      case GL_UNSIGNED_INT_8_8_8_8_REV:
        return 4;
      case GL_UNSIGNED_INT:
      case GL_INT:
      case GL_FLOAT:
        cmpsz = 4;
        break;
      case GL_UNSIGNED_SHORT:
      case GL_SHORT:
      case GL_HALF_FLOAT:
        cmpsz = 2;
        break;
      case GL_UNSIGNED_BYTE:
      case GL_BYTE:
        cmpsz = 1;
        break;
      default:
        Warning( "Unsupported Type for pixel transfer" );
        return 0;
    }
    int numcmp = 0;
    switch ( format ) {
      case GL_RGBA:
      case GL_BGRA:
        numcmp = 4;
        break;
      case GL_RGB:
      case GL_BGR:
        numcmp = 3;
        break;
      case GL_LUMINANCE_ALPHA:
      case GL_RG:
      case GL_DEPTH_STENCIL:
        numcmp = 2;
        break;
      case GL_INTENSITY:
      case GL_LUMINANCE:
      case GL_ALPHA:
      case GL_R:
      case GL_DEPTH_COMPONENT:
      case GL_STENCIL_INDEX:
        numcmp = 1;
      default:
        Warning( "Unsupported Pixel Transfer format." )
        return 0;
    }
    return cmpsz * numcmp;
  }
  
  void SubImage2D( RegalContext * ctx, GLint ifmt, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels ) {
    DispatchTable & tbl = ctx->dispatcher.emulation;
    void * vline = alloca( width * 4 * sizeof( GLint ) );
    int rowLength = ctx->xfer->unpackRowLength;
    if( rowLength == 0 ) {
      rowLength = PixelSize( format, type ) *  width;
      RegalAssert( rowLength != 0 );
    }
    // now unpack the packed formats into their canonical formats
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
  tbl.glTexImage2D( target, level, internalFormat, width, height, border, format, type, NULL );
  name2ifmt[ textureBinding2D[ activeTextureIndex ] ] = internalFormat;
  SubImage2D( ctx, internalFormat, level, 0, 0, width, height, format, type, pixels );
}

void RegalXfer::TexSubImage2D( RegalContext * ctx, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels )
{
  GLint ifmt = name2ifmt[ textureBinding2D[ activeTextureIndex ] ];
  SubImage2D( ctx, ifmt, level , xoffset, yoffset, width, height, format, type, pixels );
  
}


REGAL_NAMESPACE_END

#endif // REGAL_EMULATION










