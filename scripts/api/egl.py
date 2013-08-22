import Api
from Api import Api
from Api import Function, Typedef, Enum
from Api import Return, Parameter, Input, Output, InputOutput
from Api import Enumerant
from Api import Extension
from Api import StateType, State

egl = Api()

EGLNativeWindowType = Typedef('EGLNativeWindowType',{'': 'void *', 'win32': 'HWND', 'android': 'struct ANativeWindow *', 'x11': 'Window'})
EGLNativeWindowType.default = '0'

EGLNativePixmapType = Typedef('EGLNativePixmapType',{'': 'void *', 'win32': 'HBITMAP', 'android': 'struct egl_native_pixmap_t *', 'x11': 'Pixmap'})
EGLNativePixmapType.default = '0'

EGLNativeDisplayType = Typedef('EGLNativeDisplayType',{'': 'int', 'win32': 'HDC', 'android': 'void *', 'x11': 'Display *'})
EGLNativeDisplayType.default = '0'

NativeDisplayType = Typedef('NativeDisplayType','EGLNativeDisplayType')
NativeDisplayType.default = '0'

NativePixmapType = Typedef('NativePixmapType','EGLNativePixmapType ')
NativePixmapType.default = '0'

NativeWindowType = Typedef('NativeWindowType','EGLNativeWindowType ')
NativeWindowType.default = '0'

EGLint = Typedef('EGLint','int')
EGLint.default = '0'

EGLBoolean = Typedef('EGLBoolean','unsigned int')
EGLBoolean.default = '0'

EGLenum = Typedef('EGLenum','unsigned int')
EGLenum.default = '0'

EGLConfig = Typedef('EGLConfig','void *')
EGLConfig.default = '0'

EGLContext = Typedef('EGLContext','void *')
EGLContext.default = '0'

EGLDisplay = Typedef('EGLDisplay','void *')
EGLDisplay.default = '0'

EGLSurface = Typedef('EGLSurface','void *')
EGLSurface.default = '0'

EGLClientBuffer = Typedef('EGLClientBuffer','void *')
EGLClientBuffer.default = '0'

__eglMustCastToProperFunctionPointerType = Typedef('__eglMustCastToProperFunctionPointerType','void  (*)(void)')
__eglMustCastToProperFunctionPointerType.default = '0'

EGLSyncKHR = Typedef('EGLSyncKHR','void *')
EGLSyncKHR.category = 'EGL_KHR_fence_sync'
EGLSyncKHR.default = '0'

EGLTimeKHR = Typedef('EGLTimeKHR','uint64_t')
EGLTimeKHR.category = 'EGL_KHR_fence_sync'
EGLTimeKHR.default = '0'

EGLImageKHR = Typedef('EGLImageKHR','void *')
EGLImageKHR.category = 'EGL_KHR_image_base'
EGLImageKHR.default = '0'

EGLStreamKHR = Typedef('EGLStreamKHR','void *')
EGLStreamKHR.category = 'EGL_KHR_stream'
EGLStreamKHR.default = '0'

EGLuint64KHR = Typedef('EGLuint64KHR','uint64_t')
EGLuint64KHR.category = 'EGL_KHR_stream'
EGLuint64KHR.default = '0'

EGLNativeFileDescriptorKHR = Typedef('EGLNativeFileDescriptorKHR','int')
EGLNativeFileDescriptorKHR.category = 'EGL_KHR_stream_cross_process_fd'
EGLNativeFileDescriptorKHR.default = '0'

EGLSyncNV = Typedef('EGLSyncNV','void *')
EGLSyncNV.category = 'EGL_NV_sync'
EGLSyncNV.default = '0'

EGLTimeNV = Typedef('EGLTimeNV','uint64_t')
EGLTimeNV.category = 'EGL_NV_sync'
EGLTimeNV.default = '0'

EGLuint64NV = Typedef('EGLuint64NV','uint64_t')
EGLuint64NV.category = 'EGL_NV_system_time'
EGLuint64NV.default = '0'

egl.add(EGLNativeWindowType)
egl.add(EGLNativePixmapType)
egl.add(EGLNativeDisplayType)
egl.add(NativeDisplayType)
egl.add(NativePixmapType)
egl.add(NativeWindowType)
egl.add(EGLint)
egl.add(EGLBoolean)
egl.add(EGLenum)
egl.add(EGLConfig)
egl.add(EGLContext)
egl.add(EGLDisplay)
egl.add(EGLSurface)
egl.add(EGLClientBuffer)
egl.add(__eglMustCastToProperFunctionPointerType)
egl.add(EGLSyncKHR)
egl.add(EGLTimeKHR)
egl.add(EGLImageKHR)
egl.add(EGLStreamKHR)
egl.add(EGLuint64KHR)
egl.add(EGLNativeFileDescriptorKHR)
egl.add(EGLSyncNV)
egl.add(EGLTimeNV)
egl.add(EGLuint64NV)


defines = Enum('defines')
egl.add(defines)

#

