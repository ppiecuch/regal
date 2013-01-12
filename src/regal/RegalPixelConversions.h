/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc
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

 Regal Pixel Conversion Functions
 Lloyd Pique

 */

#ifndef __REGAL_PIXEL_CONVERSIONS_H__
#define __REGAL_PIXEL_CONVERSIONS_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <boost/cstdint.hpp>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using ::boost::uint8_t;
using ::boost::uint16_t;
using ::boost::uint32_t;

// Each unpacker function unpacks pixel data from some arbitrary format into a
// a simple full-size format.  Each packer function packs pixel data into some
// arbitrary format from a simple full-size format.  Taken together, the two
// can be used to convert from a variety of formats.
//
// The 32 bit forms pack and unpack to an intermediate 32 bit RGBA 8888 format.
//
// Note that when unpacking, if the source format does not support a particular
// component (such as Alpha or Blue), the unpacked form will have zero values
// for that component.

// typedef void Unpacker32 ( const void* src,     uint32_t* dst, size_t cnt );
// typedef void Packer32   ( const uint32_t* src, void* dst,     size_t cnt );

// These functions retrieve a specific packer or unpacker function for a
// specific OpenGL texture format and type, and return NULL if no such
// conversion is known.

// Unpacker32 *GetUnpacker32 ( GLenum format, GLenum type );
// Packer32   *GetPacker32   ( GLenum format, GLenum type );

class IConversion
{
 public:
  virtual void Unpack32 ( const void*     src, uint32_t* dst, size_t cnt ) const = 0;
  virtual void Pack32   ( const uint32_t* src, void*     dst, size_t cnt ) const = 0;
  virtual size_t GetPackedPixelByteSize() = 0;
  virtual size_t GetPackedPixelAlignmentSize() = 0;
  virtual size_t GetPackedPixelComponents() = 0;
 protected:
  ~IConversion() {};
};

IConversion *GetConversionInterface(GLenum format, GLenum type);

REGAL_NAMESPACE_END

#endif // ! __REGAL_PIXEL_CONVERSIONS_H__
