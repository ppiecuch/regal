#!/usr/bin/python -B

iffFormulae = {
    'VaPointer4EXTOverride' : {
        'entries' : [ 'gl(Vertex|Color|TexCoord)PointerEXT' ],
        'impl' : [ '_context->iff->${m1}Pointer( _context, ${arg0}, ${arg1}, ${arg2}, ${arg4} );' ],
    },
    'EdgeFlagPointerEXT' : {
        'entries' : [ 'glEdgeFlagPointerEXT' ],
        'impl' : [ '_context->iff->EdgeFlagPointer( _context, ${arg0}, ${arg2} );' ],
    },
    'NormalPointerEXT' : {
        'entries' : [ 'glNormalPointerEXT' ],
        'impl' : [ '_context->iff->NormalPointer( _context, ${arg0}, ${arg1}, ${arg3} );' ],
    },
    'PointerEXT' : {
        'entries' : [ 'gl(FogCoord|SecondaryColor)PointerEXT' ],
        'impl' : [ '_context->iff->${m1}Pointer( _context, ${arg0plus} );' ],
    },
    'VaPointerOverride' : {
        'entries' : [ 'gl(Vertex|Normal|Color|SecondaryColor|FogCoord|EdgeFlag|TexCoord)Pointer' ],
        'impl' : [ '_context->iff->${m1}Pointer( _context, ${arg0plus} );' ],
    },
    'VaClientStateOverride' : {
        'entries' : [ 'gl(Enable|Disable)ClientState' ],
        'impl' : [ '_context->iff->${m1}ClientState( _context, ${arg0} );' ],
    },
    'IsEnabled' : {
        'entries' : [ 'glIsEnabled' ],
        'impl' : [
            '{',
            '    GLboolean enabled;',
            '    if ( !_context->iff->IsEnabled( _context, ${arg0plus}, enabled ) )',
            '        return _context->dispatcher.emulation.glIsEnabled( ${arg0plus} );',
            '    return enabled;',
            '}',
        ]
    },
    'ImmShadowVao' : {
        'entries' : [ 'glBindVertexArray.*', ],
        'prefix' : [ '_context->iff->glBindVertexArray( _context, ${arg0} ); ', ],
    },
   'IsVertexArray' : {
        'entries' : [ 'glIsVertexArray(ARB|)' ],
        'impl' : [ 'return _context->iff->IsVertexArray( _context, ${arg0} );' ],
    },
   'DeleteStuff' : {
        'entries' : [ 'glDelete(Buffers|VertexArrays)(ARB|)' ],
        'impl' : [ '_context->iff->glDelete${m1}( _context, ${arg0}, ${arg1} );' ],
    },
    'ImmShadowClientActiveTexture' : {
        'entries' : [ 'glClientActiveTexture(ARB|)', ],
        'prefix' : [ '_context->iff->ShadowClientActiveTexture( ${arg0} ); ', ],
    },
    'ImmBegin' : {
        'entries' : [ 'glBegin' ],
        'impl' : [ '_context->iff->Begin( _context, ${arg0} );', ],
    },
    'ImmEnd' : {
        'entries' : [ 'glEnd' ],
        'impl' : [ '_context->iff->End( _context );', ],
    },
    'ImmAttr' : {
        'entries' : [ 'glVertexAttrib(1|2|3|4)(N|)(b|d|f|i|s|ub|us)(v|)(ARB|)' ],
        'prefix' : [ '_context->iff->Attr${m2}<${m1}>( _context, ${arg0plus} );', ],
    },
    'ImmFixedAttrf' : {
        'entries' : [ 'gl(SecondaryColor|Color|Normal|FogCoord)(2|3|4)(d|f)(v|)(EXT|)?' ],
        'impl' : [ '_context->iff->Attr<${m2}>( _context, _context->iff->AttrIndex( RFF2A_${m1} ), ${arg0plus} );', ],
    },
    'ImmVertex' : {
        'entries' : [ 'gl(Vertex)(2|3|4)(d|f|i|s)(v|)' ],
        'impl' : [ '_context->iff->Attr<${m2}>( _context, _context->iff->AttrIndex( RFF2A_${m1} ), ${arg0plus} );', ],
    },
    'ImmFixedAttri' : {
        'entries' : [ 'gl(SecondaryColor|Color|Normal)(2|3|4)(b|i|s|ub|ui|us)(v|)(EXT|)?' ],
        'impl' : [ '_context->iff->AttrN<${m2}>( _context, _context->iff->AttrIndex( RFF2A_${m1} ), ${arg0plus} );', ],
    },
    'ImmTexCoord' : {
        'entries' : [ 'glTexCoord(1|2|3|4)(d|f|i|s)(v|)' ],
        'impl' : [ '_context->iff->Attr<${m1}>( _context, _context->iff->AttrIndex( RFF2A_TexCoord ), ${arg0plus} );', ],
    },
    'ImmMultiTexCoord' : {
        'entries' : [ 'glMultiTexCoord(1|2|3|4)(d|f|i|s)(v|)(ARB|)' ],
        'impl' : [ '_context->iff->Attr<${m1}>( _context, _context->iff->AttrIndex( RFF2A_TexCoord, ${arg0} - GL_TEXTURE0 ), ${arg1plus} );', ],
    },
    'ImmRestore' : {
        'entries' : [
            'glBindBuffer',
#           'gl(Enable|Disable)ClientState',
            'glVertexAttribPointer(ARB|)',
#           'gl(Vertex|Normal|Color|SecondaryColor|FogCoord|TexCoord)Pointer',
            'glGetVertexAttrib(d|f|i|Pointer)v(ARB|)',
            ],
        'prefix' : [ '_context->iff->RestoreVao( _context );' ],
    },



    'FfnShadowARB' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'prefix' : [ '_context->iff->ShadowActiveTexture( ${arg0} ); ', ],
    },
    'FfnShadeModel' : {
        'entries' : [ 'glShadeModel' ],
        'impl' : [
          '_context->iff->ShadeModel( ${arg0plus} );',
          'if( !_context->isCore() && !_context->isES2() ) {',
          '  _context->dispatcher.emulation.glShadeModel(${arg0plus});',
          '}',
          ],
    },
    'FfnShadow' : {
        'entries' : [ 'gl(MatrixMode|BindProgramPipeline|UseProgram|Enable|Disable)' ],
        'impl' : [
            'if( ! _context->iff->Shadow${m1}( ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.gl${m1}( ${arg0plus} );',
            '}',
            ],
    },
    'FfnShadowIndexed' : {
        'entries' : [ 'gl(Enable|Disable)(i|IndexedEXT)' ],
        'impl' : [ '_context->iff->${m1}Indexed( ${arg0plus} );', ],
    },
    'FfnShadowTexBinding' : {
        'entries' : [ 'glBind(Multi|)Texture(EXT|)' ],
        'prefix' : [ '_context->iff->Shadow${m1}TexBinding( ${arg0plus} );', ],
    },
    'FfnShadowDsaTexInfo' : {
        'entries' : [ 'gl(MultiTex|Texture)Image.*EXT' ],
        'prefix' : [ '_context->iff->Shadow${m1}Info( ${arg0}, ${arg1}, ${arg3} );', ],
    },
    'FfnShadowTexInfo' : {
        'entries' : [ 'glTexImage(1|2|3|4)D(Multisample)?(EXT|ARB|SGIS|)' ],
        'prefix' : [ '_context->iff->ShadowTexInfo( ${arg0}, ${arg2} );', ],
    },
    'FfnShadowTexInfo2' : {
        'entries' : [ 'glTexImage(1|2|3|4)DMultisampleCoverageNV' ],
        'prefix' : [ '_context->iff->ShadowTexInfo( ${arg0}, ${arg3} );', ],
    },
    'FfnShadowTexInfo3' : {
        'entries' : [ 'glCopyTexImage2D' ],
        'prefix' : [ '_context->iff->ShadowTexInfo( ${arg0}, ${arg2} );', ],
    },
    'FfnTexEnv' : {
        'entries' : [ 'gl(Multi|)TexEnv(i|f)(v|)(EXT|)' ],
        'impl' : [ '_context->iff->TexEnv( ${arg0plus} );', ],
    },
    'FfnGetTexEnv' : {
        'entries' : [ 'glGetTexEnv(i|f)(v|)(EXT|)' ],
        'impl' : [
            '_context->iff->RestoreVao( _context );',
            'if ( ! _context->iff->GetTexEnv( ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.glGetTexEnv${m1}${m2}${m3}( ${arg0plus} );',
            '}',
        ],
    },
    'FfnLightMatModel' : {
        'entries' : [ 'gl(Light|Material|GetMaterial|LightModel)(i|f)(v|)' ],
        'impl' : [ '_context->iff->${m1}( ${arg0plus} );', ],
    },
    'FfnColorMaterial' : {
        'entries' : [ 'glColorMaterial' ],
        'impl' : [ '_context->iff->ColorMaterial( ${arg0plus} );', ],
    },
    'FfnTexGen' : {
        'entries' : [ 'glTexGen(i|f|d)(v|)' ],
        'impl' : [
            'if ( ! _context->iff->TexGen( ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.glTexGen${m1}${m2}( ${arg0plus} );',
            '}',
        ],
    },
    'FfnAlphaFunc' : {
        'entries' : [ 'glAlphaFunc' ],
        'impl' : [ '_context->iff->AlphaFunc( ${arg0plus} );', ],
    },
    'FfnClipPlane' : {
        'entries' : [ 'glClipPlane' ],
        'impl' : [ '_context->iff->ClipPlane( ${arg0plus} );', ],
    },
    'FfnFog' : {
        'entries' : [ 'glFog(f|i)(v|)' ],
        'impl' : [ '_context->iff->Fog( ${arg0plus} );', ],
    },
    'FfnGetBoolean' : {
        'entries' : [ 'glGetBooleanv' ],
        'impl' : [
            '_context->iff->RestoreVao( _context );',
            'if ( ! _context->iff->glGetBooleanv( _context, ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.glGetBooleanv( ${arg0plus} );',
            '}',
        ],
    },
    'FfnGet' : {
        'entries' : [ 'glGet(Integer|Float|Double)v' ],
        'impl' : [
            '_context->iff->RestoreVao( _context );',
            'if ( ! _context->iff->Get( _context, ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.glGet${m1}v( ${arg0plus} );',
            '}',
        ],
    },
    'FfnGetMultiTexGenv' : {
        'entries' : [ 'glGetMultiTexGen(d|f|i)vEXT' ],
        'impl' : [
            '_context->iff->RestoreVao( _context );',
            'if ( ! _context->iff->GetMultiTexGenv( _context, ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.glGetMultiTexGen${m1}vEXT( ${arg0plus} );',
            '}',
        ],
    },
    'FfnGetTexGenv' : {
        'entries' : [ 'glGetTexGen(d|f|i)v' ],
        'impl' : [
            '_context->iff->RestoreVao( _context );',
            'if ( ! _context->iff->GetTexGenv( _context, ${arg0plus} ) ) {',
            '    _context->dispatcher.emulation.glGetTexGen${m1}v( ${arg0plus} );',
            '}',
        ],
    },
    # TODO - GL_ARB_base_instance ?
    'FfnPreDraw' : {
        'entries' : [ 'gl(Multi|)Draw(Range|)(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [ '_context->iff->PreDraw( _context );', ],
    },
    'FfnModifyMatrix' : {
        'entries' : [
            'gl(Load|Mult)(Transpose|)(Matrix)(f|d)(ARB|)',
            'gl(LoadIdentity)()()',
            'gl(Rotate|Scale|Translate)()()(f|d)',
            'gl(Ortho|Frustum)()()',
            'gl(Push|Pop)(Matrix)()()',
            'gl(Matrix)(Load|Mult)(Transpose|)(f|d)EXT',
            'gl(Matrix)(LoadIdentity)()()EXT',
            'gl(Matrix)(Rotate|Scale|Translate)()(f|d)EXT',
            'gl(Matrix)(Ortho|Frustum)()()EXT',
            'gl(Matrix)(Push|Pop)()()EXT',
        ],
        'impl' : [ '_context->iff->${m1}${m2}${m3}( ${arg0plus} );' ],
    },
    'Viewport' : {
        'entries' : [ 'glViewport', ],
        'prefix' : [ '_context->iff->Viewport( ${arg0plus} );', ],
    },
    'DepthRange' : {
        'entries' : [ 'glDepthRange', ],
        'prefix' : [ '_context->iff->DepthRange( GLfloat(${arg0}), GLfloat(${arg1}) );', ],
    },
    'RasterPosition' : {
        'entries' : [ 'gl(Raster|Window)Pos(2|3)(i|s|f|d)', ],
        'impl' : [ '_context->iff->${m1}Position( _context, ${arg0plus} );', ],
    },
    'EnableArray' : {
        'entries' : [ 'gl(Enable|Disable)VertexAttribArray(ARB|)' ],
        'prefix' : [
            '_context->iff->RestoreVao( _context );',
            '_context->iff->${m1}Array( _context, ${arg0} );',
        ],
    },

    'ShaderSource' : {
        'entries' : [ 'glShaderSource(ARB|)?', ],
        'impl' : [ '_context->iff->ShaderSource( _context, ${arg0plus} );', ],
    },
    'LinkProgram' : {
        'entries' : [ 'glLinkProgram(ARB|)?', ],
        'impl' : [ '_context->iff->LinkProgram( _context, ${arg0} );', ],
    },
    'CreateShader' : {
        'entries' : [ 'glCreateShader(ObjectARB)?', ],
        'impl' : [ 'return _context->iff->CreateShader( _context, ${arg0} );', ],
    }
}