EGLAPI = Enumerant('EGLAPI', 'KHRONOS_APICALL', '')
EGLAPIENTRY = Enumerant('EGLAPIENTRY', 'KHRONOS_APIENTRY', '')
EGLAPIENTRYP = Enumerant('EGLAPIENTRYP', 'EGLAPIENTRY *', '')
EGL_ALPHA_FORMAT = Enumerant('EGL_ALPHA_FORMAT', 'EGL_VG_ALPHA_FORMAT', '')
EGL_ALPHA_FORMAT_NONPRE = Enumerant('EGL_ALPHA_FORMAT_NONPRE', 'EGL_VG_ALPHA_FORMAT_NONPRE', '')
EGL_ALPHA_FORMAT_PRE = Enumerant('EGL_ALPHA_FORMAT_PRE', 'EGL_VG_ALPHA_FORMAT_PRE', '')
EGL_ALPHA_MASK_SIZE = Enumerant('EGL_ALPHA_MASK_SIZE', 0x303e, '')
EGL_ALPHA_SIZE = Enumerant('EGL_ALPHA_SIZE', 0x3021, '')
EGL_BACK_BUFFER = Enumerant('EGL_BACK_BUFFER', 0x3084, '')
EGL_BAD_ACCESS = Enumerant('EGL_BAD_ACCESS', 0x3002, '')
EGL_BAD_ALLOC = Enumerant('EGL_BAD_ALLOC', 0x3003, '')
EGL_BAD_ATTRIBUTE = Enumerant('EGL_BAD_ATTRIBUTE', 0x3004, '')
EGL_BAD_CONFIG = Enumerant('EGL_BAD_CONFIG', 0x3005, '')
EGL_BAD_CONTEXT = Enumerant('EGL_BAD_CONTEXT', 0x3006, '')
EGL_BAD_CURRENT_SURFACE = Enumerant('EGL_BAD_CURRENT_SURFACE', 0x3007, '')
EGL_BAD_DISPLAY = Enumerant('EGL_BAD_DISPLAY', 0x3008, '')
EGL_BAD_MATCH = Enumerant('EGL_BAD_MATCH', 0x3009, '')
EGL_BAD_NATIVE_PIXMAP = Enumerant('EGL_BAD_NATIVE_PIXMAP', 0x300a, '')
EGL_BAD_NATIVE_WINDOW = Enumerant('EGL_BAD_NATIVE_WINDOW', 0x300b, '')
EGL_BAD_PARAMETER = Enumerant('EGL_BAD_PARAMETER', 0x300c, '')
EGL_BAD_SURFACE = Enumerant('EGL_BAD_SURFACE', 0x300d, '')
EGL_BIND_TO_TEXTURE_RGB = Enumerant('EGL_BIND_TO_TEXTURE_RGB', 0x3039, '')
EGL_BIND_TO_TEXTURE_RGBA = Enumerant('EGL_BIND_TO_TEXTURE_RGBA', 0x303a, '')
EGL_BLUE_SIZE = Enumerant('EGL_BLUE_SIZE', 0x3022, '')
EGL_BUFFER_DESTROYED = Enumerant('EGL_BUFFER_DESTROYED', 0x3095, '')
EGL_BUFFER_PRESERVED = Enumerant('EGL_BUFFER_PRESERVED', 0x3094, '')
EGL_BUFFER_SIZE = Enumerant('EGL_BUFFER_SIZE', 0x3020, '')
EGL_CLIENT_APIS = Enumerant('EGL_CLIENT_APIS', 0x308d, '')
EGL_COLORSPACE = Enumerant('EGL_COLORSPACE', 'EGL_VG_COLORSPACE', '')
EGL_COLORSPACE_LINEAR = Enumerant('EGL_COLORSPACE_LINEAR', 'EGL_VG_COLORSPACE_LINEAR', '')
EGL_COLORSPACE_sRGB = Enumerant('EGL_COLORSPACE_sRGB', 'EGL_VG_COLORSPACE_sRGB', '')
EGL_COLOR_BUFFER_TYPE = Enumerant('EGL_COLOR_BUFFER_TYPE', 0x303f, '')
EGL_CONFIG_CAVEAT = Enumerant('EGL_CONFIG_CAVEAT', 0x3027, '')
EGL_CONFIG_ID = Enumerant('EGL_CONFIG_ID', 0x3028, '')
EGL_CONFORMANT = Enumerant('EGL_CONFORMANT', 0x3042, '')
EGL_CONTEXT_CLIENT_TYPE = Enumerant('EGL_CONTEXT_CLIENT_TYPE', 0x3097, '')
EGL_CONTEXT_CLIENT_VERSION = Enumerant('EGL_CONTEXT_CLIENT_VERSION', 0x3098, '')
EGL_CONTEXT_LOST = Enumerant('EGL_CONTEXT_LOST', 0x300e, '')
EGL_CORE_NATIVE_ENGINE = Enumerant('EGL_CORE_NATIVE_ENGINE', 0x305b, '')
EGL_DEFAULT_DISPLAY = Enumerant('EGL_DEFAULT_DISPLAY', '((EGLNativeDisplayType)0)', '')
EGL_DEPTH_SIZE = Enumerant('EGL_DEPTH_SIZE', 0x3025, '')
EGL_DISPLAY_SCALING = Enumerant('EGL_DISPLAY_SCALING', 0x2710, '')
EGL_DONT_CARE = Enumerant('EGL_DONT_CARE', '((EGLint)-1)', '')
EGL_DRAW = Enumerant('EGL_DRAW', 0x3059, '')
EGL_EXTENSIONS = Enumerant('EGL_EXTENSIONS', 0x3055, '')
EGL_FALSE = Enumerant('EGL_FALSE', 0x0000, '')
EGL_GREEN_SIZE = Enumerant('EGL_GREEN_SIZE', 0x3023, '')
EGL_HEIGHT = Enumerant('EGL_HEIGHT', 0x3056, '')
EGL_HORIZONTAL_RESOLUTION = Enumerant('EGL_HORIZONTAL_RESOLUTION', 0x3090, '')
EGL_LARGEST_PBUFFER = Enumerant('EGL_LARGEST_PBUFFER', 0x3058, '')
EGL_LEVEL = Enumerant('EGL_LEVEL', 0x3029, '')
EGL_LUMINANCE_BUFFER = Enumerant('EGL_LUMINANCE_BUFFER', 0x308f, '')
EGL_LUMINANCE_SIZE = Enumerant('EGL_LUMINANCE_SIZE', 0x303d, '')
EGL_MATCH_NATIVE_PIXMAP = Enumerant('EGL_MATCH_NATIVE_PIXMAP', 0x3041, '')
EGL_MAX_PBUFFER_HEIGHT = Enumerant('EGL_MAX_PBUFFER_HEIGHT', 0x302a, '')
EGL_MAX_PBUFFER_PIXELS = Enumerant('EGL_MAX_PBUFFER_PIXELS', 0x302b, '')
EGL_MAX_PBUFFER_WIDTH = Enumerant('EGL_MAX_PBUFFER_WIDTH', 0x302c, '')
EGL_MAX_SWAP_INTERVAL = Enumerant('EGL_MAX_SWAP_INTERVAL', 0x303c, '')
EGL_MIN_SWAP_INTERVAL = Enumerant('EGL_MIN_SWAP_INTERVAL', 0x303b, '')
EGL_MIPMAP_LEVEL = Enumerant('EGL_MIPMAP_LEVEL', 0x3083, '')
EGL_MIPMAP_TEXTURE = Enumerant('EGL_MIPMAP_TEXTURE', 0x3082, '')
EGL_MULTISAMPLE_RESOLVE = Enumerant('EGL_MULTISAMPLE_RESOLVE', 0x3099, '')
EGL_MULTISAMPLE_RESOLVE_BOX = Enumerant('EGL_MULTISAMPLE_RESOLVE_BOX', 0x309b, '')
EGL_MULTISAMPLE_RESOLVE_BOX_BIT = Enumerant('EGL_MULTISAMPLE_RESOLVE_BOX_BIT', 0x0200, '')
EGL_MULTISAMPLE_RESOLVE_DEFAULT = Enumerant('EGL_MULTISAMPLE_RESOLVE_DEFAULT', 0x309a, '')
EGL_NATIVE_RENDERABLE = Enumerant('EGL_NATIVE_RENDERABLE', 0x302d, '')
EGL_NATIVE_VISUAL_ID = Enumerant('EGL_NATIVE_VISUAL_ID', 0x302e, '')
EGL_NATIVE_VISUAL_TYPE = Enumerant('EGL_NATIVE_VISUAL_TYPE', 0x302f, '')
EGL_NONE = Enumerant('EGL_NONE', 0x3038, '')
EGL_NON_CONFORMANT_CONFIG = Enumerant('EGL_NON_CONFORMANT_CONFIG', 0x3051, '')
EGL_NOT_INITIALIZED = Enumerant('EGL_NOT_INITIALIZED', 0x3001, '')
EGL_NO_CONTEXT = Enumerant('EGL_NO_CONTEXT', '((EGLContext)0)', '')
EGL_NO_DISPLAY = Enumerant('EGL_NO_DISPLAY', '((EGLDisplay)0)', '')
EGL_NO_SURFACE = Enumerant('EGL_NO_SURFACE', '((EGLSurface)0)', '')
EGL_NO_TEXTURE = Enumerant('EGL_NO_TEXTURE', 0x305c, '')
EGL_OPENGL_API = Enumerant('EGL_OPENGL_API', 0x30a2, '')
EGL_OPENGL_BIT = Enumerant('EGL_OPENGL_BIT', 0x0008, '')
EGL_OPENGL_ES2_BIT = Enumerant('EGL_OPENGL_ES2_BIT', 0x0004, '')
EGL_OPENGL_ES_API = Enumerant('EGL_OPENGL_ES_API', 0x30a0, '')
EGL_OPENGL_ES_BIT = Enumerant('EGL_OPENGL_ES_BIT', 0x0001, '')
EGL_OPENVG_API = Enumerant('EGL_OPENVG_API', 0x30a1, '')
EGL_OPENVG_BIT = Enumerant('EGL_OPENVG_BIT', 0x0002, '')
EGL_OPENVG_IMAGE = Enumerant('EGL_OPENVG_IMAGE', 0x3096, '')
EGL_PBUFFER_BIT = Enumerant('EGL_PBUFFER_BIT', 0x0001, '')
EGL_PIXEL_ASPECT_RATIO = Enumerant('EGL_PIXEL_ASPECT_RATIO', 0x3092, '')
EGL_PIXMAP_BIT = Enumerant('EGL_PIXMAP_BIT', 0x0002, '')
EGL_READ = Enumerant('EGL_READ', 0x305a, '')
EGL_RED_SIZE = Enumerant('EGL_RED_SIZE', 0x3024, '')
EGL_RENDERABLE_TYPE = Enumerant('EGL_RENDERABLE_TYPE', 0x3040, '')
EGL_RENDER_BUFFER = Enumerant('EGL_RENDER_BUFFER', 0x3086, '')
EGL_RGB_BUFFER = Enumerant('EGL_RGB_BUFFER', 0x308e, '')
EGL_SAMPLES = Enumerant('EGL_SAMPLES', 0x3031, '')
EGL_SAMPLE_BUFFERS = Enumerant('EGL_SAMPLE_BUFFERS', 0x3032, '')
EGL_SINGLE_BUFFER = Enumerant('EGL_SINGLE_BUFFER', 0x3085, '')
EGL_SLOW_CONFIG = Enumerant('EGL_SLOW_CONFIG', 0x3050, '')
EGL_STENCIL_SIZE = Enumerant('EGL_STENCIL_SIZE', 0x3026, '')
EGL_SUCCESS = Enumerant('EGL_SUCCESS', 0x3000, '')
EGL_SURFACE_TYPE = Enumerant('EGL_SURFACE_TYPE', 0x3033, '')
EGL_SWAP_BEHAVIOR = Enumerant('EGL_SWAP_BEHAVIOR', 0x3093, '')
EGL_SWAP_BEHAVIOR_PRESERVED_BIT = Enumerant('EGL_SWAP_BEHAVIOR_PRESERVED_BIT', 0x0400, '')
EGL_TEXTURE_2D = Enumerant('EGL_TEXTURE_2D', 0x305f, '')
EGL_TEXTURE_FORMAT = Enumerant('EGL_TEXTURE_FORMAT', 0x3080, '')
EGL_TEXTURE_RGB = Enumerant('EGL_TEXTURE_RGB', 0x305d, '')
EGL_TEXTURE_RGBA = Enumerant('EGL_TEXTURE_RGBA', 0x305e, '')
EGL_TEXTURE_TARGET = Enumerant('EGL_TEXTURE_TARGET', 0x3081, '')
EGL_TRANSPARENT_BLUE_VALUE = Enumerant('EGL_TRANSPARENT_BLUE_VALUE', 0x3035, '')
EGL_TRANSPARENT_GREEN_VALUE = Enumerant('EGL_TRANSPARENT_GREEN_VALUE', 0x3036, '')
EGL_TRANSPARENT_RED_VALUE = Enumerant('EGL_TRANSPARENT_RED_VALUE', 0x3037, '')
EGL_TRANSPARENT_RGB = Enumerant('EGL_TRANSPARENT_RGB', 0x3052, '')
EGL_TRANSPARENT_TYPE = Enumerant('EGL_TRANSPARENT_TYPE', 0x3034, '')
EGL_TRUE = Enumerant('EGL_TRUE', 0x0001, '')
EGL_UNKNOWN = Enumerant('EGL_UNKNOWN', '((EGLint)-1)', '')
EGL_VENDOR = Enumerant('EGL_VENDOR', 0x3053, '')
EGL_VERSION = Enumerant('EGL_VERSION', 0x3054, '')
EGL_VERSION_1_3 = Enumerant('EGL_VERSION_1_3', 0x0001, '')
EGL_VERSION_1_4 = Enumerant('EGL_VERSION_1_4', 0x0001, '')
EGL_VERTICAL_RESOLUTION = Enumerant('EGL_VERTICAL_RESOLUTION', 0x3091, '')
EGL_VG_ALPHA_FORMAT = Enumerant('EGL_VG_ALPHA_FORMAT', 0x3088, '')
EGL_VG_ALPHA_FORMAT_NONPRE = Enumerant('EGL_VG_ALPHA_FORMAT_NONPRE', 0x308b, '')
EGL_VG_ALPHA_FORMAT_PRE = Enumerant('EGL_VG_ALPHA_FORMAT_PRE', 0x308c, '')
EGL_VG_ALPHA_FORMAT_PRE_BIT = Enumerant('EGL_VG_ALPHA_FORMAT_PRE_BIT', 0x0040, '')
EGL_VG_COLORSPACE = Enumerant('EGL_VG_COLORSPACE', 0x3087, '')
EGL_VG_COLORSPACE_LINEAR = Enumerant('EGL_VG_COLORSPACE_LINEAR', 0x308a, '')
EGL_VG_COLORSPACE_LINEAR_BIT = Enumerant('EGL_VG_COLORSPACE_LINEAR_BIT', 0x0020, '')
EGL_VG_COLORSPACE_sRGB = Enumerant('EGL_VG_COLORSPACE_sRGB', 0x3089, '')
EGL_WIDTH = Enumerant('EGL_WIDTH', 0x3057, '')
EGL_WINDOW_BIT = Enumerant('EGL_WINDOW_BIT', 0x0004, '')

