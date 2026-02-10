cmake_minimum_required(VERSION 3.15)
project(cbits LANGUAGES C)

# =============================================================
# Python discovery (with VIRTUAL_ENV support)
# =============================================================
include(CheckCCompilerFlag)

set(MODULE_NAME "_cbits")

if(DEFINED ENV{VIRTUAL_ENV})
	message(STATUS "Using Python from VIRTUAL_ENV=$ENV{VIRTUAL_ENV}")
	set(Python_ROOT_DIR "$ENV{VIRTUAL_ENV}")
	set(Python_FIND_STRATEGY LOCATION)
endif()

if(APPLE)
	set(Python_FIND_FRAMEWORK NEVER)
endif()

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

message(STATUS "Found Python ${Python_VERSION_STRING}")
message(STATUS "  Executable: ${Python_EXECUTABLE}")
message(STATUS "  Include dir: ${Python_INCLUDE_DIRS}")
message(STATUS "  Libraries: ${Python_LIBRARIES}")
message(STATUS "  Module ext: ${Python_MODULE_EXTENSION}")

# =============================================================
# C core library
# =============================================================
add_library(${MODULE_NAME}_core STATIC
	src/cbits/bitvector_core.c
	src/cbits/bitvector_compare.c
	src/cbits/bitvector_range.c
	src/cbits/bitvector_rank.c

	src/compat_dispatch.c
)

target_include_directories(${MODULE_NAME}_core
	PRIVATE
		${CMAKE_SOURCE_DIR}/include
)

set_target_properties(${MODULE_NAME}_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# =============================================================
# Python extension module
# =============================================================
add_library(${MODULE_NAME} MODULE
	src/python/bitvector_iter.c
	src/python/bitvector_methods_basic.c
	src/python/bitvector_methods_copy.c
	src/python/bitvector_methods_ops.c
	src/python/bitvector_methods_slice.c
	src/python/bitvector_object.c
	src/python/bitvector_methods.c
	src/python/bitvector_methods_compare.c
	src/python/bitvector_methods_misc.c
	src/python/bitvector_methods_rank.c
	src/python/cbits_module.c
)

set_target_properties(${MODULE_NAME} PROPERTIES	PREFIX "")

target_include_directories(${MODULE_NAME}
	PRIVATE
		${Python_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/src/python
)

target_link_libraries(${MODULE_NAME}
	PRIVATE
		Python::Module
		${MODULE_NAME}_core
)

# =============================================================
# Compiler optimizations
# =============================================================
if (MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_compile_options(${MODULE_NAME} PRIVATE
			$<$<CONFIG:Release>:/O2>
			$<$<CONFIG:Release>:/arch:AVX2>
		)
		target_compile_definitions(${MODULE_NAME} PRIVATE
			$<$<CONFIG:Release>:__AVX2__=1>
		)
		check_c_compiler_flag("/arch:AVX512VPOPCNTDQ" HAS_AVX512VPOPCNTDQ)
		if(HAS_AVX512VPOPCNTDQ)
			target_compile_options(${MODULE_NAME} PRIVATE
				$<$<CONFIG:Release>:/arch:AVX512VPOPCNTDQ>
			)
			target_compile_definitions(${MODULE_NAME} PRIVATE
				$<$<CONFIG:Release>:__AVX512VPOPCNTDQ__=1>
			)
		endif()
	endif()
	set_target_properties(${MODULE_NAME} PROPERTIES SUFFIX ".pyd")
else()
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$" OR CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
		target_compile_definitions(${MODULE_NAME} PRIVATE USE_X86_INTRINSIC)
		target_compile_options(${MODULE_NAME} PRIVATE
			-O3 -funroll-loops -mavx2 -mavx512vpopcntdq
		)
	else()
		target_compile_options(${MODULE_NAME} PRIVATE -O3 -funroll-loops)
	endif()
    set_target_properties(${MODULE_NAME} PROPERTIES SUFFIX ".so")
endif()

# =============================================================
# Installation
# =============================================================
install(TARGETS ${MODULE_NAME}
	LIBRARY DESTINATION cbits
	RUNTIME DESTINATION cbits
	ARCHIVE DESTINATION cbits
)

install(FILES ${CMAKE_SOURCE_DIR}/python/cbits/__init__.py
	DESTINATION cbits
)

# =============================================================
# CTest integration for C tests
# =============================================================
enable_testing()

set(C_TEST_DIR ${CMAKE_SOURCE_DIR}/tests/c)
file(GLOB C_TEST_SOURCES "${C_TEST_DIR}/*.c")

foreach(TEST_SRC ${C_TEST_SOURCES})
	get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
	
	add_executable(${TEST_NAME} ${TEST_SRC})
	target_link_libraries(${TEST_NAME} PRIVATE ${MODULE_NAME}_core)
	
	target_include_directories(${TEST_NAME}
		PRIVATE
			${CMAKE_SOURCE_DIR}/include
			${CMAKE_SOURCE_DIR}/src/cbits
	)
	
	add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

find_program(VALGRIND_EXECUTABLE valgrind)

if (VALGRIND_EXECUTABLE)
    message(STATUS "Valgrind found: ${VALGRIND_EXECUTABLE}")

    foreach(TEST_SRC ${C_TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

        add_test(
            NAME memcheck_${TEST_NAME}
            COMMAND ${VALGRIND_EXECUTABLE}
                --leak-check=full
                --show-leak-kinds=all
                --errors-for-leak-kinds=all
                --error-exitcode=1
                ./${TEST_NAME}
        )

    endforeach()
else()
    message(WARNING "Valgrind not found; memory tests disabled")
endif()