add_executable(interpreter main.cpp)

target_link_libraries(interpreter PRIVATE runtime parser binary_operator ast value error)