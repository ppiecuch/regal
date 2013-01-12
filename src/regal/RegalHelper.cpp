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

REGAL_GLOBAL_BEGIN

#include "RegalHelper.h"

using std::size_t;

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace helper { namespace size {

size_t
get(GLenum pname)
{
  size_t size = 0;

  switch (pname)
  {
    // ES 1.1
    // http://www.khronos.org/opengles/sdk/1.1/docs/man/glGet.xml

    case GL_ACTIVE_TEXTURE:                       size = 1;  break;
    case GL_ALIASED_POINT_SIZE_RANGE:             size = 2;  break;
    case GL_ALIASED_LINE_WIDTH_RANGE:             size = 2;  break;
    case GL_ALPHA_BITS:                           size = 1;  break;
    case GL_ALPHA_TEST:                           size = 1;  break;
    case GL_ALPHA_TEST_FUNC:                      size = 1;  break;
    case GL_ALPHA_TEST_REF:                       size = 1;  break;
    case GL_ARRAY_BUFFER_BINDING:                 size = 1;  break;
    case GL_BLEND:                                size = 1;  break;
    case GL_BLEND_DST:                            size = 1;  break;
    case GL_BLEND_SRC:                            size = 1;  break;
    case GL_BLUE_BITS:                            size = 1;  break;
    case GL_CLIENT_ACTIVE_TEXTURE:                size = 1;  break;
    case GL_CLIP_PLANE0:                          size = 1;  break;
    case GL_CLIP_PLANE1:                          size = 1;  break;
    case GL_CLIP_PLANE2:                          size = 1;  break;
    case GL_CLIP_PLANE3:                          size = 1;  break;
    case GL_CLIP_PLANE4:                          size = 1;  break;
    case GL_CLIP_PLANE5:                          size = 1;  break;
//    case GL_CLIP_PLANE6:                          size = 1;  break;
//    case GL_CLIP_PLANE7:                          size = 1;  break;
    case GL_COLOR_ARRAY:                          size = 1;  break;
    case GL_COLOR_ARRAY_BUFFER_BINDING:           size = 1;  break;
    case GL_COLOR_ARRAY_SIZE:                     size = 1;  break;
    case GL_COLOR_ARRAY_STRIDE:                   size = 1;  break;
    case GL_COLOR_ARRAY_TYPE:                     size = 1;  break;
    case GL_COLOR_CLEAR_VALUE:                    size = 4;  break;
    case GL_COLOR_LOGIC_OP:                       size = 1;  break;
    case GL_COLOR_MATERIAL:                       size = 1;  break;
    case GL_COLOR_WRITEMASK:                      size = 4;  break;
    case GL_COMPRESSED_TEXTURE_FORMATS:           size = 1;  break;
    case GL_CULL_FACE:                            size = 1;  break;
    case GL_CULL_FACE_MODE:                       size = 1;  break;
    case GL_CURRENT_COLOR:                        size = 4;  break;
    case GL_CURRENT_NORMAL:                       size = 3;  break;
    case GL_CURRENT_TEXTURE_COORDS:               size = 4;  break;
    case GL_DEPTH_BITS:                           size = 1;  break;
    case GL_DEPTH_CLEAR_VALUE:                    size = 1;  break;
    case GL_DEPTH_FUNC:                           size = 1;  break;
    case GL_DEPTH_RANGE:                          size = 2;  break;
    case GL_DEPTH_TEST:                           size = 1;  break;
    case GL_DEPTH_WRITEMASK:                      size = 1;  break;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:         size = 1;  break;
    case GL_FOG:                                  size = 1;  break;
    case GL_FOG_COLOR:                            size = 4;  break;
    case GL_FOG_DENSITY:                          size = 1;  break;
    case GL_FOG_END:                              size = 1;  break;
    case GL_FOG_HINT:                             size = 1;  break;
    case GL_FOG_MODE:                             size = 1;  break;
    case GL_FOG_START:                            size = 1;  break;
    case GL_FRONT_FACE:                           size = 1;  break;
    case GL_GREEN_BITS:                           size = 1;  break;
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES: size = 1;  break;
    case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:   size = 1;  break;
    case GL_LIGHT_MODEL_AMBIENT:                  size = 4;  break;
    case GL_LIGHT_MODEL_TWO_SIDE:                 size = 1;  break;
    case GL_LIGHT0:                               size = 1;  break;
    case GL_LIGHT1:                               size = 1;  break;
    case GL_LIGHT2:                               size = 1;  break;
    case GL_LIGHT3:                               size = 1;  break;
    case GL_LIGHT4:                               size = 1;  break;
    case GL_LIGHT5:                               size = 1;  break;
    case GL_LIGHT6:                               size = 1;  break;
    case GL_LIGHT7:                               size = 1;  break;
//    case GL_LIGHT8:                               size = 1;  break;
//    case GL_LIGHT9:                               size = 1;  break;
    case GL_LIGHTING:                             size = 1;  break;
    case GL_LINE_SMOOTH:                          size = 1;  break;
    case GL_LINE_WIDTH:                           size = 1;  break;
    case GL_LOGIC_OP_MODE:                        size = 1;  break;
    case GL_MATRIX_MODE:                          size = 1;  break;
    case GL_MAX_CLIP_PLANES:                      size = 1;  break;
    case GL_MAX_LIGHTS:                           size = 1;  break;
    case GL_MAX_MODELVIEW_STACK_DEPTH:            size = 1;  break;
    case GL_MAX_PROJECTION_STACK_DEPTH:           size = 1;  break;
    case GL_MAX_TEXTURE_SIZE:                     size = 1;  break;
    case GL_MAX_TEXTURE_STACK_DEPTH:              size = 1;  break;
    case GL_MAX_TEXTURE_UNITS:                    size = 1;  break;
    case GL_MAX_VIEWPORT_DIMS:                    size = 2;  break;
    case GL_MODELVIEW_MATRIX:                     size = 16; break;
    case GL_MODELVIEW_STACK_DEPTH:                size = 1;  break;
    case GL_MULTISAMPLE:                          size = 1;  break;
    case GL_NORMAL_ARRAY:                         size = 1;  break;
    case GL_NORMAL_ARRAY_BUFFER_BINDING:          size = 1;  break;
    case GL_NORMAL_ARRAY_STRIDE:                  size = 1;  break;
    case GL_NORMAL_ARRAY_TYPE:                    size = 1;  break;
    case GL_NORMALIZE:                            size = 1;  break;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:       size = 1;  break;
    case GL_PACK_ALIGNMENT:                       size = 1;  break;
    case GL_PERSPECTIVE_CORRECTION_HINT:          size = 1;  break;
    case GL_POINT_DISTANCE_ATTENUATION:           size = 3;  break;
    case GL_POINT_FADE_THRESHOLD_SIZE:            size = 1;  break;
    case GL_POINT_SIZE:                           size = 1;  break;
//    case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:  size = 1;  break;
//    case GL_POINT_SIZE_ARRAY_OES:                 size = 1;  break;
//    case GL_POINT_SIZE_ARRAY_STRIDE_OES:          size = 1;  break;
//    case GL_POINT_SIZE_ARRAY_TYPE_OES:            size = 1;  break;
    case GL_POINT_SIZE_MAX:                       size = 1;  break;
    case GL_POINT_SIZE_MIN:                       size = 1;  break;
    case GL_POINT_SMOOTH:                         size = 1;  break;
    case GL_POINT_SMOOTH_HINT:                    size = 1;  break;
//    case GL_POINT_SPRITE_OES:                     size = 1;  break;
    case GL_POLYGON_OFFSET_FACTOR:                size = 1;  break;
    case GL_POLYGON_OFFSET_FILL:                  size = 1;  break;
    case GL_POLYGON_OFFSET_UNITS:                 size = 1;  break;
    case GL_PROJECTION_MATRIX:                    size = 16; break;
    case GL_PROJECTION_STACK_DEPTH:               size = 1;  break;
    case GL_RED_BITS:                             size = 1;  break;
    case GL_RESCALE_NORMAL:                       size = 1;  break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:             size = 1;  break;
    case GL_SAMPLE_ALPHA_TO_ONE:                  size = 1;  break;
    case GL_SAMPLE_BUFFERS:                       size = 1;  break;
    case GL_SAMPLE_COVERAGE:                      size = 1;  break;
    case GL_SAMPLE_COVERAGE_INVERT:               size = 1;  break;
    case GL_SAMPLE_COVERAGE_VALUE:                size = 1;  break;
    case GL_SAMPLES:                              size = 1;  break;
    case GL_SCISSOR_BOX:                          size = 4;  break;
    case GL_SCISSOR_TEST:                         size = 1;  break;
    case GL_SHADE_MODEL:                          size = 1;  break;
    case GL_SMOOTH_LINE_WIDTH_RANGE:              size = 2;  break;
    case GL_SMOOTH_POINT_SIZE_RANGE:              size = 2;  break;
    case GL_STENCIL_BITS:                         size = 1;  break;
    case GL_STENCIL_CLEAR_VALUE:                  size = 1;  break;
    case GL_STENCIL_FAIL:                         size = 1;  break;
    case GL_STENCIL_FUNC:                         size = 1;  break;
    case GL_STENCIL_PASS_DEPTH_FAIL:              size = 1;  break;
    case GL_STENCIL_PASS_DEPTH_PASS:              size = 1;  break;
    case GL_STENCIL_REF:                          size = 1;  break;
    case GL_STENCIL_TEST:                         size = 1;  break;
    case GL_STENCIL_VALUE_MASK:                   size = 1;  break;
    case GL_STENCIL_WRITEMASK:                    size = 1;  break;
    case GL_SUBPIXEL_BITS:                        size = 1;  break;
    case GL_TEXTURE_2D:                           size = 1;  break;
    case GL_TEXTURE_BINDING_2D:                   size = 1;  break;
    case GL_TEXTURE_COORD_ARRAY:                  size = 1;  break;
    case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:   size = 1;  break;
    case GL_TEXTURE_COORD_ARRAY_SIZE:             size = 1;  break;
    case GL_TEXTURE_COORD_ARRAY_STRIDE:           size = 1;  break;
    case GL_TEXTURE_COORD_ARRAY_TYPE:             size = 1;  break;
    case GL_TEXTURE_MATRIX:                       size = 16; break;
    case GL_TEXTURE_STACK_DEPTH:                  size = 1;  break;
    case GL_UNPACK_ALIGNMENT:                     size = 1;  break;
    case GL_VIEWPORT:                             size = 4;  break;
    case GL_VERTEX_ARRAY:                         size = 1;  break;
    case GL_VERTEX_ARRAY_BUFFER_BINDING:          size = 1;  break;
    case GL_VERTEX_ARRAY_SIZE:                    size = 1;  break;
    case GL_VERTEX_ARRAY_STRIDE:                  size = 1;  break;
    case GL_VERTEX_ARRAY_TYPE:                    size = 1;  break;

    default: break;
  }

  return size;
}

size_t
callLists(GLsizei n, GLenum type)
{
  size_t size = 0;

  switch (type)
  {
    case GL_BYTE:           size =     sizeof(GLbyte);   break;
    case GL_UNSIGNED_BYTE:  size =     sizeof(GLubyte);  break;
    case GL_SHORT:          size =     sizeof(GLshort);  break;
    case GL_UNSIGNED_SHORT: size =     sizeof(GLushort); break;
    case GL_INT:            size =     sizeof(GLint);    break;
    case GL_UNSIGNED_INT:   size =     sizeof(GLuint);   break;
    case GL_FLOAT:          size =     sizeof(GLfloat);  break;
    case GL_2_BYTES:        size = 2 * sizeof(GLubyte);  break;
    case GL_3_BYTES:        size = 3 * sizeof(GLubyte);  break;
    case GL_4_BYTES:        size = 4 * sizeof(GLubyte);  break;

    default: break;
  }

  size *= n > 0 ? (size_t)(n) : 0;

  return size;
}

size_t
fogv(GLenum pname)
{
  switch (pname)
  {
    case GL_FOG_MODE:             return 1;
    case GL_FOG_DENSITY:          return 1;
    case GL_FOG_START:            return 1;
    case GL_FOG_END:              return 1;
    case GL_FOG_INDEX:            return 1;
    case GL_FOG_COLOR:            return 4;
    case GL_FOG_COORD_SRC:        return 1;

    // GL_NV_fog_distance

    case GL_FOG_DISTANCE_MODE_NV: return 1;

    default:                      return 0;
  }
}

size_t
lightv(GLenum pname)
{
  switch (pname)
  {
    case GL_AMBIENT:               return 4;
    case GL_DIFFUSE:               return 4;
    case GL_SPECULAR:              return 4;
    case GL_POSITION:              return 4;
    case GL_SPOT_CUTOFF:           return 1;
    case GL_SPOT_DIRECTION:        return 3;
    case GL_SPOT_EXPONENT:         return 1;
    case GL_CONSTANT_ATTENUATION:  return 1;
    case GL_LINEAR_ATTENUATION:    return 1;
    case GL_QUADRATIC_ATTENUATION: return 1;
    default:                       return 0;
  }
}

size_t
lightModelv(GLenum pname)
{
  switch (pname)
  {
    case GL_LIGHT_MODEL_AMBIENT:       return 4;
    case GL_LIGHT_MODEL_COLOR_CONTROL: return 1;
    case GL_LIGHT_MODEL_LOCAL_VIEWER:  return 1;
    case GL_LIGHT_MODEL_TWO_SIDE:      return 1;
    default:                           return 0;
  }
}

size_t
materialv(GLenum pname)
{
  switch (pname)
  {
    case GL_AMBIENT:             return 4;
    case GL_DIFFUSE:             return 4;
    case GL_SPECULAR:            return 4;
    case GL_EMISSION:            return 4;
    case GL_SHININESS:           return 1;
    case GL_AMBIENT_AND_DIFFUSE: return 4;
    case GL_COLOR_INDEXES:       return 3;
    default:                     return 0;
  }
}

size_t
pixelImage(GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint pack, GLenum target)
{
  /* TODO: Review later: Does texture "level" affect size? */
  /* TODO: Review later: Properly account for effects of glPixelStore. */

  size_t numComponent = componentsPerPixel(format);
  size_t sizeElement  = 8; /* bits */

  size_t dataWidth  = 0;
  size_t dataHeight = 0;
  size_t dataDepth  = 0;

//  if (!traceGL_Native.glGetIntegerv)
//    return 0;

  if (width < 0 || height < 0 || depth < 0)
    return 0;

  if (pack != 0 && pack != 1)
    return 0;

  /* When target is a GL_PROXY_TEXUTRE_xx, size is 0. */

  if ((target == GL_PROXY_TEXTURE_1D) ||
      (target == GL_PROXY_TEXTURE_2D) ||
      (target == GL_PROXY_TEXTURE_3D))
    return 0;

  /* TODO: If an unpack buffer is bound for glDrawPixels... */
  if (!pack)
  {
#if 0
    GLint bufferBinding = 0;
    traceGL_Native.glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &bufferBinding);
    if (bufferBinding > 0)
      return 1;
#endif
  }
  else
  {
#if 0
    /* TODO: Data is treated as byte offset when a non-zero named buffer is bound. */
    GLint bufferBinding = 0;
    traceGL_Native.glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &bufferBinding);
    if (bufferBinding > 0)
      return 1;
#endif
  }

  /* Compute size of each element. */

  sizeElement *= bytesPerComponent(type);

  switch (type)
  {
    case GL_UNSIGNED_BYTE:      
    case GL_BYTE:               
    case GL_BITMAP:               
    case GL_UNSIGNED_SHORT:     
    case GL_SHORT:             
    case GL_UNSIGNED_INT:     
    case GL_INT:              
    case GL_FLOAT:           sizeElement *= numComponent; break;
    default:                                              break;
  }

  /* Special case: GL_BITMAP. */

  if (type == GL_BITMAP)
  {
     /* TODO: Confirm calculations. */
    if ((format == GL_COLOR_INDEX) || (format == GL_STENCIL_INDEX))
      sizeElement = sizeElement/8;
    else
      return 0;
  }

  /* Take into account client state set by glPixelStore. */

  if (depth)
  {
#if 0
    GLint skipImages = 0;

    /* GL_PACK_SKIP_IMAGES */
    if (pack)
      traceGL_Native.glGetIntegerv(GL_PACK_SKIP_IMAGES, &skipImages);
    else
      traceGL_Native.glGetIntegerv(GL_UNPACK_SKIP_IMAGES, &skipImages);
    dataDepth += skipImages > 0 ? skipImages : 0;
#endif
  }
  else
  {
    dataDepth = 1;
  }

  if (height)
  {
    GLint imageHeight = 0;
    GLint skipRows    = 0;
    GLint rowLength   = 0;
    GLint alignment   = 1; /* Allowed values: 1,2,4,8 */

#if 0
    /* GL_PACK_IMAGE_HEIGHT */
    if (pack)
      traceGL_Native.glGetIntegerv(GL_PACK_IMAGE_HEIGHT, &imageHeight);
    else
      traceGL_Native.glGetIntegerv(GL_UNPACK_IMAGE_HEIGHT, &imageHeight);
#endif
    dataHeight = imageHeight > 0 ? imageHeight : height;

#if 0
    /* GL_PACK_SKIP_ROWS */
    if (pack)
      traceGL_Native.glGetIntegerv(GL_PACK_SKIP_ROWS, &skipRows);
    else
      traceGL_Native.glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);
#endif
    dataHeight += skipRows > 0 ? skipRows : 0;

#if 0
    /* GL_PACK_ROW_LENGTH */
    if (pack)
      traceGL_Native.glGetIntegerv(GL_PACK_ROW_LENGTH, &rowLength);
    else
      traceGL_Native.glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
#endif
    dataWidth = rowLength > 0 ? rowLength : width;

    /* Align byte boundary */
    dataWidth = (dataWidth * sizeElement + 7)/8;

#if 0
    /* GL_PACK_ALIGNMENT */
    if (pack)
      traceGL_Native.glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);
    else
      traceGL_Native.glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
