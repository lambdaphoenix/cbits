cmake_minimum_required(VERSION 3.15)
project(cbits LANGUAGES C)

set(MODULE_NAME "_cbits")

find_package(Python
	COMPONENTS Interpreter Development.Module REQUIRED
)

add_library(${MODULE_NAME} MODULE
	src/bitvector.c
	python/binding.c
)

target_include_directories(${MODULE_NAME}
	PRIVATE
		${CMAKE_SOURCE_DIR}/include
		${Python_INCLUDE_DIRS}
)

if(WIN32)
set_target_properties(_cbits PROPERTIES SUFFIX ".pyd")
else()
set_target_properties(_cbits PROPERTIES SUFFIX ".so")
endif()

if (MSVC)
	target_compile_options(${MODULE_NAME} PRIVATE /O2 /arch:AVX2)
	target_compile_definitions(${MODULE_NAME} PRIVATE __AVX2__)
else()
	target_compile_options(${MODULE_NAME} PRIVATE -O3 -march=native -funroll-loops)
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