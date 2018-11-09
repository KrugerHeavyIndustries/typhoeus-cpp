AM_CPPFLAGS = -std=c++11 -I${top_srcdir}/include
pkginclude_HEADERS= ${top_srcdir}/include/typhoeus/typhoeus.h

lib_LTLIBRARIES = libtyphoeus.la 
libtyphoeus_la_SOURCES = typhoeus.cpp connection.cpp curl.cpp pool.cpp hydra.cpp factory.cpp
libtyphoeus_la_CPPFLAGS = -I${top_srcdir}/include -fPIC -std=c++11
libtyphoeus_la_LDFLAGS = -version-info 1:0:0

