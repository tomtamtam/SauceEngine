#pragma once

#include <print>

#include "utils_hpp/escapes.hpp"

namespace Sauce
{
	#define LOG_INFO(msg) std::println("{} {}", Escapes::ColoredText("[INFO]", Escapes::Color::YELLOW), msg)
	#define LOG_ERROR(msg) std::println("{} {}", Escapes::ColoredText("[ERROR]", Escapes::Color::RED), msg)

	#define LOG_DEBUG(msg) std::println("{} {}", Escapes::ColoredText("[DEBUG]", Escapes::Color::CYAN), msg)
}
