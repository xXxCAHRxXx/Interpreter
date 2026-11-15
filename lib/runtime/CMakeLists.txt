add_library(runtime
    interpreter/TreeInterpreter.cpp
    interpreter/BytecodeInterpreter.cpp
    interpreter/factory/TreeInterpreterFactory.cpp
    interpreter/factory/BytecodeInterpreterFactory.cpp

    environment/Environment.cpp

    evaluator/ExpressionEvaluator.cpp

    statement/StatementExecutor.cpp

    machine/Machine.cpp
    machine/ExecutionContext.cpp

    io/StreamInput.cpp
    io/StreamOutput.cpp
)

target_include_directories(runtime PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(runtime
    PUBLIC ast value error parser instruction compiler Microsoft.GSL::GSL
)
