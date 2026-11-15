#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "binary_operator/registry/factory/BinaryOperatorRegistryFactory.hpp"
#include "frontend/ast/expression/BinOp.hpp"
#include "frontend/ast/expression/Const.hpp"
#include "frontend/ast/expression/Var.hpp"
#include "frontend/ast/statement/Block.hpp"
#include "frontend/parser/json/JsonParser.hpp"
#include "frontend/parser/json/expression_parser/factory/JsonExpressionParserRegistryFactory.hpp"
#include "frontend/parser/json/statement_parser/factory/JsonStatementParserRegistryFactory.hpp"

inline std::shared_ptr<JsonParser> CreateTestParser() {
    JsonExpressionParserRegistryFactory expressions(std::make_shared<BinaryOperatorRegistryFactory>());

    return std::make_shared<JsonParser>(JsonStatementParserRegistryFactory().Create(), expressions.Create());
}

inline IExprPtr Constant(std::int32_t value) {
    return std::make_shared<Const>(Value(value));
}

inline IExprPtr Variable(std::string name) {
    return std::make_shared<Var>(std::move(name));
}

template <class Operator>
IExprPtr Binary(IExprPtr lhs, IExprPtr rhs) {
    return std::make_shared<BinOp>(std::make_shared<Operator>(), std::move(lhs), std::move(rhs));
}

inline IStmtPtr Sequence(std::vector<IStmtPtr> statements) {
    return std::make_shared<Block>(std::move(statements));
}
