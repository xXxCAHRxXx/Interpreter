add_library(parser
    json/JsonParser.cpp
    json/JsonUtils.cpp

    json/statement_parser/JsonStatementParserRegistry.cpp
    json/statement_parser/AssignParser.cpp
    json/statement_parser/DoWhileParser.cpp
    json/statement_parser/IfParser.cpp
    json/statement_parser/ReadParser.cpp
    json/statement_parser/SeqParser.cpp
    json/statement_parser/SkipParser.cpp
    json/statement_parser/WhileParser.cpp
    json/statement_parser/WriteParser.cpp

    json/expression_parser/JsonExpressionParserRegistry.cpp
    json/expression_parser/BinOpParser.cpp
    json/expression_parser/ConstParser.cpp
    json/expression_parser/VarParser.cpp
)

target_include_directories(parser PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(parser
    PUBLIC ast value error binary_operator nlohmann_json::nlohmann_json Microsoft.GSL::GSL
)