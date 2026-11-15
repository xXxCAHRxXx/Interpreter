FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_executable(interpreter_tests
    value/ValueTests.cpp

    binary_operator/BinaryOperatorTests.cpp
    binary_operator/BinaryOperatorRegistryTests.cpp

    frontend/parser/JsonParserTests.cpp
    frontend/parser/JsonParserRegistryTests.cpp
    frontend/parser/JsonUtilsTests.cpp

    backend/runtime/EnvironmentTests.cpp
    backend/runtime/StreamInputTests.cpp
    backend/runtime/StreamOutputTests.cpp

    backend/tree/ExpressionEvaluatorTests.cpp
    backend/tree/StatementExecutorTests.cpp

    backend/bytecode/ProgramTests.cpp
    backend/bytecode/ExecutionContextTests.cpp
    backend/bytecode/InstructionTests.cpp
    backend/bytecode/MachineTests.cpp
    backend/bytecode/CompilerTests.cpp

    interpreter/InterpreterTests.cpp
)

target_include_directories(interpreter_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(interpreter_tests PRIVATE
    GTest::gtest_main
    lib
)

include(GoogleTest)
gtest_discover_tests(interpreter_tests)
