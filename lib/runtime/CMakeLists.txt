add_library(runtime
    interpreter/Interpreter.cpp

    environment/Environment.cpp

    evaluator/ExpressionEvaluator.cpp

    statement/StatementExecutor.cpp

    io/StreamInput.cpp
    io/StreamOutput.cpp
)

target_include_directories(runtime PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(runtime
    PUBLIC ast value error parser Microsoft.GSL::GSL
)