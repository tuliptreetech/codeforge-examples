cmake_minimum_required(VERSION 3.22)

add_subdirectory(codeforge-qpcpp)

set(QPCPP_SDK_PATH "${QPCPP_TOP_DIR}")

set(CODEFORGE_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/codeforge-qpcpp/codeforge-qpcpp-lib/cmake")

include(${CODEFORGE_CMAKE_DIR}/codeforge_qpcpp.cmake)
create_a_codeforge_fuzzer(if-statements if-statements.cpp)
create_a_codeforge_fuzzer(magic-key magic-key.cpp)
