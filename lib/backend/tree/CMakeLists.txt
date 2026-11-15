add_library(tree
    evaluator/ExpressionEvaluator.cpp

    statement/StatementExecutor.cpp
)

target_include_directories(tree PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(tree PUBLIC ast runtime value Microsoft.GSL::GSL)