#endif
    dataWidth = alignment > 0 ? alignment * (dataWidth + alignment - 1)/alignment : dataWidth;
  }
  else
  {
    dataHeight = 1;

    /* Align byte boundary */
    dataWidth = (width * sizeElement + 7)/8;
  }

  return dataWidth * dataHeight * dataDepth;
}

/* TODO: Extension enums. */

size_t
texParameterv(GLenum pname)
{
  switch (pname)
  {
    /* Dec 2012 - http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml */

    case GL_DEPTH_STENCIL_TEXTURE_MODE: return 1;
    case GL_TEXTURE_BASE_LEVEL:         return 1;
    case GL_TEXTURE_BORDER_COLOR:       return 4;
    case GL_TEXTURE_COMPARE_FUNC:       return 1;
    case GL_TEXTURE_COMPARE_MODE:       return 1;
    case GL_TEXTURE_LOD_BIAS:           return 1;
    case GL_TEXTURE_MIN_FILTER:         return 1;
    case GL_TEXTURE_MAG_FILTER:         return 1;
    case GL_TEXTURE_MIN_LOD:            return 1;
    case GL_TEXTURE_MAX_LOD:            return 1;
    case GL_TEXTURE_MAX_LEVEL:          return 1;
    case GL_TEXTURE_SWIZZLE_R:          return 1;
    case GL_TEXTURE_SWIZZLE_G:          return 1;
    case GL_TEXTURE_SWIZZLE_B:          return 1;
    case GL_TEXTURE_SWIZZLE_A:          return 1;
    case GL_TEXTURE_SWIZZLE_RGBA:       return 4;
    case GL_TEXTURE_WRAP_S:             return 1;
    case GL_TEXTURE_WRAP_T:             return 1;
    case GL_TEXTURE_WRAP_R:             return 1;

    /* others */

    case GL_TEXTURE_PRIORITY:               return 1;
    case GL_DEPTH_TEXTURE_MODE:             return 1;
    case GL_GENERATE_MIPMAP:                return 1;

    // GL_EXT_texture_filter_anisotropic

    case GL_TEXTURE_MAX_ANISOTROPY_EXT:     return 1;

    // GL_SGIX_shadow

    case GL_TEXTURE_COMPARE_SGIX:           return 1;
    case GL_TEXTURE_COMPARE_OPERATOR_SGIX:  return 1;

    // GL_NV_texture_expand_normal

    case GL_TEXTURE_UNSIGNED_REMAP_MODE_NV: return 1;

    // GL_NV_texture_shader

    case GL_TEXTURE_BORDER_VALUES_NV:       return 4;

    default:                                return 0;
  }
}

