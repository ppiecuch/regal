/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.4 --api wgl 4.4 --api glx 4.4 --api cgl 1.4 --api egl 1.0 --outdir .
*/

/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
  Copyright (c) 2012-2013 Google Inc.
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
  Intended formatting conventions:
  $ astyle --style=allman --indent=spaces=2 --indent-switches
*/

/*

 Regal emulation information and runtime emulation limits
 Nigel Stewart, Scott Nations

 */

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include "RegalEmu.h"
#include "RegalEmuInfo.h"

using namespace std;

#include <boost/print/string_list.hpp>
using namespace boost::print;

#include "RegalToken.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalEmu.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

EmuInfo::EmuInfo()
:
  gl_arb_draw_buffers(false),
  gl_arb_draw_elements_base_vertex(false),
  gl_arb_multitexture(false),
  gl_arb_sampler_objects(false),
  gl_arb_texture_cube_map(false),
  gl_arb_texture_env_combine(false),
  gl_arb_texture_env_dot3(false),
  gl_arb_texture_storage(false),
  gl_arb_vertex_array_object(false),
  gl_ati_draw_buffers(false),
  gl_ext_blend_color(false),
  gl_ext_blend_subtract(false),
  gl_ext_direct_state_access(false),
  gl_ext_framebuffer_blit(false),
  gl_ext_framebuffer_object(false),
  gl_ext_texture_cube_map(false),
  gl_ext_texture_edge_clamp(false),
  gl_ext_texture_env_combine(false),
  gl_ext_texture_env_dot3(false),
  gl_ibm_texture_mirrored_repeat(false),
  gl_nv_blend_square(false),
  gl_nv_path_rendering(false),

  gl_max_attrib_stack_depth(0),
  gl_max_client_attrib_stack_depth(0),
  gl_max_combined_texture_image_units(0),
  gl_max_debug_message_length(0),
  gl_max_draw_buffers(0),
  gl_max_texture_coords(0),
  gl_max_texture_units(0),
  gl_max_vertex_attrib_bindings(0),
  gl_max_vertex_attribs(0),
  gl_max_viewports(0),
  gl_max_varying_floats(0)

{
   Internal("EmuInfo::EmuInfo","()");
}

EmuInfo::~EmuInfo()
{
   Internal("EmuInfo::~EmuInfo","()");
}

void
EmuInfo::init(const ContextInfo &contextInfo)
{
  // TODO - filter out extensions Regal doesn't support?

#ifdef REGAL_GL_VENDOR
  vendor = REGAL_EQUOTE(REGAL_GL_VENDOR);
#else
  vendor = contextInfo.vendor;
#endif

#ifdef REGAL_GL_RENDERER
  renderer = REGAL_EQUOTE(REGAL_GL_RENDERER);
#else
  renderer = contextInfo.renderer;
#endif

#ifdef REGAL_GL_VERSION
  version = REGAL_EQUOTE(REGAL_GL_VERSION);
#else
  version = contextInfo.version;
#endif

#ifdef REGAL_GL_EXTENSIONS
  {
    string_list<string> extList;
    extList.split(REGAL_EQUOTE(REGAL_GL_EXTENSIONS),' ');
    extensionsSet.clear();
    extensionsSet.insert(extList.begin(),extList.end());
  }
#else
  static const char *ourExtensions[9] = {
    "GL_REGAL_log",
    "GL_REGAL_enable",
    "GL_REGAL_error_string",
    "GL_REGAL_extension_query",
    "GL_REGAL_ES1_0_compatibility",
    "GL_REGAL_ES1_1_compatibility",
    "GL_EXT_debug_marker",
    "GL_GREMEDY_string_marker",
    "GL_GREMEDY_frame_terminator"
  };
  extensionsSet = contextInfo.extensionsSet;
  extensionsSet.insert(&ourExtensions[0],&ourExtensions[9]);
#endif

#ifndef REGAL_NO_GETENV
  {
    getEnv("REGAL_GL_VENDOR",   vendor);
    getEnv("REGAL_GL_RENDERER", renderer);
    getEnv("REGAL_GL_VERSION",  version);

    const char *extensionsEnv = getEnv("REGAL_GL_EXTENSIONS");
    if (extensionsEnv)
    {
      string_list<string> extList;
      extList.split(extensionsEnv,' ');
      extensionsSet.clear();
      extensionsSet.insert(extList.begin(),extList.end());
    }
  }
#endif

  // Form Regal extension string from the set

  extensions = ::boost::print::detail::join(extensionsSet,string(" "));

  Info("Regal vendor     : ",vendor);
  Info("Regal renderer   : ",renderer);
  Info("Regal version    : ",version);
  Info("Regal extensions : ",extensions);

  gl_arb_draw_buffers                   = false;
  gl_arb_draw_elements_base_vertex      = false;
  gl_arb_multitexture                   = false;
  gl_arb_sampler_objects                = false;
  gl_arb_texture_cube_map               = false;
  gl_arb_texture_env_combine            = false;
  gl_arb_texture_env_dot3               = false;
  gl_arb_texture_storage                = false;
  gl_arb_vertex_array_object            = false;
  gl_ati_draw_buffers                   = false;
  gl_ext_blend_color                    = false;
  gl_ext_blend_subtract                 = false;
  gl_ext_direct_state_access            = false;
  gl_ext_framebuffer_blit               = false;
  gl_ext_framebuffer_object             = false;
  gl_ext_texture_cube_map               = false;
  gl_ext_texture_edge_clamp             = false;
  gl_ext_texture_env_combine            = false;
  gl_ext_texture_env_dot3               = false;
  gl_ibm_texture_mirrored_repeat        = false;
  gl_nv_blend_square                    = false;
  gl_nv_path_rendering                  = false;

  gl_max_vertex_attribs = 8;
  gl_max_attrib_stack_depth             = contextInfo.gl_max_attrib_stack_depth;
  gl_max_client_attrib_stack_depth      = contextInfo.gl_max_client_attrib_stack_depth;
  gl_max_combined_texture_image_units   = contextInfo.gl_max_combined_texture_image_units;
  gl_max_debug_message_length           = contextInfo.gl_max_debug_message_length;
  gl_max_draw_buffers                   = contextInfo.gl_max_draw_buffers;
  gl_max_texture_coords                 = contextInfo.gl_max_texture_coords;
  gl_max_texture_units                  = contextInfo.gl_max_texture_units;
  gl_max_vertex_attrib_bindings         = contextInfo.gl_max_vertex_attrib_bindings;
  gl_max_vertex_attribs                 = contextInfo.gl_max_vertex_attribs;
  gl_max_viewports                      = contextInfo.gl_max_viewports;

  if (gl_max_vertex_attribs > REGAL_EMU_MAX_VERTEX_ATTRIBS)
      gl_max_vertex_attribs = REGAL_EMU_MAX_VERTEX_ATTRIBS;

  // Qualcomm fails with float4 attribs with 256 byte stride, so artificially limit to 8 attribs (n*16 is used
  // as the stride in RegalIFF).  WebGL (and Pepper) explicitly disallows stride > 255 as well.

  if (contextInfo.vendor == "Qualcomm" || contextInfo.vendor == "Chromium" || contextInfo.webgl)
    gl_max_vertex_attribs = std::min<GLuint>(gl_max_vertex_attribs,8);

  Info("Regal  v attribs : ",gl_max_vertex_attribs);
}

