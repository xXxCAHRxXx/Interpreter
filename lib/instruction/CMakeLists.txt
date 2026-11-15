add_library(instruction
    Program.cpp
    Label.cpp

    ReadInstruction.cpp
    WriteInstruction.cpp
    LoadInstruction.cpp
    StoreInstruction.cpp
    ConstantInstruction.cpp
    BinaryOperationInstruction.cpp

    JumpInstruction.cpp
    JumpZeroInstruction.cpp
    JumpNotZeroInstruction.cpp
)

target_include_directories(instruction PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(instruction PUBLIC binary_operator value error Microsoft.GSL::GSL)
