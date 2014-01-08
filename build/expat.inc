# expat.inc
#
# Generic gnumake .inc for building expat support into Regal
# http://expat.sourceforge.net/
#

# expat sources

EXPAT.C :=
EXPAT.C += src/expat/src/xmlparse.c
EXPAT.C += src/expat/src/xmlrole.c
EXPAT.C += src/expat/src/xmltok.c
	
EXPAT.H :=
EXPAT.H += src/expat/src/amigaconfig.h
EXPAT.H += src/expat/src/ascii.h
EXPAT.H += src/expat/src/asciitab.h
EXPAT.H += src/expat/src/iasciitab.h
EXPAT.H += src/expat/src/internal.h
EXPAT.H += src/expat/src/latin1tab.h
EXPAT.H += src/expat/src/macconfig.h
EXPAT.H += src/expat/src/nametab.h
EXPAT.H += src/expat/src/utf8tab.h
EXPAT.H += src/expat/src/winconfig.h
EXPAT.H += src/expat/src/xmlrole.h
EXPAT.H += src/expat/src/xmltok.h
EXPAT.H += src/expat/src/xmltok_impl.h

EXPAT.H.OTHER :=
EXPAT.H.OTHER += src/expat/include/expat.h
EXPAT.H.OTHER += src/expat/include/expat_external.h

#

EXPAT.CFLAGS     := -Isrc/src/expat/include -Isrc/expat/src
EXPAT.STATIC     := libexpat.a
