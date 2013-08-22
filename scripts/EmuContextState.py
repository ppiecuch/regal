#!/usr/bin/python -B

# Basic tracking at RegalContext level for global
# things such as glBegin/glEnd, glPush/glPop depth, etc

formulae = {
  'Begin' : {
    'entries' : [ 'glBegin' ],
    'impl' : [ 'RegalAssert(_context);',
               '_context->depthBeginEnd++;' ]
  },

  'End' : {
    'entries' : [ 'glEnd' ],
    'prefix'  : [ 'if (_context)',
                  '  _context->depthBeginEnd--;' ]
  },

  'PushMatrix' : {
    'entries' : [ 'glPushMatrix|glMatrixPushEXT' ],
    'impl' : [ 'RegalAssert(_context);',
               '_context->depthPushMatrix++;' ]
  },

  'PopMatrix' : {
    'entries' : [ 'glPopMatrix|glMatrixPopEXT' ],
    'prefix'  : [ 'if (_context)',
                  '  _context->depthPushMatrix--;' ]
  },

  'PushAttrib' : {
    'entries' : [ 'glPushAttrib' ],
    'impl' : [ 'RegalAssert(_context);',
               '_context->depthPushAttrib++;' ]
  },

  'PopAttrib' : {
    'entries' : [ 'glPopAttrib' ],
    'prefix'  : [ 'if (_context)',
                  '  _context->depthPushAttrib--;' ]
  },

  'NewList' : {
    'entries' : [ 'glNewList' ],
    'impl' : [ 'RegalAssert(_context);',
               '_context->depthNewList++;' ]
  },

  'EndList' : {
    'entries' : [ 'glEndList' ],
    'prefix'  : [ 'if (_context)',
                  '  _context->depthNewList--;' ]
  },

}
