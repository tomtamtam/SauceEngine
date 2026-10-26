#pragma once

#include <print>

#include "Engine/vendor/utils_hpp/escapes.hpp"

namespace Sauce
{
	#define LOG_INFO(msg) std::println("{} {}", Escapes::ColoredText("[INFO]", Escapes::Color::YELLOW), msg)
	#define LOG_RROR std::println("{} {}", Escapes::ColoredText("[ERROR]", Escapes::Color::RED), msg)
}
