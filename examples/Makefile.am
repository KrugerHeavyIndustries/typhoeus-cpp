AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = basic request hydra

AM_CPPFLAGS = -I$(top_srcdir)/include -std=c++11

basic_SOURCES = basic.cpp
basic_LDADD = $(top_builddir)/src/libtyphoeus.la

request_SOURCES = request.cpp
request_LDADD = $(top_builddir)/src/libtyphoeus.la

hydra_SOURCES = hydra.cpp
hydra_LDADD = $(top_builddir)/src/libtyphoeus.la
