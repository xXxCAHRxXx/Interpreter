add_library(value
    Value.cpp
)

target_include_directories(value PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(value PUBLIC error)