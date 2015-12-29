cmake_minimum_required(VERSION 2.8.12.2)

SET(LLVM_ENABLE_RTTI ON CACHE BOOL "Enable run time type information")
SET(LLVM_ENABLE_EH ON CACHE BOOL "Enable Exception handling")
#option(LLVM_ENABLE_RTTI OFF CACHE BOOL "Enable run time type information")
#set(LLVM_REQUIRES_EH 1)
#set(LLVM_REQUIRES_RTTI ON)
#message(FATAL_ERROR "test######################################################")

add_subdirectory(llvm)

# setup conan (dependency manager) for boost_interprocess
INCLUDE(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

add_subdirectory(source)
