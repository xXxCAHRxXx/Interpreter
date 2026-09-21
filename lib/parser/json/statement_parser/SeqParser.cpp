#include "SeqParser.hpp"

#include "ast/statement/Block.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/JsonUtils.hpp"

IStmtPtr SeqParser::Parse(const nlohmann::json& node, const JsonParser& parser) const {
    std::vector<IStmtPtr> statements;
    collect(node, parser, statements);

    return std::make_shared<Block>(std::move(statements));
}

void SeqParser::collect(const nlohmann::json& node,
                        const JsonParser& parser,
                        std::vector<IStmtPtr>& statements) {
    if (node.is_object() && node.contains("seq")) {
        const nlohmann::json& sequence = node.at("seq");
        collect(GetField(sequence, "left"), parser, statements);
        collect(GetField(sequence, "right"), parser, statements);
        return;
    }

    statements.push_back(parser.ParseStatement(node));
}