add_library(bytecode
    program/Program.cpp

    instruction/Label.cpp

    instruction/ReadInstruction.cpp
    instruction/WriteInstruction.cpp
    instruction/LoadInstruction.cpp
    instruction/StoreInstruction.cpp
    instruction/ConstantInstruction.cpp
    instruction/BinaryOperationInstruction.cpp

    instruction/JumpInstruction.cpp
    instruction/JumpZeroInstruction.cpp
    instruction/JumpNotZeroInstruction.cpp

    compiler/Compiler.cpp

    machine/Machine.cpp
    machine/ExecutionContext.cpp
)

target_include_directories(bytecode PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(bytecode PUBLIC ast runtime binary_operator value error Microsoft.GSL::GSL)
