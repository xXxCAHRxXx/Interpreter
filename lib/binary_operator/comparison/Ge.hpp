#pragma once

#include "Gt.hpp"
#include "combinator/Swapped.hpp"
#include "logical/Negated.hpp"

using Ge = Negated<Swapped<Gt>>;