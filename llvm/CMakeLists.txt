cmake_minimum_required(VERSION 2.8)
set(LLVM_DIR "${LLVM_ROOT}/share/llvm/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LLVM_DIR})

include_directories("include")

find_package(LLVM)
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_subdirectory(lib/Mementos)
