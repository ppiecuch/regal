#!/usr/bin/python -B

formulae = {
    'GetExtension' : {
        'entries' : [ 'glGetExtensionREGAL' ],
        'impl' : '''
RegalAssert(_context->info);
RegalAssert(_context->emuInfo)
return _context->emuInfo->getExtension(*_context->info,ext) ? GL_TRUE : GL_FALSE;'''
    },
    'IsSupported' : {
        'entries' : [ 'glIsSupportedREGAL' ],
        'impl' : '''
RegalAssert(_context->info);
RegalAssert(_context->emuInfo)
return _context->emuInfo->isSupported(*_context->info,ext) ? GL_TRUE : GL_FALSE;'''
    },
}
