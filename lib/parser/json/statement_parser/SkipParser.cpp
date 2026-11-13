#include "SkipParser.hpp"

#include "ast/statement/Skip.hpp"

IStmtPtr SkipParser::Parse(const JsonParser&, const nlohmann::json&) const {
    return std::make_shared<Skip>();
}