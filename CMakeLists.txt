CMAKE_MINIMUM_REQUIRED(VERSION 3.13.0)
project (ori-helper LANGUAGES CXX VERSION 0.1)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O0 -pg")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(thirdparty)

set(THIRDPARTY_DIR ${PROJECT_BINARY_DIR}/thirdparty)
set(SDSL_BUILD_DIR ${THIRDPARTY_DIR}/sdsl/src/SDSL-build)
set(SDSL_INCLUDE ${SDSL_BUILD_DIR}/include)
set(SDSL_LIB_DIR ${SDSL_BUILD_DIR}/lib)
set(BCLI_INCLUDE ${PROJECT_SOURCE_DIR}/thirdparty/bcli/include)

include_directories(${SDSL_INCLUDE} ${BCLI_INCLUDE})
link_directories(${SDSL_LIB_DIR})
link_libraries(sdsl)

add_executable(ori-helper src/main.cpp)
add_dependencies(ori-helper SDSL)