bool
EmuInfo::getExtension(const ContextInfo &contextInfo, const char *ext) const
{
  Internal("EmuInfo::getExtension ",boost::print::quote(ext,'"'));

  // If the context supports it, we're done.

  if (contextInfo.getExtension(ext))
    return true;

  if (!strcmp(ext,"GL_ARB_draw_buffers"))               return gl_arb_draw_buffers;
  if (!strcmp(ext,"GL_ARB_draw_elements_base_vertex"))  return gl_arb_draw_elements_base_vertex;
  if (!strcmp(ext,"GL_ARB_multitexture"))               return gl_arb_multitexture;
  if (!strcmp(ext,"GL_ARB_sampler_objects"))            return gl_arb_sampler_objects;
  if (!strcmp(ext,"GL_ARB_texture_cube_map"))           return gl_arb_texture_cube_map;
  if (!strcmp(ext,"GL_ARB_texture_env_combine"))        return gl_arb_texture_env_combine;
  if (!strcmp(ext,"GL_ARB_texture_env_dot3"))           return gl_arb_texture_env_dot3;
  if (!strcmp(ext,"GL_ARB_texture_storage"))            return gl_arb_texture_storage;
  if (!strcmp(ext,"GL_ARB_vertex_array_object"))        return gl_arb_vertex_array_object;
  if (!strcmp(ext,"GL_ATI_draw_buffers"))               return gl_ati_draw_buffers;
  if (!strcmp(ext,"GL_EXT_blend_color"))                return gl_ext_blend_color;
  if (!strcmp(ext,"GL_EXT_blend_subtract"))             return gl_ext_blend_subtract;
  if (!strcmp(ext,"GL_EXT_debug_marker"))               return true;
  if (!strcmp(ext,"GL_EXT_direct_state_access"))        return gl_ext_direct_state_access;
  if (!strcmp(ext,"GL_EXT_framebuffer_blit"))           return gl_ext_framebuffer_blit;
  if (!strcmp(ext,"GL_EXT_framebuffer_object"))         return gl_ext_framebuffer_object;
  if (!strcmp(ext,"GL_EXT_texture_cube_map"))           return gl_ext_texture_cube_map;
  if (!strcmp(ext,"GL_EXT_texture_edge_clamp"))         return gl_ext_texture_edge_clamp;
  if (!strcmp(ext,"GL_EXT_texture_env_combine"))        return gl_ext_texture_env_combine;
  if (!strcmp(ext,"GL_EXT_texture_env_dot3"))           return gl_ext_texture_env_dot3;
  if (!strcmp(ext,"GL_IBM_texture_mirrored_repeat"))    return gl_ibm_texture_mirrored_repeat;
  if (!strcmp(ext,"GL_NV_blend_square"))                return gl_nv_blend_square;
  if (!strcmp(ext,"GL_NV_path_rendering"))              return gl_nv_path_rendering;
  if (!strcmp(ext,"GL_REGAL_ES1_0_compatibility"))      return true;
  if (!strcmp(ext,"GL_REGAL_ES1_1_compatibility"))      return true;
  if (!strcmp(ext,"GL_REGAL_enable"))                   return true;
  if (!strcmp(ext,"GL_REGAL_error_string"))             return true;
  if (!strcmp(ext,"GL_REGAL_extension_query"))          return true;
  if (!strcmp(ext,"GL_REGAL_log"))                      return true;

  return false;
}

bool
EmuInfo::isSupported(const ContextInfo &contextInfo, const char *ext) const
{
  Internal("EmuInfo::isSupported ",boost::print::quote(ext,'"'));

  string_list<string> e;
  e.split(ext,' ');
  for (string_list<string>::const_iterator i=e.begin(); i!=e.end(); ++i)
    if (i->length() && !getExtension(contextInfo, i->c_str())) return false;
  return true;
}

REGAL_NAMESPACE_END
