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

#ifndef __REGAL_CLIENT_STATE_H__
#define __REGAL_CLIENT_STATE_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <cstring>    // For memset, memcpy

#include <string>
#include <algorithm>  // For std::swap

#include <boost/print/print_string.hpp>
#include <boost/print/string_list.hpp>

#include "RegalEmu.h"
#include "RegalToken.h"
#include "RegalDispatch.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

//
// OpenGL Client State Tracking
//
// Motivating requirements:
//
//  - Emulation of glPushClientAttrib and glPopClientAttrib for OpenGL ES and
//    core OpenGL that lack the functionality.
//
//  - OpenGL client state capture, browsing, diff and serialization.
//
//  - Emulation of BaseVertex commands
//
// See also:
//
//  - Gallium3D
//    http://wiki.freedesktop.org/wiki/Software/gallium
//    http://dri.freedesktop.org/doxygen/gallium/p__state_8h-source.html
//
//  - Tracking Graphics State For Networked Rendering
//    Ian Buck, Greg Humphreys and Pat Hanrahan.
//    Stanford University
//    Proceedings of the 2000 Eurographics/SIGGRAPH Workshop on Graphics Hardware
//    http://graphics.stanford.edu/papers/state_tracking/
//
//  - Chromium: A Stream Processing Framework for Interactive Rendering on Clusters
//    Greg Humphreys, Mike Houston, Ren Ng
//    Stanford University
//    SIGGRAPH 2002
//    http://graphics.stanford.edu/papers/cr/
//

namespace Client
{

namespace State
{

  using   ::boost::print::hex;
  using   ::boost::print::print_string;
  typedef ::boost::print::string_list<std::string> string_list;

  inline static void enable(DispatchTableGL &dt, const GLenum cap, const GLboolean enable)
  {
    if (enable)
      dt.call(&dt.glEnableClientState)(cap);
    else
      dt.call(&dt.glDisableClientState)(cap);
  }

  inline static void enablei(DispatchTableGL &dt, const GLenum cap, const GLuint index, const GLboolean enable)
  {
    if (enable)
      dt.call(&dt.glEnableClientStateiEXT)(cap,index);
    else
      dt.call(&dt.glDisableClientStateiEXT)(cap,index);
  }

  inline static void enableToString(string_list& tmp, const GLboolean b, const char* bEnum, const char *delim = "\n")
  {
      tmp << print_string(b ? "glEnableClientState(" : "glDisableClientState(",bEnum,");",delim);
  }

  inline static void enableiToString(string_list& tmp, const GLboolean b, const char* bEnum, const GLuint index, const char *delim = "\n")
  {
      tmp << print_string(b ? "glEnableClientStateiEXT(" : "glDisableClientStateiEXT(",bEnum,",",index,");",delim);
  }

  //
  // glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
  //

  const GLuint nNamedArrays = 7 + REGAL_EMU_MAX_TEXTURE_COORDS;

  typedef enum _vaName
  {
    VERTEX          = 0,
    NORMAL          = 1,
    FOG_COORD       = 2,
    COLOR           = 3,
    SECONDARY_COLOR = 4,
    INDEX           = 5,
    EDGE_FLAG       = 6,
    TEX_COORD       = 7
  } vaName;

  const GLenum vaEnum[][6] =
  {
    { GL_VERTEX_ARRAY,          GL_VERTEX_ARRAY_POINTER,          GL_VERTEX_ARRAY_BUFFER_BINDING,          GL_VERTEX_ARRAY_SIZE,          GL_VERTEX_ARRAY_TYPE,          GL_VERTEX_ARRAY_STRIDE          },
    { GL_NORMAL_ARRAY,          GL_NORMAL_ARRAY_POINTER,          GL_NORMAL_ARRAY_BUFFER_BINDING,          GL_ZERO,                       GL_NORMAL_ARRAY_TYPE,          GL_NORMAL_ARRAY_STRIDE          },
    { GL_FOG_COORD_ARRAY,       GL_FOG_COORD_ARRAY_POINTER,       GL_FOG_COORD_ARRAY_BUFFER_BINDING,       GL_ZERO,                       GL_FOG_COORD_ARRAY_TYPE,       GL_FOG_COORD_ARRAY_STRIDE       },
    { GL_COLOR_ARRAY,           GL_COLOR_ARRAY_POINTER,           GL_COLOR_ARRAY_BUFFER_BINDING,           GL_COLOR_ARRAY_SIZE,           GL_COLOR_ARRAY_TYPE,           GL_COLOR_ARRAY_STRIDE           },
    { GL_SECONDARY_COLOR_ARRAY, GL_SECONDARY_COLOR_ARRAY_POINTER, GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING, GL_SECONDARY_COLOR_ARRAY_SIZE, GL_SECONDARY_COLOR_ARRAY_TYPE, GL_SECONDARY_COLOR_ARRAY_STRIDE },
    { GL_INDEX_ARRAY,           GL_INDEX_ARRAY_POINTER,           GL_INDEX_ARRAY_BUFFER_BINDING,           GL_ZERO,                       GL_INDEX_ARRAY_TYPE,           GL_INDEX_ARRAY_STRIDE           },
    { GL_EDGE_FLAG_ARRAY,       GL_EDGE_FLAG_ARRAY_POINTER,       GL_EDGE_FLAG_ARRAY_BUFFER_BINDING,       GL_ZERO,                       GL_ZERO,                       GL_EDGE_FLAG_ARRAY_STRIDE       },
    { GL_TEXTURE_COORD_ARRAY,   GL_TEXTURE_COORD_ARRAY_POINTER,   GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING,   GL_TEXTURE_COORD_ARRAY_SIZE,   GL_TEXTURE_COORD_ARRAY_TYPE,   GL_TEXTURE_COORD_ARRAY_STRIDE   },
  };

  struct NamedVertexArray
  {
    GLboolean  enabled;  // GL_x_ARRAY
    const GLvoid* pointer;  // GL_x_ARRAY_POINTER
    GLuint     buffer;   // GL_x_ARRAY_BUFFER_BINDING
    GLint      size;     // GL_x_ARRAY_SIZE
    GLenum     type;     // GL_x_ARRAY_TYPE
    GLint      stride;   // GL_x_ARRAY_STRIDE

    inline NamedVertexArray()
    : enabled(GL_FALSE)
    , pointer(NULL)
    , buffer(0)
    , size(4)
    , type(GL_FLOAT)
    , stride(0)
    {
    }

