cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD                11)
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_C_COMPILER             "/usr/bin/clang")
set(CMAKE_ASM_COMPILER           "/usr/bin/clang")
set(CMAKE_CXX_COMPILER           "/usr/bin/clang++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=fuzzer,address -fcoverage-mapping -fprofile-instr-generate -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED")

function(create_a_codeforge_fuzzer NAME SOURCES)
        set(EXE_NAME "codeforge-${NAME}-fuzz")
        add_executable(${EXE_NAME} ${SOURCES})

        target_link_libraries(${EXE_NAME} PRIVATE ${CPPUTEST_LDFLAGS})
endfunction()

create_a_codeforge_fuzzer(cpp-if-statements if-statements.cpp)
create_a_codeforge_fuzzer(state-machine state-machine.cpp)
