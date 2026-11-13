#include "BinaryOperatorRegistryFactory.hpp"

#include <memory>

#include "binary_operator/arithmetic/Add.hpp"
#include "binary_operator/arithmetic/Div.hpp"
#include "binary_operator/arithmetic/Mod.hpp"
#include "binary_operator/arithmetic/Mul.hpp"
#include "binary_operator/arithmetic/Sub.hpp"
#include "binary_operator/comparison/Eq.hpp"
#include "binary_operator/comparison/Ge.hpp"
#include "binary_operator/comparison/Gt.hpp"
#include "binary_operator/comparison/Le.hpp"
#include "binary_operator/comparison/Lt.hpp"
#include "binary_operator/comparison/Ne.hpp"
#include "binary_operator/logical/And.hpp"
#include "binary_operator/logical/Or.hpp"

BinaryOperatorRegistry BinaryOperatorRegistryFactory::Create() const {
    BinaryOperatorRegistry::Builder builder;

    builder.Add("+", std::make_shared<Add>())
        .Add("-", std::make_shared<Sub>())
        .Add("*", std::make_shared<Mul>())
        .Add("/", std::make_shared<Div>())
        .Add("%", std::make_shared<Mod>())
        .Add("<", std::make_shared<Lt>())
        .Add("<=", std::make_shared<Le>())
        .Add(">", std::make_shared<Gt>())
        .Add(">=", std::make_shared<Ge>())
        .Add("==", std::make_shared<Eq>())
        .Add("!=", std::make_shared<Ne>())
        .Add("&&", std::make_shared<And>())
        .Add("!!", std::make_shared<Or>());

    return builder.Build();
}
