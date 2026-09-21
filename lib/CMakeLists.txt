add_subdirectory(error)
add_subdirectory(value)
add_subdirectory(binary_operator)
add_subdirectory(ast)
add_subdirectory(parser)
add_subdirectory(runtime)

add_library(lib INTERFACE)
target_link_libraries(lib INTERFACE
    error
    value
    runtime
    binary_operator
    ast
    parser
)