    inline NamedVertexArray &swap(NamedVertexArray &other)
    {
      std::swap(enabled,other.enabled);
      std::swap(pointer,other.pointer);
      std::swap(buffer,other.buffer);
      std::swap(size,other.size);
      std::swap(type,other.type);
      std::swap(stride,other.stride);
      return *this;
    }

    NamedVertexArray &get(DispatchTableGL &dt, vaName va)
    {
      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (vaInt >= 0)
      {
        if (vaInt < 7)
        {
          enabled = dt.call(&dt.glIsEnabled)(vaEnum[vaInt][0]);
          dt.call(&dt.glGetPointerv)(vaEnum[vaInt][1],const_cast<GLvoid**>(&pointer));
          dt.call(&dt.glGetIntegerv)(vaEnum[vaInt][2],reinterpret_cast<GLint*>(&buffer));
          if (vaEnum[vaInt][3] != GL_ZERO)
            dt.call(&dt.glGetIntegerv)(vaEnum[vaInt][3],&size);
          if (vaEnum[vaInt][4] != GL_ZERO)
            dt.call(&dt.glGetIntegerv)(vaEnum[vaInt][4],reinterpret_cast<GLint*>(&type));
          dt.call(&dt.glGetIntegerv)(vaEnum[vaInt][5],&stride);
        }
        else
        {
          GLuint index = static_cast<GLuint>(vaInt - 7);
          enabled = dt.call(&dt.glIsEnabledi)(vaEnum[7][0],index);
          dt.call(&dt.glGetPointeri_vEXT)(vaEnum[7][1],index,const_cast<GLvoid**>(&pointer));
          dt.call(&dt.glGetIntegeri_v)(vaEnum[7][2],index,reinterpret_cast<GLint*>(&buffer));
          dt.call(&dt.glGetIntegeri_v)(vaEnum[7][3],index,&size);
          dt.call(&dt.glGetIntegeri_v)(vaEnum[7][4],index,reinterpret_cast<GLint*>(&type));
          dt.call(&dt.glGetIntegeri_v)(vaEnum[7][5],index,&stride);
        }
      }
      return *this;
    }

    const NamedVertexArray &set(DispatchTableGL &dt, vaName va) const
    {
      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (vaInt >= 0)
      {
        if (vaInt < 7)
        {
          enable(dt,vaEnum[vaInt][0],enabled);
          dt.call(&dt.glBindBuffer)(GL_ARRAY_BUFFER,buffer);
          switch (vaInt)
          {
            case VERTEX:
              dt.call(&dt.glVertexPointer)(size,type,stride,pointer);
              break;
            case NORMAL:
              dt.call(&dt.glNormalPointer)(type,stride,pointer);
              break;
            case FOG_COORD:
              dt.call(&dt.glFogCoordPointer)(type,stride,pointer);
              break;
            case COLOR:
              dt.call(&dt.glColorPointer)(size,type,stride,pointer);
              break;
            case SECONDARY_COLOR:
              dt.call(&dt.glSecondaryColorPointer)(size,type,stride,pointer);
              break;
            case INDEX:
              dt.call(&dt.glIndexPointer)(type,stride,pointer);
              break;
            case EDGE_FLAG:
              dt.call(&dt.glEdgeFlagPointer)(stride,pointer);
              break;
            default:
              break;
          }
        }
        else
        {
          GLuint index = static_cast<GLuint>(vaInt - 7);
          enablei(dt,vaEnum[7][0],index,enabled);
          dt.call(&dt.glBindBuffer)(GL_ARRAY_BUFFER,buffer);
          dt.call(&dt.glMultiTexCoordPointerEXT)(GL_TEXTURE0+index,size,type,stride,pointer);
        }
      }
      return *this;
    }

    std::string toString(vaName va, const char *delim = "\n") const
    {
      string_list tmp;

      GLint vaInt = static_cast<GLint>(va);
      RegalAssert(vaInt >= 0 && vaInt < static_cast<GLint>(nNamedArrays));
      if (vaInt >= 0)
      {
        if (vaInt < 7)
        {
          enableToString(tmp, enabled, Token::toString(vaEnum[vaInt][0]), delim);
          tmp << print_string("glBindBuffer(GL_ARRAY_BUFFER,",buffer,");",delim);
          switch (vaInt)
          {
            case VERTEX:
              tmp << print_string("glVertexPointer(",size,",",type,",",stride,",0x",pointer,");",delim);
              break;
            case NORMAL:
              tmp << print_string("glNormalPointer(",type,",",stride,",0x",pointer,");",delim);
              break;
            case FOG_COORD:
              tmp << print_string("glFogCoordPointer(",type,",",stride,",0x",pointer,");",delim);
              break;
            case COLOR:
              tmp << print_string("glColorPointer(",size,",",type,",",stride,",0x",pointer,");",delim);
              break;
            case SECONDARY_COLOR:
              tmp << print_string("glSecondaryColorPointer(",size,",",type,",",stride,",0x",pointer,");",delim);
              break;
            case INDEX:
              tmp << print_string("glIndexPointer(",type,",",stride,",0x",pointer,");",delim);
              break;
            case EDGE_FLAG:
              tmp << print_string("glEdgeFlagPointer(",stride,",0x",pointer,");",delim);
              break;
            default:
              break;
          }
        }
        else
        {
          GLuint index = static_cast<GLuint>(vaInt - 7);
          enableiToString(tmp, enabled, Token::toString(vaEnum[vaInt][0]), index, delim);
          tmp << print_string("glBindBuffer(GL_ARRAY_BUFFER,",buffer,");",delim);
          tmp << print_string("glMultiTexCoordPointerEXT(",Token::toString(GL_TEXTURE0+index),",",size,",",type,",",stride,",0x",pointer,");",delim);
        }
      }
      return tmp;
    }
  };

  struct VertexBufferBindPoint
  {
    GLuint    buffer;   // GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
    GLintptr  offset;   // GL_VERTEX_ATTRIB_RELATIVE_OFFSET
    GLsizei   stride;   // GL_VERTEX_ATTRIB_ARRAY_STRIDE
    GLuint    divisor;  // GL_VERTEX_ATTRIB_ARRAY_DIVISOR

    inline VertexBufferBindPoint()
    : buffer(0)
    , offset(0)
    , stride(0)
    , divisor(0)
    {
    }

