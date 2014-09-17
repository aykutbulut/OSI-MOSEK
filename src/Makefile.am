lib_LTLIBRARIES = libOsimosek.la

libOsimosek_la_SOURCES=\
	OsiMosekSolverInterface.hpp\
	OsiMosekSolverInterface.cpp

libOsimosek_la_LIBADD=\
	$(OSICONIC_LIBS)\
	$(OSIMOSEK_LIBS)\
	$(OSI_LIBS)\
	$(COINUTILS_LIBS)\
	$(LIBS)

AM_CPPFLAGS=\
	-I../../include\
	$(OSICONIC_CFLAGS)\
	$(OSIMOSEK_CFLAGS)\
	$(OSI_CFLAGS)\
	$(COINUTILS_CFLAGS)

# header files to be installed to $(includedir)
include_HEADERS=\
	OsiMosekSolverInterface.hpp

# this are in the disctribution but not in include
#noinst_HEADERS=