size_t
texEnvv(GLenum target, GLenum pname)
{
  switch (target)
  {
    case GL_TEXTURE_ENV:
      switch (pname)
      {
        case GL_TEXTURE_ENV_MODE:  return 1;
        case GL_TEXTURE_ENV_COLOR: return 4;
        case GL_COMBINE_RGB:       return 1;
        case GL_COMBINE_ALPHA:     return 1;
        case GL_RGB_SCALE:         return 1;
        case GL_ALPHA_SCALE:       return 1;
        case GL_SRC0_RGB:
        case GL_SRC1_RGB:
        case GL_SRC2_RGB:          return 1;
        case GL_SRC0_ALPHA:
        case GL_SRC1_ALPHA:
        case GL_SRC2_ALPHA:        return 1;
        case GL_OPERAND0_RGB:
        case GL_OPERAND1_RGB:
        case GL_OPERAND2_RGB:      return 1;
        case GL_OPERAND0_ALPHA:
        case GL_OPERAND1_ALPHA:
        case GL_OPERAND2_ALPHA:    return 1;
        default:                   return 0;
      }

    case GL_TEXTURE_FILTER_CONTROL:
      switch (pname)
      {
        case GL_TEXTURE_LOD_BIAS:  return 1;
        default:                   return 0;
      }
  }
  return 0;
}

