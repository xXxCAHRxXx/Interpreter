add_executable(interpreter main.cpp)

target_link_libraries(interpreter PRIVATE interpreter_lib parser binary_operator runtime error)
