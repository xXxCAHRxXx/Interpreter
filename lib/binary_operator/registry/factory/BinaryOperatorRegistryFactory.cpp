#include "BinaryOperatorRegistryFactory.hpp"

#include "arithmetic/Add.hpp"
#include "arithmetic/Div.hpp"
#include "arithmetic/Mod.hpp"
#include "arithmetic/Mul.hpp"
#include "arithmetic/Sub.hpp"
#include "comparison/Eq.hpp"
#include "comparison/Ge.hpp"
#include "comparison/Gt.hpp"
#include "comparison/Le.hpp"
#include "comparison/Lt.hpp"
#include "comparison/Ne.hpp"
#include "logical/And.hpp"
#include "logical/Or.hpp"

BinaryOperatorRegistry BinaryOperatorRegistryFactory::Create() const {
    BinaryOperatorRegistry::Builder builder;

    builder.Add("+",  std::make_shared<Add>())
           .Add("-",  std::make_shared<Sub>())
           .Add("*",  std::make_shared<Mul>())
           .Add("/",  std::make_shared<Div>())
           .Add("%",  std::make_shared<Mod>())
           .Add("<",  std::make_shared<Lt>())
           .Add("<=", std::make_shared<Le>())
           .Add(">",  std::make_shared<Gt>())
           .Add(">=", std::make_shared<Ge>())
           .Add("==", std::make_shared<Eq>())
           .Add("!=", std::make_shared<Ne>())
           .Add("&&", std::make_shared<And>())
           .Add("!!", std::make_shared<Or>());

    return builder.Build();
}