size_t
texGenv(GLenum pname)
{
  switch (pname)
  {
    case GL_TEXTURE_GEN_MODE: return 1;
    case GL_OBJECT_PLANE:     return 4;
    case GL_EYE_PLANE:        return 4;
    default:                  return 0;
  }
}

/* TODO: Extension enums. */

size_t
samplerParameterv(GLenum pname)
{
  switch (pname)
  {
    /* Dec 2012 - http://www.opengl.org/sdk/docs/man4/xhtml/glSamplerParameter.xml */

    case GL_TEXTURE_MIN_FILTER:         return 1;
    case GL_TEXTURE_MAG_FILTER:         return 1;
    case GL_TEXTURE_MIN_LOD:            return 1;
    case GL_TEXTURE_MAX_LOD:            return 1;
    case GL_TEXTURE_WRAP_S:             return 1;
    case GL_TEXTURE_WRAP_T:             return 1;
    case GL_TEXTURE_WRAP_R:             return 1;
    case GL_TEXTURE_BORDER_COLOR:       return 4;
    case GL_TEXTURE_COMPARE_MODE:       return 1;
    case GL_TEXTURE_COMPARE_FUNC:       return 1;

    /* Extensions */

    case GL_TEXTURE_LOD_BIAS:           return 1;
    case GL_TEXTURE_MAX_ANISOTROPY_EXT: return 1;
    case GL_TEXTURE_SRGB_DECODE_EXT:    return 1;

    default:                            return 0;
  }
}

