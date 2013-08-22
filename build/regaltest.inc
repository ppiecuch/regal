# regaltest.inc
#
# Generic gnumake .inc for building regaltest
#

# Sources

REGALTEST.CXX += tests/test_main.cpp
REGALTEST.CXX += tests/testRegalTexC.cpp
REGALTEST.CXX += tests/testRegalPixelConversions.cpp
REGALTEST.CXX += tests/testStringList.cpp
REGALTEST.CXX += tests/testRegalState.cpp
REGALTEST.CXX += tests/testRegalPpa.cpp
REGALTEST.CXX += tests/testRegalPpca.cpp
REGALTEST.CXX += tests/testRegalHelper.cpp
REGALTEST.CXX += tests/testRegalToken.cpp
REGALTEST.CXX += tests/testRegalJson.cpp
REGALTEST.CXX += tests/RegalDispatchGMock.cpp
