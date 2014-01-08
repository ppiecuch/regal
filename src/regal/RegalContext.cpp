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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalContext.h"
#include "RegalEmuInfo.h"
#include "RegalDebugInfo.h"
#include "RegalContextInfo.h"
#include "RegalStatistics.h"

#include "RegalMarker.h"
#include "RegalFrame.h"
#if REGAL_EMULATION
#include "RegalObj.h"
#include "RegalHint.h"
#include "RegalPpa.h"
#include "RegalPpca.h"
#include "RegalBin.h"
#include "RegalXfer.h"
#include "RegalTexSto.h"
#include "RegalBaseVertex.h"
#include "RegalRect.h"
#include "RegalIff.h"
#include "RegalQuads.h"
#include "RegalSo.h"
#include "RegalDsa.h"
#include "RegalVao.h"
#include "RegalTexC.h"
#include "RegalFilt.h"
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace Logging;

RegalContext::RegalContext()
: initialized(false),
  dispatcher(),
  dbg(NULL),
  info(NULL),
#if REGAL_STATISTICS
  statistics(new Statistics()),
#endif
  marker(NULL),
#if REGAL_FRAME
  frame(NULL),
#endif /* REGAL_FRAME */
#if REGAL_EMULATION
  emuLevel(0),
  obj(NULL),
  hint(NULL),
  ppa(NULL),
  ppca(NULL),
  bin(NULL),
  xfer(NULL),
  texsto(NULL),
  bv(NULL),
  rect(NULL),
  iff(NULL),
  quads(NULL),
  so(NULL),
  dsa(NULL),
  vao(NULL),
  texc(NULL),
  filt(NULL),
#endif
#if REGAL_SYS_PPAPI
  ppapiES2(NULL),
  ppapiResource(0),
  sysCtx(0),
#else
  sysCtx(NULL),
#endif
  thread(0),
#if REGAL_SYS_X11
  x11Display(NULL),
#endif
#if REGAL_SYS_GLX
  x11Drawable(0),
#endif
  logCallback(NULL),
#if REGAL_CODE
  codeSource(NULL),
  codeHeader(NULL),
  codeInputNext(0),
  codeOutputNext(0),
  codeShaderNext(0),
  codeProgramNext(0),
  codeTextureNext(0),
#endif
  depthBeginEnd(0),
  depthPushMatrix(0),
  depthPushAttrib(0),
  depthNewList(0)
{
  Internal("RegalContext::RegalContext","()");

  if (Config::enableDebug)
  {
    dbg = new DebugInfo();
    dbg->Init(this);
  }

  shareGroup.push_back(this);
}