size_t
drawElements(GLenum mode, GLsizei count, GLenum type)
{
  /* TODO: Something about client state VBO affects output? */

  switch (mode)
  {
    case GL_POINTS:
    case GL_LINE_STRIP:
    case GL_LINE_LOOP:
    case GL_LINES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_TRIANGLES:
    case GL_QUAD_STRIP:
    case GL_QUADS:
    case GL_POLYGON:         break;

    default: return 0;
  }

  size_t size = count > 0 ? count : 0;

  switch (type)
  {
    case GL_UNSIGNED_BYTE:  size *= sizeof(GLubyte);  break;
    case GL_UNSIGNED_SHORT: size *= sizeof(GLushort); break;
    case GL_UNSIGNED_INT:   size *= sizeof(GLuint);   break;
    default:                                          break;
  }

  return size;
}

size_t
namedString(const GLint namelen, const GLchar *name)
{
  if (namelen > 0)
    return (size_t) namelen;

  return name ? strlen(name) : 0;
}

size_t
namedStringParams(const GLenum pname)
{
  switch (pname)
  {
    case GL_NAMED_STRING_LENGTH_ARB: return 1;
    case GL_NAMED_STRING_TYPE_ARB:   return 1;
    default:                         return 0;
  }
}

}

/* Convert glShaderSource parameters into NUL-terminated string. */

char *
shaderSourceStrings(const GLsizei count, const GLchar **string,  const GLint *length)
{
  size_t total;  /* Total size of copy (bytes)      */
  char *tmp;     /* Copy of string array            */
  GLsizei i;     /* Input iterator                  */
  GLchar *j;     /* Output iterator                 */
  size_t  k;     /* Scratch space for string length */

  if (count<=0)
    return NULL;

  /* Determine single-allocation memory size */

  total = 0;
  for (i=0; i<count; ++i)
    total += length ? length[i] : (string[i] ? strlen(string[i]) : 0);
  total += count;                        /* One NUL-terminator per string */
  total *= sizeof(GLchar);
  total += 1;                            /*  NULL terminator */

  /* Do the allocation */

  tmp = (char *) malloc(total);

  /* Copy the strings */

  j = tmp;
  for (i=0; i<count; ++i)
  {
    k = length ? length[i] : (string[i] ? strlen(string[i]) : 0);
    memcpy(j,string[i],k);
    j += k;
  }
  *(j++) = '\0';

  RegalAssert(tmp+total == j);

  return tmp;
}

}

REGAL_NAMESPACE_END
