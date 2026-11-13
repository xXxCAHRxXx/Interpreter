#pragma once

#include "Gt.hpp"
#include "binary_operator/combinator/Swapped.hpp"
#include "binary_operator/logical/Negated.hpp"

using Ge = Negated<Swapped<Gt>>;