defines.add(EGLAPI)
defines.add(EGLAPIENTRY)
defines.add(EGLAPIENTRYP)
defines.add(EGL_ALPHA_FORMAT)
defines.add(EGL_ALPHA_FORMAT_NONPRE)
defines.add(EGL_ALPHA_FORMAT_PRE)
defines.add(EGL_ALPHA_MASK_SIZE)
defines.add(EGL_ALPHA_SIZE)
defines.add(EGL_BACK_BUFFER)
defines.add(EGL_BAD_ACCESS)
defines.add(EGL_BAD_ALLOC)
defines.add(EGL_BAD_ATTRIBUTE)
defines.add(EGL_BAD_CONFIG)
defines.add(EGL_BAD_CONTEXT)
defines.add(EGL_BAD_CURRENT_SURFACE)
defines.add(EGL_BAD_DISPLAY)
defines.add(EGL_BAD_MATCH)
defines.add(EGL_BAD_NATIVE_PIXMAP)
defines.add(EGL_BAD_NATIVE_WINDOW)
defines.add(EGL_BAD_PARAMETER)
defines.add(EGL_BAD_SURFACE)
defines.add(EGL_BIND_TO_TEXTURE_RGB)
defines.add(EGL_BIND_TO_TEXTURE_RGBA)
defines.add(EGL_BLUE_SIZE)
defines.add(EGL_BUFFER_DESTROYED)
defines.add(EGL_BUFFER_PRESERVED)
defines.add(EGL_BUFFER_SIZE)
defines.add(EGL_CLIENT_APIS)
defines.add(EGL_COLORSPACE)
defines.add(EGL_COLORSPACE_LINEAR)
defines.add(EGL_COLORSPACE_sRGB)
defines.add(EGL_COLOR_BUFFER_TYPE)
defines.add(EGL_CONFIG_CAVEAT)
defines.add(EGL_CONFIG_ID)
defines.add(EGL_CONFORMANT)
defines.add(EGL_CONTEXT_CLIENT_TYPE)
defines.add(EGL_CONTEXT_CLIENT_VERSION)
defines.add(EGL_CONTEXT_LOST)
defines.add(EGL_CORE_NATIVE_ENGINE)
defines.add(EGL_DEFAULT_DISPLAY)
defines.add(EGL_DEPTH_SIZE)
defines.add(EGL_DISPLAY_SCALING)
defines.add(EGL_DONT_CARE)
defines.add(EGL_DRAW)
defines.add(EGL_EXTENSIONS)
defines.add(EGL_FALSE)
defines.add(EGL_GREEN_SIZE)
defines.add(EGL_HEIGHT)
defines.add(EGL_HORIZONTAL_RESOLUTION)
defines.add(EGL_LARGEST_PBUFFER)
defines.add(EGL_LEVEL)
defines.add(EGL_LUMINANCE_BUFFER)
defines.add(EGL_LUMINANCE_SIZE)
defines.add(EGL_MATCH_NATIVE_PIXMAP)
defines.add(EGL_MAX_PBUFFER_HEIGHT)
defines.add(EGL_MAX_PBUFFER_PIXELS)
defines.add(EGL_MAX_PBUFFER_WIDTH)
defines.add(EGL_MAX_SWAP_INTERVAL)
defines.add(EGL_MIN_SWAP_INTERVAL)
defines.add(EGL_MIPMAP_LEVEL)
defines.add(EGL_MIPMAP_TEXTURE)
defines.add(EGL_MULTISAMPLE_RESOLVE)
defines.add(EGL_MULTISAMPLE_RESOLVE_BOX)
defines.add(EGL_MULTISAMPLE_RESOLVE_BOX_BIT)
defines.add(EGL_MULTISAMPLE_RESOLVE_DEFAULT)
defines.add(EGL_NATIVE_RENDERABLE)
defines.add(EGL_NATIVE_VISUAL_ID)
defines.add(EGL_NATIVE_VISUAL_TYPE)
defines.add(EGL_NONE)
defines.add(EGL_NON_CONFORMANT_CONFIG)
defines.add(EGL_NOT_INITIALIZED)
defines.add(EGL_NO_CONTEXT)
defines.add(EGL_NO_DISPLAY)
defines.add(EGL_NO_SURFACE)
defines.add(EGL_NO_TEXTURE)
defines.add(EGL_OPENGL_API)
defines.add(EGL_OPENGL_BIT)
defines.add(EGL_OPENGL_ES2_BIT)
defines.add(EGL_OPENGL_ES_API)
defines.add(EGL_OPENGL_ES_BIT)
defines.add(EGL_OPENVG_API)
defines.add(EGL_OPENVG_BIT)
defines.add(EGL_OPENVG_IMAGE)
defines.add(EGL_PBUFFER_BIT)
defines.add(EGL_PIXEL_ASPECT_RATIO)
defines.add(EGL_PIXMAP_BIT)
defines.add(EGL_READ)
defines.add(EGL_RED_SIZE)
defines.add(EGL_RENDERABLE_TYPE)
defines.add(EGL_RENDER_BUFFER)
defines.add(EGL_RGB_BUFFER)
defines.add(EGL_SAMPLES)
defines.add(EGL_SAMPLE_BUFFERS)
defines.add(EGL_SINGLE_BUFFER)
defines.add(EGL_SLOW_CONFIG)
defines.add(EGL_STENCIL_SIZE)
defines.add(EGL_SUCCESS)
defines.add(EGL_SURFACE_TYPE)
defines.add(EGL_SWAP_BEHAVIOR)
defines.add(EGL_SWAP_BEHAVIOR_PRESERVED_BIT)
defines.add(EGL_TEXTURE_2D)
defines.add(EGL_TEXTURE_FORMAT)
defines.add(EGL_TEXTURE_RGB)
defines.add(EGL_TEXTURE_RGBA)
defines.add(EGL_TEXTURE_TARGET)
defines.add(EGL_TRANSPARENT_BLUE_VALUE)
defines.add(EGL_TRANSPARENT_GREEN_VALUE)
defines.add(EGL_TRANSPARENT_RED_VALUE)
defines.add(EGL_TRANSPARENT_RGB)
defines.add(EGL_TRANSPARENT_TYPE)
defines.add(EGL_TRUE)
defines.add(EGL_UNKNOWN)
defines.add(EGL_VENDOR)
defines.add(EGL_VERSION)
defines.add(EGL_VERSION_1_3)
defines.add(EGL_VERSION_1_4)
defines.add(EGL_VERTICAL_RESOLUTION)
defines.add(EGL_VG_ALPHA_FORMAT)
defines.add(EGL_VG_ALPHA_FORMAT_NONPRE)
defines.add(EGL_VG_ALPHA_FORMAT_PRE)
defines.add(EGL_VG_ALPHA_FORMAT_PRE_BIT)
defines.add(EGL_VG_COLORSPACE)
defines.add(EGL_VG_COLORSPACE_LINEAR)
defines.add(EGL_VG_COLORSPACE_LINEAR_BIT)
defines.add(EGL_VG_COLORSPACE_sRGB)
defines.add(EGL_WIDTH)
defines.add(EGL_WINDOW_BIT)

# EGL_ANGLE_surface_d3d_texture_2d_share_handle

EGL_D3D_TEXTURE_2D_SHARE_HANDLE_ANGLE = Enumerant('EGL_D3D_TEXTURE_2D_SHARE_HANDLE_ANGLE', 0x3200, 'EGL_ANGLE_surface_d3d_texture_2d_share_handle')

defines.add(EGL_D3D_TEXTURE_2D_SHARE_HANDLE_ANGLE)

# EGL_EXT_create_context_robustness

EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT = Enumerant('EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT', 0x3138, 'EGL_EXT_create_context_robustness')
EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT = Enumerant('EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT', 0x30bf, 'EGL_EXT_create_context_robustness')
EGL_LOSE_CONTEXT_ON_RESET_EXT = Enumerant('EGL_LOSE_CONTEXT_ON_RESET_EXT', 0x31bf, 'EGL_EXT_create_context_robustness')
EGL_NO_RESET_NOTIFICATION_EXT = Enumerant('EGL_NO_RESET_NOTIFICATION_EXT', 0x31be, 'EGL_EXT_create_context_robustness')

defines.add(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT)
defines.add(EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT)
defines.add(EGL_LOSE_CONTEXT_ON_RESET_EXT)
defines.add(EGL_NO_RESET_NOTIFICATION_EXT)

# EGL_EXT_multiview_window

EGL_MULTIVIEW_VIEW_COUNT_EXT = Enumerant('EGL_MULTIVIEW_VIEW_COUNT_EXT', 0x3134, 'EGL_EXT_multiview_window')

defines.add(EGL_MULTIVIEW_VIEW_COUNT_EXT)

# EGL_HI_colorformats

EGL_COLOR_ARGB_HI = Enumerant('EGL_COLOR_ARGB_HI', 0x8f73, 'EGL_HI_colorformats')
EGL_COLOR_FORMAT_HI = Enumerant('EGL_COLOR_FORMAT_HI', 0x8f70, 'EGL_HI_colorformats')
EGL_COLOR_RGBA_HI = Enumerant('EGL_COLOR_RGBA_HI', 0x8f72, 'EGL_HI_colorformats')
EGL_COLOR_RGB_HI = Enumerant('EGL_COLOR_RGB_HI', 0x8f71, 'EGL_HI_colorformats')

defines.add(EGL_COLOR_ARGB_HI)
defines.add(EGL_COLOR_FORMAT_HI)
defines.add(EGL_COLOR_RGBA_HI)
defines.add(EGL_COLOR_RGB_HI)

# EGL_IMG_context_priority

EGL_CONTEXT_PRIORITY_HIGH_IMG = Enumerant('EGL_CONTEXT_PRIORITY_HIGH_IMG', 0x3101, 'EGL_IMG_context_priority')
EGL_CONTEXT_PRIORITY_LEVEL_IMG = Enumerant('EGL_CONTEXT_PRIORITY_LEVEL_IMG', 0x3100, 'EGL_IMG_context_priority')
EGL_CONTEXT_PRIORITY_LOW_IMG = Enumerant('EGL_CONTEXT_PRIORITY_LOW_IMG', 0x3103, 'EGL_IMG_context_priority')
EGL_CONTEXT_PRIORITY_MEDIUM_IMG = Enumerant('EGL_CONTEXT_PRIORITY_MEDIUM_IMG', 0x3102, 'EGL_IMG_context_priority')

defines.add(EGL_CONTEXT_PRIORITY_HIGH_IMG)
defines.add(EGL_CONTEXT_PRIORITY_LEVEL_IMG)
defines.add(EGL_CONTEXT_PRIORITY_LOW_IMG)
defines.add(EGL_CONTEXT_PRIORITY_MEDIUM_IMG)

# EGL_KHR_config_attribs

EGL_CONFORMANT_KHR = Enumerant('EGL_CONFORMANT_KHR', 0x3042, 'EGL_KHR_config_attribs')
EGL_VG_ALPHA_FORMAT_PRE_BIT_KHR = Enumerant('EGL_VG_ALPHA_FORMAT_PRE_BIT_KHR', 0x0040, 'EGL_KHR_config_attribs')
EGL_VG_COLORSPACE_LINEAR_BIT_KHR = Enumerant('EGL_VG_COLORSPACE_LINEAR_BIT_KHR', 0x0020, 'EGL_KHR_config_attribs')

defines.add(EGL_CONFORMANT_KHR)
defines.add(EGL_VG_ALPHA_FORMAT_PRE_BIT_KHR)
defines.add(EGL_VG_COLORSPACE_LINEAR_BIT_KHR)

# EGL_KHR_create_context

