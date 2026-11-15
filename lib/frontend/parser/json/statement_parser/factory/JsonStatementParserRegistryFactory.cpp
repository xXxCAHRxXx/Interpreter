#include "JsonStatementParserRegistryFactory.hpp"

#include <memory>

#include "frontend/parser/json/statement_parser/AssignParser.hpp"
#include "frontend/parser/json/statement_parser/DoWhileParser.hpp"
#include "frontend/parser/json/statement_parser/IfParser.hpp"
#include "frontend/parser/json/statement_parser/ReadParser.hpp"
#include "frontend/parser/json/statement_parser/SeqParser.hpp"
#include "frontend/parser/json/statement_parser/SkipParser.hpp"
#include "frontend/parser/json/statement_parser/WhileParser.hpp"
#include "frontend/parser/json/statement_parser/WriteParser.hpp"

JsonStatementParserRegistry JsonStatementParserRegistryFactory::Create() const {
    JsonStatementParserRegistry::Builder builder;

    builder.Add("skip", std::make_shared<SkipParser>())
        .Add("assn", std::make_shared<AssignParser>())
        .Add("read", std::make_shared<ReadParser>())
        .Add("write", std::make_shared<WriteParser>())
        .Add("if", std::make_shared<IfParser>())
        .Add("while", std::make_shared<WhileParser>())
        .Add("do", std::make_shared<DoWhileParser>())
        .Add("seq", std::make_shared<SeqParser>());

    return builder.Build();
}
