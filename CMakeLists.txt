project(smecc)

cmake_minimum_required(VERSION 2.8.12)

#
# Defaults
#
set(SMECC_USE_LOOKUPS TRUE CACHE BOOL "Use Lookup Table")
set(SMECC_WARNFREE FALSE CACHE BOOL "Enable Error on Compile Warnings")
set(SMECC_COMPILE_EXAMPLES FALSE CACHE BOOL "Compile Example Code")
set(SMECC_COMPILE_TESTS TRUE CACHE BOOL "Compile Unit Tests")


#
# Source Files
#
set(SMECC_HEADER_FILES 
	src/smecc.h)

set(SMECC_SOURCE_FILES
	src/smecc_bitcount.c
	src/smecc_check.c
	src/smecc_check_correct.c
	src/smecc_check_correct_multi.c
	src/smecc_check_multi.c
	src/smecc_compute.c
	src/smecc_compute_multi.c
	src/smecc_deserialize.c
	src/smecc_serialize.c)


#
# [lib]smecc
#
add_library(
	smecc
		${SMECC_SOURCE_FILES}
		${SMECC_HEADER_FILES})

if(SMECC_USE_LOOKUPS)
	target_compile_definitions(
		smecc
			PRIVATE "-DSMECC_USE_LOOKUPS")
endif()


#
# Platform Specific
#
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_definitions("-D_GNU_SOURCE")
endif()

if(MSVC)
	set(TEST_COMPILE_FLAGS "/wd4127")

	set(RA_COMPILE_FLAGS "/W4 /wd4054 /wd4100")

	if(SMECC_WARNFREE)
		set(RA_COMPILE_FLAGS "${EXTRA_COMPILE_FLAGS} /WX")
	endif()

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RA_COMPILE_FLAGS}")

	# Get rid of annoying D9025 warning for /W3 /W4
	string(REPLACE "/W3 " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})

	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
else(MSVC)
	set(TEST_COMPILE_FLAGS "")

	set(RA_COMPILE_FLAGS "-Wall -pedantic")

	if(SMECC_WARNFREE)
		set(RA_COMPILE_FLAGS "${EXTRA_COMPILE_FLAGS} -Werror")
	endif()

	if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes -Wstrict-prototypes")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RA_COMPILE_FLAGS}")
	endif()
endif(MSVC)


#
# Shared Linking
#
if(BUILD_SHARED_LIBS)
	add_definitions("-DSMECC_LINK_SHARED")
endif()


#
# Special static/shared naming
#
if(NOT DEFINED LIBNAME_POSTFIX)
	if(BUILD_SHARED_LIBS)
		if(DEFINED LIBNAME_POSTFIX_SHARED)
			set(LIBNAME_POSTFIX ${LIBNAME_POSTFIX_SHARED})
		endif()
	else()
		if(DEFINED LIBNAME_POSTFIX_STATIC)
			set(LIBNAME_POSTFIX ${LIBNAME_POSTFIX_STATIC})
		endif()
	endif()
endif(NOT DEFINED LIBNAME_POSTFIX)

if(DEFINED LIBNAME_POSTFIX)
	set_target_properties(
		smecc PROPERTIES
			OUTPUT_NAME "smecc${LIBNAME_POSTFIX}")
endif(DEFINED LIBNAME_POSTFIX)


#
# Install
#
install(
	TARGETS
		smecc
	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(
	FILES
		${SMECC_HEADER_FILES}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include)


#
# Sample Code
#
if(SMECC_COMPILE_EXAMPLES)
	#
	# ECC Reader
	#
	add_executable(
		example_eccread
			examples/ecctools/eccread.c
			examples/ecctools/eccfile.h
			${SMECC_HEADER_FILES})

	target_include_directories(
		example_eccread
			PRIVATE src)

	target_link_libraries(
		example_eccread
			smecc)

	#
	# ECC Writer
	#
	add_executable(
		example_eccwrite
			examples/ecctools/eccwrite.c
			examples/ecctools/eccfile.h
			${SMECC_HEADER_FILES})

	target_include_directories(
		example_eccwrite
			PRIVATE src)

	target_link_libraries(
		example_eccwrite
			smecc)

	#
	# ECC Verifier
	#
	add_executable(
		example_eccverify
			examples/ecctools/eccverify.c
			examples/ecctools/eccfile.h
			${SMECC_HEADER_FILES})

	target_include_directories(
		example_eccverify
			PRIVATE src)

	target_link_libraries(
		example_eccverify
			smecc)
endif(SMECC_COMPILE_EXAMPLES)


#
# Unit Tests
#
if(SMECC_COMPILE_TESTS)
	#
	# Compute ECC
	#
	add_executable(
		test_compute
			tests/compute/test_compute.c
			${SMECC_HEADER_FILES})

	target_include_directories(
		test_compute
			PRIVATE src)

	target_compile_options(
		test_compute
			PRIVATE ${TEST_COMPILE_FLAGS})

	target_link_libraries(
		test_compute
			smecc)

	#
	# Correctable Error
	#
	add_executable(
		test_correctable
			tests/correctable/test_correctable.c
			${SMECC_HEADER_FILES})

	target_include_directories(
		test_correctable
			PRIVATE src)

	target_compile_options(
		test_correctable
			PRIVATE ${TEST_COMPILE_FLAGS})

	target_link_libraries(
		test_correctable
			smecc)

	#
	# Multiple Compute ECC sizes
	#
	add_executable(
		test_multi_compute_size
			tests/multi_compute_size/test_multi_compute_size.c
			${SMECC_HEADER_FILES})

	target_include_directories(
		test_multi_compute_size
			PRIVATE src)

	target_compile_options(
		test_multi_compute_size
			PRIVATE ${TEST_COMPILE_FLAGS})

	target_link_libraries(
		test_multi_compute_size
			smecc)
endif(SMECC_COMPILE_TESTS)
