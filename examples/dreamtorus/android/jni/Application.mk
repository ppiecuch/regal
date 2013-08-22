APP_STL := stlport_static

ifeq ($(NDK_DEBUG),1)
  $(warning NDK_DEBUG set, enabling debug build.)
  APP_OPTIM := debug
endif
