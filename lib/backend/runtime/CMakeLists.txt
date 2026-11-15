add_library(runtime
    environment/Environment.cpp

    io/StreamInput.cpp
    io/StreamOutput.cpp
)

target_include_directories(runtime PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(runtime PUBLIC value error Microsoft.GSL::GSL)
