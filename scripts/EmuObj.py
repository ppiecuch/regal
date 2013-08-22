#!/usr/bin/python -B

objFormulae = {
    'Buffers' : {
        'entries' : [ 'gl(GenBuffers|DeleteBuffers|BindBuffer)(ARB|)' ],
        'impl' : [ '_context->obj->${m1}(*_context, ${arg0plus});', ],
    },
    'VAOs' : {
        'entries' : [ 'gl(GenVertexArrays|DeleteVertexArrays|BindVertexArray)(ARB|)' ],
        'impl' : [ '_context->obj->${m1}(*_context, ${arg0plus});', ],
    },
    'Textures' : {
        'entries' : [ 'gl(GenTextures|DeleteTextures|BindTexture)' ],
        'impl' : [ '_context->obj->${m1}(*_context, ${arg0plus});', ],
    },
    'FramebufferTexture' : {
        'entries' : [ 'glFramebufferTexture(1D|1DEXT|2D|2DEXT|2DOES|2DMultisampleEXT|2DMultisampleOES|3DEXT|3DOES)' ],
        'impl' : ['_dispatch.call(&_dispatch.glFramebufferTexture${m1})(${arg0}, ${arg1}, ${arg2}, _context->obj->textureNames.ToDriverName(${arg3}), ${arg4plus});', ],
     },
    'GetFramebufferAttachmentParameteriv' : {
        'entries' : [ 'glGetFramebufferAttachmentParameteriv(EXT|)' ],
        'impl' : [
            '_dispatch.call(&_dispatch.glGetFramebufferAttachmentParameteriv${m1})(${arg0plus});',
            'if (${arg2} == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME)',
            '{',
            '  GLint attachType = GL_RENDERBUFFER;',
            '  _dispatch.call(&_dispatch.glGetFramebufferAttachmentParameteriv${m1})(${arg0}, ${arg1}, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachType);',
            '  if (attachType == GL_TEXTURE)',
            '    *${arg3} = _context->obj->textureNames.ToAppName(*${arg3});',
            '}',
        ],
    },
    'IsObj' : {
        'entries' : [ 'glIs(Buffer|VertexArray|Texture)(ARB|)' ],
        'impl' : [ 'return _context->obj->Is${m1}(*_context, ${arg0plus});', ],
    },
}
