add_subdirectory(error)
add_subdirectory(value)
add_subdirectory(binary_operator)
add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(interpreter)

add_library(lib INTERFACE)
target_link_libraries(lib INTERFACE
    error
    value
    binary_operator
    ast
    parser
    runtime
    tree
    bytecode
    interpreter_lib
)