EGL_CONTEXT_FLAGS_KHR = Enumerant('EGL_CONTEXT_FLAGS_KHR', 0x30fc, 'EGL_KHR_create_context')
EGL_CONTEXT_MAJOR_VERSION_KHR = Enumerant('EGL_CONTEXT_MAJOR_VERSION_KHR', 0x3098, 'EGL_KHR_create_context')
EGL_CONTEXT_MINOR_VERSION_KHR = Enumerant('EGL_CONTEXT_MINOR_VERSION_KHR', 0x30fb, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR = Enumerant('EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR', 0x0002, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR = Enumerant('EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR', 0x0001, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR = Enumerant('EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR', 0x0001, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR = Enumerant('EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR', 0x0002, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR = Enumerant('EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR', 0x30fd, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR = Enumerant('EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR', 0x31bd, 'EGL_KHR_create_context')
EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR = Enumerant('EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR', 0x0004, 'EGL_KHR_create_context')
EGL_LOSE_CONTEXT_ON_RESET_KHR = Enumerant('EGL_LOSE_CONTEXT_ON_RESET_KHR', 0x31bf, 'EGL_KHR_create_context')
EGL_NO_RESET_NOTIFICATION_KHR = Enumerant('EGL_NO_RESET_NOTIFICATION_KHR', 0x31be, 'EGL_KHR_create_context')
EGL_OPENGL_ES3_BIT_KHR = Enumerant('EGL_OPENGL_ES3_BIT_KHR', 0x0040, 'EGL_KHR_create_context')

defines.add(EGL_CONTEXT_FLAGS_KHR)
defines.add(EGL_CONTEXT_MAJOR_VERSION_KHR)
defines.add(EGL_CONTEXT_MINOR_VERSION_KHR)
defines.add(EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR)
defines.add(EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR)
defines.add(EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR)
defines.add(EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR)
defines.add(EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR)
defines.add(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR)
defines.add(EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR)
defines.add(EGL_LOSE_CONTEXT_ON_RESET_KHR)
defines.add(EGL_NO_RESET_NOTIFICATION_KHR)
defines.add(EGL_OPENGL_ES3_BIT_KHR)

# EGL_KHR_gl_texture_cubemap_image

EGL_GL_RENDERBUFFER_KHR = Enumerant('EGL_GL_RENDERBUFFER_KHR', 0x30b9, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_2D_KHR = Enumerant('EGL_GL_TEXTURE_2D_KHR', 0x30b1, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_3D_KHR = Enumerant('EGL_GL_TEXTURE_3D_KHR', 0x30b2, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR = Enumerant('EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR', 0x30b4, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR = Enumerant('EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR', 0x30b6, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR = Enumerant('EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR', 0x30b8, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR = Enumerant('EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR', 0x30b3, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR = Enumerant('EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR', 0x30b5, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR = Enumerant('EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR', 0x30b7, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_LEVEL_KHR = Enumerant('EGL_GL_TEXTURE_LEVEL_KHR', 0x30bc, 'EGL_KHR_gl_texture_cubemap_image')
EGL_GL_TEXTURE_ZOFFSET_KHR = Enumerant('EGL_GL_TEXTURE_ZOFFSET_KHR', 0x30bd, 'EGL_KHR_gl_texture_cubemap_image')

defines.add(EGL_GL_RENDERBUFFER_KHR)
defines.add(EGL_GL_TEXTURE_2D_KHR)
defines.add(EGL_GL_TEXTURE_3D_KHR)
defines.add(EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR)
defines.add(EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR)
defines.add(EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR)
defines.add(EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR)
defines.add(EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR)
defines.add(EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR)
defines.add(EGL_GL_TEXTURE_LEVEL_KHR)
defines.add(EGL_GL_TEXTURE_ZOFFSET_KHR)

# EGL_KHR_image_base

EGL_IMAGE_PRESERVED_KHR = Enumerant('EGL_IMAGE_PRESERVED_KHR', 0x30d2, 'EGL_KHR_image_base')

defines.add(EGL_IMAGE_PRESERVED_KHR)

# EGL_KHR_image_pixmap

EGL_NATIVE_PIXMAP_KHR = Enumerant('EGL_NATIVE_PIXMAP_KHR', 0x30b0, 'EGL_KHR_image_pixmap')

defines.add(EGL_NATIVE_PIXMAP_KHR)

# EGL_KHR_lock_surface2

EGL_BITMAP_ORIGIN_KHR = Enumerant('EGL_BITMAP_ORIGIN_KHR', 0x30c8, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PITCH_KHR = Enumerant('EGL_BITMAP_PITCH_KHR', 0x30c7, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR = Enumerant('EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR', 0x30cc, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR = Enumerant('EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR', 0x30cb, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR = Enumerant('EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR', 0x30ca, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR = Enumerant('EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR', 0x30cd, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PIXEL_RED_OFFSET_KHR = Enumerant('EGL_BITMAP_PIXEL_RED_OFFSET_KHR', 0x30c9, 'EGL_KHR_lock_surface2')
EGL_BITMAP_PIXEL_SIZE_KHR = Enumerant('EGL_BITMAP_PIXEL_SIZE_KHR', 0x3110, 'EGL_KHR_lock_surface2')
EGL_BITMAP_POINTER_KHR = Enumerant('EGL_BITMAP_POINTER_KHR', 0x30c6, 'EGL_KHR_lock_surface2')
EGL_FORMAT_RGBA_8888_EXACT_KHR = Enumerant('EGL_FORMAT_RGBA_8888_EXACT_KHR', 0x30c2, 'EGL_KHR_lock_surface2')
EGL_FORMAT_RGBA_8888_KHR = Enumerant('EGL_FORMAT_RGBA_8888_KHR', 0x30c3, 'EGL_KHR_lock_surface2')
EGL_FORMAT_RGB_565_EXACT_KHR = Enumerant('EGL_FORMAT_RGB_565_EXACT_KHR', 0x30c0, 'EGL_KHR_lock_surface2')
EGL_FORMAT_RGB_565_KHR = Enumerant('EGL_FORMAT_RGB_565_KHR', 0x30c1, 'EGL_KHR_lock_surface2')
EGL_LOCK_SURFACE_BIT_KHR = Enumerant('EGL_LOCK_SURFACE_BIT_KHR', 0x0080, 'EGL_KHR_lock_surface2')
EGL_LOCK_USAGE_HINT_KHR = Enumerant('EGL_LOCK_USAGE_HINT_KHR', 0x30c5, 'EGL_KHR_lock_surface2')
EGL_LOWER_LEFT_KHR = Enumerant('EGL_LOWER_LEFT_KHR', 0x30ce, 'EGL_KHR_lock_surface2')
EGL_MAP_PRESERVE_PIXELS_KHR = Enumerant('EGL_MAP_PRESERVE_PIXELS_KHR', 0x30c4, 'EGL_KHR_lock_surface2')
EGL_MATCH_FORMAT_KHR = Enumerant('EGL_MATCH_FORMAT_KHR', 0x3043, 'EGL_KHR_lock_surface2')
EGL_OPTIMAL_FORMAT_BIT_KHR = Enumerant('EGL_OPTIMAL_FORMAT_BIT_KHR', 0x0100, 'EGL_KHR_lock_surface2')
EGL_READ_SURFACE_BIT_KHR = Enumerant('EGL_READ_SURFACE_BIT_KHR', 0x0001, 'EGL_KHR_lock_surface2')
EGL_UPPER_LEFT_KHR = Enumerant('EGL_UPPER_LEFT_KHR', 0x30cf, 'EGL_KHR_lock_surface2')
EGL_WRITE_SURFACE_BIT_KHR = Enumerant('EGL_WRITE_SURFACE_BIT_KHR', 0x0002, 'EGL_KHR_lock_surface2')

defines.add(EGL_BITMAP_ORIGIN_KHR)
defines.add(EGL_BITMAP_PITCH_KHR)
defines.add(EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR)
defines.add(EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR)
defines.add(EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR)
defines.add(EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR)
defines.add(EGL_BITMAP_PIXEL_RED_OFFSET_KHR)
defines.add(EGL_BITMAP_PIXEL_SIZE_KHR)
defines.add(EGL_BITMAP_POINTER_KHR)
defines.add(EGL_FORMAT_RGBA_8888_EXACT_KHR)
defines.add(EGL_FORMAT_RGBA_8888_KHR)
defines.add(EGL_FORMAT_RGB_565_EXACT_KHR)
defines.add(EGL_FORMAT_RGB_565_KHR)
defines.add(EGL_LOCK_SURFACE_BIT_KHR)
defines.add(EGL_LOCK_USAGE_HINT_KHR)
defines.add(EGL_LOWER_LEFT_KHR)
defines.add(EGL_MAP_PRESERVE_PIXELS_KHR)
defines.add(EGL_MATCH_FORMAT_KHR)
defines.add(EGL_OPTIMAL_FORMAT_BIT_KHR)
defines.add(EGL_READ_SURFACE_BIT_KHR)
defines.add(EGL_UPPER_LEFT_KHR)
defines.add(EGL_WRITE_SURFACE_BIT_KHR)

# EGL_KHR_reusable_sync

EGL_SYNC_REUSABLE_KHR = Enumerant('EGL_SYNC_REUSABLE_KHR', 0x30fa, 'EGL_KHR_reusable_sync')

defines.add(EGL_SYNC_REUSABLE_KHR)

# EGL_KHR_stream

EGL_BAD_STATE_KHR = Enumerant('EGL_BAD_STATE_KHR', 0x321c, 'EGL_KHR_stream')
EGL_BAD_STREAM_KHR = Enumerant('EGL_BAD_STREAM_KHR', 0x321b, 'EGL_KHR_stream')
EGL_CONSUMER_FRAME_KHR = Enumerant('EGL_CONSUMER_FRAME_KHR', 0x3213, 'EGL_KHR_stream')
EGL_CONSUMER_LATENCY_USEC_KHR = Enumerant('EGL_CONSUMER_LATENCY_USEC_KHR', 0x3210, 'EGL_KHR_stream')
EGL_PRODUCER_FRAME_KHR = Enumerant('EGL_PRODUCER_FRAME_KHR', 0x3212, 'EGL_KHR_stream')
EGL_STREAM_STATE_CONNECTING_KHR = Enumerant('EGL_STREAM_STATE_CONNECTING_KHR', 0x3216, 'EGL_KHR_stream')
EGL_STREAM_STATE_CREATED_KHR = Enumerant('EGL_STREAM_STATE_CREATED_KHR', 0x3215, 'EGL_KHR_stream')
EGL_STREAM_STATE_DISCONNECTED_KHR = Enumerant('EGL_STREAM_STATE_DISCONNECTED_KHR', 0x321a, 'EGL_KHR_stream')
EGL_STREAM_STATE_EMPTY_KHR = Enumerant('EGL_STREAM_STATE_EMPTY_KHR', 0x3217, 'EGL_KHR_stream')
EGL_STREAM_STATE_KHR = Enumerant('EGL_STREAM_STATE_KHR', 0x3214, 'EGL_KHR_stream')
EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR = Enumerant('EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR', 0x3218, 'EGL_KHR_stream')
EGL_STREAM_STATE_OLD_FRAME_AVAILABLE_KHR = Enumerant('EGL_STREAM_STATE_OLD_FRAME_AVAILABLE_KHR', 0x3219, 'EGL_KHR_stream')

defines.add(EGL_BAD_STATE_KHR)
defines.add(EGL_BAD_STREAM_KHR)
defines.add(EGL_CONSUMER_FRAME_KHR)
defines.add(EGL_CONSUMER_LATENCY_USEC_KHR)
defines.add(EGL_PRODUCER_FRAME_KHR)
defines.add(EGL_STREAM_STATE_CONNECTING_KHR)
defines.add(EGL_STREAM_STATE_CREATED_KHR)
defines.add(EGL_STREAM_STATE_DISCONNECTED_KHR)
defines.add(EGL_STREAM_STATE_EMPTY_KHR)
defines.add(EGL_STREAM_STATE_KHR)
defines.add(EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR)
defines.add(EGL_STREAM_STATE_OLD_FRAME_AVAILABLE_KHR)

# EGL_KHR_stream_consumer_gltexture

EGL_CONSUMER_ACQUIRE_TIMEOUT_USEC_KHR = Enumerant('EGL_CONSUMER_ACQUIRE_TIMEOUT_USEC_KHR', 0x321e, 'EGL_KHR_stream_consumer_gltexture')

defines.add(EGL_CONSUMER_ACQUIRE_TIMEOUT_USEC_KHR)

# EGL_KHR_stream_fifo

EGL_STREAM_FIFO_LENGTH_KHR = Enumerant('EGL_STREAM_FIFO_LENGTH_KHR', 0x31fc, 'EGL_KHR_stream_fifo')
EGL_STREAM_TIME_CONSUMER_KHR = Enumerant('EGL_STREAM_TIME_CONSUMER_KHR', 0x31fe, 'EGL_KHR_stream_fifo')
EGL_STREAM_TIME_NOW_KHR = Enumerant('EGL_STREAM_TIME_NOW_KHR', 0x31fd, 'EGL_KHR_stream_fifo')
EGL_STREAM_TIME_PRODUCER_KHR = Enumerant('EGL_STREAM_TIME_PRODUCER_KHR', 0x31ff, 'EGL_KHR_stream_fifo')

defines.add(EGL_STREAM_FIFO_LENGTH_KHR)
defines.add(EGL_STREAM_TIME_CONSUMER_KHR)
defines.add(EGL_STREAM_TIME_NOW_KHR)
defines.add(EGL_STREAM_TIME_PRODUCER_KHR)

# EGL_KHR_stream_producer_eglsurface

EGL_STREAM_BIT_KHR = Enumerant('EGL_STREAM_BIT_KHR', 0x0800, 'EGL_KHR_stream_producer_eglsurface')

defines.add(EGL_STREAM_BIT_KHR)

# EGL_KHR_vg_parent_image

EGL_VG_PARENT_IMAGE_KHR = Enumerant('EGL_VG_PARENT_IMAGE_KHR', 0x30ba, 'EGL_KHR_vg_parent_image')

defines.add(EGL_VG_PARENT_IMAGE_KHR)

# EGL_MESA_drm_image

EGL_DRM_BUFFER_FORMAT_ARGB32_MESA = Enumerant('EGL_DRM_BUFFER_FORMAT_ARGB32_MESA', 0x31d2, 'EGL_MESA_drm_image')
EGL_DRM_BUFFER_FORMAT_MESA = Enumerant('EGL_DRM_BUFFER_FORMAT_MESA', 0x31d0, 'EGL_MESA_drm_image')
EGL_DRM_BUFFER_MESA = Enumerant('EGL_DRM_BUFFER_MESA', 0x31d3, 'EGL_MESA_drm_image')
EGL_DRM_BUFFER_STRIDE_MESA = Enumerant('EGL_DRM_BUFFER_STRIDE_MESA', 0x31d4, 'EGL_MESA_drm_image')
EGL_DRM_BUFFER_USE_MESA = Enumerant('EGL_DRM_BUFFER_USE_MESA', 0x31d1, 'EGL_MESA_drm_image')
EGL_DRM_BUFFER_USE_SCANOUT_MESA = Enumerant('EGL_DRM_BUFFER_USE_SCANOUT_MESA', 0x0001, 'EGL_MESA_drm_image')
EGL_DRM_BUFFER_USE_SHARE_MESA = Enumerant('EGL_DRM_BUFFER_USE_SHARE_MESA', 0x0002, 'EGL_MESA_drm_image')

defines.add(EGL_DRM_BUFFER_FORMAT_ARGB32_MESA)
defines.add(EGL_DRM_BUFFER_FORMAT_MESA)
defines.add(EGL_DRM_BUFFER_MESA)
defines.add(EGL_DRM_BUFFER_STRIDE_MESA)
defines.add(EGL_DRM_BUFFER_USE_MESA)
defines.add(EGL_DRM_BUFFER_USE_SCANOUT_MESA)
defines.add(EGL_DRM_BUFFER_USE_SHARE_MESA)

# EGL_NV_coverage_sample

EGL_COVERAGE_BUFFERS_NV = Enumerant('EGL_COVERAGE_BUFFERS_NV', 0x30e0, 'EGL_NV_coverage_sample')
EGL_COVERAGE_SAMPLES_NV = Enumerant('EGL_COVERAGE_SAMPLES_NV', 0x30e1, 'EGL_NV_coverage_sample')
GL_COVERAGE_ALL_FRAGMENTS_NV = Enumerant('GL_COVERAGE_ALL_FRAGMENTS_NV', 0x8ed5, 'EGL_NV_coverage_sample')
GL_COVERAGE_ATTACHMENT_NV = Enumerant('GL_COVERAGE_ATTACHMENT_NV', 0x8ed2, 'EGL_NV_coverage_sample')
GL_COVERAGE_AUTOMATIC_NV = Enumerant('GL_COVERAGE_AUTOMATIC_NV', 0x8ed7, 'EGL_NV_coverage_sample')
GL_COVERAGE_BUFFERS_NV = Enumerant('GL_COVERAGE_BUFFERS_NV', 0x8ed3, 'EGL_NV_coverage_sample')
GL_COVERAGE_BUFFER_BIT_NV = Enumerant('GL_COVERAGE_BUFFER_BIT_NV', 0x8000, 'EGL_NV_coverage_sample')
GL_COVERAGE_COMPONENT4_NV = Enumerant('GL_COVERAGE_COMPONENT4_NV', 0x8ed1, 'EGL_NV_coverage_sample')
GL_COVERAGE_COMPONENT_NV = Enumerant('GL_COVERAGE_COMPONENT_NV', 0x8ed0, 'EGL_NV_coverage_sample')
GL_COVERAGE_EDGE_FRAGMENTS_NV = Enumerant('GL_COVERAGE_EDGE_FRAGMENTS_NV', 0x8ed6, 'EGL_NV_coverage_sample')

defines.add(EGL_COVERAGE_BUFFERS_NV)
defines.add(EGL_COVERAGE_SAMPLES_NV)
defines.add(GL_COVERAGE_ALL_FRAGMENTS_NV)
defines.add(GL_COVERAGE_ATTACHMENT_NV)
defines.add(GL_COVERAGE_AUTOMATIC_NV)
defines.add(GL_COVERAGE_BUFFERS_NV)
defines.add(GL_COVERAGE_BUFFER_BIT_NV)
defines.add(GL_COVERAGE_COMPONENT4_NV)
defines.add(GL_COVERAGE_COMPONENT_NV)
defines.add(GL_COVERAGE_EDGE_FRAGMENTS_NV)

# EGL_NV_coverage_sample_resolve

EGL_COVERAGE_SAMPLE_RESOLVE_DEFAULT_NV = Enumerant('EGL_COVERAGE_SAMPLE_RESOLVE_DEFAULT_NV', 0x3132, 'EGL_NV_coverage_sample_resolve')
EGL_COVERAGE_SAMPLE_RESOLVE_NONE_NV = Enumerant('EGL_COVERAGE_SAMPLE_RESOLVE_NONE_NV', 0x3133, 'EGL_NV_coverage_sample_resolve')
EGL_COVERAGE_SAMPLE_RESOLVE_NV = Enumerant('EGL_COVERAGE_SAMPLE_RESOLVE_NV', 0x3131, 'EGL_NV_coverage_sample_resolve')

defines.add(EGL_COVERAGE_SAMPLE_RESOLVE_DEFAULT_NV)
defines.add(EGL_COVERAGE_SAMPLE_RESOLVE_NONE_NV)
defines.add(EGL_COVERAGE_SAMPLE_RESOLVE_NV)

# EGL_NV_post_sub_buffer

EGL_POST_SUB_BUFFER_SUPPORTED_NV = Enumerant('EGL_POST_SUB_BUFFER_SUPPORTED_NV', 0x30be, 'EGL_NV_post_sub_buffer')

defines.add(EGL_POST_SUB_BUFFER_SUPPORTED_NV)

# EGL_NV_sync

EGL_ALREADY_SIGNALED_NV = Enumerant('EGL_ALREADY_SIGNALED_NV', 0x30ea, 'EGL_NV_sync')
EGL_CONDITION_SATISFIED_NV = Enumerant('EGL_CONDITION_SATISFIED_NV', 0x30ec, 'EGL_NV_sync')
EGL_FOREVER_NV = Enumerant('EGL_FOREVER_NV', 0xffffffffffffffff, 'EGL_NV_sync')
EGL_SIGNALED_NV = Enumerant('EGL_SIGNALED_NV', 0x30e8, 'EGL_NV_sync')
EGL_SYNC_CONDITION_NV = Enumerant('EGL_SYNC_CONDITION_NV', 0x30ee, 'EGL_NV_sync')
EGL_SYNC_FENCE_NV = Enumerant('EGL_SYNC_FENCE_NV', 0x30ef, 'EGL_NV_sync')
EGL_SYNC_FLUSH_COMMANDS_BIT_NV = Enumerant('EGL_SYNC_FLUSH_COMMANDS_BIT_NV', 0x0001, 'EGL_NV_sync')
EGL_SYNC_PRIOR_COMMANDS_COMPLETE_NV = Enumerant('EGL_SYNC_PRIOR_COMMANDS_COMPLETE_NV', 0x30e6, 'EGL_NV_sync')
EGL_SYNC_STATUS_NV = Enumerant('EGL_SYNC_STATUS_NV', 0x30e7, 'EGL_NV_sync')
EGL_SYNC_TYPE_NV = Enumerant('EGL_SYNC_TYPE_NV', 0x30ed, 'EGL_NV_sync')
EGL_TIMEOUT_EXPIRED_NV = Enumerant('EGL_TIMEOUT_EXPIRED_NV', 0x30eb, 'EGL_NV_sync')
EGL_UNSIGNALED_NV = Enumerant('EGL_UNSIGNALED_NV', 0x30e9, 'EGL_NV_sync')

defines.add(EGL_ALREADY_SIGNALED_NV)
defines.add(EGL_CONDITION_SATISFIED_NV)
defines.add(EGL_FOREVER_NV)
defines.add(EGL_SIGNALED_NV)
defines.add(EGL_SYNC_CONDITION_NV)
defines.add(EGL_SYNC_FENCE_NV)
defines.add(EGL_SYNC_FLUSH_COMMANDS_BIT_NV)
defines.add(EGL_SYNC_PRIOR_COMMANDS_COMPLETE_NV)
defines.add(EGL_SYNC_STATUS_NV)
defines.add(EGL_SYNC_TYPE_NV)
defines.add(EGL_TIMEOUT_EXPIRED_NV)
defines.add(EGL_UNSIGNALED_NV)

# GL_NV_depth_nonlinear

EGL_DEPTH_ENCODING_NONE_NV = Enumerant('EGL_DEPTH_ENCODING_NONE_NV', 0x0000, 'GL_NV_depth_nonlinear')
EGL_DEPTH_ENCODING_NONLINEAR_NV = Enumerant('EGL_DEPTH_ENCODING_NONLINEAR_NV', 0x30e3, 'GL_NV_depth_nonlinear')
EGL_DEPTH_ENCODING_NV = Enumerant('EGL_DEPTH_ENCODING_NV', 0x30e2, 'GL_NV_depth_nonlinear')
GL_DEPTH_COMPONENT16_NONLINEAR_NV = Enumerant('GL_DEPTH_COMPONENT16_NONLINEAR_NV', 0x8e2c, 'GL_NV_depth_nonlinear')

defines.add(EGL_DEPTH_ENCODING_NONE_NV)
defines.add(EGL_DEPTH_ENCODING_NONLINEAR_NV)
defines.add(EGL_DEPTH_ENCODING_NV)
defines.add(GL_DEPTH_COMPONENT16_NONLINEAR_NV)

# GL_VG_KHR_EGL_sync

EGL_CONDITION_SATISFIED_KHR = Enumerant('EGL_CONDITION_SATISFIED_KHR', 0x30f6, 'GL_VG_KHR_EGL_sync')
EGL_FOREVER_KHR = Enumerant('EGL_FOREVER_KHR', 0xffffffffffffffff, 'GL_VG_KHR_EGL_sync')
EGL_SIGNALED_KHR = Enumerant('EGL_SIGNALED_KHR', 0x30f2, 'GL_VG_KHR_EGL_sync')
EGL_SYNC_CONDITION_KHR = Enumerant('EGL_SYNC_CONDITION_KHR', 0x30f8, 'GL_VG_KHR_EGL_sync')
EGL_SYNC_FENCE_KHR = Enumerant('EGL_SYNC_FENCE_KHR', 0x30f9, 'GL_VG_KHR_EGL_sync')
EGL_SYNC_FLUSH_COMMANDS_BIT_KHR = Enumerant('EGL_SYNC_FLUSH_COMMANDS_BIT_KHR', 0x0001, 'GL_VG_KHR_EGL_sync')
EGL_SYNC_PRIOR_COMMANDS_COMPLETE_KHR = Enumerant('EGL_SYNC_PRIOR_COMMANDS_COMPLETE_KHR', 0x30f0, 'GL_VG_KHR_EGL_sync')
EGL_SYNC_STATUS_KHR = Enumerant('EGL_SYNC_STATUS_KHR', 0x30f1, 'GL_VG_KHR_EGL_sync')
EGL_SYNC_TYPE_KHR = Enumerant('EGL_SYNC_TYPE_KHR', 0x30f7, 'GL_VG_KHR_EGL_sync')
EGL_TIMEOUT_EXPIRED_KHR = Enumerant('EGL_TIMEOUT_EXPIRED_KHR', 0x30f5, 'GL_VG_KHR_EGL_sync')
EGL_UNSIGNALED_KHR = Enumerant('EGL_UNSIGNALED_KHR', 0x30f3, 'GL_VG_KHR_EGL_sync')

defines.add(EGL_CONDITION_SATISFIED_KHR)
defines.add(EGL_FOREVER_KHR)
defines.add(EGL_SIGNALED_KHR)
defines.add(EGL_SYNC_CONDITION_KHR)
defines.add(EGL_SYNC_FENCE_KHR)
defines.add(EGL_SYNC_FLUSH_COMMANDS_BIT_KHR)
defines.add(EGL_SYNC_PRIOR_COMMANDS_COMPLETE_KHR)
defines.add(EGL_SYNC_STATUS_KHR)
defines.add(EGL_SYNC_TYPE_KHR)
defines.add(EGL_TIMEOUT_EXPIRED_KHR)
defines.add(EGL_UNSIGNALED_KHR)

# EGL_ANGLE_query_surface_pointer

eglQuerySurfacePointerANGLE = Function('eglQuerySurfacePointerANGLE')
eglQuerySurfacePointerANGLE.ret = Return('EGLBoolean')
eglQuerySurfacePointerANGLE.add( Input( 'dpy','EGLDisplay' ))
eglQuerySurfacePointerANGLE.add( Input( 'surface','EGLSurface' ))
eglQuerySurfacePointerANGLE.add( Input( 'attribute','EGLint' ))
eglQuerySurfacePointerANGLE.add( Input( 'value','GLvoid **' ))
eglQuerySurfacePointerANGLE.version = ''
eglQuerySurfacePointerANGLE.category = 'EGL_ANGLE_query_surface_pointer'
eglQuerySurfacePointerANGLE.trace = True
eglQuerySurfacePointerANGLE.play = True
egl.add(eglQuerySurfacePointerANGLE)

# EGL_KHR_fence_sync

eglClientWaitSyncKHR = Function('eglClientWaitSyncKHR')
eglClientWaitSyncKHR.ret = Return('EGLint')
eglClientWaitSyncKHR.add( Input( 'dpy','EGLDisplay' ))
eglClientWaitSyncKHR.add( Input( 'GLsync','EGLSyncKHR' ))
eglClientWaitSyncKHR.add( Input( 'flags','EGLint' ))
eglClientWaitSyncKHR.add( Input( 'timeout','EGLTimeKHR' ))
eglClientWaitSyncKHR.version = ''
eglClientWaitSyncKHR.category = 'EGL_KHR_fence_sync'
eglClientWaitSyncKHR.trace = True
eglClientWaitSyncKHR.play = True
egl.add(eglClientWaitSyncKHR)

eglCreateSyncKHR = Function('eglCreateSyncKHR')
eglCreateSyncKHR.ret = Return('EGLSyncKHR')
eglCreateSyncKHR.add( Input( 'dpy','EGLDisplay' ))
eglCreateSyncKHR.add( Input( 'type','EGLenum' ))
eglCreateSyncKHR.add( Input( 'attrib_list','const EGLint *' ))
eglCreateSyncKHR.version = ''
eglCreateSyncKHR.category = 'EGL_KHR_fence_sync'
eglCreateSyncKHR.trace = True
eglCreateSyncKHR.play = True
egl.add(eglCreateSyncKHR)

eglDestroySyncKHR = Function('eglDestroySyncKHR')
eglDestroySyncKHR.ret = Return('EGLBoolean')
eglDestroySyncKHR.add( Input( 'dpy','EGLDisplay' ))
eglDestroySyncKHR.add( Input( 'GLsync','EGLSyncKHR' ))
eglDestroySyncKHR.version = ''
eglDestroySyncKHR.category = 'EGL_KHR_fence_sync'
eglDestroySyncKHR.trace = True
eglDestroySyncKHR.play = True
egl.add(eglDestroySyncKHR)

eglGetSyncAttribKHR = Function('eglGetSyncAttribKHR')
eglGetSyncAttribKHR.ret = Return('EGLBoolean')
eglGetSyncAttribKHR.add( Input( 'dpy','EGLDisplay' ))
eglGetSyncAttribKHR.add( Input( 'GLsync','EGLSyncKHR' ))
eglGetSyncAttribKHR.add( Input( 'attribute','EGLint' ))
eglGetSyncAttribKHR.add( Input( 'value','EGLint *' ))
eglGetSyncAttribKHR.version = ''
eglGetSyncAttribKHR.category = 'EGL_KHR_fence_sync'
eglGetSyncAttribKHR.trace = True
eglGetSyncAttribKHR.play = True
egl.add(eglGetSyncAttribKHR)

# EGL_KHR_image_base

eglCreateImageKHR = Function('eglCreateImageKHR')
eglCreateImageKHR.ret = Return('EGLImageKHR')
eglCreateImageKHR.add( Input( 'dpy','EGLDisplay' ))
eglCreateImageKHR.add( Input( 'ctx','EGLContext' ))
eglCreateImageKHR.add( Input( 'target','EGLenum' ))
eglCreateImageKHR.add( Input( 'buffer','EGLClientBuffer' ))
eglCreateImageKHR.add( Input( 'attrib_list','const EGLint *' ))
eglCreateImageKHR.version = ''
eglCreateImageKHR.category = 'EGL_KHR_image_base'
eglCreateImageKHR.trace = True
eglCreateImageKHR.play = True
egl.add(eglCreateImageKHR)

eglDestroyImageKHR = Function('eglDestroyImageKHR')
eglDestroyImageKHR.ret = Return('EGLBoolean')
eglDestroyImageKHR.add( Input( 'dpy','EGLDisplay' ))
eglDestroyImageKHR.add( Input( 'image','EGLImageKHR' ))
eglDestroyImageKHR.version = ''
eglDestroyImageKHR.category = 'EGL_KHR_image_base'
eglDestroyImageKHR.trace = True
eglDestroyImageKHR.play = True
egl.add(eglDestroyImageKHR)

# EGL_KHR_lock_surface

eglLockSurfaceKHR = Function('eglLockSurfaceKHR')
eglLockSurfaceKHR.ret = Return('EGLBoolean')
eglLockSurfaceKHR.add( Input( 'display','EGLDisplay' ))
eglLockSurfaceKHR.add( Input( 'surface','EGLSurface' ))
eglLockSurfaceKHR.add( Input( 'attrib_list','const EGLint *' ))
eglLockSurfaceKHR.version = ''
eglLockSurfaceKHR.category = 'EGL_KHR_lock_surface'
eglLockSurfaceKHR.trace = True
eglLockSurfaceKHR.play = True
egl.add(eglLockSurfaceKHR)

eglUnlockSurfaceKHR = Function('eglUnlockSurfaceKHR')
eglUnlockSurfaceKHR.ret = Return('EGLBoolean')
eglUnlockSurfaceKHR.add( Input( 'display','EGLDisplay' ))
eglUnlockSurfaceKHR.add( Input( 'surface','EGLSurface' ))
eglUnlockSurfaceKHR.version = ''
eglUnlockSurfaceKHR.category = 'EGL_KHR_lock_surface'
eglUnlockSurfaceKHR.trace = True
eglUnlockSurfaceKHR.play = True
egl.add(eglUnlockSurfaceKHR)

# EGL_KHR_stream_consumer_gltexture

eglStreamConsumerAcquireKHR = Function('eglStreamConsumerAcquireKHR')
eglStreamConsumerAcquireKHR.ret = Return('EGLBoolean')
eglStreamConsumerAcquireKHR.add( Input( 'dpy','EGLDisplay' ))
eglStreamConsumerAcquireKHR.add( Input( 'stream','EGLStreamKHR' ))
eglStreamConsumerAcquireKHR.version = ''
eglStreamConsumerAcquireKHR.category = 'EGL_KHR_stream_consumer_gltexture'
eglStreamConsumerAcquireKHR.trace = True
eglStreamConsumerAcquireKHR.play = True
egl.add(eglStreamConsumerAcquireKHR)

eglStreamConsumerGLTextureExternalKHR = Function('eglStreamConsumerGLTextureExternalKHR')
eglStreamConsumerGLTextureExternalKHR.ret = Return('EGLBoolean')
eglStreamConsumerGLTextureExternalKHR.add( Input( 'dpy','EGLDisplay' ))
eglStreamConsumerGLTextureExternalKHR.add( Input( 'stream','EGLStreamKHR' ))
eglStreamConsumerGLTextureExternalKHR.version = ''
eglStreamConsumerGLTextureExternalKHR.category = 'EGL_KHR_stream_consumer_gltexture'
eglStreamConsumerGLTextureExternalKHR.trace = True
eglStreamConsumerGLTextureExternalKHR.play = True
egl.add(eglStreamConsumerGLTextureExternalKHR)

eglStreamConsumerReleaseKHR = Function('eglStreamConsumerReleaseKHR')
eglStreamConsumerReleaseKHR.ret = Return('EGLBoolean')
eglStreamConsumerReleaseKHR.add( Input( 'dpy','EGLDisplay' ))
eglStreamConsumerReleaseKHR.add( Input( 'stream','EGLStreamKHR' ))
eglStreamConsumerReleaseKHR.version = ''
eglStreamConsumerReleaseKHR.category = 'EGL_KHR_stream_consumer_gltexture'
eglStreamConsumerReleaseKHR.trace = True
eglStreamConsumerReleaseKHR.play = True
egl.add(eglStreamConsumerReleaseKHR)

# EGL_KHR_stream_cross_process_fd

eglCreateStreamFromFileDescriptorKHR = Function('eglCreateStreamFromFileDescriptorKHR')
eglCreateStreamFromFileDescriptorKHR.ret = Return('EGLStreamKHR')
eglCreateStreamFromFileDescriptorKHR.add( Input( 'dpy','EGLDisplay' ))
eglCreateStreamFromFileDescriptorKHR.add( Input( 'file_descriptor','EGLNativeFileDescriptorKHR' ))
eglCreateStreamFromFileDescriptorKHR.version = ''
eglCreateStreamFromFileDescriptorKHR.category = 'EGL_KHR_stream_cross_process_fd'
eglCreateStreamFromFileDescriptorKHR.trace = True
eglCreateStreamFromFileDescriptorKHR.play = True
egl.add(eglCreateStreamFromFileDescriptorKHR)

eglGetStreamFileDescriptorKHR = Function('eglGetStreamFileDescriptorKHR')
eglGetStreamFileDescriptorKHR.ret = Return('EGLNativeFileDescriptorKHR')
eglGetStreamFileDescriptorKHR.add( Input( 'dpy','EGLDisplay' ))
eglGetStreamFileDescriptorKHR.add( Input( 'stream','EGLStreamKHR' ))
eglGetStreamFileDescriptorKHR.version = ''
eglGetStreamFileDescriptorKHR.category = 'EGL_KHR_stream_cross_process_fd'
eglGetStreamFileDescriptorKHR.trace = True
eglGetStreamFileDescriptorKHR.play = True
egl.add(eglGetStreamFileDescriptorKHR)

# EGL_KHR_stream_producer_eglsurface

eglCreateStreamProducerSurfaceKHR = Function('eglCreateStreamProducerSurfaceKHR')
eglCreateStreamProducerSurfaceKHR.ret = Return('EGLSurface')
eglCreateStreamProducerSurfaceKHR.add( Input( 'dpy','EGLDisplay' ))
eglCreateStreamProducerSurfaceKHR.add( Input( 'config','EGLConfig' ))
eglCreateStreamProducerSurfaceKHR.add( Input( 'stream','EGLStreamKHR' ))
eglCreateStreamProducerSurfaceKHR.add( Input( 'attrib_list','const EGLint *' ))
eglCreateStreamProducerSurfaceKHR.version = ''
eglCreateStreamProducerSurfaceKHR.category = 'EGL_KHR_stream_producer_eglsurface'
eglCreateStreamProducerSurfaceKHR.trace = True
eglCreateStreamProducerSurfaceKHR.play = True
egl.add(eglCreateStreamProducerSurfaceKHR)

# EGL_KHR_wait_sync

eglWaitSyncKHR = Function('eglWaitSyncKHR')
eglWaitSyncKHR.ret = Return('EGLint')
eglWaitSyncKHR.add( Input( 'dpy','EGLDisplay' ))
eglWaitSyncKHR.add( Input( 'GLsync','EGLSyncKHR' ))
eglWaitSyncKHR.add( Input( 'flags','EGLint' ))
eglWaitSyncKHR.version = ''
eglWaitSyncKHR.category = 'EGL_KHR_wait_sync'
eglWaitSyncKHR.trace = True
eglWaitSyncKHR.play = True
egl.add(eglWaitSyncKHR)

# EGL_MESA_drm_image

eglCreateDRMImageMESA = Function('eglCreateDRMImageMESA')
eglCreateDRMImageMESA.ret = Return('EGLImageKHR')
eglCreateDRMImageMESA.add( Input( 'dpy','EGLDisplay' ))
eglCreateDRMImageMESA.add( Input( 'attrib_list','const EGLint *' ))
eglCreateDRMImageMESA.version = ''
eglCreateDRMImageMESA.category = 'EGL_MESA_drm_image'
eglCreateDRMImageMESA.trace = True
eglCreateDRMImageMESA.play = True
egl.add(eglCreateDRMImageMESA)

eglExportDRMImageMESA = Function('eglExportDRMImageMESA')
eglExportDRMImageMESA.ret = Return('EGLBoolean')
eglExportDRMImageMESA.add( Input( 'dpy','EGLDisplay' ))
eglExportDRMImageMESA.add( Input( 'image','EGLImageKHR' ))
eglExportDRMImageMESA.add( Input( 'name','EGLint *' ))
eglExportDRMImageMESA.add( Input( 'handle','EGLint *' ))
eglExportDRMImageMESA.add( Input( 'stride','EGLint *' ))
eglExportDRMImageMESA.version = ''
eglExportDRMImageMESA.category = 'EGL_MESA_drm_image'
eglExportDRMImageMESA.trace = True
eglExportDRMImageMESA.play = True
egl.add(eglExportDRMImageMESA)

# EGL_NV_coverage_sample

eglCoverageMaskNV = Function('eglCoverageMaskNV')
eglCoverageMaskNV.ret = Return('void')
eglCoverageMaskNV.add( Input( 'mask','GLboolean' ))
eglCoverageMaskNV.version = ''
eglCoverageMaskNV.category = 'EGL_NV_coverage_sample'
eglCoverageMaskNV.trace = True
eglCoverageMaskNV.play = True
egl.add(eglCoverageMaskNV)

eglCoverageOperationNV = Function('eglCoverageOperationNV')
eglCoverageOperationNV.ret = Return('void')
eglCoverageOperationNV.add( Input( 'operation','GLenum' ))
eglCoverageOperationNV.version = ''
eglCoverageOperationNV.category = 'EGL_NV_coverage_sample'
eglCoverageOperationNV.trace = True
eglCoverageOperationNV.play = True
egl.add(eglCoverageOperationNV)

# EGL_NV_post_sub_buffer

eglPostSubBufferNV = Function('eglPostSubBufferNV')
eglPostSubBufferNV.ret = Return('EGLBoolean')
eglPostSubBufferNV.add( Input( 'dpy','EGLDisplay' ))
eglPostSubBufferNV.add( Input( 'surface','EGLSurface' ))
eglPostSubBufferNV.add( Input( 'x','EGLint' ))
eglPostSubBufferNV.add( Input( 'y','EGLint' ))
eglPostSubBufferNV.add( Input( 'width','EGLint' ))
eglPostSubBufferNV.add( Input( 'height','EGLint' ))
eglPostSubBufferNV.version = ''
eglPostSubBufferNV.category = 'EGL_NV_post_sub_buffer'
eglPostSubBufferNV.trace = True
eglPostSubBufferNV.play = True
egl.add(eglPostSubBufferNV)

# EGL_NV_sync

eglClientWaitSyncNV = Function('eglClientWaitSyncNV')
eglClientWaitSyncNV.ret = Return('EGLint')
eglClientWaitSyncNV.add( Input( 'GLsync','EGLSyncNV' ))
eglClientWaitSyncNV.add( Input( 'flags','EGLint' ))
eglClientWaitSyncNV.add( Input( 'timeout','EGLTimeNV' ))
eglClientWaitSyncNV.version = ''
eglClientWaitSyncNV.category = 'EGL_NV_sync'
eglClientWaitSyncNV.trace = True
eglClientWaitSyncNV.play = True
egl.add(eglClientWaitSyncNV)

eglCreateFenceSyncNV = Function('eglCreateFenceSyncNV')
eglCreateFenceSyncNV.ret = Return('EGLSyncNV')
eglCreateFenceSyncNV.add( Input( 'dpy','EGLDisplay' ))
eglCreateFenceSyncNV.add( Input( 'condition','EGLenum' ))
eglCreateFenceSyncNV.add( Input( 'attrib_list','const EGLint *' ))
eglCreateFenceSyncNV.version = ''
eglCreateFenceSyncNV.category = 'EGL_NV_sync'
eglCreateFenceSyncNV.trace = True
eglCreateFenceSyncNV.play = True
egl.add(eglCreateFenceSyncNV)

eglDestroySyncNV = Function('eglDestroySyncNV')
eglDestroySyncNV.ret = Return('EGLBoolean')
eglDestroySyncNV.add( Input( 'GLsync','EGLSyncNV' ))
eglDestroySyncNV.version = ''
eglDestroySyncNV.category = 'EGL_NV_sync'
eglDestroySyncNV.trace = True
eglDestroySyncNV.play = True
egl.add(eglDestroySyncNV)

eglFenceNV = Function('eglFenceNV')
eglFenceNV.ret = Return('EGLBoolean')
eglFenceNV.add( Input( 'GLsync','EGLSyncNV' ))
eglFenceNV.version = ''
eglFenceNV.category = 'EGL_NV_sync'
eglFenceNV.trace = True
eglFenceNV.play = True
egl.add(eglFenceNV)

eglGetSyncAttribNV = Function('eglGetSyncAttribNV')
eglGetSyncAttribNV.ret = Return('EGLBoolean')
eglGetSyncAttribNV.add( Input( 'GLsync','EGLSyncNV' ))
eglGetSyncAttribNV.add( Input( 'attribute','EGLint' ))
eglGetSyncAttribNV.add( Input( 'value','EGLint *' ))
eglGetSyncAttribNV.version = ''
eglGetSyncAttribNV.category = 'EGL_NV_sync'
eglGetSyncAttribNV.trace = True
eglGetSyncAttribNV.play = True
egl.add(eglGetSyncAttribNV)

eglSignalSyncNV = Function('eglSignalSyncNV')
eglSignalSyncNV.ret = Return('EGLBoolean')
eglSignalSyncNV.add( Input( 'GLsync','EGLSyncNV' ))
eglSignalSyncNV.add( Input( 'mode','EGLenum' ))
eglSignalSyncNV.version = ''
eglSignalSyncNV.category = 'EGL_NV_sync'
eglSignalSyncNV.trace = True
eglSignalSyncNV.play = True
egl.add(eglSignalSyncNV)

# EGL_NV_system_time

eglGetSystemTimeFrequencyNV = Function('eglGetSystemTimeFrequencyNV')
eglGetSystemTimeFrequencyNV.ret = Return('EGLuint64NV')
eglGetSystemTimeFrequencyNV.version = ''
eglGetSystemTimeFrequencyNV.category = 'EGL_NV_system_time'
eglGetSystemTimeFrequencyNV.trace = True
eglGetSystemTimeFrequencyNV.play = True
egl.add(eglGetSystemTimeFrequencyNV)

eglGetSystemTimeNV = Function('eglGetSystemTimeNV')
eglGetSystemTimeNV.ret = Return('EGLuint64NV')
eglGetSystemTimeNV.version = ''
eglGetSystemTimeNV.category = 'EGL_NV_system_time'
eglGetSystemTimeNV.trace = True
eglGetSystemTimeNV.play = True
egl.add(eglGetSystemTimeNV)

# EGL_VERSION_1_0

eglChooseConfig = Function('eglChooseConfig')
eglChooseConfig.ret = Return('EGLBoolean')
eglChooseConfig.add( Input( 'dpy','EGLDisplay' ))
eglChooseConfig.add( Input( 'attrib_list','const EGLint *' ))
eglChooseConfig.add( Input( 'configs','EGLConfig *' ))
eglChooseConfig.add( Input( 'config_size','EGLint' ))
eglChooseConfig.add( Input( 'num_config','EGLint *' ))
eglChooseConfig.version = ''
eglChooseConfig.category = 'EGL_VERSION_1_0'
eglChooseConfig.trace = True
eglChooseConfig.play = True
egl.add(eglChooseConfig)

eglCopyBuffers = Function('eglCopyBuffers')
eglCopyBuffers.ret = Return('EGLBoolean')
eglCopyBuffers.add( Input( 'dpy','EGLDisplay' ))
eglCopyBuffers.add( Input( 'surface','EGLSurface' ))
eglCopyBuffers.add( Input( 'target','EGLNativePixmapType' ))
eglCopyBuffers.version = ''
eglCopyBuffers.category = 'EGL_VERSION_1_0'
eglCopyBuffers.trace = True
eglCopyBuffers.play = True
egl.add(eglCopyBuffers)

eglCreateContext = Function('eglCreateContext')
eglCreateContext.ret = Return('EGLContext')
eglCreateContext.add( Input( 'dpy','EGLDisplay' ))
eglCreateContext.add( Input( 'config','EGLConfig' ))
eglCreateContext.add( Input( 'share_context','EGLContext' ))
eglCreateContext.add( Input( 'attrib_list','const EGLint *' ))
eglCreateContext.version = ''
eglCreateContext.category = 'EGL_VERSION_1_0'
eglCreateContext.trace = True
eglCreateContext.play = True
egl.add(eglCreateContext)

eglCreatePbufferSurface = Function('eglCreatePbufferSurface')
eglCreatePbufferSurface.ret = Return('EGLSurface')
eglCreatePbufferSurface.add( Input( 'dpy','EGLDisplay' ))
eglCreatePbufferSurface.add( Input( 'config','EGLConfig' ))
eglCreatePbufferSurface.add( Input( 'attrib_list','const EGLint *' ))
eglCreatePbufferSurface.version = ''
eglCreatePbufferSurface.category = 'EGL_VERSION_1_0'
eglCreatePbufferSurface.trace = True
eglCreatePbufferSurface.play = True
egl.add(eglCreatePbufferSurface)

eglCreatePixmapSurface = Function('eglCreatePixmapSurface')
eglCreatePixmapSurface.ret = Return('EGLSurface')
eglCreatePixmapSurface.add( Input( 'dpy','EGLDisplay' ))
eglCreatePixmapSurface.add( Input( 'config','EGLConfig' ))
eglCreatePixmapSurface.add( Input( 'pixmap','EGLNativePixmapType' ))
eglCreatePixmapSurface.add( Input( 'attrib_list','const EGLint *' ))
eglCreatePixmapSurface.version = ''
eglCreatePixmapSurface.category = 'EGL_VERSION_1_0'
eglCreatePixmapSurface.trace = True
eglCreatePixmapSurface.play = True
egl.add(eglCreatePixmapSurface)

eglCreateWindowSurface = Function('eglCreateWindowSurface')
eglCreateWindowSurface.ret = Return('EGLSurface')
eglCreateWindowSurface.add( Input( 'dpy','EGLDisplay' ))
eglCreateWindowSurface.add( Input( 'config','EGLConfig' ))
eglCreateWindowSurface.add( Input( 'win','EGLNativeWindowType' ))
eglCreateWindowSurface.add( Input( 'attrib_list','const EGLint *' ))
eglCreateWindowSurface.version = ''
eglCreateWindowSurface.category = 'EGL_VERSION_1_0'
eglCreateWindowSurface.trace = True
eglCreateWindowSurface.play = True
egl.add(eglCreateWindowSurface)

eglDestroyContext = Function('eglDestroyContext')
eglDestroyContext.ret = Return('EGLBoolean')
eglDestroyContext.add( Input( 'dpy','EGLDisplay' ))
eglDestroyContext.add( Input( 'ctx','EGLContext' ))
eglDestroyContext.version = ''
eglDestroyContext.category = 'EGL_VERSION_1_0'
eglDestroyContext.trace = True
eglDestroyContext.play = True
egl.add(eglDestroyContext)

eglDestroySurface = Function('eglDestroySurface')
eglDestroySurface.ret = Return('EGLBoolean')
eglDestroySurface.add( Input( 'dpy','EGLDisplay' ))
eglDestroySurface.add( Input( 'surface','EGLSurface' ))
eglDestroySurface.version = ''
eglDestroySurface.category = 'EGL_VERSION_1_0'
eglDestroySurface.trace = True
eglDestroySurface.play = True
egl.add(eglDestroySurface)

eglGetConfigAttrib = Function('eglGetConfigAttrib')
eglGetConfigAttrib.ret = Return('EGLBoolean')
eglGetConfigAttrib.add( Input( 'dpy','EGLDisplay' ))
eglGetConfigAttrib.add( Input( 'config','EGLConfig' ))
eglGetConfigAttrib.add( Input( 'attribute','EGLint' ))
eglGetConfigAttrib.add( Input( 'value','EGLint *' ))
eglGetConfigAttrib.version = ''
eglGetConfigAttrib.category = 'EGL_VERSION_1_0'
eglGetConfigAttrib.trace = True
eglGetConfigAttrib.play = True
egl.add(eglGetConfigAttrib)

eglGetConfigs = Function('eglGetConfigs')
eglGetConfigs.ret = Return('EGLBoolean')
eglGetConfigs.add( Input( 'dpy','EGLDisplay' ))
eglGetConfigs.add( Input( 'configs','EGLConfig *' ))
eglGetConfigs.add( Input( 'config_size','EGLint' ))
eglGetConfigs.add( Input( 'num_config','EGLint *' ))
eglGetConfigs.version = ''
eglGetConfigs.category = 'EGL_VERSION_1_0'
eglGetConfigs.trace = True
eglGetConfigs.play = True
egl.add(eglGetConfigs)

eglGetCurrentContext = Function('eglGetCurrentContext')
eglGetCurrentContext.ret = Return('EGLContext')
eglGetCurrentContext.version = ''
eglGetCurrentContext.category = 'EGL_VERSION_1_0'
eglGetCurrentContext.trace = True
eglGetCurrentContext.play = True
egl.add(eglGetCurrentContext)

eglGetCurrentDisplay = Function('eglGetCurrentDisplay')
eglGetCurrentDisplay.ret = Return('EGLDisplay')
eglGetCurrentDisplay.version = ''
eglGetCurrentDisplay.category = 'EGL_VERSION_1_0'
eglGetCurrentDisplay.trace = True
eglGetCurrentDisplay.play = True
egl.add(eglGetCurrentDisplay)

eglGetCurrentSurface = Function('eglGetCurrentSurface')
eglGetCurrentSurface.ret = Return('EGLSurface')
eglGetCurrentSurface.add( Input( 'readdraw','EGLint' ))
eglGetCurrentSurface.version = ''
eglGetCurrentSurface.category = 'EGL_VERSION_1_0'
eglGetCurrentSurface.trace = True
eglGetCurrentSurface.play = True
egl.add(eglGetCurrentSurface)

eglGetDisplay = Function('eglGetDisplay')
eglGetDisplay.ret = Return('EGLDisplay')
eglGetDisplay.add( Input( 'display_id','EGLNativeDisplayType' ))
eglGetDisplay.version = ''
eglGetDisplay.category = 'EGL_VERSION_1_0'
eglGetDisplay.trace = True
eglGetDisplay.play = True
egl.add(eglGetDisplay)

eglGetError = Function('eglGetError')
eglGetError.ret = Return('EGLint')
eglGetError.version = ''
eglGetError.category = 'EGL_VERSION_1_0'
eglGetError.trace = True
eglGetError.play = True
egl.add(eglGetError)

eglGetProcAddress = Function('eglGetProcAddress')
eglGetProcAddress.ret = Return('__eglMustCastToProperFunctionPointerType')
eglGetProcAddress.add( Input( 'procname','const char *' ))
eglGetProcAddress.version = ''
eglGetProcAddress.category = 'EGL_VERSION_1_0'
eglGetProcAddress.trace = True
eglGetProcAddress.play = True
egl.add(eglGetProcAddress)

eglInitialize = Function('eglInitialize')
eglInitialize.ret = Return('EGLBoolean')
eglInitialize.add( Input( 'dpy','EGLDisplay' ))
eglInitialize.add( Output( 'major','EGLint *' ))
eglInitialize.add( Output( 'minor','EGLint *' ))
eglInitialize.version = ''
eglInitialize.category = 'EGL_VERSION_1_0'
eglInitialize.trace = True
eglInitialize.play = True
egl.add(eglInitialize)

eglMakeCurrent = Function('eglMakeCurrent')
eglMakeCurrent.ret = Return('EGLBoolean')
eglMakeCurrent.add( Input( 'dpy','EGLDisplay' ))
eglMakeCurrent.add( Input( 'draw','EGLSurface' ))
eglMakeCurrent.add( Input( 'read','EGLSurface' ))
eglMakeCurrent.add( Input( 'ctx','EGLContext' ))
eglMakeCurrent.version = ''
eglMakeCurrent.category = 'EGL_VERSION_1_0'
eglMakeCurrent.trace = True
eglMakeCurrent.play = True
egl.add(eglMakeCurrent)

eglQueryContext = Function('eglQueryContext')
eglQueryContext.ret = Return('EGLBoolean')
eglQueryContext.add( Input( 'dpy','EGLDisplay' ))
eglQueryContext.add( Input( 'ctx','EGLContext' ))
eglQueryContext.add( Input( 'attribute','EGLint' ))
eglQueryContext.add( Output( 'value','EGLint *' ))
eglQueryContext.version = ''
eglQueryContext.category = 'EGL_VERSION_1_0'
eglQueryContext.trace = True
eglQueryContext.play = True
egl.add(eglQueryContext)

eglQueryString = Function('eglQueryString')
eglQueryString.ret = Return('const char *')
eglQueryString.add( Input( 'dpy','EGLDisplay' ))
eglQueryString.add( Input( 'name','EGLint' ,regalLog = 'EGLenumToString(name)' ))
eglQueryString.version = ''
eglQueryString.category = 'EGL_VERSION_1_0'
eglQueryString.trace = True
eglQueryString.play = True
egl.add(eglQueryString)

eglQuerySurface = Function('eglQuerySurface')
eglQuerySurface.ret = Return('EGLBoolean')
eglQuerySurface.add( Input( 'dpy','EGLDisplay' ))
eglQuerySurface.add( Input( 'surface','EGLSurface' ))
eglQuerySurface.add( Input( 'attribute','EGLint' ))
eglQuerySurface.add( Output( 'value','EGLint *' ))
eglQuerySurface.version = ''
eglQuerySurface.category = 'EGL_VERSION_1_0'
eglQuerySurface.trace = True
eglQuerySurface.play = True
egl.add(eglQuerySurface)

eglSwapBuffers = Function('eglSwapBuffers')
eglSwapBuffers.ret = Return('EGLBoolean')
eglSwapBuffers.add( Input( 'dpy','EGLDisplay' ))
eglSwapBuffers.add( Input( 'surface','EGLSurface' ))
eglSwapBuffers.version = ''
eglSwapBuffers.category = 'EGL_VERSION_1_0'
eglSwapBuffers.trace = True
eglSwapBuffers.play = True
egl.add(eglSwapBuffers)

eglTerminate = Function('eglTerminate')
eglTerminate.ret = Return('EGLBoolean')
eglTerminate.add( Input( 'dpy','EGLDisplay' ))
eglTerminate.version = ''
eglTerminate.category = 'EGL_VERSION_1_0'
eglTerminate.trace = True
eglTerminate.play = True
egl.add(eglTerminate)

eglWaitGL = Function('eglWaitGL')
eglWaitGL.ret = Return('EGLBoolean')
eglWaitGL.version = ''
eglWaitGL.category = 'EGL_VERSION_1_0'
eglWaitGL.trace = True
eglWaitGL.play = True
egl.add(eglWaitGL)

eglWaitNative = Function('eglWaitNative')
eglWaitNative.ret = Return('EGLBoolean')
eglWaitNative.add( Input( 'engine','EGLint' ))
eglWaitNative.version = ''
eglWaitNative.category = 'EGL_VERSION_1_0'
eglWaitNative.trace = True
eglWaitNative.play = True
egl.add(eglWaitNative)

# EGL_VERSION_1_1

eglBindTexImage = Function('eglBindTexImage')
eglBindTexImage.ret = Return('EGLBoolean')
eglBindTexImage.add( Input( 'dpy','EGLDisplay' ))
eglBindTexImage.add( Input( 'surface','EGLSurface' ))
eglBindTexImage.add( Input( 'buffer','EGLint' ))
eglBindTexImage.version = ''
eglBindTexImage.category = 'EGL_VERSION_1_1'
eglBindTexImage.trace = True
eglBindTexImage.play = True
egl.add(eglBindTexImage)

eglReleaseTexImage = Function('eglReleaseTexImage')
eglReleaseTexImage.ret = Return('EGLBoolean')
eglReleaseTexImage.add( Input( 'dpy','EGLDisplay' ))
eglReleaseTexImage.add( Input( 'surface','EGLSurface' ))
eglReleaseTexImage.add( Input( 'buffer','EGLint' ))
eglReleaseTexImage.version = ''
eglReleaseTexImage.category = 'EGL_VERSION_1_1'
eglReleaseTexImage.trace = True
eglReleaseTexImage.play = True
egl.add(eglReleaseTexImage)

# EGL_VERSION_1_2

eglBindAPI = Function('eglBindAPI')
eglBindAPI.ret = Return('EGLBoolean')
eglBindAPI.add( Input( 'api','EGLenum' ))
eglBindAPI.version = ''
eglBindAPI.category = 'EGL_VERSION_1_2'
eglBindAPI.trace = True
eglBindAPI.play = True
egl.add(eglBindAPI)

eglCreatePbufferFromClientBuffer = Function('eglCreatePbufferFromClientBuffer')
eglCreatePbufferFromClientBuffer.ret = Return('EGLSurface')
eglCreatePbufferFromClientBuffer.add( Input( 'dpy',' EGLDisplay' ))
eglCreatePbufferFromClientBuffer.add( Input( 'buftype','EGLenum' ))
eglCreatePbufferFromClientBuffer.add( Input( 'buffer','EGLClientBuffer' ))
eglCreatePbufferFromClientBuffer.add( Input( 'config','EGLConfig' ))
eglCreatePbufferFromClientBuffer.add( Input( 'attrib_list','const EGLint *' ))
eglCreatePbufferFromClientBuffer.version = ''
eglCreatePbufferFromClientBuffer.category = 'EGL_VERSION_1_2'
eglCreatePbufferFromClientBuffer.trace = True
eglCreatePbufferFromClientBuffer.play = True
egl.add(eglCreatePbufferFromClientBuffer)

eglQueryAPI = Function('eglQueryAPI')
eglQueryAPI.ret = Return('EGLenum')
eglQueryAPI.version = ''
eglQueryAPI.category = 'EGL_VERSION_1_2'
eglQueryAPI.trace = True
eglQueryAPI.play = True
egl.add(eglQueryAPI)

eglReleaseThread = Function('eglReleaseThread')
eglReleaseThread.ret = Return('EGLBoolean')
eglReleaseThread.version = ''
eglReleaseThread.category = 'EGL_VERSION_1_2'
eglReleaseThread.trace = True
eglReleaseThread.play = True
egl.add(eglReleaseThread)

eglSurfaceAttrib = Function('eglSurfaceAttrib')
eglSurfaceAttrib.ret = Return('EGLBoolean')
eglSurfaceAttrib.add( Input( 'dpy','EGLDisplay' ))
eglSurfaceAttrib.add( Input( 'surface','EGLSurface' ))
eglSurfaceAttrib.add( Input( 'attribute','EGLint' ))
eglSurfaceAttrib.add( Input( 'value','EGLint' ))
eglSurfaceAttrib.version = ''
eglSurfaceAttrib.category = 'EGL_VERSION_1_2'
eglSurfaceAttrib.trace = True
eglSurfaceAttrib.play = True
egl.add(eglSurfaceAttrib)

eglSwapInterval = Function('eglSwapInterval')
eglSwapInterval.ret = Return('EGLBoolean')
eglSwapInterval.add( Input( 'dpy','EGLDisplay' ))
eglSwapInterval.add( Input( 'interval','EGLint' ))
eglSwapInterval.version = ''
eglSwapInterval.category = 'EGL_VERSION_1_2'
eglSwapInterval.trace = True
eglSwapInterval.play = True
egl.add(eglSwapInterval)

eglWaitClient = Function('eglWaitClient')
eglWaitClient.ret = Return('EGLBoolean')
eglWaitClient.version = ''
eglWaitClient.category = 'EGL_VERSION_1_2'
eglWaitClient.trace = True
eglWaitClient.play = True
egl.add(eglWaitClient)