void
RegalContext::Init()
{
  Internal("RegalContext::Init","()");

  RegalAssert(!initialized);

  RegalAssert(this);
  if (!info)
  {
    info = new ContextInfo();
    RegalAssert(info);
    info->init(*this);
  }

  if (!emuInfo)
  {
    emuInfo = new EmuInfo();
    RegalAssert(emuInfo);
    emuInfo->init(*info);
  }

  if (!marker)
  {
    marker = new Marker;
  }
  #if REGAL_FRAME
  if (!frame)
  {
    frame = new Frame;
  }
  #endif /* REGAL_FRAME */

#if REGAL_EMULATION
  const bool forceEmuObj     = Config::forceEmuObj        || REGAL_FORCE_EMU_OBJ;
  const bool forceEmuHint    = Config::forceEmuHint       || REGAL_FORCE_EMU_HINT;
  const bool forceEmuPpa     = Config::forceEmuPpa        || REGAL_FORCE_EMU_PPA;
  const bool forceEmuPpca    = Config::forceEmuPpca       || REGAL_FORCE_EMU_PPCA;
  const bool forceEmuBin     = Config::forceEmuBin        || REGAL_FORCE_EMU_BIN;
  const bool forceEmuXfer    = Config::forceEmuXfer       || REGAL_FORCE_EMU_XFER;
  const bool forceEmuTexsto  = Config::forceEmuTexSto     || REGAL_FORCE_EMU_TEXSTO;
  const bool forceEmuBv      = Config::forceEmuBaseVertex || REGAL_FORCE_EMU_BASEVERTEX;
  const bool forceEmuRect    = Config::forceEmuRect       || REGAL_FORCE_EMU_RECT;
  const bool forceEmuIff     = Config::forceEmuIff        || REGAL_FORCE_EMU_IFF;
  const bool forceEmuQuads   = Config::forceEmuQuads      || REGAL_FORCE_EMU_QUADS;
  const bool forceEmuSo      = Config::forceEmuSo         || REGAL_FORCE_EMU_SO;
  const bool forceEmuDsa     = Config::forceEmuDsa        || REGAL_FORCE_EMU_OBJ;
  const bool forceEmuVao     = Config::forceEmuVao        || REGAL_FORCE_EMU_DSA;
  const bool forceEmuTexc    = Config::forceEmuTexC       || REGAL_FORCE_EMU_TEXC;
  const bool forceEmuFilt    = Config::forceEmuFilter     || REGAL_FORCE_EMU_FILTER;

  bool enableEmuObj    = forceEmuObj     || (Config::enableEmuObj);
  bool enableEmuHint   = forceEmuHint    || (Config::enableEmuHint);
  bool enableEmuPpa    = forceEmuPpa     || (Config::enableEmuPpa);
  bool enableEmuPpca   = forceEmuPpca    || (Config::enableEmuPpca);
  bool enableEmuBin    = forceEmuBin     || (Config::enableEmuBin);
  bool enableEmuXfer   = forceEmuXfer    || (Config::enableEmuXfer);
  bool enableEmuTexsto = forceEmuTexsto  || (Config::enableEmuTexSto);
  bool enableEmuBv     = forceEmuBv      || (Config::enableEmuBaseVertex);
  bool enableEmuRect   = forceEmuRect    || (Config::enableEmuRect);
  bool enableEmuIff    = forceEmuIff     || (Config::enableEmuIff);
  bool enableEmuQuads  = forceEmuQuads   || (Config::enableEmuQuads);
  bool enableEmuSo     = forceEmuSo      || (Config::enableEmuSo);
  bool enableEmuDsa    = forceEmuDsa     || (Config::enableEmuDsa);
  bool enableEmuVao    = forceEmuVao     || (Config::enableEmuVao);
  bool enableEmuTexc   = forceEmuTexc    || (Config::enableEmuTexC);
  bool enableEmuFilt   = forceEmuFilt    || (Config::enableEmuFilter);

  const bool forceAnyEmu     = forceEmuObj || forceEmuHint || forceEmuPpa || forceEmuPpca || forceEmuBin || forceEmuXfer || forceEmuTexsto || forceEmuBv || forceEmuRect || forceEmuIff || forceEmuQuads || forceEmuSo || forceEmuDsa || forceEmuVao || forceEmuTexc || forceEmuFilt || false;

  if (!Config::forceEmulation)
  {
    // Disable ES 2.0 - specific layers, as necessary

    if (!isES2())
    {
      enableEmuXfer = false;
      enableEmuTexc = false;
    }

    // Disable emulated sampler objects, if possible

    if (info->gl_arb_sampler_objects)
      enableEmuSo = false;

#if REGAL_EMU_PATH
    // Path rendering needs gp4 or gp5, for now

    if (!info->gl_nv_gpu_program4 && !info->gl_nv_gpu_program5)
      enableEmuPath = false;

    // Disable emulated path rendering, if possible

    if (info->gl_nv_path_rendering)
      enableEmuPath = false;
#endif

    // Disable emulated DSA, if possible

    if (info->gl_ext_direct_state_access)
      enableEmuDsa = false;

    // Vao needs Iff

    if (!enableEmuIff)
      enableEmuVao = false;

    // Disable emulated fixed-function, etc, for compatibility contexts

    if (isCompat())
    {
      enableEmuObj  = false;
      enableEmuHint = false;
      enableEmuPpa  = false;
      enableEmuPpca = false;
      enableEmuBin  = false;
      enableEmuXfer = false;
      enableEmuRect = false;
      enableEmuIff  = false;
    }

  }

  bool enableAnyEmu    = enableEmuObj || enableEmuHint || enableEmuPpa || enableEmuPpca || enableEmuBin || enableEmuXfer || enableEmuTexsto || enableEmuBv || enableEmuRect || enableEmuIff || enableEmuQuads || enableEmuSo || enableEmuDsa || enableEmuVao || enableEmuTexc || enableEmuFilt || false;

  // Enable emulation except for fully featured compatibility contexts

  const bool enableEmulation = Config::enableEmulation && (isCore() || isES2() || (isCompat() && !info->gl_ext_direct_state_access));

#if !REGAL_FORCE_EMULATION
  if (Config::forceEmulation || (enableEmulation && enableAnyEmu) || forceAnyEmu)
#endif
  {
    // emu
    emuLevel = 16;
    #if REGAL_EMU_FILTER
    if (enableEmuFilt || forceEmuFilt)
    {
      Info("Activating emulation layer REGAL_EMU_FILTER");
      if (!info->gl_arb_draw_buffers && ((info->gl_version_major >= 2) || info->gl_nv_draw_buffers))
      {
        Info("Activating ARB_draw_buffers emulation.");
        emuInfo->gl_arb_draw_buffers = true;
        emuInfo->extensionsSet.insert("GL_ARB_draw_buffers");
      }
      if (!info->gl_arb_multitexture)
      {
        Info("Activating ARB_multitexture emulation.");
        emuInfo->gl_arb_multitexture = true;
        emuInfo->extensionsSet.insert("GL_ARB_multitexture");
      }
      if (!info->gl_arb_texture_cube_map)
      {
        Info("Activating ARB_texture_cube_map emulation.");
        emuInfo->gl_arb_texture_cube_map = true;
        emuInfo->extensionsSet.insert("GL_ARB_texture_cube_map");
      }
      if (!info->gl_ati_draw_buffers && ((info->gl_version_major >= 2) || info->gl_nv_draw_buffers))
      {
        Info("Activating ATI_draw_buffers emulation.");
        emuInfo->gl_ati_draw_buffers = true;
        emuInfo->extensionsSet.insert("GL_ATI_draw_buffers");
      }
      if (!info->gl_ext_blend_color)
      {
        Info("Activating EXT_blend_color emulation.");
        emuInfo->gl_ext_blend_color = true;
        emuInfo->extensionsSet.insert("GL_EXT_blend_color");
      }
      if (!info->gl_ext_blend_subtract)
      {
        Info("Activating EXT_blend_subtract emulation.");
        emuInfo->gl_ext_blend_subtract = true;
        emuInfo->extensionsSet.insert("GL_EXT_blend_subtract");
      }
      if (!info->gl_ext_framebuffer_blit && ((info->gl_version_major >= 3) || info->gl_nv_framebuffer_blit))
      {
        Info("Activating EXT_framebuffer_blit emulation.");
        emuInfo->gl_ext_framebuffer_blit = true;
        emuInfo->extensionsSet.insert("GL_EXT_framebuffer_blit");
      }
      if (!info->gl_ext_framebuffer_object)
      {
        Info("Activating EXT_framebuffer_object emulation.");
        emuInfo->gl_ext_framebuffer_object = true;
        emuInfo->extensionsSet.insert("GL_EXT_framebuffer_object");
      }
      if (!info->gl_ext_texture_cube_map)
      {
        Info("Activating EXT_texture_cube_map emulation.");
        emuInfo->gl_ext_texture_cube_map = true;
        emuInfo->extensionsSet.insert("GL_EXT_texture_cube_map");
      }
      if (!info->gl_ext_texture_edge_clamp)
      {
        Info("Activating EXT_texture_edge_clamp emulation.");
        emuInfo->gl_ext_texture_edge_clamp = true;
        emuInfo->extensionsSet.insert("GL_EXT_texture_edge_clamp");
      }
      if (!info->gl_ibm_texture_mirrored_repeat)
      {
        Info("Activating IBM_texture_mirrored_repeat emulation.");
        emuInfo->gl_ibm_texture_mirrored_repeat = true;
        emuInfo->extensionsSet.insert("GL_IBM_texture_mirrored_repeat");
      }
      if (!info->gl_nv_blend_square)
      {
        Info("Activating NV_blend_square emulation.");
        emuInfo->gl_nv_blend_square = true;
        emuInfo->extensionsSet.insert("GL_NV_blend_square");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      filt = new Emu::Filt;
      emuLevel = 0;
      filt->Init(*this);
    }
    #endif /* REGAL_EMU_FILTER */
    #if REGAL_EMU_TEXC
    if (enableEmuTexc || forceEmuTexc)
    {
      Info("Activating emulation layer REGAL_EMU_TEXC");
      texc = new Emu::TexC;
      emuLevel = 1;
      texc->Init(*this);
    }
    #endif /* REGAL_EMU_TEXC */
    #if REGAL_EMU_VAO
    if (enableEmuVao || forceEmuVao)
    {
      Info("Activating emulation layer REGAL_EMU_VAO");
      if (!info->gl_arb_vertex_array_object)
      {
        Info("Activating ARB_vertex_array_object emulation.");
        emuInfo->gl_arb_vertex_array_object = true;
        emuInfo->extensionsSet.insert("GL_ARB_vertex_array_object");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      vao = new Emu::Vao;
      emuLevel = 2;
      vao->Init(*this);
    }
    #endif /* REGAL_EMU_VAO */
    #if REGAL_EMU_DSA
    if (enableEmuDsa || forceEmuDsa)
    {
      Info("Activating emulation layer REGAL_EMU_DSA");
      if (!info->gl_ext_direct_state_access)
      {
        Info("Activating EXT_direct_state_access emulation.");
        emuInfo->gl_ext_direct_state_access = true;
        emuInfo->extensionsSet.insert("GL_EXT_direct_state_access");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      dsa = new Emu::Dsa;
      emuLevel = 3;
      dsa->Init(*this);
    }
    #endif /* REGAL_EMU_DSA */
    #if REGAL_EMU_SO
    if (enableEmuSo || forceEmuSo)
    {
      Info("Activating emulation layer REGAL_EMU_SO");
      if (!info->gl_arb_sampler_objects)
      {
        Info("Activating ARB_sampler_objects emulation.");
        emuInfo->gl_arb_sampler_objects = true;
        emuInfo->extensionsSet.insert("GL_ARB_sampler_objects");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      so = new Emu::So;
      emuLevel = 4;
      so->Init(*this);
    }
    #endif /* REGAL_EMU_SO */
    #if REGAL_EMU_QUADS
    if (enableEmuQuads || forceEmuQuads)
    {
      Info("Activating emulation layer REGAL_EMU_QUADS");
      quads = new Emu::Quads;
      emuLevel = 5;
      quads->Init(*this);
    }
    #endif /* REGAL_EMU_QUADS */
    #if REGAL_EMU_IFF
    if (enableEmuIff || forceEmuIff)
    {
      Info("Activating emulation layer REGAL_EMU_IFF");
      if (!info->gl_arb_texture_env_combine)
      {
        Info("Activating ARB_texture_env_combine emulation.");
        emuInfo->gl_arb_texture_env_combine = true;
        emuInfo->extensionsSet.insert("GL_ARB_texture_env_combine");
      }
      if (!info->gl_arb_texture_env_dot3)
      {
        Info("Activating ARB_texture_env_dot3 emulation.");
        emuInfo->gl_arb_texture_env_dot3 = true;
        emuInfo->extensionsSet.insert("GL_ARB_texture_env_dot3");
      }
      if (!info->gl_ext_texture_env_combine)
      {
        Info("Activating EXT_texture_env_combine emulation.");
        emuInfo->gl_ext_texture_env_combine = true;
        emuInfo->extensionsSet.insert("GL_EXT_texture_env_combine");
      }
      if (!info->gl_ext_texture_env_dot3)
      {
        Info("Activating EXT_texture_env_dot3 emulation.");
        emuInfo->gl_ext_texture_env_dot3 = true;
        emuInfo->extensionsSet.insert("GL_EXT_texture_env_dot3");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      iff = new Emu::Iff;
      emuLevel = 6;
      iff->Init(*this);
    }
    #endif /* REGAL_EMU_IFF */
    #if REGAL_EMU_RECT
    if (enableEmuRect || forceEmuRect)
    {
      Info("Activating emulation layer REGAL_EMU_RECT");
      rect = new Emu::Rect;
      emuLevel = 7;
      rect->Init(*this);
    }
    #endif /* REGAL_EMU_RECT */
    #if REGAL_EMU_BASEVERTEX
    if (enableEmuBv || forceEmuBv)
    {
      Info("Activating emulation layer REGAL_EMU_BASEVERTEX");
      if (!info->gl_arb_draw_elements_base_vertex)
      {
        Info("Activating ARB_draw_elements_base_vertex emulation.");
        emuInfo->gl_arb_draw_elements_base_vertex = true;
        emuInfo->extensionsSet.insert("GL_ARB_draw_elements_base_vertex");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      bv = new Emu::BaseVertex;
      emuLevel = 8;
      bv->Init(*this);
    }
    #endif /* REGAL_EMU_BASEVERTEX */
    #if REGAL_EMU_TEXSTO
    if (enableEmuTexsto || forceEmuTexsto)
    {
      Info("Activating emulation layer REGAL_EMU_TEXSTO");
      if (!info->gl_arb_texture_storage)
      {
        Info("Activating ARB_texture_storage emulation.");
        emuInfo->gl_arb_texture_storage = true;
        emuInfo->extensionsSet.insert("GL_ARB_texture_storage");
      }
      emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));
      texsto = new Emu::TexSto;
      emuLevel = 9;
      texsto->Init(*this);
    }
    #endif /* REGAL_EMU_TEXSTO */
    #if REGAL_EMU_XFER
    if (enableEmuXfer || forceEmuXfer)
    {
      Info("Activating emulation layer REGAL_EMU_XFER");
      xfer = new Emu::Xfer;
      emuLevel = 10;
      xfer->Init(*this);
    }
    #endif /* REGAL_EMU_XFER */
    #if REGAL_EMU_BIN
    if (enableEmuBin || forceEmuBin)
    {
      Info("Activating emulation layer REGAL_EMU_BIN");
      bin = new Emu::Bin;
      emuLevel = 11;
      bin->Init(*this);
    }
    #endif /* REGAL_EMU_BIN */
    #if REGAL_EMU_PPCA
    if (enableEmuPpca || forceEmuPpca)
    {
      Info("Activating emulation layer REGAL_EMU_PPCA");
      ppca = new Emu::Ppca;
      emuLevel = 12;
      ppca->Init(*this);
    }
    #endif /* REGAL_EMU_PPCA */
    #if REGAL_EMU_PPA
    if (enableEmuPpa || forceEmuPpa)
    {
      Info("Activating emulation layer REGAL_EMU_PPA");
      ppa = new Emu::Ppa;
      emuLevel = 13;
      ppa->Init(*this);
    }
    #endif /* REGAL_EMU_PPA */
    #if REGAL_EMU_HINT
    if (enableEmuHint || forceEmuHint)
    {
      Info("Activating emulation layer REGAL_EMU_HINT");
      hint = new Emu::Hint;
      emuLevel = 14;
      hint->Init(*this);
    }
    #endif /* REGAL_EMU_HINT */
    #if REGAL_EMU_OBJ
    if (enableEmuObj || forceEmuObj)
    {
      Info("Activating emulation layer REGAL_EMU_OBJ");
      obj = new Emu::Obj;
      emuLevel = 15;
      obj->Init(*this);
    }
    #endif /* REGAL_EMU_OBJ */
    emuLevel = 16;

  }
#endif

#if REGAL_CODE
  if (Config::enableCode && !codeSource && !codeHeader)
  {
    if (Config::codeSourceFile.length())
    {
      codeSource = fopen(Config::codeSourceFile.c_str(),"wt");
      if (!codeSource)
        Warning("Failed to open file ",Config::codeSourceFile," for writing code source.");
    }
    if (Config::codeHeaderFile.length())
    {
      if (Config::codeHeaderFile==Config::codeSourceFile)
        codeHeader = codeSource;
      else
        codeHeader = fopen(Config::codeHeaderFile.c_str(),"wt");
      if (!codeHeader)
        Warning("Failed to open file ",Config::codeHeaderFile," for writing code header.");
    }
  }
#endif

  initialized = true;
}

// Note that Cleanup() may or may not have been called prior to destruction
RegalContext::~RegalContext()
{
  Internal("RegalContext::~RegalContext","()");

  #if REGAL_STATISTICS
  if (statistics && !Logging::frameStatistics)
  {
    statistics->log();
    statistics->reset();
  }
  #endif

  // Remove this context from the share group.

  shareGroup->remove(this);

#if REGAL_CODE
  if (codeSource)
    fclose(codeSource);

  if (codeHeader)
    fclose(codeHeader);
#endif
}

// Called prior to deletion, if this context is still set for this thread.
// Need to:
// 1) clean up GL state we've modified
// 2) leave the RegalContext in a state where Init() could be called again
void
RegalContext::Cleanup()
{
  Internal("RegalContext::Cleanup","()");

#if REGAL_EMULATION
  // emu
  #if REGAL_EMU_OBJ
  if (obj)
  {
    emuLevel = 15;
    obj->Cleanup(*this);
    obj.reset(NULL);
  }
  #endif /* REGAL_EMU_OBJ */
  #if REGAL_EMU_HINT
  if (hint)
  {
    emuLevel = 14;
    hint->Cleanup(*this);
    hint.reset(NULL);
  }
  #endif /* REGAL_EMU_HINT */
  #if REGAL_EMU_PPA
  if (ppa)
  {
    emuLevel = 13;
    ppa->Cleanup(*this);
    ppa.reset(NULL);
  }
  #endif /* REGAL_EMU_PPA */
  #if REGAL_EMU_PPCA
  if (ppca)
  {
    emuLevel = 12;
    ppca->Cleanup(*this);
    ppca.reset(NULL);
  }
  #endif /* REGAL_EMU_PPCA */
  #if REGAL_EMU_BIN
  if (bin)
  {
    emuLevel = 11;
    bin->Cleanup(*this);
    bin.reset(NULL);
  }
  #endif /* REGAL_EMU_BIN */
  #if REGAL_EMU_XFER
  if (xfer)
  {
    emuLevel = 10;
    xfer->Cleanup(*this);
    xfer.reset(NULL);
  }
  #endif /* REGAL_EMU_XFER */
  #if REGAL_EMU_TEXSTO
  if (texsto)
  {
    emuLevel = 9;
    texsto->Cleanup(*this);
    texsto.reset(NULL);
  }
  #endif /* REGAL_EMU_TEXSTO */
  #if REGAL_EMU_BASEVERTEX
  if (bv)
  {
    emuLevel = 8;
    bv->Cleanup(*this);
    bv.reset(NULL);
  }
  #endif /* REGAL_EMU_BASEVERTEX */
  #if REGAL_EMU_RECT
  if (rect)
  {
    emuLevel = 7;
    rect->Cleanup(*this);
    rect.reset(NULL);
  }
  #endif /* REGAL_EMU_RECT */
  #if REGAL_EMU_IFF
  if (iff)
  {
    emuLevel = 6;
    iff->Cleanup(*this);
    iff.reset(NULL);
  }
  #endif /* REGAL_EMU_IFF */
  #if REGAL_EMU_QUADS
  if (quads)
  {
    emuLevel = 5;
    quads->Cleanup(*this);
    quads.reset(NULL);
  }
  #endif /* REGAL_EMU_QUADS */
  #if REGAL_EMU_SO
  if (so)
  {
    emuLevel = 4;
    so->Cleanup(*this);
    so.reset(NULL);
  }
  #endif /* REGAL_EMU_SO */
  #if REGAL_EMU_DSA
  if (dsa)
  {
    emuLevel = 3;
    dsa->Cleanup(*this);
    dsa.reset(NULL);
  }
  #endif /* REGAL_EMU_DSA */
  #if REGAL_EMU_VAO
  if (vao)
  {
    emuLevel = 2;
    vao->Cleanup(*this);
    vao.reset(NULL);
  }
  #endif /* REGAL_EMU_VAO */
  #if REGAL_EMU_TEXC
  if (texc)
  {
    emuLevel = 1;
    texc->Cleanup(*this);
    texc.reset(NULL);
  }
  #endif /* REGAL_EMU_TEXC */
  #if REGAL_EMU_FILTER
  if (filt)
  {
    emuLevel = 0;
    filt->Cleanup(*this);
    filt.reset(NULL);
  }
  #endif /* REGAL_EMU_FILTER */
#endif

  initialized = false;
}

bool
RegalContext::groupInitialized() const
{
  Internal("RegalContext::groupInitialized","()");

  for (shared_list<RegalContext *>::const_iterator i = shareGroup.begin(); i!=shareGroup.end(); ++i)
  {
    RegalAssert(*i);
    if ((*i)->initialized)
      return true;
  }

  return false;
}

RegalContext *
RegalContext::groupInitializedContext()
{
  Internal("RegalContext::groupInitializedContext","()");

  // Look for any initialized context in the share group.
  // The only way this would be expected to fail is if none
  // of the contexts have been made current, triggering
  // initialization.
  //
  // Note - linear search, but shouldn't need to look at too many
  // contexts in the share group.

  for (shared_list<RegalContext *>::iterator i = shareGroup.begin(); i!=shareGroup.end(); ++i)
  {
    RegalAssert(*i);
    if ((*i)->initialized)
      return *i;
  }

  return NULL;
}

REGAL_NAMESPACE_END