    inline VertexBufferBindPoint &swap(VertexBufferBindPoint &other)
    {
      std::swap(buffer,other.buffer);
      std::swap(offset,other.offset);
      std::swap(stride,other.stride);
      std::swap(divisor,other.divisor);
      return *this;
    }

    VertexBufferBindPoint &get(DispatchTableGL &dt, GLuint index)
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&buffer));
        dt.call(&dt.glGetVertexAttribPointerv)(index,GL_VERTEX_ATTRIB_ARRAY_POINTER,reinterpret_cast<GLvoid **>(&offset));
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_STRIDE,reinterpret_cast<GLint*>(&stride));
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_DIVISOR,reinterpret_cast<GLint*>(&divisor));
      }
      return *this;
    }

    const VertexBufferBindPoint &set(DispatchTableGL &dt, GLuint index) const
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        dt.call(&dt.glBindVertexBuffer)(index,buffer,offset,stride);
        dt.call(&dt.glVertexBindingDivisor)(index,divisor);
      }
      return *this;
    }

    std::string toString(GLuint index, const char *delim = "\n") const
    {
      string_list tmp;
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS)
      {
        tmp << print_string("glBindVertexBuffer(",index,",",buffer,",",offset,",",stride,");",delim);
        tmp << print_string("glVertexBindingDivisor(",index,",",divisor,");",delim);
      }
      return tmp;
    }
  };

  struct GenericVertexArray
  {
    GLboolean  enabled;         // GL_VERTEX_ATTRIB_ARRAY_ENABLED
    GLint      size;            // GL_VERTEX_ATTRIB_ARRAY_SIZE
    GLenum     type;            // GL_VERTEX_ATTRIB_ARRAY_TYPE
    GLuint     relativeOffset;  // GL_VERTEX_ATTRIB_RELATIVE_OFFSET
    GLboolean  normalized;      // GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
    GLboolean  isInteger;       // GL_VERTEX_ATTRIB_ARRAY_INTEGER
    GLboolean  isLong;          // GL_VERTEX_ATTRIB_ARRAY_LONG
    GLuint     bindingIndex;    // GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING

    inline GenericVertexArray()
    : enabled(GL_FALSE)
    , size(4)
    , type(GL_FLOAT)
    , relativeOffset(0)
    , normalized(GL_FALSE)
    , isInteger(GL_FALSE)
    , isLong(GL_FALSE)
    , bindingIndex(0)
    {
    }

    inline GenericVertexArray &swap(GenericVertexArray &other)
    {
      std::swap(enabled,other.enabled);
      std::swap(size,other.size);
      std::swap(type,other.type);
      std::swap(relativeOffset,other.relativeOffset);
      std::swap(normalized,other.normalized);
      std::swap(isInteger,other.isInteger);
      std::swap(isLong,other.isLong);
      std::swap(bindingIndex,other.bindingIndex);
      return *this;
    }

    GenericVertexArray &get(DispatchTableGL &dt, GLuint index)
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        GLint val;

        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&val);
        enabled = static_cast<GLboolean>(val);
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_SIZE,&size);
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_TYPE,reinterpret_cast<GLint*>(&type));
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_RELATIVE_OFFSET,reinterpret_cast<GLint*>(&relativeOffset));
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,&val);
        normalized = static_cast<GLboolean>(val);
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_INTEGER,&val);
        isInteger = static_cast<GLboolean>(val);
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_LONG,&val);
        isLong = static_cast<GLboolean>(val);
        dt.call(&dt.glGetVertexAttribiv)(index,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&bindingIndex));
      }
      return *this;
    }

    const GenericVertexArray &set(DispatchTableGL &dt, GLuint index) const
    {
      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        if (enabled)
          dt.call(&dt.glEnableVertexAttribArray)(index);
        else
          dt.call(&dt.glDisableVertexAttribArray)(index);
        if (isInteger)
          dt.call(&dt.glVertexAttribIFormat)(index,size,type,relativeOffset);
        else if (isLong)
          dt.call(&dt.glVertexAttribLFormat)(index,size,type,relativeOffset);
        else
          dt.call(&dt.glVertexAttribFormat)(index,size,type,normalized,relativeOffset);
        dt.call(&dt.glVertexAttribBinding)(index,bindingIndex);
      }
      return *this;
    }

    std::string toString(GLuint index, const char *delim = "\n") const
    {
      string_list tmp;

      RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
      if (index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      {
        if (enabled)
          tmp << print_string("glEnableVertexAttribArray(",index,");",delim);
        else
          tmp << print_string("glDisableVertexAttribArray(",index,");",delim);
        if (isInteger)
          tmp << print_string("glVertexAttribIFormat(",index,",",size,",",type,",",relativeOffset,");",delim);
        else if (isLong)
          tmp << print_string("glVertexAttribLFormat(",index,",",size,",",type,",",relativeOffset,");",delim);
        else
          tmp << print_string("glVertexAttribFormat(",index,",",size,",",type,",",normalized,",",relativeOffset,");",delim);
        tmp << print_string("glVertexAttribBinding(",index,",",bindingIndex,");",delim);
      }
      return tmp;
    }
  };

  struct VertexArray
  {
    NamedVertexArray      named[nNamedArrays];
    VertexBufferBindPoint bindings[REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS];
    GenericVertexArray    generic[REGAL_EMU_MAX_VERTEX_ATTRIBS];

    GLuint     elementArrayBufferBinding;        // GL_ELEMENT_ARRAY_BUFFER_BINDING

    // Table 23.8. Vertex Array Data (not in Vertex Array objects)
    //
    //  dsn: primitiveRestartFixedIndex is not in this table but should be (khronos bug 10250)

    GLenum     clientActiveTexture;              // GL_CLIENT_ACTIVE_TEXTURE
    GLboolean  primitiveRestartFixedIndex;       // GL_PRIMITIVE_RESTART_FIXED_INDEX
    GLboolean  primitiveRestart;                 // GL_PRIMITIVE_RESTART
    GLuint     primitiveRestartIndex;            // GL_PRIMITIVE_RESTART_INDEX
    GLuint     arrayBufferBinding;               // GL_ARRAY_BUFFER_BINDING
    GLuint     vertexArrayBinding;               // GL_VERTEX_ARRAY_BINDING

    inline VertexArray()
    : elementArrayBufferBinding(0)
    , clientActiveTexture(GL_TEXTURE0)
    , primitiveRestartFixedIndex(GL_FALSE)
    , primitiveRestart(GL_FALSE)
    , primitiveRestartIndex(0)
    , arrayBufferBinding(0)
    , vertexArrayBinding(0)
    {
      for (GLuint ii=0; ii<nNamedArrays; ii++)
        named[ii]= NamedVertexArray();

      named[NORMAL].size = 3;
      named[FOG_COORD].size = 1;
      named[SECONDARY_COLOR].size = 3;
      named[INDEX].size = 1;
      named[EDGE_FLAG].size = 1;
      named[EDGE_FLAG].type = GL_BOOL;

      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
        bindings[ii] = VertexBufferBindPoint();

      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
        generic[ii] = GenericVertexArray();
    }

    inline VertexArray &swap(VertexArray &other)
    {
      for (GLuint ii=0; ii<nNamedArrays; ii++)
        named[ii].swap(other.named[ii]);
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
        bindings[ii].swap(other.bindings[ii]);
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
        generic[ii].swap(other.generic[ii]);
      std::swap(elementArrayBufferBinding,other.elementArrayBufferBinding);
      std::swap(clientActiveTexture,other.clientActiveTexture);
      std::swap(primitiveRestartFixedIndex,other.primitiveRestartFixedIndex);
      std::swap(primitiveRestart,other.primitiveRestart);
      std::swap(primitiveRestartIndex,other.primitiveRestartIndex);
      std::swap(arrayBufferBinding,other.arrayBufferBinding);
      std::swap(vertexArrayBinding,other.vertexArrayBinding);
      return *this;
    }

    inline VertexArray &get(DispatchTableGL &dt)
    {
      dt.call(&dt.glGetIntegerv)(GL_VERTEX_ARRAY_BINDING,reinterpret_cast<GLint*>(&vertexArrayBinding));
      if (vertexArrayBinding)
        dt.call(&dt.glBindVertexArray)(0);
      for (GLuint ii=0; ii<nNamedArrays; ii++)
        named[ii].get(dt,static_cast<vaName>(ii));
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
        bindings[ii].get(dt,ii);
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
        generic[ii].get(dt,ii);
      dt.call(&dt.glGetIntegerv)(GL_ELEMENT_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&elementArrayBufferBinding));
      dt.call(&dt.glGetIntegerv)(GL_CLIENT_ACTIVE_TEXTURE,reinterpret_cast<GLint*>(&clientActiveTexture));
      primitiveRestartFixedIndex = dt.call(&dt.glIsEnabled)(GL_PRIMITIVE_RESTART_FIXED_INDEX);
      primitiveRestart = dt.call(&dt.glIsEnabled)(GL_PRIMITIVE_RESTART);
      dt.call(&dt.glGetIntegerv)(GL_PRIMITIVE_RESTART_INDEX,reinterpret_cast<GLint*>(&primitiveRestartIndex));
      dt.call(&dt.glGetIntegerv)(GL_ARRAY_BUFFER_BINDING,reinterpret_cast<GLint*>(&arrayBufferBinding));
      if (vertexArrayBinding)
        dt.call(&dt.glBindVertexArray)(vertexArrayBinding);
      return *this;
    }

    inline const VertexArray &set(DispatchTableGL &dt) const
    {
      dt.call(&dt.glBindVertexArray)(0);
      for (GLuint ii=0; ii<nNamedArrays; ii++)
        named[ii].set(dt,static_cast<vaName>(ii));
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
        bindings[ii].set(dt,ii);
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
        generic[ii].set(dt,ii);
      dt.call(&dt.glBindBuffer)(GL_ELEMENT_ARRAY_BUFFER,elementArrayBufferBinding);
      dt.call(&dt.glClientActiveTexture)(clientActiveTexture);
      enable(dt,GL_PRIMITIVE_RESTART_FIXED_INDEX,primitiveRestartFixedIndex);
      enable(dt,GL_PRIMITIVE_RESTART,primitiveRestart);
      dt.call(&dt.glPrimitiveRestartIndex)(primitiveRestartIndex);
      dt.call(&dt.glBindBuffer)(GL_ARRAY_BUFFER,arrayBufferBinding);
      dt.call(&dt.glBindVertexArray)(vertexArrayBinding);
      return *this;
    }

    inline std::string toString(const char *delim = "\n") const
    {
      string_list tmp;
      for (GLuint ii=0; ii<nNamedArrays; ii++)
        named[ii].toString(static_cast<vaName>(ii),delim);
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
        bindings[ii].toString(ii,delim);
      for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
        generic[ii].toString(ii,delim);

      tmp << print_string("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER",elementArrayBufferBinding,");",delim);
      tmp << print_string("glClientActiveTexture(",clientActiveTexture,");",delim);
      enableToString(tmp,primitiveRestartFixedIndex,"GL_PRIMITIVE_RESTART_FIXED_INDEX",delim);
      enableToString(tmp,primitiveRestart,"GL_PRIMITIVE_RESTART",delim);
      tmp << print_string("glPrimitiveRestartIndex(",primitiveRestartIndex,");",delim);
      tmp << print_string("glBindBuffer(GL_ARRAY_BUFFER,",arrayBufferBinding,");",delim);
      tmp << print_string("glBindVertexArray(",vertexArrayBinding,");",delim);
      return tmp;
    }

    void SetEnablei(GLenum cap, GLuint index, GLboolean enabled)
    {
      UNUSED_PARAMETER(index);
      switch (cap)
      {
        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
          primitiveRestartFixedIndex = enabled;
          break;
        case GL_PRIMITIVE_RESTART:
          primitiveRestart = enabled;
          break;
        default:
          break;
      }
    }

    inline void glEnable(GLenum cap)
    {
      SetEnablei(cap, 0, GL_TRUE);
    }

    inline void glDisable(GLenum cap)
    {
      SetEnablei(cap, 0, GL_FALSE);
    }

    inline void glEnablei(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_TRUE);
    }

    inline void glDisablei(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_FALSE);
    }

    inline void glEnableIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_TRUE);
    }

    inline void glDisableIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnablei(cap, index, GL_FALSE);
    }

    void SetEnableClientStatei(GLuint vao, GLenum cap, GLuint index, GLboolean enabled)
    {
      // only handle these if no VAO is bound

      if (vao)
        return;

      switch (cap)
      {
        case GL_VERTEX_ARRAY:
          named[VERTEX].enabled = enabled;
          break;
        case GL_NORMAL_ARRAY:
          named[NORMAL].enabled = enabled;
          break;
        case GL_FOG_COORD_ARRAY:
          named[FOG_COORD].enabled = enabled;
          break;
        case GL_COLOR_ARRAY:
          named[COLOR].enabled = enabled;
          break;
        case GL_SECONDARY_COLOR_ARRAY:
          named[SECONDARY_COLOR].enabled = enabled;
          break;
        case GL_INDEX_ARRAY:
          named[INDEX].enabled = enabled;
          break;
        case GL_EDGE_FLAG_ARRAY:
          named[EDGE_FLAG].enabled = enabled;
          break;
        case GL_TEXTURE_COORD_ARRAY:
          named[index+7].enabled = enabled;
          break;
        default:
          break;
      }
    }

    inline void glEnableClientState(GLenum cap)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, 0, GL_TRUE);
    }

    inline void glDisableClientState(GLenum cap)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, 0, GL_FALSE);
    }

    inline void glEnableClientStateiEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_TRUE);
    }

    inline void glDisableClientStateiEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_FALSE);
    }

    inline void glEnableClientStateIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_TRUE);
    }

    inline void glDisableClientStateIndexedEXT(GLenum cap, GLuint index)
    {
      SetEnableClientStatei(vertexArrayBinding, cap, index, GL_FALSE);
    }

    void glBindBuffer( GLenum target, GLuint buffer )
    {
      switch (target)
      {
        case GL_ARRAY_BUFFER:
          arrayBufferBinding = buffer;
          break;
        case GL_ELEMENT_ARRAY_BUFFER:
          if (!vertexArrayBinding)
            elementArrayBufferBinding = buffer;
          break;
        default:
          break;
      }
    }

    inline void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayVertexOffsetEXT(vertexArrayBinding, arrayBufferBinding, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayNormalOffsetEXT(vertexArrayBinding, arrayBufferBinding, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayColorOffsetEXT(vertexArrayBinding, arrayBufferBinding, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glSecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArraySecondaryColorOffsetEXT(vertexArrayBinding, arrayBufferBinding, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glIndexPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayIndexOffsetEXT(vertexArrayBinding, arrayBufferBinding, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glEdgeFlagPointer(GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayEdgeFlagOffsetEXT(vertexArrayBinding, arrayBufferBinding, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glFogCoordPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayFogCoordOffsetEXT(vertexArrayBinding, arrayBufferBinding, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glMultiTexCoordPointerEXT(GLenum index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayMultiTexCoordOffsetEXT(vertexArrayBinding, arrayBufferBinding, index, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayMultiTexCoordOffsetEXT(vertexArrayBinding, arrayBufferBinding, clientActiveTexture, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    GLsizei computeEffectiveStride(GLsizei stride, GLint size, GLenum type)
    {
      if (stride != 0)
        return stride;

      // when stride == 0, compute effectiveStride based on size and type

      if (size == GL_BGRA)
        return 4;

      switch (type)
      {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
          break;
        case GL_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
          // size must be 4 for these. Just return that.
          break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_HALF_FLOAT:
          size *= 2;
          break;
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
        case GL_FIXED:
          size *= 4;
          break;
        case GL_DOUBLE:
          size *= 8;
          break;
        default:
          size = 0;
          break;
      }

      return size;
    }

    inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayVertexAttribOffsetEXT(vertexArrayBinding, arrayBufferBinding, index, size, type, normalized, stride, ((char *)pointer - (char *)NULL));
    }

    inline void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
    {
      if (!vertexArrayBinding)
        glVertexArrayVertexAttribIOffsetEXT(vertexArrayBinding, arrayBufferBinding, index, size, type, stride, ((char *)pointer - (char *)NULL));
    }

    void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
    {
      if (!vertexArrayBinding)
      {
        glVertexAttribLFormat(index, size, type, 0);
        glVertexAttribBinding(index, index);
        GLsizei effectiveStride = computeEffectiveStride(stride, size, type);
        glBindVertexBuffer(index, arrayBufferBinding, (char *)pointer - (char *)NULL, effectiveStride);
      }
    }

    void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
    {
      if (!vertexArrayBinding)
      {
        bindings[bindingindex].buffer = buffer;
        bindings[bindingindex].offset = offset;
        bindings[bindingindex].stride = stride;
      }
    }

    inline void glVertexAttribDivisor(GLuint index, GLuint divisor)
    {
      if (!vertexArrayBinding)
      {
        generic[index].bindingIndex = index;
        bindings[index].divisor = divisor;
      }
    }

    inline void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor)
    {
      if (!vertexArrayBinding)
        bindings[bindingindex].divisor = divisor;
    }

    inline void glEnableVertexAttribArray(GLuint index)
    {
      if (!vertexArrayBinding)
        generic[index].enabled = GL_TRUE;
    }

    inline void glDisableVertexAttribArray(GLuint index)
    {
      if (!vertexArrayBinding)
        generic[index].enabled = GL_FALSE;
    }

    inline void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex)
    {
      if (!vertexArrayBinding)
        generic[attribindex].bindingIndex = bindingindex;
    }

    inline void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
    {
      if (!vertexArrayBinding)
      {
        generic[attribindex].size = size;
        generic[attribindex].type = type;
        generic[attribindex].relativeOffset = relativeoffset;
        generic[attribindex].normalized = GL_FALSE;
        generic[attribindex].isInteger = GL_TRUE;
        generic[attribindex].isLong = GL_FALSE;
      }
    }

    inline void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
    {
      if (!vertexArrayBinding)
      {
        generic[attribindex].size = size;
        generic[attribindex].type = type;
        generic[attribindex].relativeOffset = relativeoffset;
        generic[attribindex].normalized = GL_FALSE;
        generic[attribindex].isInteger = GL_FALSE;
        generic[attribindex].isLong = GL_TRUE;
      }
    }

    inline void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
    {
      if (!vertexArrayBinding)
      {
        generic[attribindex].size = size;
        generic[attribindex].type = type;
        generic[attribindex].relativeOffset = relativeoffset;
        generic[attribindex].normalized = normalized;
        generic[attribindex].isInteger = GL_FALSE;
        generic[attribindex].isLong = GL_FALSE;
      }
    }

    inline void glClientActiveTexture(GLenum texture)
    {
      clientActiveTexture = texture;
    }

    inline void glPrimitiveRestartIndex(GLuint index)
    {
      primitiveRestartIndex = index;
    }

    inline void glBindVertexArray(GLuint array)
    {
      vertexArrayBinding = array;
    }

    inline void glEnableVertexArrayEXT(GLuint vaobj, GLenum array)
    {
      if (!vaobj)
        SetEnableClientStatei(vaobj, array, 0, GL_TRUE);
    }

    inline void glDisableVertexArrayEXT(GLuint vaobj, GLenum array)
    {
      if (!vaobj)
        SetEnableClientStatei(vaobj, array, 0, GL_FALSE);
    }

    inline void glEnableVertexArrayAttribEXT(GLuint vaobj, GLuint index)
    {
      if (!vaobj)
        generic[index].enabled = GL_TRUE;
    }

    inline void glDisableVertexArrayAttribEXT(GLuint vaobj, GLuint index)
    {
      if (!vaobj)
        generic[index].enabled = GL_FALSE;
    }

    void glVertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[VERTEX].buffer = buffer;
        named[VERTEX].size = size;
        named[VERTEX].type = type;
        named[VERTEX].stride = stride;
        named[VERTEX].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[NORMAL].buffer = buffer;
        named[NORMAL].type = type;
        named[NORMAL].stride = stride;
        named[NORMAL].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[COLOR].buffer = buffer;
        named[COLOR].size = size;
        named[COLOR].type = type;
        named[COLOR].stride = stride;
        named[COLOR].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[SECONDARY_COLOR].buffer = buffer;
        named[SECONDARY_COLOR].size = size;
        named[SECONDARY_COLOR].type = type;
        named[SECONDARY_COLOR].stride = stride;
        named[SECONDARY_COLOR].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[INDEX].buffer = buffer;
        named[INDEX].type = type;
        named[INDEX].stride = stride;
        named[INDEX].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[EDGE_FLAG].buffer = buffer;
        named[EDGE_FLAG].stride = stride;
        named[EDGE_FLAG].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        named[FOG_COORD].buffer = buffer;
        named[FOG_COORD].type = type;
        named[FOG_COORD].stride = stride;
        named[FOG_COORD].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    void glVertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        GLuint ii = 7 + (index - GL_TEXTURE0);
        named[ii].buffer = buffer;
        named[ii].size = size;
        named[ii].type = type;
        named[ii].stride = stride;
        named[ii].pointer = reinterpret_cast<const GLvoid *>(offset);
      }
    }

    inline void glVertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      glVertexArrayMultiTexCoordOffsetEXT(vaobj, buffer, clientActiveTexture, size, type, stride, offset);
    }

    void glVertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        glVertexAttribFormat(index, size, type, normalized, 0);
        glVertexAttribBinding(index, index);
        GLsizei effectiveStride = computeEffectiveStride(stride, size, type);
        glBindVertexBuffer(index, buffer, offset, effectiveStride);
      }
    }

    void glVertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
    {
      if (!vaobj)
      {
        glVertexAttribIFormat(index, size, type, 0);
        glVertexAttribBinding(index, index);
        GLsizei effectiveStride = computeEffectiveStride(stride, size, type);
        glBindVertexBuffer(index, buffer, offset, effectiveStride);
      }
    }

    void glInterleavedArrays(GLenum format, GLsizei stride, const GLvoid *pointer)
    {
      if (vertexArrayBinding)
        return;

      // from glspec43.compatibility.20130214.withchanges.pdf
      // p. 365, section 10.5.1 "Interleaved Arrays"
      //
      // set et; ec; en; st; sc; sv; tc; pc; pn; pv; and s as a function
      // of table 10.6 and the value of format.

      GLboolean et, ec, en;
      GLenum    tc;
      GLsizei   st, sc, sv;
      GLsizei   pc, pn, pv;
      GLsizei   s;

      const int f = sizeof(GL_FLOAT);
      const int c = 4 * sizeof(GL_UNSIGNED_BYTE);

      switch(format)
      {
        case GL_V2F:
          et = GL_FALSE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 0;
          sc = 0;
          sv = 2;
          pc = 0;
          pn = 0;
          pv = 0;
          pv = 0;
          s  = 2 * f;
          break;
        case GL_V3F:
          et = GL_FALSE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 0;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 0;
          s  = 3 * f;
          break;
        case GL_C4UB_V2F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_UNSIGNED_BYTE;
          st = 0;
          sc = 4;
          sv = 2;
          pc = 0;
          pn = 0;
          pv = c;
          s  = c + 2 * f;
          break;
        case GL_C4UB_V3F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_UNSIGNED_BYTE;
          st = 0;
          sc = 4;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = c;
          s  = c + 3 * f;
          break;
        case GL_C3F_V3F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 0;
          sc = 3;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 3 * f;
          s  = 6 * f;
          break;
        case GL_N3F_V3F:
          et = GL_FALSE;
          ec = GL_FALSE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 0;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 3 * f;
          s  = 6 * f;
          break;
        case GL_C4F_N3F_V3F:
          et = GL_FALSE;
          ec = GL_TRUE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 0;
          sc = 4;
          sv = 3;
          pc = 0;
          pn = 4 * f;
          pv = 7 * f;
          s  = 10 * f;
          break;
        case GL_T2F_V3F:
          et = GL_TRUE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 2;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 0;
          pv = 2 * f;
          s  = 5 * f;
          break;
        case GL_T4F_V4F:
          et = GL_TRUE;
          ec = GL_FALSE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 4;
          sc = 0;
          sv = 4;
          pc = 0;
          pn = 0;
          pv = 4 * f;
          s  = 8 * f;
          break;
        case GL_T2F_C4UB_V3F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_UNSIGNED_BYTE;
          st = 2;
          sc = 4;
          sv = 3;
          pc = 2 * f;
          pn = 0;
          pv = c + 2 * f;
          s  = c + 5 * f;
          break;
        case GL_T2F_C3F_V3F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_FALSE;
          tc = GL_FLOAT;
          st = 2;
          sc = 3;
          sv = 3;
          pc = 2 * f;
          pn = 0;
          pv = 5 * f;
          s  = 8 * f;
          break;
        case GL_T2F_N3F_V3F:
          et = GL_TRUE;
          ec = GL_FALSE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 2;
          sc = 0;
          sv = 3;
          pc = 0;
          pn = 2 * f;
          pv = 5 * f;
          s  = 8 * f;
          break;
        case GL_T2F_C4F_N3F_V3F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 2;
          sc = 4;
          sv = 3;
          pc = 2 * f;
          pn = 6 * f;
          pv = 9 * f;
          s  = 12 * f;
          break;
        case GL_T4F_C4F_N3F_V4F:
          et = GL_TRUE;
          ec = GL_TRUE;
          en = GL_TRUE;
          tc = GL_FLOAT;
          st = 4;
          sc = 4;
          sv = 4;
          pc = 4 * f;
          pn = 8 * f;
          pv = 11 * f;
          s  = 15 * f;
          break;
        default:
          RegalAssert( !"unhandled format value" );
          return;
      }

      if ( stride == 0 )
        stride = s;

      glDisableClientState(GL_EDGE_FLAG_ARRAY);
      glDisableClientState(GL_INDEX_ARRAY);
      glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
      glDisableClientState(GL_FOG_COORD_ARRAY);

      GLubyte *p = static_cast<GLubyte *>( const_cast<GLvoid *>(pointer) );

      if (et)
      {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(st, GL_FLOAT, stride, p);
      }
      else
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

      if (ec)
      {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(sc, tc, stride, p + pc);
      }
      else
        glDisableClientState(GL_COLOR_ARRAY);

      if (en)
      {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, stride, p + pn);
      }
      else
        glDisableClientState(GL_NORMAL_ARRAY);

      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(sv, GL_FLOAT, stride, p + pv);
    }
  };

  struct PixelStore
  {
    GLboolean  unpackSwapBytes;           // GL_UNPACK_SWAP_BYTES
    GLboolean  unpackLsbFirst;            // GL_UNPACK_LSB_FIRST
    GLint      unpackImageHeight;         // GL_UNPACK_IMAGE_HEIGHT
    GLint      unpackSkipImages;          // GL_UNPACK_SKIP_IMAGES
    GLint      unpackRowLength;           // GL_UNPACK_ROW_LENGTH
    GLint      unpackSkipRows;            // GL_UNPACK_SKIP_ROWS
    GLint      unpackSkipPixels;          // GL_UNPACK_SKIP_PIXELS
    GLint      unpackAlignment;           // GL_UNPACK_ALIGNMENT
    GLboolean  packSwapBytes;             // GL_PACK_SWAP_BYTES
    GLboolean  packLsbFirst;              // GL_PACK_LSB_FIRST
    GLint      packImageHeight;           // GL_PACK_IMAGE_HEIGHT
    GLint      packSkipImages;            // GL_PACK_SKIP_IMAGES
    GLint      packRowLength;             // GL_PACK_ROW_LENGTH
    GLint      packSkipRows;              // GL_PACK_SKIP_ROWS
    GLint      packSkipPixels;            // GL_PACK_SKIP_PIXELS
    GLint      packAlignment;             // GL_PACK_ALIGNMENT
    GLint      pixelUnpackBufferBinding;  // GL_PIXEL_UNPACK_BUFFER_BINDING
    GLint      pixelPackBufferBinding;    // GL_PIXEL_PACK_BUFFER_BINDING

    inline PixelStore()
      : unpackSwapBytes(GL_FALSE)
      , unpackLsbFirst(GL_FALSE)
      , unpackImageHeight(0)
      , unpackSkipImages(0)
      , unpackRowLength(0)
      , unpackSkipRows(0)
      , unpackSkipPixels(0)
      , unpackAlignment(4)
      , packSwapBytes(GL_FALSE)
      , packLsbFirst(GL_FALSE)
      , packImageHeight(0)
      , packSkipImages(0)
      , packRowLength(0)
      , packSkipRows(0)
      , packSkipPixels(0)
      , packAlignment(4)
      , pixelUnpackBufferBinding(0)
      , pixelPackBufferBinding(0)
    {
    }

    inline PixelStore &swap(PixelStore &other)
    {
      std::swap(unpackSwapBytes,other.unpackSwapBytes);
      std::swap(unpackLsbFirst,other.unpackLsbFirst);
      std::swap(unpackImageHeight,other.unpackImageHeight);
      std::swap(unpackSkipImages,other.unpackSkipImages);
      std::swap(unpackRowLength,other.unpackRowLength);
      std::swap(unpackSkipRows,other.unpackSkipRows);
      std::swap(unpackSkipPixels,other.unpackSkipPixels);
      std::swap(unpackAlignment,other.unpackAlignment);
      std::swap(packSwapBytes,other.packSwapBytes);
      std::swap(packLsbFirst,other.packLsbFirst);
      std::swap(packImageHeight,other.packImageHeight);
      std::swap(packSkipImages,other.packSkipImages);
      std::swap(packRowLength,other.packRowLength);
      std::swap(packSkipRows,other.packSkipRows);
      std::swap(packSkipPixels,other.packSkipPixels);
      std::swap(packAlignment,other.packAlignment);
      std::swap(pixelUnpackBufferBinding,other.pixelUnpackBufferBinding);
      std::swap(pixelPackBufferBinding,other.pixelPackBufferBinding);
      return *this;
    }

    inline PixelStore &get(DispatchTableGL &dt)
    {
      dt.call(&dt.glGetBooleanv)(GL_UNPACK_SWAP_BYTES,&unpackSwapBytes);
      dt.call(&dt.glGetBooleanv)(GL_UNPACK_LSB_FIRST,&unpackLsbFirst);
      dt.call(&dt.glGetIntegerv)(GL_UNPACK_IMAGE_HEIGHT,&unpackImageHeight);
      dt.call(&dt.glGetIntegerv)(GL_UNPACK_SKIP_IMAGES,&unpackSkipImages);
      dt.call(&dt.glGetIntegerv)(GL_UNPACK_ROW_LENGTH,&unpackRowLength);
      dt.call(&dt.glGetIntegerv)(GL_UNPACK_SKIP_ROWS,&unpackSkipRows);
      dt.call(&dt.glGetIntegerv)(GL_UNPACK_SKIP_PIXELS,&unpackSkipPixels);
      dt.call(&dt.glGetIntegerv)(GL_UNPACK_ALIGNMENT,&unpackAlignment);
      dt.call(&dt.glGetBooleanv)(GL_PACK_SWAP_BYTES,&packSwapBytes);
      dt.call(&dt.glGetBooleanv)(GL_PACK_LSB_FIRST,&packLsbFirst);
      dt.call(&dt.glGetIntegerv)(GL_PACK_IMAGE_HEIGHT,&packImageHeight);
      dt.call(&dt.glGetIntegerv)(GL_PACK_SKIP_IMAGES,&packSkipImages);
      dt.call(&dt.glGetIntegerv)(GL_PACK_ROW_LENGTH,&packRowLength);
      dt.call(&dt.glGetIntegerv)(GL_PACK_SKIP_ROWS,&packSkipRows);
      dt.call(&dt.glGetIntegerv)(GL_PACK_SKIP_PIXELS,&packSkipPixels);
      dt.call(&dt.glGetIntegerv)(GL_PACK_ALIGNMENT,&packAlignment);
      dt.call(&dt.glGetIntegerv)(GL_PIXEL_UNPACK_BUFFER_BINDING,&pixelUnpackBufferBinding);
      dt.call(&dt.glGetIntegerv)(GL_PIXEL_PACK_BUFFER_BINDING,&pixelPackBufferBinding);
      return *this;
    }

    inline const PixelStore &set(DispatchTableGL &dt) const
    {
      dt.call(&dt.glPixelStorei)(GL_UNPACK_SWAP_BYTES,unpackSwapBytes);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_LSB_FIRST,unpackLsbFirst);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_IMAGE_HEIGHT,unpackImageHeight);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_SKIP_IMAGES,unpackSkipImages);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_ROW_LENGTH,unpackRowLength);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_SKIP_ROWS,unpackSkipRows);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_SKIP_PIXELS,unpackSkipPixels);
      dt.call(&dt.glPixelStorei)(GL_UNPACK_ALIGNMENT,unpackAlignment);
      dt.call(&dt.glPixelStorei)(GL_PACK_SWAP_BYTES,packSwapBytes);
      dt.call(&dt.glPixelStorei)(GL_PACK_LSB_FIRST,packLsbFirst);
      dt.call(&dt.glPixelStorei)(GL_PACK_IMAGE_HEIGHT,packImageHeight);
      dt.call(&dt.glPixelStorei)(GL_PACK_SKIP_IMAGES,packSkipImages);
      dt.call(&dt.glPixelStorei)(GL_PACK_ROW_LENGTH,packRowLength);
      dt.call(&dt.glPixelStorei)(GL_PACK_SKIP_ROWS,packSkipRows);
      dt.call(&dt.glPixelStorei)(GL_PACK_SKIP_PIXELS,packSkipPixels);
      dt.call(&dt.glPixelStorei)(GL_PACK_ALIGNMENT,packAlignment);
      dt.call(&dt.glBindBuffer)(GL_PIXEL_UNPACK_BUFFER_BINDING,pixelUnpackBufferBinding);
      dt.call(&dt.glBindBuffer)(GL_PIXEL_PACK_BUFFER_BINDING,pixelPackBufferBinding);
      return *this;
    }

    inline std::string toString(const char *delim = "\n") const
    {
      string_list tmp;
      tmp << print_string("glPixelStorei(GL_UNPACK_SWAP_BYTES,",unpackSwapBytes,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_LSB_FIRST,",unpackLsbFirst,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,",unpackImageHeight,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_SKIP_IMAGES,",unpackSkipImages,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_ROW_LENGTH,",unpackRowLength,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_SKIP_ROWS,",unpackSkipRows,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_SKIP_PIXELS,",unpackSkipPixels,");",delim);
      tmp << print_string("glPixelStorei(GL_UNPACK_ALIGNMENT,",unpackAlignment,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SWAP_BYTES,",packSwapBytes,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_LSB_FIRST,",packLsbFirst,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_IMAGE_HEIGHT,",packImageHeight,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SKIP_IMAGES,",packSkipImages,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_ROW_LENGTH,",packRowLength,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SKIP_ROWS,",packSkipRows,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_SKIP_PIXELS,",packSkipPixels,");",delim);
      tmp << print_string("glPixelStorei(GL_PACK_ALIGNMENT,",packAlignment,");",delim);
      tmp << print_string("glBindBuffer(GL_PIXEL_UNPACK_BUFFER_BINDING,",pixelUnpackBufferBinding,");",delim);
      tmp << print_string("glBindBuffer(GL_PIXEL_PACK_BUFFER_BINDING,",pixelPackBufferBinding,");",delim);
      return tmp;
    }

    template <typename T> void glPixelStore( GLenum pname, T param )
    {
      switch (pname)
      {
        case GL_UNPACK_SWAP_BYTES:   unpackSwapBytes   = param; break;
        case GL_UNPACK_LSB_FIRST:    unpackLsbFirst    = param; break;
        case GL_UNPACK_IMAGE_HEIGHT: unpackImageHeight = param; break;
        case GL_UNPACK_SKIP_IMAGES:  unpackSkipImages  = param; break;
        case GL_UNPACK_ROW_LENGTH:   unpackRowLength   = param; break;
        case GL_UNPACK_SKIP_ROWS:    unpackSkipRows    = param; break;
        case GL_UNPACK_SKIP_PIXELS:  unpackSkipPixels  = param; break;
        case GL_UNPACK_ALIGNMENT:    unpackAlignment   = param; break;
        case GL_PACK_SWAP_BYTES:     packSwapBytes     = param; break;
        case GL_PACK_LSB_FIRST:      packLsbFirst      = param; break;
        case GL_PACK_IMAGE_HEIGHT:   packImageHeight   = param; break;
        case GL_PACK_SKIP_IMAGES:    packSkipImages    = param; break;
        case GL_PACK_ROW_LENGTH:     packRowLength     = param; break;
        case GL_PACK_SKIP_ROWS:      packSkipRows      = param; break;
        case GL_PACK_SKIP_PIXELS:    packSkipPixels    = param; break;
        case GL_PACK_ALIGNMENT:      packAlignment     = param; break;
        default:
          break;
      }
    }

    void glBindBuffer( GLenum target, GLuint buffer )
    {
      switch (target)
      {
        case GL_PIXEL_UNPACK_BUFFER_BINDING:
          pixelUnpackBufferBinding = buffer;
          break;
        case GL_PIXEL_PACK_BUFFER_BINDING:
          pixelPackBufferBinding = buffer;
          break;
        default:
          break;
      }
    }
  };

}

}

REGAL_NAMESPACE_END

#endif

#endif
