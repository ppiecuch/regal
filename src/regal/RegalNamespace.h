/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012-2013 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
  Copyright (c) 2012 Google Inc.
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

#ifndef REGAL_NAMESPACE_H
#define REGAL_NAMESPACE_H

#define REGAL_NAMESPACE_INTERNAL Regal

// REGAL_GLOBAL_BEGIN and REGAL_GLOBAL_END for scoping externals
// such as C, C++ and boost includes.

// VC8, VC9  - C4127: conditional expression is constant in std::list
// VC10      - C4512:
// VC9, VC10 - C4996: 'vsprintf': This function or variable may be unsafe
// VC10      - C4127: conditional expression is constant
// VC10      - C4706: assignment within conditional expression

#ifdef _MSC_VER
# define REGAL_GLOBAL_BEGIN         \
  __pragma(pack(push))              \
  __pragma(pack(8))                 \
  __pragma(warning(push))           \
  __pragma(warning(disable : 4127)) \
  __pragma(warning(disable : 4512)) \
  __pragma(warning(disable : 4996)) \
  __pragma(warning(disable : 4706))
# define REGAL_GLOBAL_END           \
  __pragma(warning(pop))            \
  __pragma(pack(pop))
#else
# define REGAL_GLOBAL_BEGIN
# define REGAL_GLOBAL_END
#endif

// REGAL_NAMESPACE_BEGIN and REGAL_NAMESPACE_END for scoping Regal internals

#ifdef _MSC_VER
# define REGAL_NAMESPACE_BEGIN         \
  __pragma(pack(push))                 \
  __pragma(pack(8))                    \
  __pragma(warning(push))              \
  __pragma(warning(disable : 4996))    \
  __pragma(warning(disable : 4127))    \
  __pragma(warning(disable : 4706))    \
  namespace REGAL_NAMESPACE_INTERNAL   \
  {
# define REGAL_NAMESPACE_END           \
  }                                    \
  __pragma(warning(pop))               \
  __pragma(pack(pop))
#else
# define REGAL_NAMESPACE_BEGIN         \
  namespace REGAL_NAMESPACE_INTERNAL   \
  {
# define REGAL_NAMESPACE_END           \
  }
#endif

#endif
