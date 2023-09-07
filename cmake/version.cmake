# ##############################################################################
# Get info git and genereted version.h from version.h.in./
# ##############################################################################
if(NOT EXISTS "${CMAKE_BINARY_DIR}/generated/version.hpp.in")
  file(WRITE "${CMAKE_BINARY_DIR}/generated/version.hpp.in" "#include <iostream>
#include <string>  
#define GIT_BRANCH \"\@GIT_BRANCH\@\"
#define GIT_COMMIT_HASH \"\@GIT_COMMIT_HASH\@\"
#define CMAKE_BUILD_TYPE \"\@CMAKE_BUILD_TYPE\@\"
#define PROJECT_VERSION \"\@PROJECT_VERSION\@\"

void version(const std::string& name_software);

void version(const std::string& name_software) {
    std::cout << \"******************************************\" << std::endl;
    std::cout << \"[+] *** \" << name_software << \" ***\" << std::endl;
    std::cout << \"[+] Day compilation : \" << __DATE__ << \" \" << __TIME__ << std::endl;
    std::cout << \"[+] Version (\" << CMAKE_BUILD_TYPE << \") : \" << PROJECT_VERSION << std::endl;
    std::cout << \"[+] Git branch : \" << GIT_BRANCH << std::endl;
    std::cout << \"[+] Git commit hash : \" << GIT_COMMIT_HASH << std::endl;
    std::cout << \"******************************************\" << std::endl;
}")
endif(NOT EXISTS "${CMAKE_BINARY_DIR}/generated/version.hpp.in")

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(DEFINED RELEASE_VERSION)
    set(PROJECT_VERSION "${RELEASE_VERSION}")
  else()
    execute_process(
      COMMAND bash -c "git for-each-ref --sort=-creatordate --count 1 --format='%(refname:short)' 'refs/tags/*.*.*'"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE PROJECT_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif()

else(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(GIT_BRANCH "")
  set(GIT_COMMIT_HASH "")
endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

configure_file(
  ${CMAKE_BINARY_DIR}/generated/version.hpp.in
  ${CMAKE_BINARY_DIR}/generated/version.hpp
)

if(PROJECT_VERSION STREQUAL "")
  set(PROJECT_VERSION 0.0.0)
endif()

message(STATUS "PROJECT_VERSION=${PROJECT_VERSION}")

# include_directories(${CMAKE_BINARY_DIR}/generated)
