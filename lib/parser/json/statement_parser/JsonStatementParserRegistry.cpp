#include "JsonStatementParserRegistry.hpp"

#include <utility>

#include "error/LogicError.hpp"
#include "error/SyntaxError.hpp"

JsonStatementParserRegistry::Builder& JsonStatementParserRegistry::Builder::Add(std::string_view key,
                                                                                IJsonStatementParserPtr parser) {
    bool inserted = parsers_.emplace(std::string(key), std::move(parser)).second;
    if (!inserted) {
        throw LogicError("JsonStatementParserRegistry::Builder::Add: duplicate key " +
                         std::string(key));
    }

    return *this;
}

JsonStatementParserRegistry JsonStatementParserRegistry::Builder::Build() {
    return JsonStatementParserRegistry(std::move(parsers_));
}

IStmtPtr JsonStatementParserRegistry::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    if (node.is_string()) {
        auto entry = parsers_.find(node.get<std::string>());
        if (entry != parsers_.end()) {
            return entry->second->Parse(node, parser);
        }
    }

    if (node.is_object()) {
        for (auto item = node.begin(); item != node.end(); ++item) {
            auto entry = parsers_.find(item.key());
            if (entry != parsers_.end()) {
                return entry->second->Parse(node, parser);
            }
        }
    }

    throw SyntaxError("unknown statement node: " + node.dump());
}

JsonStatementParserRegistry::JsonStatementParserRegistry(
std::unordered_map<std::string, IJsonStatementParserPtr> parsers)
: parsers_(std::move(parsers))
{ }