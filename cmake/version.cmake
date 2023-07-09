###############################################################################
# Get info git and genereted version.h from version.h.in./ 
###############################################################################
if (NOT EXISTS "${CMAKE_BINARY_DIR}/generated/version.h.in")
    file(WRITE "${CMAKE_BINARY_DIR}/generated/version.h.in" "#define GIT_BRANCH \"\@GIT_BRANCH\@\"
#define GIT_COMMIT_HASH \"\@GIT_COMMIT_HASH\@\"
#define CMAKE_BUILD_TYPE \"\@CMAKE_BUILD_TYPE\@\"
#define VERSION_PROJECT \"\@VERSION_PROJECT\@\"
#include <iostream>
#include <string>

void version(const std::string& name_software);
void version(const std::string& name_software) {
    std::cout << \"******************************************\" << std::endl;
    std::cout << \"[+] *** \" << name_software << \" ***\" << std::endl;
    std::cout << \"[+] Day compilation : \" << __DATE__ << \" \" << __TIME__ << std::endl;
    std::cout << \"[+] Version (\" << CMAKE_BUILD_TYPE << \") : \" << VERSION_PROJECT << std::endl;
    std::cout << \"[+] Git branch : \" << GIT_BRANCH << std::endl;
    std::cout << \"[+] Git commit hash : \" << GIT_COMMIT_HASH << std::endl;
    std::cout << \"******************************************\" << std::endl;
}")
endif(NOT EXISTS "${CMAKE_BINARY_DIR}/generated/version.h.in")

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

  if (DEFINED RELEASE_VERSION)
    set(VERSION_PROJECT "${RELEASE_VERSION}")
  else()
    execute_process(
      COMMAND bash -c "git tag -l *.*.* | sort -V | tail -1"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE VERSION_PROJECT
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif()

  
else(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(GIT_BRANCH "")
  set(GIT_COMMIT_HASH "")
endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

configure_file(
  ${CMAKE_BINARY_DIR}/generated/version.h.in
  ${CMAKE_BINARY_DIR}/generated/version.h
)

message(STATUS "VERSION_PROJECT=${VERSION_PROJECT}")

# include_directories(${CMAKE_BINARY_DIR}/generated)
