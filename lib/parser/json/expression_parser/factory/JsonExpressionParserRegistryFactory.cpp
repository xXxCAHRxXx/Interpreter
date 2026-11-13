#include "JsonExpressionParserRegistryFactory.hpp"

#include <memory>
#include <utility>

#include "parser/json/expression_parser/BinOpParser.hpp"
#include "parser/json/expression_parser/ConstParser.hpp"
#include "parser/json/expression_parser/VarParser.hpp"

JsonExpressionParserRegistryFactory::JsonExpressionParserRegistryFactory(IBinaryOperatorRegistryFactoryPtr operators)
: operators_(std::move(operators))
{ }

JsonExpressionParserRegistry JsonExpressionParserRegistryFactory::Create() const {
    JsonExpressionParserRegistry::Builder builder;

    builder.Add("var",   std::make_shared<VarParser>())
           .Add("const", std::make_shared<ConstParser>())
           .Add("binop", std::make_shared<BinOpParser>(operators_->Create()));

    return builder.Build();
}
