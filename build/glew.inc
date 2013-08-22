# glew.inc
#
# Generic gnumake .inc for building GLEW
#

# Sources

GLEW.C += src/glew/src/glew.c

#

GLEW.CFLAGS      := -Isrc/glew/include -Isrc/glu/include -DGLEW_EXPORTS -DGLEW_BUILD -DGLEW_REGAL

GLEW.SHARED      := libRegalGLEW.so
GLEW.STATIC      := libRegalGLEWlib.a

ifneq ($(filter darwin%,$(SYSTEM)),)
GLEW.SHARED     := libRegalGLEW.dylib
endif

ifneq ($(filter nacl%,$(SYSTEM)),)
GLEW.SHARED     :=
endif
