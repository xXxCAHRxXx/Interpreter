#include "JsonExpressionParserRegistry.hpp"

#include <utility>

#include "error/LogicError.hpp"
#include "error/SyntaxError.hpp"

JsonExpressionParserRegistry::Builder& JsonExpressionParserRegistry::Builder::Add(std::string_view key,
                                                                                  IJsonExpressionParserPtr parser) {
    bool inserted = parsers_.emplace(std::string(key), std::move(parser)).second;
    
    if (!inserted) {
        throw LogicError("JsonExpressionParserRegistry::Builder::Add: duplicate key " + std::string(key));
    }

    return *this;
}

JsonExpressionParserRegistry JsonExpressionParserRegistry::Builder::Build() {
    return JsonExpressionParserRegistry(std::move(parsers_));
}

IExprPtr JsonExpressionParserRegistry::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    if (node.is_object()) {
        for (auto item = node.begin(); item != node.end(); ++item) {
            auto entry = parsers_.find(item.key());
            if (entry != parsers_.end()) {
                return entry->second->Parse(node, parser);
            }
        }
    }

    throw SyntaxError("unknown expression node: " + node.dump());
}

JsonExpressionParserRegistry::JsonExpressionParserRegistry(
std::unordered_map<std::string, IJsonExpressionParserPtr> parsers)
: parsers_(std::move(parsers))
{ }