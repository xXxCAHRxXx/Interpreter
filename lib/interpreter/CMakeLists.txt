add_library(interpreter_lib
    TreeInterpreter.cpp
    BytecodeInterpreter.cpp

    factory/TreeInterpreterFactory.cpp
    factory/BytecodeInterpreterFactory.cpp
)

target_include_directories(interpreter_lib PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(interpreter_lib PUBLIC parser tree bytecode runtime Microsoft.GSL::GSL)
