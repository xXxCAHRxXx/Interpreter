#include "SkipParser.hpp"

#include "ast/statement/Skip.hpp"

IStmtPtr SkipParser::Parse(const nlohmann::json&, const JsonParser&) const {
    return std::make_shared<Skip>();
}