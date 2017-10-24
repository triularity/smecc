
A library for performing SmartMedia(tm) compatible ECC.

When compiling code that will link against a shared smecc library, include the compiler flag: -DSMECC_LINK_SHARED


Supported OS's:
	Windows (tested on Windows 7, 64-bit)
		Visual Studio 2015 (MSVC 14) 32-bit/64-bit
		MinGW/gcc 4.8 32-bit
		MinGW/gcc 4.9 64-bit
	FreeBSD (tested on 10.x, 64-bit)
		clang 3.4 64-bit
		gcc 6.4 64-bit
	NetBSD (tested on 6.1.5, 64-bit)
		gcc 4.5 64-bit
	OpenBSD (tested on 5.7, 64-bit)
		gcc 4.2 64-bit
	Linux  (tested on CentOS 7, 64-bit)
		gcc 4.8 64-bit


COMPILING
---------

This project uses Cmake (2.8.12+). Configuration options are:

	SMECC_USE_TABLES	Enable use of lookup tables (default: ON).

	SMECC_WARNFREE		Cause the build to fail on compile warnings.


FILES
-----

docs/			Documentation.
examples/		Example code.
src/			Source code.
tests/			Unit tests.
utils/			Utility scripts.

