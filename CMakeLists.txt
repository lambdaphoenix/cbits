cmake_minimum_required(VERSION 3.15)
project(cbits LANGUAGES C)

set(MODULE_NAME "_cbits")

find_package(Python
	COMPONENTS Interpreter Development.Module REQUIRED
)

add_library(${MODULE_NAME} MODULE
	src/bitvector.c
	src/compat_dispatch.c
	python/binding.c
)

target_include_directories(${MODULE_NAME}
	PRIVATE
		${CMAKE_SOURCE_DIR}/include
		${Python_INCLUDE_DIRS}
)

if (MSVC)
	target_compile_options(${MODULE_NAME} PRIVATE /O2 /arch:AVX2 /arch:AVX512)
	target_compile_definitions(${MODULE_NAME} PRIVATE 
		__AVX2__=1
		__AVX512VPOPCNTDQ__=1
	)
	  set_target_properties(${MODULE_NAME} PROPERTIES SUFFIX ".pyd")
else()
	target_compile_options(${MODULE_NAME} PRIVATE
		-O3
		-funroll-loops
		-mavx2
		-mavx512vpopcntdq
	)
	set_target_properties(${MODULE_NAME} PROPERTIES SUFFIX ".so")
endif()


target_link_libraries(${MODULE_NAME}
	PRIVATE ${Python_LIBRARY}
)

install(TARGETS ${MODULE_NAME}
	RUNTIME DESTINATION cbits
	LIBRARY DESTINATION cbits
	ARCHIVE DESTINATION cbits
)

install(FILES ${CMAKE_SOURCE_DIR}/python/cbits/__init__.py
	DESTINATION cbits
)