#!/usr/bin/python -B

#
# Selectively initialize Regal for known initialization calls
# in addition to Regal API entry points.
#
# eglGetDisplay is needed for apitrace eglretrace tool.
#

formulae = {
  'EmuInit' : {
    'entries' : [ 'CGLChoosePixelFormat', 'eglGetDisplay' ],
    'prefix'  : [ 'Init::init();' ]
  }
}
