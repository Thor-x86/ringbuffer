cmake_minimum_required (VERSION 3.22.0)
include("./version.cmake")
project(
	ringbuffer
	VERSION
		${RINGBUFFER_VERSION_MAJOR}.${RINGBUFFER_VERSION_MINOR}.${RINGBUFFER_VERSION_PATCH}.${RINGBUFFER_VERSION_TWEAK}
	LANGUAGES C CXX
)

# Microchip microcontroller support
if (MICROCHIP_MCU)
	set(
		CMAKE_TOOLCHAIN_FILE
		${PROJECT_SOURCE_DIR}/external/cmake-microchip/toolchain.cmake
	)
endif()

# Compile output directory (./out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")

# Make the compilation more verbose for safety reasons
if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Designated initializers require at least C++20
set(CMAKE_CXX_STANDARD 20)

# Download latest GoogleTest
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG main
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Prevent install GoogleTest on root
option(INSTALL_GTEST "Enable installation of googletest." OFF)

# Install GoogleTest only in project directory
FetchContent_MakeAvailable(googletest)

##### Core Configuration #####

# Take the entire source files in src directory
file(
	GLOB_RECURSE ${PROJECT_NAME}-files-src
	${PROJECT_SOURCE_DIR}/src/*.c
)

# Group those compiled source to prevent redundant
# compilation process
add_library(
	${PROJECT_NAME} OBJECT
	"${${PROJECT_NAME}-files-src}"
)

# Solve problem with shared object
set_property(
	TARGET ${PROJECT_NAME}
	PROPERTY POSITION_INDEPENDENT_CODE ON
)

# Pass version to binary
target_compile_definitions(
	${PROJECT_NAME}
	PUBLIC RINGBUFFER_VERSION
	PUBLIC RINGBUFFER_VERSION_MAJOR=${RINGBUFFER_VERSION_MAJOR}
	PUBLIC RINGBUFFER_VERSION_MINOR=${RINGBUFFER_VERSION_MINOR}
	PUBLIC RINGBUFFER_VERSION_PATCH=${RINGBUFFER_VERSION_PATCH}
	PUBLIC RINGBUFFER_VERSION_TWEAK=${RINGBUFFER_VERSION_TWEAK}
	PRIVATE __RINGBUFFER_INTERNAL # Fixes problem with Windows target
)

enable_testing()
include(GoogleTest)

# Load CMake-compatible dependencies
#add_subdirectory(${PROJECT_SOURCE_DIR}/external/foobar)

# Link dependencies
#target_link_libraries(
#	${PROJECT_NAME}
#	foobar
#)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include

	# Dependency headers
	#${PROJECT_SOURCE_DIR}/external/foobar/include
)

##### Static Library Configuration #####

add_library(
	${PROJECT_NAME}-static STATIC
	$<TARGET_OBJECTS:${PROJECT_NAME}>
)

set_target_properties(
	${PROJECT_NAME}-static
	PROPERTIES
		OUTPUT_NAME "ringbuffer"
)

target_link_libraries(
	${PROJECT_NAME}-static
	#PRIVATE foobar
)

target_include_directories(
	${PROJECT_NAME}-static
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	#${PROJECT_SOURCE_DIR}/external/foobar/include
)

##### Dynamic Library Configuration #####

add_library(
	${PROJECT_NAME}-dynamic SHARED
	$<TARGET_OBJECTS:${PROJECT_NAME}>
)

set_target_properties(
	${PROJECT_NAME}-dynamic
	PROPERTIES
		OUTPUT_NAME "ringbuffer"
)

target_link_libraries(
	${PROJECT_NAME}-dynamic
	#PRIVATE foobar
)

target_include_directories(
	${PROJECT_NAME}-dynamic
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	#${PROJECT_SOURCE_DIR}/external/foobar/include
)

install(
	TARGETS ${PROJECT_NAME}-dynamic
)

install(
	DIRECTORY "include/"
	TYPE INCLUDE
)

##### Unit Test Configuration #####

file(
	GLOB_RECURSE ${PROJECT_NAME}-files-test-unit
	${PROJECT_SOURCE_DIR}/tests/unit/*.cpp
)

add_executable(
	${PROJECT_NAME}-test-unit
	"${${PROJECT_NAME}-files-test-unit}"
)

target_link_libraries(
	${PROJECT_NAME}-test-unit
	${PROJECT_NAME}-static
	GTest::gtest_main
)

target_compile_definitions(
	${PROJECT_NAME}-test-unit PRIVATE
	TEST_UNIT
	PRIVATE __RINGBUFFER_INTERNAL # Fixes problem with Windows target
)

target_include_directories(
	${PROJECT_NAME}-test-unit
	PRIVATE ${PROJECT_SOURCE_DIR}
)

# Designated initializers require at least C++20
target_compile_features(${PROJECT_NAME}-test-unit PRIVATE cxx_std_20)

gtest_discover_tests(${PROJECT_NAME}-test-unit)

##### Integration Test Configuration #####

file(
	GLOB_RECURSE ${PROJECT_NAME}-files-test-integration
	${PROJECT_SOURCE_DIR}/tests/integration/*.c
)

add_executable(
	${PROJECT_NAME}-test-integration
	"${${PROJECT_NAME}-files-test-integration}"
)

target_compile_definitions(
	${PROJECT_NAME}-test-integration PRIVATE
	TEST_INTEGRATION
	PRIVATE __RINGBUFFER_INTERNAL # Fixes problem with Windows target
)

target_link_libraries(
	${PROJECT_NAME}-test-integration
	${PROJECT_NAME}-static
)

target_include_directories(
	${PROJECT_NAME}-test-integration
	PRIVATE ${PROJECT_SOURCE_DIR}
)

# Designated initializers require at least C++20
target_compile_features(${PROJECT_NAME}-test-integration PRIVATE cxx_std_20)

##### Simple Example Configuration #####

add_executable(
	${PROJECT_NAME}-example-simple
	${PROJECT_SOURCE_DIR}/examples/simple/main.c
)

target_link_libraries(
	${PROJECT_NAME}-example-simple
	${PROJECT_NAME}-dynamic
)

target_include_directories(
	${PROJECT_NAME}-example-simple
	PRIVATE ${PROJECT_SOURCE_DIR}
)

# Designated initializers require at least C++20
target_compile_features(${PROJECT_NAME}-example-simple PRIVATE cxx_std_20)

##### Pager Implementation Example Configuration #####

add_executable(
	${PROJECT_NAME}-example-pager
	${PROJECT_SOURCE_DIR}/examples/pager/main.c
)

target_link_libraries(
	${PROJECT_NAME}-example-pager
	${PROJECT_NAME}-dynamic
)

target_include_directories(
	${PROJECT_NAME}-example-pager
	PRIVATE ${PROJECT_SOURCE_DIR}
)

# Designated initializers require at least C++20
target_compile_features(${PROJECT_NAME}-example-pager PRIVATE cxx_std_20)
