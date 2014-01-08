/*
  Copyright (c) 2011-2013 NVIDIA Corporation
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

 Regal fixed-function emulation.
 Cass Everitt

 */

#ifndef __REGAL_IFF_H__
#define __REGAL_IFF_H__

#include "RegalUtil.h"

#include "lookup3.h"

#define REGAL_IMMEDIATE_BUFFER_SIZE 8192

#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS 8
#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS  2

#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE  (1<<REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS)
#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET   (1<<REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS)
#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_MASK  ((1<<(REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS-REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS))-1)


#define REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH 128
#define REGAL_FIXED_FUNCTION_MAX_LIGHTS           8
#define REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES      8

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <climits>
#include <cstring>

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "RegalEmu.h"
#include "RegalPrivate.h"
#include "RegalToken.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalEmuInfo.h"
#include "RegalSharedMap.h"
#include "RegalFloat4.h"
#include "linear.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu
{

// lookup array, Must Be kept in sync with enum TexenvMode; TEM_* values
const GLenum texenvModeGL[] =
{
  GL_FALSE,
  GL_REPLACE,
  GL_MODULATE,
  GL_ADD,
  GL_DECAL,
  GL_BLEND,
  GL_COMBINE
};

// lookup array, Must Be kept in sync with enum TexenvCombine; TEC_* values
const GLenum texenvCombineGL[] =
{
  GL_FALSE,
  GL_REPLACE,
  GL_MODULATE,
  GL_ADD,
  GL_ADD_SIGNED,
  GL_INTERPOLATE,
  GL_SUBTRACT,
  GL_DOT3_RGB,
  GL_DOT3_RGBA
};

// lookup array, Must Be kept in sync with enum TexenvCombineSrc; TCS_* values
const GLenum texenvCombineSrcGL[] =
{
  GL_FALSE,
  GL_CONSTANT,
  GL_PRIMARY_COLOR,
  GL_PREVIOUS,
  GL_TEXTURE,
  GL_TEXTURE0,
  GL_TEXTURE1,
  GL_TEXTURE2,
  GL_TEXTURE3
};

// lookup array, Must Be kept in sync with enum TexenvCombineOp; TCO_* values
const GLenum texenvCombineOpGL[] =
{
  GL_FALSE,
  GL_SRC_COLOR,
  GL_ONE_MINUS_SRC_COLOR,
  GL_SRC_ALPHA,
  GL_ONE_MINUS_SRC_ALPHA
};

/*
 From GLSLang spec 1.20.8:

 //
 // Matrix state. p. 31, 32, 37, 39, 40.
 //
 uniform mat4  gl_ModelViewMatrix;
 uniform mat4  gl_ProjectionMatrix;
 uniform mat4  gl_ModelViewProjectionMatrix;
 uniform mat4  gl_TextureMatrix[gl_MaxTextureCoords];

 //
 // Derived matrix state that provides inverse and transposed versions
 // of the matrices above.  Poorly conditioned matrices may result
 // in unpredictable values in their inverse forms.
 //
 uniform mat3  gl_NormalMatrix; // transpose of the inverse of the
                                // upper leftmost 3x3 of gl_ModelViewMatrix

 uniform mat4  gl_ModelViewMatrixInverse;
 uniform mat4  gl_ProjectionMatrixInverse;
 uniform mat4  gl_ModelViewProjectionMatrixInverse;
 uniform mat4  gl_TextureMatrixInverse[gl_MaxTextureCoords];
 uniform mat4  gl_ModelViewMatrixTranspose;
 uniform mat4  gl_ProjectionMatrixTranspose;
 uniform mat4  gl_ModelViewProjectionMatrixTranspose;
 uniform mat4  gl_TextureMatrixTranspose[gl_MaxTextureCoords];
 uniform mat4  gl_ModelViewMatrixInverseTranspose;
 uniform mat4  gl_ProjectionMatrixInverseTranspose;
 uniform mat4  gl_ModelViewProjectionMatrixInverseTranspose;
 uniform mat4  gl_TextureMatrixInverseTranspose[gl_MaxTextureCoords];

 //
 // Normal scaling p. 39.
 //
 uniform float gl_NormalScale;

 //
 // Depth range in window coordinates, p. 33
 //
 struct gl_DepthRangeParameters {
   float near;   // n
   float far;    // f
   float diff;   // f - n
 };
 uniform gl_DepthRangeParameters gl_DepthRange;

 //
 // Clip planes p. 42.
 //
 uniform vec4  gl_ClipPlane[gl_MaxClipPlanes];

 //
 // Point Size, p. 66, 67.
 //
 struct gl_PointParameters {
   float size;
   float sizeMin;
   float sizeMax;
   float fadeThresholdSize;
   float distanceConstantAttenuation;
   float distanceLinearAttenuation;
   float distanceQuadraticAttenuation;
 };
 uniform gl_PointParameters gl_Point;

 //
 // Material State p. 50, 55.
 //

 struct gl_MaterialParameters {
   vec4  emission;   // Ecm
   vec4  ambient;    // Acm
   vec4  diffuse;    // Dcm
   vec4  specular;   // Scm
   float shininess;  // Srm
 };
 uniform gl_MaterialParameters  gl_FrontMaterial;
 uniform gl_MaterialParameters  gl_BackMaterial;

 //
 // Light State p 50, 53, 55.
 //
 struct gl_LightSourceParameters {
   vec4  ambient;        // Acli
   vec4  diffuse;        // Dcli
   vec4  specular;       // Scli
   vec4  position;       // Ppli
   vec4  halfVector;     // Hi
   vec3  spotDirection;  // Sdli
   float spotExponent;   // Srli
   float spotCutoff;     // Crli (range: [0,90], 180)
   float spotCosCutoff;  // cos(Crli) (range: [1,0], -1)
   float constantAttenuation; // K0
   float linearAttenuation;   // K1
   float quadraticAttenuation;// K2
 };
 uniform gl_LightSourceParameters  gl_LightSource[gl_MaxLights];

 struct gl_LightModelParameters {
   vec4  ambient;       // Acs
 };
 uniform gl_LightModelParameters  gl_LightModel;

 //
 // Derived state from products of light and material.
 //
 struct gl_LightModelProducts {
   vec4  sceneColor;     // Derived. Ecm + Acm * Acs
 };
 uniform gl_LightModelProducts gl_FrontLightModelProduct;
 uniform gl_LightModelProducts gl_BackLightModelProduct;

 struct gl_LightProducts {
   vec4  ambient;   // Acm * Acli
   vec4  diffuse;   // Dcm * Dcli
   vec4  specular;  // Scm * Scli
 };
 uniform gl_LightProducts gl_FrontLightProduct[gl_MaxLights];
 uniform gl_LightProducts gl_BackLightProduct[gl_MaxLights];

 //
 // Texture Environment and Generation, p. 152, p. 40-42.
 //
 uniform vec4  gl_TextureEnvColor[gl_MaxTextureUnits];
 uniform vec4  gl_EyePlaneS[gl_MaxTextureCoords];
 uniform vec4  gl_EyePlaneT[gl_MaxTextureCoords];
 uniform vec4  gl_EyePlaneR[gl_MaxTextureCoords];
 uniform vec4  gl_EyePlaneQ[gl_MaxTextureCoords];
 uniform vec4  gl_ObjectPlaneS[gl_MaxTextureCoords];
 uniform vec4  gl_ObjectPlaneT[gl_MaxTextureCoords];
 uniform vec4  gl_ObjectPlaneR[gl_MaxTextureCoords];
 uniform vec4  gl_ObjectPlaneQ[gl_MaxTextureCoords];

 //
 // Fog p. 161
 //
 struct gl_FogParameters {
   vec4 color;
   float density;
   float start;
   float end;
   float scale;  // 1 / (end - start)
 };
 uniform gl_FogParameters gl_Fog;
 */


enum RegalFFUniformEnum
{
  FFU_foo = 0,
  FFU_ModelView,
  FFU_ModelViewInverse,
  FFU_ModelViewInverseTranspose,
  FFU_Projection,
  FFU_ProjectionInverse,
  FFU_ModelViewProjection,
  FFU_ModelViewProjectionInverse,
  FFU_TextureMatrix0,
  FFU_TextureMatrix1,
  FFU_TextureMatrix2,
  FFU_TextureMatrix3,
  FFU_TextureMatrix4,
  FFU_TextureMatrix5,
  FFU_TextureMatrix6,
  FFU_TextureMatrix7,
  FFU_TextureMatrix8,
  FFU_TextureMatrix9,
  FFU_TextureMatrix10,
  FFU_TextureMatrix11,
  FFU_TextureMatrix12,
  FFU_TextureMatrix13,
  FFU_TextureMatrix14,
  FFU_TextureMatrix15,
  FFU_TextureEnvColor0,
  FFU_TextureEnvColor1,
  FFU_TextureEnvColor2,
  FFU_TextureEnvColor3,
  FFU_TextureEnvColor4,
  FFU_TextureEnvColor5,
  FFU_TextureEnvColor6,
  FFU_TextureEnvColor7,
  FFU_TextureEnvColor8,
  FFU_TextureEnvColor9,
  FFU_TextureEnvColor10,
  FFU_TextureEnvColor11,
  FFU_TextureEnvColor12,
  FFU_TextureEnvColor13,
  FFU_TextureEnvColor14,
  FFU_TextureEnvColor15,
  FFU_Texgen0ObjS,
  FFU_Texgen0ObjT,
  FFU_Texgen0ObjR,
  FFU_Texgen0ObjQ,
  FFU_Texgen0EyeS,
  FFU_Texgen0EyeT,
  FFU_Texgen0EyeR,
  FFU_Texgen0EyeQ,
  FFU_Texgen1ObjS,
  FFU_Texgen1ObjT,
  FFU_Texgen1ObjR,
  FFU_Texgen1ObjQ,
  FFU_Texgen1EyeS,
  FFU_Texgen1EyeT,
  FFU_Texgen1EyeR,
  FFU_Texgen1EyeQ,
  FFU_Texgen2ObjS,
  FFU_Texgen2ObjT,
  FFU_Texgen2ObjR,
  FFU_Texgen2ObjQ,
  FFU_Texgen2EyeS,
  FFU_Texgen2EyeT,
  FFU_Texgen2EyeR,
  FFU_Texgen2EyeQ,
  FFU_Texgen3ObjS,
  FFU_Texgen3ObjT,
  FFU_Texgen3ObjR,
  FFU_Texgen3ObjQ,
  FFU_Texgen3EyeS,
  FFU_Texgen3EyeT,
  FFU_Texgen3EyeR,
  FFU_Texgen3EyeQ,
  FFU_Light0,
  FFU_Light1,
  FFU_Light2,
  FFU_Light3,
  FFU_Light4,
  FFU_Light5,
  FFU_Light6,
  FFU_Light7,
  FFU_MaterialFront,
  FFU_MaterialBack,
  FFU_LightModelAmbient,
  FFU_ClipPlane0,
  FFU_ClipPlane1,
  FFU_ClipPlane2,
  FFU_ClipPlane3,
  FFU_ClipPlane4,
  FFU_ClipPlane5,
  FFU_ClipPlane6,
  FFU_ClipPlane7,
  FFU_Fog,
  FFU_ConstantColor,
  FFU_Attrib,
  FFU_AlphaRef,
};

struct RegalFFUniformInfo
{
  RegalFFUniformEnum val;
  const GLchar * name;
};

static const RegalFFUniformInfo regalFFUniformInfo[] =
{
  { FFU_foo, "foo" },
  { FFU_ModelView, "rglModelView" },
  { FFU_ModelViewInverse, "rglModelViewInverse" },
  { FFU_ModelViewInverseTranspose, "rglModelViewInverseTranspose" },
  { FFU_Projection, "rglProjection" },
  { FFU_ProjectionInverse, "rglProjectionInverse" },
  { FFU_ModelViewProjection, "rglModelViewProjection" },
  { FFU_ModelViewProjectionInverse, "rglModelViewProjectionInverse" },
  { FFU_TextureMatrix0, "rglTextureMatrix0" },
  { FFU_TextureMatrix1, "rglTextureMatrix1" },
  { FFU_TextureMatrix2, "rglTextureMatrix2" },
  { FFU_TextureMatrix3, "rglTextureMatrix3" },
  { FFU_TextureMatrix4, "rglTextureMatrix4" },
  { FFU_TextureMatrix5, "rglTextureMatrix5" },
  { FFU_TextureMatrix6, "rglTextureMatrix6" },
  { FFU_TextureMatrix7, "rglTextureMatrix7" },
  { FFU_TextureMatrix8, "rglTextureMatrix8" },
  { FFU_TextureMatrix9, "rglTextureMatrix9" },
  { FFU_TextureMatrix10, "rglTextureMatrix10" },
  { FFU_TextureMatrix11, "rglTextureMatrix11" },
  { FFU_TextureMatrix12, "rglTextureMatrix12" },
  { FFU_TextureMatrix13, "rglTextureMatrix13" },
  { FFU_TextureMatrix14, "rglTextureMatrix14" },
  { FFU_TextureMatrix15, "rglTextureMatrix15" },
  { FFU_TextureEnvColor0, "rglTexEnvColor0" },
  { FFU_TextureEnvColor1, "rglTexEnvColor1" },
  { FFU_TextureEnvColor2, "rglTexEnvColor2" },
  { FFU_TextureEnvColor3, "rglTexEnvColor3" },
  { FFU_TextureEnvColor4, "rglTexEnvColor4" },
  { FFU_TextureEnvColor5, "rglTexEnvColor5" },
  { FFU_TextureEnvColor6, "rglTexEnvColor6" },
  { FFU_TextureEnvColor7, "rglTexEnvColor7" },
  { FFU_TextureEnvColor8, "rglTexEnvColor8" },
  { FFU_TextureEnvColor9, "rglTexEnvColor9" },
  { FFU_TextureEnvColor10, "rglTexEnvColor10" },
  { FFU_TextureEnvColor11, "rglTexEnvColor11" },
  { FFU_TextureEnvColor12, "rglTexEnvColor12" },
  { FFU_TextureEnvColor13, "rglTexEnvColor13" },
  { FFU_TextureEnvColor14, "rglTexEnvColor14" },
  { FFU_TextureEnvColor15, "rglTexEnvColor15" },
  { FFU_Texgen0ObjS, "rglTexGen0ObjS" },
  { FFU_Texgen0ObjT, "rglTexGen0ObjT" },
  { FFU_Texgen0ObjR, "rglTexGen0ObjR" },
  { FFU_Texgen0ObjQ, "rglTexGen0ObjQ" },
  { FFU_Texgen0EyeS, "rglTexGen0EyeS" },
  { FFU_Texgen0EyeT, "rglTexGen0EyeT" },
  { FFU_Texgen0EyeR, "rglTexGen0EyeR" },
  { FFU_Texgen0EyeQ, "rglTexGen0EyeQ" },
  { FFU_Texgen1ObjS, "rglTexGen1ObjS" },
  { FFU_Texgen1ObjT, "rglTexGen1ObjT" },
  { FFU_Texgen1ObjR, "rglTexGen1ObjR" },
  { FFU_Texgen1ObjQ, "rglTexGen1ObjQ" },
  { FFU_Texgen1EyeS, "rglTexGen1EyeS" },
  { FFU_Texgen1EyeT, "rglTexGen1EyeT" },
  { FFU_Texgen1EyeR, "rglTexGen1EyeR" },
  { FFU_Texgen1EyeQ, "rglTexGen1EyeQ" },
  { FFU_Texgen2ObjS, "rglTexGen2ObjS" },
  { FFU_Texgen2ObjT, "rglTexGen2ObjT" },
  { FFU_Texgen2ObjR, "rglTexGen2ObjR" },
  { FFU_Texgen2ObjQ, "rglTexGen2ObjQ" },
  { FFU_Texgen2EyeS, "rglTexGen2EyeS" },
  { FFU_Texgen2EyeT, "rglTexGen2EyeT" },
  { FFU_Texgen2EyeR, "rglTexGen2EyeR" },
  { FFU_Texgen2EyeQ, "rglTexGen2EyeQ" },
  { FFU_Texgen3ObjS, "rglTexGen3ObjS" },
  { FFU_Texgen3ObjT, "rglTexGen3ObjT" },
  { FFU_Texgen3ObjR, "rglTexGen3ObjR" },
  { FFU_Texgen3ObjQ, "rglTexGen3ObjQ" },
  { FFU_Texgen3EyeS, "rglTexGen3EyeS" },
  { FFU_Texgen3EyeT, "rglTexGen3EyeT" },
  { FFU_Texgen3EyeR, "rglTexGen3EyeR" },
  { FFU_Texgen3EyeQ, "rglTexGen3EyeQ" },
  { FFU_Light0, "rglLight0[0]" },
  { FFU_Light1, "rglLight1[0]" },
  { FFU_Light2, "rglLight2[0]" },
  { FFU_Light3, "rglLight3[0]" },
  { FFU_Light4, "rglLight4[0]" },
  { FFU_Light5, "rglLight5[0]" },
  { FFU_Light6, "rglLight6[0]" },
  { FFU_Light7, "rglLight7[0]" },
  { FFU_MaterialFront, "rglMaterialFront" },
  { FFU_MaterialBack, "rglMaterialBack" },
  { FFU_LightModelAmbient, "rglLightModelAmbient" },
  { FFU_ClipPlane0, "rglClipPlane0" },
  { FFU_ClipPlane1, "rglClipPlane1" },
  { FFU_ClipPlane2, "rglClipPlane2" },
  { FFU_ClipPlane3, "rglClipPlane3" },
  { FFU_ClipPlane4, "rglClipPlane4" },
  { FFU_ClipPlane5, "rglClipPlane5" },
  { FFU_ClipPlane6, "rglClipPlane6" },
  { FFU_ClipPlane7, "rglClipPlane7" },
  { FFU_Fog, "rglFog" },
  { FFU_ConstantColor, "rglConstantColor" },
  { FFU_Attrib, "rglAttrib[0]" },
  { FFU_AlphaRef, "rglAlphaRef" },
};

template <typename T> bool RFFIsVector( const T p )
{
  UNUSED_PARAMETER(p);
  return false;
}

template <typename T> bool RFFIsVector( const T * p )
{
  UNUSED_PARAMETER(p);
  return true;
}

template <typename T> GLfloat RFFToFloat( int i, const T p )
{
  UNUSED_PARAMETER(i);
  return GLfloat( p );
}

template <typename T> GLfloat RFFToFloat( int i, const T * p )
{
  return GLfloat( p[i] );
}

template <typename T> GLfloat RFFToFloatN( int i, const T p )
{
  UNUSED_PARAMETER(i);
  return GLfloat( p );
}

template <typename T> GLfloat RFFToFloatN( int i, const T * p )
{
  return GLfloat( p[i] );
}

template <> inline GLfloat RFFToFloatN( int i, const GLint p )
{
  UNUSED_PARAMETER(i);
  return GLfloat( double( p ) / double( INT_MAX ) );
}

template <> inline GLfloat RFFToFloatN( int i, const int * p )
{
  return GLfloat( double( p[i] ) / double( INT_MAX ) );
}

struct Iff
{
  Iff();
  void Cleanup(RegalContext &ctx);
  void InitVertexArray(RegalContext &ctx);
  GLuint ClientStateToIndex(GLenum state);
  void EnableClientState( RegalContext * ctx, GLenum state );
  void DisableClientState( RegalContext * ctx, GLenum state );
  void VertexPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  void NormalPointer( RegalContext * ctx, GLenum type, GLsizei stride, const GLvoid *pointer );
  void ColorPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  void SecondaryColorPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  void FogCoordPointer( RegalContext * ctx, GLenum type, GLsizei stride, const GLvoid *pointer );
  void EdgeFlagPointer( RegalContext * ctx, GLsizei stride, const GLvoid *pointer );
  void TexCoordPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  void GetAttrib( RegalContext * ctx, GLuint index, GLenum pname, GLdouble * d );
  void GetAttrib( RegalContext * ctx, GLuint index, GLenum pname, GLfloat * f );
  void GetAttrib( RegalContext * ctx, GLuint index, GLenum pname, GLint * i );

  int progcount;

  // Vertex arrays
  GLuint catIndex;
  GLuint ffAttrMap[ REGAL_EMU_MAX_VERTEX_ATTRIBS ];
  GLuint ffAttrInvMap[ REGAL_EMU_MAX_VERTEX_ATTRIBS ];
  GLuint ffAttrTexBegin;
  GLuint ffAttrTexEnd;
  GLuint ffAttrNumTex;
  GLuint max_vertex_attribs;

  template <typename T> bool VaGet( RegalContext * ctx, GLenum pname, T * params )
  {
    GLuint index = 0;
    switch (pname)
    {
      case GL_VERTEX_ARRAY_BUFFER_BINDING:
      case GL_VERTEX_ARRAY_SIZE:
      case GL_VERTEX_ARRAY_TYPE:
      case GL_VERTEX_ARRAY_STRIDE:
      case GL_VERTEX_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_Vertex ];
        break;
      case GL_NORMAL_ARRAY_BUFFER_BINDING:
      case GL_NORMAL_ARRAY_TYPE:
      case GL_NORMAL_ARRAY_STRIDE:
      case GL_NORMAL_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_Normal ];
        break;
      case GL_COLOR_ARRAY_BUFFER_BINDING:
      case GL_COLOR_ARRAY_SIZE:
      case GL_COLOR_ARRAY_TYPE:
      case GL_COLOR_ARRAY_STRIDE:
      case GL_COLOR_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_Color ];
        break;
      case GL_SECONDARY_COLOR_ARRAY_SIZE:
        // This is a convenient lie. --Cass
        *params = 3;
        break;
      case GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING:
      case GL_SECONDARY_COLOR_ARRAY_TYPE:
      case GL_SECONDARY_COLOR_ARRAY_STRIDE:
      case GL_SECONDARY_COLOR_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_SecondaryColor ];
        break;
      case GL_FOG_COORD_ARRAY_BUFFER_BINDING:
      case GL_FOG_COORD_ARRAY_TYPE:
      case GL_FOG_COORD_ARRAY_STRIDE:
      case GL_FOG_COORD_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_FogCoord ];
        break;
      case GL_EDGE_FLAG_ARRAY_BUFFER_BINDING:
      case GL_EDGE_FLAG_ARRAY_STRIDE:
      case GL_EDGE_FLAG_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_EdgeFlag ];
        break;
      case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
      case GL_TEXTURE_COORD_ARRAY_SIZE:
      case GL_TEXTURE_COORD_ARRAY_TYPE:
      case GL_TEXTURE_COORD_ARRAY_STRIDE:
      case GL_TEXTURE_COORD_ARRAY_POINTER:
        if (catIndex >= ffAttrNumTex)
        {
          // FIXME need to set an error here!
          return false;
        }
        index = ffAttrTexBegin + catIndex;
        break;

        // INDEX arrays are not supported

      case GL_INDEX_ARRAY_TYPE:
        *params = GL_FLOAT;
        break;
      case GL_INDEX_ARRAY_BUFFER_BINDING:
      case GL_INDEX_ARRAY_STRIDE:
      case GL_INDEX_ARRAY_POINTER:
      default:
        return false;
    }

    switch (pname)
    {
      case GL_VERTEX_ARRAY_BUFFER_BINDING:
      case GL_NORMAL_ARRAY_BUFFER_BINDING:
      case GL_COLOR_ARRAY_BUFFER_BINDING:
      case GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING:
      case GL_FOG_COORD_ARRAY_BUFFER_BINDING:
      case GL_EDGE_FLAG_ARRAY_BUFFER_BINDING:
      case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
        pname = GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING;
        break;
      case GL_VERTEX_ARRAY_SIZE:
      case GL_COLOR_ARRAY_SIZE:
      case GL_TEXTURE_COORD_ARRAY_SIZE:
        pname = GL_VERTEX_ATTRIB_ARRAY_SIZE;
        break;
      case GL_VERTEX_ARRAY_TYPE:
      case GL_NORMAL_ARRAY_TYPE:
      case GL_COLOR_ARRAY_TYPE:
      case GL_SECONDARY_COLOR_ARRAY_TYPE:
      case GL_FOG_COORD_ARRAY_TYPE:
      case GL_TEXTURE_COORD_ARRAY_TYPE:
        pname = GL_VERTEX_ATTRIB_ARRAY_TYPE;
        break;
      case GL_VERTEX_ARRAY_STRIDE:
      case GL_NORMAL_ARRAY_STRIDE:
      case GL_COLOR_ARRAY_STRIDE:
      case GL_SECONDARY_COLOR_ARRAY_STRIDE:
      case GL_FOG_COORD_ARRAY_STRIDE:
      case GL_EDGE_FLAG_ARRAY_STRIDE:
      case GL_TEXTURE_COORD_ARRAY_STRIDE:
        pname = GL_VERTEX_ATTRIB_ARRAY_STRIDE;
        break;
      case GL_VERTEX_ARRAY_POINTER:
      case GL_NORMAL_ARRAY_POINTER:
      case GL_COLOR_ARRAY_POINTER:
      case GL_SECONDARY_COLOR_ARRAY_POINTER:
      case GL_FOG_COORD_ARRAY_POINTER:
      case GL_EDGE_FLAG_ARRAY_POINTER:
      case GL_TEXTURE_COORD_ARRAY_POINTER:
        pname = GL_VERTEX_ATTRIB_ARRAY_POINTER;
        break;
      default:
        return false;
    }
    RestoreVao( ctx );
    if (index == RFF2A_Invalid)
    {
      // What to return in this case?
      return false;
    }
    GetAttrib( ctx, index, pname, params );
    return true;
  }

  bool IsEnabled( RegalContext * ctx, GLenum pname, GLboolean &enabled );

  // immediate mode

  bool    immActive;
  GLuint  immProvoking;
  GLuint  immCurrent;
  GLenum  immPrim;
  Float4  immVab[ REGAL_EMU_MAX_VERTEX_ATTRIBS ];
  GLubyte immArray[ REGAL_IMMEDIATE_BUFFER_SIZE * REGAL_EMU_MAX_VERTEX_ATTRIBS * sizeof(Float4) ];

  GLuint  immVbo;
  GLuint  immVao;
  GLuint  immShadowVao;

  void InitImmediate(RegalContext &ctx);
  void glDeleteVertexArrays( RegalContext * ctx, GLsizei n, const GLuint * arrays );
  void glDeleteBuffers( RegalContext * ctx, GLsizei n, const GLuint * buffers );
  GLboolean IsVertexArray( RegalContext * ctx, GLuint name );
  void glBindVertexArray( RegalContext *ctx, GLuint vao );
  void ShadowClientActiveTexture( GLenum texture );
  void Begin( RegalContext * ctx, GLenum mode );
  void End( RegalContext * ctx );
  void RestoreVao( RegalContext * ctx );
  void Flush( RegalContext * ctx );
  void Provoke( RegalContext * ctx );

  template <int N, bool Norm, typename T> void Attribute( RegalContext * ctx, GLuint idx, const T * v )
  {
    if (idx >= max_vertex_attribs)
    {
      // FIXME: set an error
      return;
    }
    RegalAssertArrayIndex( immVab, idx );
    Float4 & a = immVab[ idx ];
    a.x = ToFloat<Norm>( v[0] );
    a.y = N > 1 ? ToFloat<Norm>( v[1] ) : 0.0f;
    a.z = N > 2 ? ToFloat<Norm>( v[2] ) : 0.0f;
    a.w = N > 3 ? ToFloat<Norm>( v[3] ) : 1.0f;
    ffstate.uniform.ver = ffstate.uniform.vabVer = ver.Update();
    if (idx == immProvoking)
      Provoke( ctx );
  }

  template <int N, typename T> void Attr( RegalContext *ctx, GLuint idx, T x, T y = 0, T z = 0, T w = 1 )
  {
    T v[4] = { x, y, z, w };
    Attribute<N,false>( ctx, idx,v );
  }

  template <int N, typename T> void AttrN( RegalContext *ctx, GLuint idx, T x, T y = 0, T z = 0, T w = 1 )
  {
    T v[4] = { x, y, z, w };
    Attribute<N,true>( ctx, idx, v );
  }

  template <int N, typename T> void Attr( RegalContext *ctx, GLuint idx, const T * v )
  {
    Attribute<N,false>( ctx, idx, v );
  }

  template <int N, typename T> void AttrN( RegalContext *ctx, GLuint idx, const T * v )
  {
    Attribute<N,true>( ctx, idx, v );
  }

  GLuint AttrIndex( RegalFixedFunctionAttrib attr, int cat = -1 ) const;

  // fixed function

  enum CompareFunc
  {
    CF_Invalid,
    CF_Never,
    CF_Less,
    CF_Equal,
    CF_Lequal,
    CF_Greater,
    CF_NotEqual,
    CF_Gequal,
    CF_Always
  };

  enum TexturePriority
  {
    TP_1D = 0,
    TP_2D = 1,
    TP_Rect = 2,
    TP_3D = 3,
    TP_CubeMap = 4
  };

  enum TextureTargetBitfield
  {
    TT_None    = 0,
    TT_1D      = 1<<TP_1D,
    TT_2D      = 1<<TP_2D,
    TT_Rect    = 1<<TP_Rect,
    TT_3D      = 1<<TP_3D,
    TT_CubeMap = 1<<TP_CubeMap
  };

  enum TexgenMode
  {
    TG_ObjectLinear = 1,
    TG_EyeLinear = 2,
    TG_SphereMap = 3,
    TG_NormalMap = 4,
    TG_ReflectionMap = 5
  };

  enum FogMode
  {
    FG_Linear = 1,
    FG_Exp = 2,
    FG_Exp2 = 3
  };

  enum ColorMaterialMode
  {
    CM_None = 0,
    CM_Emission = 1,
    CM_Ambient = 2,
    CM_Diffuse = 3,
    CM_Specular = 4,
    CM_AmbientAndDiffuse = 5
  };

  // enums must be in sync with look up array texenvModeGL

  enum TexenvMode
  {
    TEM_Invalid,
    TEM_Replace,
    TEM_Modulate,
    TEM_Add,
    TEM_Decal,
    TEM_Blend,
    TEM_Combine
  };

  // enums must be in sync with look up array texenvCombineGL

  enum TexenvCombine
  {
    TEC_Invalid,
    TEC_Replace,
    TEC_Modulate,
    TEC_Add,
    TEC_AddSigned,
    TEC_Interpolate,
    TEC_Subtract,
    TEC_Dot3Rgb,
    TEC_Dot3Rgba
  };

  // enums must be in sync with look up array texenvCombineSrcGL

  enum TexenvCombineSrc
  {
    TCS_Invalid,
    TCS_Constant,
    TCS_PrimaryColor,
    TCS_Previous,
    TCS_Texture,
    TCS_Texture0,
    TCS_Texture1,
    TCS_Texture2,
    TCS_Texture3,
  };

  // enums must be in sync with look up array texenvCombineOpGL

  enum TexenvCombineOp
  {
    TCO_Invalid,
    TCO_Color,
    TCO_OneMinusColor,
    TCO_Alpha,
    TCO_OneMinusAlpha
  };

  struct TexenvCombineState
  {
    TexenvCombineState(bool isRgb)
      : mode(TEC_Modulate)
      , src0(TCS_Texture)
      , src1(TCS_Previous)
      , src2(TCS_Constant)
      , op0(isRgb ? TCO_Color : TCO_Alpha)
      , op1(isRgb ? TCO_Color : TCO_Alpha)
      , op2(TCO_Alpha)
      , scale(1.0f)
    {
    }

    TexenvCombine    mode;
    TexenvCombineSrc src0;
    TexenvCombineSrc src1;
    TexenvCombineSrc src2;
    TexenvCombineOp  op0;
    TexenvCombineOp  op1;
    TexenvCombineOp  op2;
    GLfloat          scale;
  };

  struct TextureEnv
  {
    TextureEnv()
      : mode(TEM_Modulate)
      , rgb(true)
      , a(false)
    {
    }

    TexenvMode mode;
    TexenvCombineState rgb, a;
  };

  struct TextureUnit
  {
    TextureUnit()
      : ttb(TT_None)
      , fmt(GL_RGBA)  //<> what should this be really?
    {
    }

    TextureEnv            env;
    TextureTargetBitfield ttb;
    GLenum                fmt;
  };

  struct Version
  {
    inline Version()
      : val( 0 )
      , updated( false )
    {
    }

    inline GLuint64 Current() const
    {
      return val;
    }

    GLuint64 Update()
    {
      if (updated == false)
      {
        val++;
        updated = true;
      }
      return val;
    }

    inline void Reset()
    {
      updated = false;
    }

    GLuint64 val;
    bool updated;
  };

  struct State
  {
    struct Texgen
    {
      Texgen()
        : mode(TG_EyeLinear)
        , enable(GL_FALSE)
      {
      }

      TexgenMode mode;
      GLboolean enable;
    };

    struct TexgenUniform
    {
      TexgenUniform()
        : objVer(0)
        , eyeVer(0)
        , obj( 0, 0, 0, 0 )
        , eye( 0, 0, 0, 0 )
      {
      }

      GLuint64 objVer;
      GLuint64 eyeVer;
      Float4 obj;
      Float4 eye;
    };

    struct Texture
    {
      Texture()
        : enables(0)
        , useMatrix(false)
        , pad1(false)
        , pad2(false)
        , pad3(false)
      {
      }

      Texgen      texgen[4];
      TextureUnit unit;
      GLuint      enables;
      bool        useMatrix;
      bool        pad1;
      bool        pad2;
      bool        pad3;
    };

    struct TextureUniform
    {
      TextureUniform()
      {
        texgen[0].obj = texgen[0].eye = Float4( 1, 0, 0, 0 );
        texgen[1].obj = texgen[1].eye = Float4( 0, 1, 0, 0 );
      }

      TexgenUniform texgen[4];
    };

    struct AlphaTest
    {
      AlphaTest()
        : comp(CF_Always)
        , enable(false)
      {
      }

      CompareFunc comp;
      GLboolean enable;
    };

    struct AlphaTestUniform
    {
      AlphaTestUniform()
        : ver( 0 )
        , alphaRef( 0 )
      {
      }

      GLuint64 ver;
      GLfloat alphaRef;
    };

    struct ClipUniform
    {
      ClipUniform()
        : ver( 0 )
        , plane( 0, 0, 0, 0 )
      {
      }

      GLuint64 ver;
      Float4 plane;
    };

    struct Fog
    {
      Fog()
        : mode(FG_Exp)
        , enable(false)
        , useDepth(true)
      {
      }

      FogMode mode;
      bool    enable;
      bool    useDepth;
    };

    struct FogUniform
    {
      FogUniform()
        : ver( 0 )
      {
        params[0] = Float4( 1, 0, 1, 0 );
        params[1] = Float4( 0, 0, 0, 0 );
      }

      GLuint64 ver;
      Float4 params[2]; // .x = density, .y = start, .z = end, .w = d/c
    };

    struct Light
    {
      Light()
        : enable(false)
        , spotlight(false)
        , attenuate(false)
        , local(false)
      {
      }

      bool enable;
      bool spotlight;
      bool attenuate;
      bool local;
    };

    struct LightUniform
    {
      LightUniform()
        : ver( 0 )
        , ambient( 0, 0, 0, 0 )
        , diffuse( 0, 0, 0, 0 )
        , specular( 0, 0, 0, 0 )
        , position( 0, 0, 1, 0 )
        , spotDirection( 0.0f, 0.0f, -1.0f, 180.0f )
        , attenuation( 1, 0, 0, 0 )
      {
      }

      GLuint64 ver;
      Float4   ambient;
      Float4   diffuse;
      Float4   specular;
      Float4   position;
      Float4   spotDirection; // spotCutoff is in .w
      Float4   attenuation;   // spotExponent   is in .w
    };

    struct MaterialUniform
    {
      MaterialUniform()
        : ver( 0 )
        , ambient( 0.2f, 0.2f, 0.2f, 1.0f )
        , diffuse( 0.8f, 0.8f, 0.8f, 1.0f )
        , specular( 0, 0, 0, 1 )
        , emission( 0, 0, 0, 1 )
        , shininess( 0, 0, 0, 0 )
      {
      }

      GLuint64 ver;
      Float4   ambient;
      Float4   diffuse;
      Float4   specular;
      Float4   emission;
      Float4   shininess;  // shininess is in .x
    };

    // Iff::State::Store

    struct Store
    {
      Store()
      {
        // Using memset here is potentially evil since it only works
        // on POD and clobbers any vtables & etc.  See:
        //
        //    http://stackoverflow.com/questions/2481654/memset-for-initialization-in-c
        //
        // But we use every byte of Store() for the hash computation,
        // including any padding added by the compiler, so we need to
        // make sure that it all starts with the same initial value so...

        memset(this,0,sizeof(*this));

        // now reset any variables for which 0 is not the right initial value

        colorMaterialTarget0 = CM_AmbientAndDiffuse;
        colorMaterialTarget1 = CM_AmbientAndDiffuse;
        alphaTest.comp = CF_Always;
        fog.mode = FG_Exp;
        fog.useDepth = true;

        size_t n =  array_size( tex );
        for (size_t ii=0; ii<n; ii++)
        {
          RegalAssertArrayIndex( tex, ii );
          Texture & t = tex[ii];
          t.unit.fmt = GL_RGBA;
          t.unit.ttb = TT_None;
          t.texgen[0].mode = t.texgen[1].mode = t.texgen[2].mode = t.texgen[3].mode = TG_EyeLinear;
          t.unit.env.mode = TEM_Modulate;
          t.unit.env.rgb = true;
        }
      }

      GLuint64  ver;
      GLuint    hash;
      Light     light[ REGAL_FIXED_FUNCTION_MAX_LIGHTS ];
      Texture   tex[ REGAL_EMU_MAX_TEXTURE_UNITS ];
      bool      clipPlaneEnabled[ REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES ];
      GLuint    attrArrayFlags;
      ColorMaterialMode colorMaterialTarget0;
      ColorMaterialMode colorMaterialTarget1;
      AlphaTest alphaTest;
      Fog       fog;
      bool      colorSum;
      bool      rescaleNormal;
      bool      normalize;
      bool      shadeModelFlat;
      bool      lighting;
      bool      lightModelLocalViewer;
      bool      lightModelTwoSide;
      bool      lightModelSeparateSpecular;
      bool      colorMaterial;
    };

    struct StoreUniform
    {
      StoreUniform()
        : ver(0)
        , vabVer(0)
      {
        size_t n =  array_size( tex );
        for (size_t ii=0; ii<n; ii++)
        {
          RegalAssertArrayIndex( tex, ii );
          tex[ii].texgen[0].obj = tex[ii].texgen[0].eye = Float4( 1, 0, 0, 0 );
          tex[ii].texgen[1].obj = tex[ii].texgen[1].eye = Float4( 0, 1, 0, 0 );
        }
        light[0].ambient = Float4( 0, 0, 0, 1 );
        light[0].diffuse = light[0].specular = Float4( 1, 1, 1, 1 );
        mat[0].diffuse = mat[1].diffuse = Float4( 0.8f, 0.8f, 0.8f, 1.0f );
        lightModelAmbient = mat[0].ambient = mat[1].ambient = Float4( 0.2f, 0.2f, 0.2f, 1.0f );
      }

      GLuint64 ver;
      GLuint64 vabVer;

      TextureUniform   tex[ REGAL_EMU_MAX_TEXTURE_UNITS ];
      AlphaTestUniform alphaTest;
      ClipUniform      clip[ REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES ];
      LightUniform     light[ REGAL_FIXED_FUNCTION_MAX_LIGHTS ];
      MaterialUniform  mat[ 2 ];
      FogUniform       fog;
      Float4           lightModelAmbient;
    };

    State()
    {
    }

    Store raw;
    Store processed;
    StoreUniform uniform;

    bool SetEnable( Iff * ffn, bool enable, GLenum cap );
    void SetTexInfo( Iff::Version & ver, GLuint activeTex, Iff::TextureUnit & unit );
    void SetLight( Iff * ffn, GLenum light, GLenum pname, const GLfloat * params );
    void SetMaterial( Iff * ffn, GLenum face, GLenum pname, const GLfloat * params );
    void GetMaterial( Iff * ffn, GLenum face, GLenum pname, GLfloat * params );
    void SetTexgen( Iff * ffn, int coord, GLenum space, const GLfloat * params );
    void GetTexgen( Iff * ffn, int coord, GLenum space, GLfloat * params );
    void SetAlphaFunc( Iff * ffn, CompareFunc comp, GLfloat ref );
    void SetClip( Iff * ffn, GLenum plane, const GLfloat * equation );
    TextureTargetBitfield GetTextureEnable( size_t unit ) const;
    inline GLuint64 Ver() const;
    GLuint HighestPriorityTextureEnable( GLuint enables );
    void Process( Iff * ffn );
  };

  struct MatrixStack
  {
    struct El
    {
      El()
        : ver( 1 )
      {
      }

      GLuint64 ver;
      r3::Matrix4f mat;
    };

    inline MatrixStack()
    {
      stack.push_back( El() );
    }

    inline void Push()
    {
      RegalAssert( stack.size() < REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH );
      if (stack.size() < REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH)
        stack.push_back( stack.back() );
    }

    inline void Pop()
    {
      /* The stack size warning messages ought to be the responsibility
       of the debug layer, rather than emulation.  (Opt-in only)
       Should emulation set an error in the underflow situation here? */

      /* RegalAssert( stack.size()>1 ); */
      if ( stack.size()>1 )
        stack.pop_back();
    }

    inline r3::Matrix4f &Top()
    {
      RegalAssert( stack.size() );
      return stack.back().mat;
    }

    inline const r3::Matrix4f &Top() const
    {
      RegalAssert( stack.size() );
      return stack.back().mat;
    }

    inline GLuint64 &Ver()
    {
      RegalAssert( stack.size() );
      return stack.back().ver;
    }

    inline const GLuint64 &Ver() const
    {
      RegalAssert( stack.size() );
      return stack.back().ver;
    }

    inline std::size_t size() const
    {
      return stack.size();
    }

    inline El &operator[](const std::size_t i)
    {
      RegalAssert( stack.size() );
      return stack[i];
    }

    inline const El &operator[](const std::size_t i) const
    {
      RegalAssert( stack.size() );
      return stack[i];
    }

  private:
    std::vector<El> stack;
  };

  // Iff::Program

  struct Program
  {
    // Iff::Program::UniformInfo

    struct UniformInfo
    {
      UniformInfo(GLuint64 v = 0, GLint s = -1)
        : ver(v)
        , slot(s)
      {
      }

      GLuint64 ver;
      GLint    slot;
    };

    // Iff::Program

    Program()
      : ver(0)
      , pg(0)
      , vs(0)
      , fs(0)
      , progcount(0)
    {
    }

    GLuint64 ver;
    GLuint   pg;
    GLuint   vs;
    GLuint   fs;
    int      progcount;

    std::map< RegalFFUniformEnum, UniformInfo> uniforms;
    State::Store store;

    void Init( RegalContext * ctx, const State::Store & sstore, const GLchar *vsSrc, const GLchar *fsSrc );
    void Shader( RegalContext * ctx, DispatchTableGL & tbl, GLenum type, GLuint & shader, const GLchar *src );
    void Attribs( RegalContext * ctx );
    void UserShaderModeAttribs( RegalContext * ctx );
    void Samplers( RegalContext * ctx, DispatchTableGL & tbl );
    void Uniforms( RegalContext * ctx, DispatchTableGL & tbl );
  };

  MatrixStack modelview;
  MatrixStack projection;
  MatrixStack texture[ REGAL_EMU_MAX_TEXTURE_UNITS ];

  GLenum shadowMatrixMode;
  TextureUnit textureUnit[ REGAL_EMU_MAX_TEXTURE_UNITS ];
  Float4 textureEnvColor[ REGAL_EMU_MAX_TEXTURE_UNITS ];
  GLuint64 textureEnvColorVer[ REGAL_EMU_MAX_TEXTURE_UNITS ];
  GLuint textureBinding[ REGAL_EMU_MAX_TEXTURE_UNITS];
  GLuint shadowActiveTextureIndex;
  GLuint activeTextureIndex;
  GLuint programPipeline;
  GLuint program;
  Program * currprog;

  MatrixStack *currMatrixStack;

  Version ver;
  State ffstate;

  Program  ffprogs[ REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE ];

  shared_map<GLuint, GLenum> textureObjToFmt;

  // Program uniforms are tied to context state, so we cannot share IFF
  // programs, however we share user programs in general.

  std::map<GLenum, GLenum>  fmtmap;
  std::map<GLuint, GLenum>  shaderTypeMap;
  std::map<GLuint, Program> shprogmap;

  GLuint currVao;
  std::map<GLuint, GLuint> vaoAttrMap;

  bool gles;   // what about ES1?
  bool legacy; // 2.x mac

  void InitFixedFunction(RegalContext &ctx);
  void PreDraw( RegalContext * ctx );
  void SetCurrentMatrixStack( GLenum mode );
  bool ShadowMatrixMode( GLenum mode );
  void ShadowActiveTexture( GLenum texture );
  bool ShadowEnable( GLenum cap );
  bool ShadowDisable( GLenum cap );
  bool EnableIndexed( GLenum cap, GLuint index );
  bool DisableIndexed( GLenum cap, GLuint index );
  bool ShadowUseProgram( GLuint prog );
  bool ShadowBindProgramPipeline( GLuint progPipeline );
  void ShadowMultiTexBinding( GLenum texunit, GLenum target, GLuint obj );
  void ShadowTexBinding( GLenum target, GLuint obj );
  void ShadowTextureInfo( GLuint obj, GLenum target, GLint internalFormat );
  void ShadowMultiTexInfo( GLenum texunit, GLenum target, GLint internalFormat );
  void ShadowTexInfo( GLenum target, GLint internalFormat );
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLfloat *v );
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLint *v );
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, GLfloat v );
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, GLint v );

  template <typename T>
  void TexEnv( GLenum target, GLenum pname, T v )
  {
    TexEnv( GL_TEXTURE0 + shadowActiveTextureIndex, target, pname, v );
  }

  template <typename T>
  bool GetTexEnv( GLenum target, GLenum pname, T * params )
  {
    if (target != GL_TEXTURE_ENV)
      return false;

    switch( pname )
    {
      case GL_TEXTURE_ENV_MODE:
      {
        RegalAssert(activeTextureIndex < array_size( textureUnit ));
        RegalAssert(static_cast<size_t>(textureUnit[ activeTextureIndex ].env.mode) < array_size( texenvModeGL ));
        *params =  static_cast<T>(texenvModeGL[ textureUnit[ activeTextureIndex ].env.mode ]);
      }
      break;
      case GL_TEXTURE_ENV_COLOR:
      {
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        Float4 & c = textureEnvColor[ activeTextureIndex ];
        params[0] = T( c.x );
        params[1] = T( c.y );
        params[2] = T( c.z );
        params[3] = T( c.w );
      }
      break;
      case GL_COMBINE_RGB:
      case GL_COMBINE_ALPHA:
      {
        RegalAssert(activeTextureIndex < array_size( textureUnit ));
        TexenvCombineState & c = (pname == GL_COMBINE_RGB ?
                                  textureUnit[ activeTextureIndex ].env.rgb :
                                  textureUnit[ activeTextureIndex ].env.a);
        RegalAssert(static_cast<size_t>(c.mode) < array_size(texenvCombineGL));
        *params = static_cast<T>(texenvCombineGL[ c.mode ]);
      }
      break;
      case GL_SOURCE0_RGB:
      case GL_SOURCE1_RGB:
      case GL_SOURCE2_RGB:
      case GL_SOURCE0_ALPHA:
      case GL_SOURCE1_ALPHA:
      case GL_SOURCE2_ALPHA:
      {
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        int idx = pname - GL_SOURCE0_RGB;
        bool isRgb = true;
        if ( idx > 3 )
        {
          isRgb = false;
          idx = pname - GL_SOURCE0_ALPHA;
        }
        TexenvCombineState & c = (isRgb ?
                                  textureUnit[ activeTextureIndex ].env.rgb :
                                  textureUnit[ activeTextureIndex ].env.a);
        TexenvCombineSrc src = c.src0;
        switch ( idx )
        {
          case 0:
            src = c.src0;
            break;
          case 1:
            src = c.src1;
            break;
          case 2:
            src = c.src2;
            break;
          default:
            break;
        }
        RegalAssert(static_cast<size_t>(src) < array_size(texenvCombineSrcGL));
        *params = static_cast<T>(texenvCombineSrcGL[ src ]);
      }
      break;
      case GL_OPERAND0_RGB:
      case GL_OPERAND1_RGB:
      case GL_OPERAND2_RGB:
      case GL_OPERAND0_ALPHA:
      case GL_OPERAND1_ALPHA:
      case GL_OPERAND2_ALPHA:
      {
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        int idx = pname - GL_OPERAND0_RGB;
        bool isRgb = true;
        if (idx > 3)
        {
          isRgb = false;
          idx = pname - GL_OPERAND0_ALPHA;
        }
        TexenvCombineState & c = (isRgb ?
                                  textureUnit[ activeTextureIndex ].env.rgb :
                                  textureUnit[ activeTextureIndex ].env.a);
        TexenvCombineOp op = c.op0;
        switch ( idx )
        {
          case 0:
            op = c.op0;
            break;
          case 1:
            op = c.op1;
            break;
          case 2:
            op = c.op2;
            break;
          default:
            break;
        }
        RegalAssert(static_cast<size_t>(op) < array_size(texenvCombineOpGL));
        *params = static_cast<T>(texenvCombineOpGL[ op ]);
      }
      break;
      case GL_RGB_SCALE:
      case GL_ALPHA_SCALE:
      {
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        TexenvCombineState & c = (pname == GL_RGB_SCALE ?
                                  textureUnit[ activeTextureIndex ].env.rgb :
                                  textureUnit[ activeTextureIndex ].env.a);
        *params = static_cast<T>(c.scale);
      }
      break;
      default:
        return false;
    }
    return true;
  }

  void ShadeModel( GLenum mode );

  template <typename T> void Light( GLenum light, GLenum pname, const T param )
  {
    int comp = 4;
    switch( pname )
    {
      case GL_AMBIENT:
      case GL_DIFFUSE:
      case GL_SPECULAR:
      case GL_POSITION:
        break;
      case GL_SPOT_DIRECTION:
        comp = 3;
        break;
      case GL_SPOT_EXPONENT:
      case GL_SPOT_CUTOFF:
      case GL_CONSTANT_ATTENUATION:
      case GL_LINEAR_ATTENUATION:
      case GL_QUADRATIC_ATTENUATION:
        comp = 1;
        break;
      default:
        return;
    }
    GLfloat v[4];
    for (int i = 0; i < comp; i++)
      v[ i ] = RFFToFloatN( i, param );
    ffstate.SetLight( this, light, pname, v );
  }

  template <typename T> void Material( GLenum face, GLenum pname, const T param )
  {
    int comp = 4;
    switch( pname )
    {
      case GL_AMBIENT:
      case GL_DIFFUSE:
      case GL_SPECULAR:
      case GL_AMBIENT_AND_DIFFUSE:
      case GL_EMISSION:
        break;
      case GL_SHININESS:
        comp = 1;
        break;
      default:
        return;
    }
    GLfloat v[4];
    for (int i = 0; i < comp; i++)
      v[ i ] = RFFToFloatN( i, param );
    ffstate.SetMaterial( this, face, pname, v );
  }

  template <typename T> void GetMaterial( GLenum face, GLenum pname, T *params )
  {
    size_t comp = 4;
    switch( pname )
    {
      case GL_AMBIENT:
      case GL_DIFFUSE:
      case GL_SPECULAR:
      case GL_EMISSION:
        break;
      case GL_SHININESS:
        comp = 1;
        break;
      default:
        return;
    }
    GLfloat v[4];
    ffstate.GetMaterial( this, face, pname, v );
    for (size_t i = 0; i < comp; i++)
      params[i] = static_cast<T>(v[i]);
  }

  template <typename T> void LightModel( GLenum pname, const T param )
  {
    State::Store & r = ffstate.raw;
    State::StoreUniform & u = ffstate.uniform;
    switch( pname )
    {
      case GL_LIGHT_MODEL_AMBIENT:
      {
        GLfloat *v = & u.lightModelAmbient.x;
        for (int i = 0; i < 4; i++)
          v[i] = RFFToFloatN( i, param );
      }
      break;
      case GL_LIGHT_MODEL_LOCAL_VIEWER:
        r.lightModelLocalViewer = RFFToFloat( 0, param ) != 0.0f;
        break;
      case GL_LIGHT_MODEL_TWO_SIDE:
        r.lightModelTwoSide = RFFToFloat( 0, param ) != 0.0f;
        break;
      case GL_LIGHT_MODEL_COLOR_CONTROL:
        r.lightModelSeparateSpecular = RFFToFloat(0, param) == GL_SEPARATE_SPECULAR_COLOR;
        break;
      default:
        return;
    }
    u.ver = r.ver = ver.Update();
  }

  void ColorMaterial( GLenum face, GLenum mode );

  template <typename T> bool TexGen( GLenum coord, GLenum pname, const T param )
  {
    RegalAssert(activeTextureIndex < REGAL_EMU_MAX_TEXTURE_UNITS);
    if (activeTextureIndex >= REGAL_EMU_MAX_TEXTURE_UNITS)
      return false;

    State::Store & st = ffstate.raw;
    int idx = 0;
    switch( coord )
    {
      case GL_S:
      case GL_T:
      case GL_R:
      case GL_Q:
        idx = coord - GL_S;
        break;
      default:
        RegalAssert(coord==GL_S || coord==GL_T || coord==GL_R || coord==GL_Q || coord==GL_S);
        return false;
    }
    switch( pname )
    {
      case GL_TEXTURE_GEN_MODE:
      {
        State::Texgen & tg = st.tex[ activeTextureIndex ].texgen[ idx ];
        switch( GLenum( RFFToFloat( 0, param ) ) )
        {
          case GL_OBJECT_LINEAR:
            tg.mode = TG_ObjectLinear;
            break;
          case GL_EYE_LINEAR:
            tg.mode = TG_EyeLinear;
            break;
          case GL_SPHERE_MAP:
            tg.mode = TG_SphereMap;
            break;
          case GL_NORMAL_MAP:
            tg.mode = TG_NormalMap;
            break;
          case GL_REFLECTION_MAP:
            tg.mode = TG_ReflectionMap;
            break;
          default:
            RegalAssert(0);
            return false;
        }
      }
      break;
      case GL_EYE_PLANE:
      case GL_OBJECT_PLANE:
      {
        if (!RFFIsVector( param ))
        {
          RegalAssert(0);
          return false;
        }
        Float4 plane;
        plane.x = RFFToFloat( 0, param );
        plane.y = RFFToFloat( 1, param );
        plane.z = RFFToFloat( 2, param );
        plane.w = RFFToFloat( 3, param );
        ffstate.SetTexgen( this, idx, pname, & plane.x );
        //<> dsn: is this 'return' right?  Shouldn't we be updating ver?
        return true;
      }
      break;
      default:
        RegalAssert(0);
        return false;
    }
    ffstate.uniform.ver = st.ver = ver.Update();
    return true;
  }

  void AlphaFunc( GLenum comp, const GLfloat ref );
  void ClipPlane( GLenum plane, const GLdouble * equation );

  template <typename T> void Fog( GLenum pname, const T param )
  {
    State::Store & r = ffstate.raw;
    State::StoreUniform & u = ffstate.uniform;
    switch( pname )
    {
      case GL_FOG_MODE:
      {
        FogMode m = FG_Exp;
        switch( GLenum( RFFToFloat( 0, param ) ) )
        {
          case GL_LINEAR:
            m = FG_Linear;
            break;
          case GL_EXP:
            m = FG_Exp;
            break;
          case GL_EXP2:
            m = FG_Exp2;
            break;
          default:
            return;
        }
        r.fog.mode = m;
      }
      break;
      case GL_FOG_DENSITY:
        u.fog.params[0].x = RFFToFloat( 0, param );
        u.fog.ver = ver.Update();
        break;
      case GL_FOG_START:
        u.fog.params[0].y = RFFToFloat( 0, param );
        u.fog.ver = ver.Update();
        break;
      case GL_FOG_END:
        u.fog.params[0].z = RFFToFloat( 0, param );
        u.fog.ver = ver.Update();
        break;
      case GL_FOG_COLOR:
        u.fog.params[1].x = RFFToFloat( 0, param );
        u.fog.params[1].y = RFFToFloat( 1, param );
        u.fog.params[1].z = RFFToFloat( 2, param );
        u.fog.params[1].w = RFFToFloat( 3, param );
        u.fog.ver = ver.Update();
        break;
      case GL_FOG_COORD_SRC:
      {
        bool d = true;
        switch( GLenum( RFFToFloat( 0, param ) ) )
        {
          case GL_FRAGMENT_DEPTH:
            d = true;
            break;
          case GL_FOG_COORD:
            d = false;
            break;
          default:
            return;
        }
        r.fog.useDepth = d;
      }
      break;
      default:
        return;
    }
    u.ver = r.ver = ver.Update();
  }

  template <typename T> bool GetIndexedTexGenv( RegalContext * ctx, GLuint textureIndex,
      GLenum coord, GLenum pname, T * params )
  {
    UNUSED_PARAMETER(ctx);

    if (textureIndex >= REGAL_EMU_MAX_TEXTURE_UNITS)
      return false;

    int idx = 0;
    switch( coord )
    {
      case GL_S:
      case GL_T:
      case GL_R:
      case GL_Q:
        idx = coord - GL_S;
        break;
      default:
        return false;
    }

    switch( pname )
    {
      case GL_TEXTURE_GEN_MODE:
      {
        GLenum glmode = GL_EYE_LINEAR;
        switch ( ffstate.raw.tex[ textureIndex ].texgen[ idx ].mode )
        {
          case TG_ObjectLinear:
            glmode = GL_OBJECT_LINEAR;
            break;
          case TG_EyeLinear:
            glmode = GL_EYE_LINEAR;
            break;
          case TG_SphereMap:
            glmode = GL_SPHERE_MAP;
            break;
          case TG_NormalMap:
            glmode = GL_NORMAL_MAP;
            break;
          case TG_ReflectionMap:
            glmode = GL_REFLECTION_MAP;
            break;
          default:
            return false;
        }
        *params = static_cast<T>(glmode);
      }
      break;
      case GL_OBJECT_PLANE:
      case GL_EYE_PLANE:
      {
        Float4 plane;
        ffstate.GetTexgen( this, idx, pname, & plane.x );
        *(params+0) = static_cast<T>(plane.x);
        *(params+1) = static_cast<T>(plane.y);
        *(params+2) = static_cast<T>(plane.z);
        *(params+3) = static_cast<T>(plane.w);
      }
      break;
      default:
        return false;
    }
    return true;
  }

  template <typename T> bool GetTexGenv( RegalContext * ctx, GLenum coord,
                                         GLenum pname, T * params )
  {
    return GetIndexedTexGenv( ctx, shadowActiveTextureIndex, coord, pname, params );
  }

  template <typename T> bool GetMultiTexGenv( RegalContext * ctx, GLenum texunit,
      GLenum coord, GLenum pname, T * params )
  {
    return GetIndexedTexGenv( ctx, texunit - GL_TEXTURE0, coord, pname, params );
  }

  bool glGetBooleanv( RegalContext * ctx, GLenum pname, GLboolean * param )
  {
    State::Store & r = ffstate.raw;
    State::StoreUniform & u = ffstate.uniform;

    // FIXME: implement all FF gets!

    GLint p;
    if (VaGet( ctx, pname, &p ))
    {
      *param = ((p == 0) ? GL_FALSE : GL_TRUE);
      return true;
    }

    if (IsEnabled( ctx, pname, *param ))
      return true;

    switch( pname )
    {
      case GL_MAX_VERTEX_ATTRIBS:
        *param = ((ctx->emuInfo->gl_max_vertex_attribs != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_MAX_TEXTURE_COORDS:
        *param = ((ctx->emuInfo->gl_max_texture_coords != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_MAX_TEXTURE_UNITS:
        *param = ((ctx->emuInfo->gl_max_texture_units != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_CURRENT_PROGRAM:
        *param = ((program != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_MAX_MODELVIEW_STACK_DEPTH:
      case GL_MAX_PROJECTION_STACK_DEPTH:
      case GL_MAX_TEXTURE_STACK_DEPTH:
        *param = ((REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_SMOOTH_POINT_SIZE_RANGE:
      case GL_SMOOTH_LINE_WIDTH_RANGE:
        // FIXME: Pass through actual GL's limit.
        *param = GL_TRUE;
        break;
      case GL_MODELVIEW_STACK_DEPTH:
        *param = ((modelview.size() != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_PROJECTION_STACK_DEPTH:
        *param = ((projection.size() != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_TEXTURE_STACK_DEPTH:
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        *param = ((texture[activeTextureIndex].size() != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_MODELVIEW_MATRIX:
        {
          const GLfloat * p = modelview.Top().Ptr();
          *param = GL_FALSE;
          for (size_t i = 0; i < 16; i++)
          {
            if (p[i] != 0)
            {
              *param = GL_TRUE;
              break;
            }
          }
        }
        break;
      case GL_PROJECTION_MATRIX:
        {
          const GLfloat * p = projection.Top().Ptr();
          *param = GL_FALSE;
          for (size_t i = 0; i < 16; i++)
          {
            if (p[i] != 0)
            {
              *param = GL_TRUE;
              break;
            }
          }
        }
        break;
      case GL_TEXTURE_MATRIX:
        {
          RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
          const GLfloat * p = texture[ activeTextureIndex ].Top().Ptr();
          *param = GL_FALSE;
          for (size_t i = 0; i < 16; i++)
          {
            if (p[i] != 0)
            {
              *param = GL_TRUE;
              break;
            }
          }
        }
        break;
      case GL_MATRIX_MODE:
        *param = ((shadowMatrixMode != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_MAX_LIGHTS:
        *param = ((REGAL_FIXED_FUNCTION_MAX_LIGHTS != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_MAX_CLIP_PLANES:
        *param = ((REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_FOG_MODE:
        *param = ((r.fog.mode != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_FOG_DENSITY:
        *param = ((u.fog.params[0].x != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_FOG_START:
        *param = ((u.fog.params[0].y != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_FOG_END:
        *param = ((u.fog.params[0].z != 0) ? GL_TRUE : GL_FALSE);
        break;
      case GL_FOG_COLOR:
        {
          const GLfloat * p = &u.fog.params[1].x;
          *param = GL_FALSE;
          for (size_t i = 0; i < 4; i++)
          {
            if (p[i] != 0)
            {
              *param = GL_TRUE;
              break;
            }
          }
        }
        break;
      default:
        return false;
    }
    return true;
  }

  template <typename T> bool Get( RegalContext * ctx, GLenum pname, T * params )
  {
    State::Store & r = ffstate.raw;
    State::StoreUniform & u = ffstate.uniform;

    // FIXME: implement all FF gets!

    if (VaGet( ctx, pname, params ))
      return true;

    GLboolean enabled = GL_FALSE;
    if (IsEnabled( ctx, pname, enabled ))
    {
      params[0] = static_cast<T>(enabled);
      return true;
    }

    switch( pname )
    {
      case GL_MAX_VERTEX_ATTRIBS:
        *params = static_cast<T>(ctx->emuInfo->gl_max_vertex_attribs);
        break;
      case GL_MAX_TEXTURE_COORDS:
        *params = static_cast<T>(ctx->emuInfo->gl_max_texture_coords);
        break;
      case GL_MAX_TEXTURE_UNITS:
        *params = static_cast<T>(ctx->emuInfo->gl_max_texture_units);
        break;
      case GL_CURRENT_PROGRAM:
        *params = static_cast<T>(program);
        break;
      case GL_MAX_MODELVIEW_STACK_DEPTH:
      case GL_MAX_PROJECTION_STACK_DEPTH:
      case GL_MAX_TEXTURE_STACK_DEPTH:
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH);
        break;
      case GL_SMOOTH_POINT_SIZE_RANGE:
        // FIXME: Pass through actual GL's limit.
        *params = static_cast<T>(1);
        break;
      case GL_SMOOTH_LINE_WIDTH_RANGE:
        // FIXME: Pass through actual GL's limit.
        *params = static_cast<T>(1);
        break;
      case GL_MODELVIEW_STACK_DEPTH:
        *params = static_cast<T>(modelview.size());
        break;
      case GL_PROJECTION_STACK_DEPTH:
        *params = static_cast<T>(projection.size());
        break;
      case GL_TEXTURE_STACK_DEPTH:
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        *params = static_cast<T>(texture[activeTextureIndex].size());
        break;
      case GL_MODELVIEW_MATRIX:
      {
        const GLfloat * p = modelview.Top().Ptr();
        for (size_t i = 0; i < 16; i++)
          params[i] = static_cast<T>(p[i]);
      }
      break;
      case GL_PROJECTION_MATRIX:
      {
        const GLfloat * p = projection.Top().Ptr();
        for (size_t i = 0; i < 16; i++)
          params[i] = static_cast<T>(p[i]);
      }
      break;
      case GL_TEXTURE_MATRIX:
      {
        RegalAssert(activeTextureIndex<REGAL_EMU_MAX_TEXTURE_UNITS);
        const GLfloat * p = texture[ activeTextureIndex ].Top().Ptr();
        for (size_t i = 0; i < 16; i++)
          params[i] = static_cast<T>(p[i]);
      }
      break;
      case GL_MATRIX_MODE:
        *params = static_cast<T>(shadowMatrixMode);
        break;
      case GL_MAX_LIGHTS:
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MAX_LIGHTS);
        break;
      case GL_MAX_CLIP_PLANES:
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES);
        break;
      case GL_FOG_MODE:
        *params = static_cast<T>(r.fog.mode);
        break;
      case GL_FOG_DENSITY:
        *params = static_cast<T>(u.fog.params[0].x);
        break;
      case GL_FOG_START:
        *params = static_cast<T>(u.fog.params[0].y);
        break;
      case GL_FOG_END:
        *params = static_cast<T>(u.fog.params[0].z);
        break;
      case GL_FOG_COLOR:
      {
        const GLfloat * p = &u.fog.params[1].x;
        for (size_t i = 0; i < 4; i++)
          params[i] = static_cast<T>(p[i]);
      }
      break;
      default:
        return false;
    }
    return true;
  }

  void MatrixPush( GLenum mode );
  void MatrixPop( GLenum mode );
  void UpdateMatrixVer();
  void MatrixLoadIdentity( GLenum mode );
  void MatrixLoad( GLenum mode, const r3::Matrix4f & m );
  void MatrixLoadTranspose( GLenum mode, const r3::Matrix4f & m );
  void MatrixMult( GLenum mode, const r3::Matrix4f & m );
  void MatrixMultTranspose( GLenum mode, const r3::Matrix4f & m );

  template <typename T> void MatrixRotate( GLenum mode, T angle, T x, T y, T z )
  {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Rotationf( r3::Vec3f( x, y, z ), r3::ToRadians( static_cast<float>(angle) ) ).GetMatrix4() );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixTranslate( GLenum mode, T x, T y, T z )
  {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Matrix4f::Translate( r3::Vec3f( x, y, z ) ) );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixScale( GLenum mode, T x, T y, T z )
  {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Matrix4f::Scale( r3::Vec3f( x, y, z ) ) );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixFrustum( GLenum mode, T left, T right, T bottom, T top, T zNear, T zFar )
  {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Frustum<float>( static_cast<float>(left), static_cast<float>(right), static_cast<float>(bottom), static_cast<float>(top), static_cast<float>(zNear), static_cast<float>(zFar) ) );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixOrtho( GLenum mode, T left, T right, T bottom, T top, T zNear, T zFar )
  {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Ortho<float>( static_cast<float>(left), static_cast<float>(right), static_cast<float>(bottom), static_cast<float>(top), static_cast<float>(zNear), static_cast<float>(zFar) ) );
    UpdateMatrixVer();
  }

  void PushMatrix();
  void PopMatrix();
  void LoadIdentity();
  void LoadMatrix( const r3::Matrix4f & m );
  void LoadTransposeMatrix( const r3::Matrix4f & m );
  void MultMatrix( const r3::Matrix4f & m );
  void MultTransposeMatrix( const r3::Matrix4f & m );

  template <typename T> void Rotate( T angle, T x, T y, T z )
  {
    MatrixRotate( shadowMatrixMode, angle, x, y, z );
  }

  template <typename T> void Translate( T x, T y, T z )
  {
    MatrixTranslate( shadowMatrixMode, x, y, z );
  }

  template <typename T> void Scale( T x, T y, T z )
  {
    MatrixScale( shadowMatrixMode, x, y, z );
  }

  template <typename T> void Frustum( T left, T right, T bottom, T top, T zNear, T zFar )
  {
    MatrixFrustum( shadowMatrixMode, left, right, bottom, top, zNear, zFar );
  }

  template <typename T> void Ortho( T left, T right, T bottom, T top, T zNear, T zFar )
  {
    MatrixOrtho( shadowMatrixMode, left, right, bottom, top, zNear, zFar );
  }

  // cache viewport
  struct Viewport
  {
    Viewport()
      : x(0)
      , y(0)
      , w(0)
      , h(0)
      , zn( 0.0f )
      , zf( 1.0f )
    {
    }

    GLint x, y;
    GLsizei w, h;
    GLfloat zn, zf;
  };

  Viewport viewport;

  void Viewport( GLint x, GLint y, GLsizei w, GLsizei h );
  void DepthRange( GLfloat znear, GLfloat zfar );

  template <typename T> void RasterPosition( RegalContext * ctx, T x, T y, T z = 0 )
  {
    const GLdouble xd = ToDouble<true>(x);
    const GLdouble yd = ToDouble<true>(y);
    const GLdouble zd = ToDouble<true>(z);
    RasterPos( ctx, xd, yd, zd );
  }

  template <typename T> void WindowPosition( RegalContext * ctx, T x, T y, T z = 0 )
  {
    const GLdouble xd = ToDouble<true>(x);
    const GLdouble yd = ToDouble<true>(y);
    const GLdouble zd = ToDouble<true>(z);
    WindowPos( ctx, xd, yd, zd );
  }

  void RasterPos( RegalContext * ctx, GLdouble x, GLdouble y, GLdouble z );
  void WindowPos( RegalContext * ctx, GLdouble x, GLdouble y, GLdouble z );
  void BindVertexArray( RegalContext * ctx, GLuint vao );
  void EnableArray( RegalContext * ctx, GLuint index );
  void DisableArray( RegalContext * ctx, GLuint index );
  void UpdateUniforms( RegalContext * ctx );
  void UseFixedFunctionProgram( RegalContext * ctx );
  void UseShaderProgram( RegalContext * ctx );
  void ShaderSource( RegalContext *ctx, GLuint shader, GLsizei count, const GLchar * const * string, const GLint *length);
  void LinkProgram( RegalContext *ctx, GLuint program );
  GLuint CreateShader( RegalContext *ctx, GLenum shaderType );
  void Init( RegalContext &ctx );
};

}; // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // __REGAL_FIXED_FUNCTION_H__
