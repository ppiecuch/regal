#!/usr/bin/python -B

rectFormulae = {
  'rect'       : {
    'entries' : [ 'glRect(d|f|i|s)(v|)' ],
    'impl'    : [
      '_context->rect->glRect${m2}( _context, ${arg0plus} );',
    ],
  },
}
