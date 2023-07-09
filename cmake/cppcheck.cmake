## > cppcheck ##################################################################
set(CPPCHECK_TO_APPLY     "apps/*.cpp"  
                          "test/*.cpp" 
                          "test/*.h" 
                          "test/*.hpp") 

FIND_PROGRAM(CPPCHECK_EXECUTABLE cppcheck)

SET(PROJECT_TRDPARTY_DIR third-party)

add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE} --enable=all  --suppressions-list=${CMAKE_CURRENT_SOURCE_DIR}"/cppcheck-suppress" --error-exitcode=1 --quiet --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)" --language=c++  --std=c++20  -I ${CMAKE_CURRENT_SOURCE_DIR}/include/ -I ${CMAKE_CURRENT_BINARY_DIR}/generated/  ${CMAKE_CURRENT_SOURCE_DIR}/src/* ${CMAKE_CURRENT_SOURCE_DIR}/apps ${CMAKE_CURRENT_SOURCE_DIR}/test/unit/*
)
## < cppcheck
