#!/usr/bin/python -B

xferFormulae = {
    'PixelStore' : {
        'entries' : [ 'glPixelStore(i|f)' ],
        'prefix' : [ '_context->xfer->PixelStore( _context, ${arg0plus} );', ],
    },
   'ActiveTexture' : {
        'entries' : [ 'glActiveTexture' ],
        'impl' : [ '_context->xfer->ShadowActiveTexture( ${arg0} );', ],
    },
    'TexImage2D' : {
        'entries' : [ 'glTex(Sub|)Image2D' ],
        'impl' : [ '_context->xfer->Tex${m1}Image2D( _context, ${arg0plus} );', ],
    }
}
