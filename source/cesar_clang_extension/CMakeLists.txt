set(LLVM_LINK_COMPONENTS support)
#set(LLVM_USED_LIBS libclang clangTooling clangBasic clangAST)

#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

add_clang_executable(clang-test-tool2
  main.cpp

#  LINK_LIBS
#  libclang
#  clangTooling
#  clangBasic
#  clangASTMatchers
#        ${CONAN_LIBS}
  )
#target_compile_options(clang-test-tool PRIVATE -fexceptions)

target_link_libraries(clang-test-tool2
#  libclang
#  clangTooling
#  clangBasic
#  clangASTMatchers
    libclang
    clangTooling
    clangBasic
    clangASTMatchers
    ${CONAN_LIBS}

)

