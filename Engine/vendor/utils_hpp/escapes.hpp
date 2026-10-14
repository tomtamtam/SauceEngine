// Copyright (c) 2026 Tom Iwan
// PERMISSION IS HEREBY GARANTED TO ANYONE TO DO ANYTHING WITH THIS SHIT EXEPT FOR THE USE IN PROJECTS ACCUMULATING CAPITAL
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.

#pragma once

#include <cstdint>
#include <cstdio>

#include <format>
#include <string>
#include <unordered_map>

namespace Escapes
{
    //colors
	enum Color {
		BLACK,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE
	};

	inline const std::unordered_map<Color, int> COLOR_MAP = {
		{BLACK,   0},
		{RED,     1},
		{GREEN,   2},
		{YELLOW,  3},
		{BLUE,    4},
		{MAGENTA, 5},
		{CYAN,    6},
		{WHITE,   7}
	};

	const int COLOR_OFFSET = 30;

	const int COLOR_OFFSET_BASE = 40;
	const int UNSATTURATED_OFFSET = 60;

    struct ColorRGB
    {
        uint8_t r, g, b;
    };

    //styles
    enum Style
    {
        BOLD,
        DIM,
        ITALIC,
        UNDERLINE,
        BLINKING,
        INVERSE,
        HIDDEN,
        STRIKETHROUGH
    };

    inline const std::unordered_map<Style, int> STYLE_MAP = {
        {BOLD, 1},
        {DIM, 2},
        {ITALIC, 3},
        {UNDERLINE, 4},
        {BLINKING, 5},
        {INVERSE, 7},
        {HIDDEN, 8},
        {STRIKETHROUGH, 9}
    };

    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    const std::unordered_map<Direction, char> DIRECTION_MAP
    {
        {UP, 'A'},
        {DOWN, 'B'},
        {LEFT, 'D'},
        {RIGHT, 'C'}
    };
    
    //defs
	ColorRGB RGB(uint8_t r, uint8_t g, uint8_t b); //there because if not in namespace, no brace initialization
	std::string ColoredText(const std::string &s, Color c);
	std::string ColoredTextLight(const std::string &s, Color c);
    std::string ColoredTextRGB(const std::string &s, ColorRGB c);
    std::string ColoredBase(const std::string &s, Color c);
    std::string ColoredBaseLight(const std::string &s, Color c);
    std::string ColoredBaseRGB(const std::string &s, ColorRGB c);
    void BeginColor(Color c);
    void EndColor();
    void BeginColorLight(Color c);
    void BeginColorBase(Color c);
    void BeginColorLightBase(Color c);
    void BeginColorRGB(ColorRGB c);
    void BeginColorBaseRGB(ColorRGB c);

    void ResetStyles();
    void SetStyle(Style s);

    void ClearLine();
    void ClearDisplay();
    void ClearDisplayCursorToEnd();
    void ClearDisplayCursorToBegin();

    void BeginPrevLine();
    void CursorPos(uint32_t line, uint32_t column);
    void MoveCursorDir(Direction dir, uint32_t n = 1);

	#ifdef ESCAPES_HPP_IMPLIMENTATION
    ColorRGB RGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return {r, g, b};
    }

	std::string ColoredText(const std::string &s, Color c)
	{
		return std::format("\x1B[{}m{}\033[0m", COLOR_OFFSET + COLOR_MAP.at(c), s);
	}

	std::string ColoredTextLight(const std::string &s, Color c)
	{
		return std::format("\x1B[{}m{}\033[0m", COLOR_OFFSET + UNSATTURATED_OFFSET + COLOR_MAP.at(c), s);
	}

    std::string ColoredTextRGB(const std::string &s, ColorRGB c)
    {
        return std::format("\x1B[38;2;{};{};{}m{}\x1B[0m", c.r, c.g, c.b, s);
    }

    std::string ColoredBase(const std::string &s, Color c)
    {
        return std::format("\x1B[{}m{}\033[0m", COLOR_OFFSET_BASE + COLOR_MAP.at(c), s);
    }

    std::string ColoredBaseLight(const std::string &s, Color c)
    {
		return std::format("\033[3;{};30m{}\033[0m", COLOR_OFFSET_BASE + UNSATTURATED_OFFSET+ COLOR_MAP.at(c), s);
    }

    std::string ColoredBaseRGB(const std::string &s, ColorRGB c)
    {
        return std::format("\x1B[48;2;{};{};{}m{}\x1B[0m", c.r, c.g, c.b, s);
    }

    void BeginColor(Color c)
    {
        std::printf("\x1b[%um", COLOR_OFFSET + COLOR_MAP.at(c));
    }

    void EndColor()
    {
        std::printf("\033[0m");
    }

    void BeginColorLight(Color c)
    {
        std::printf("\x1b[%um", UNSATTURATED_OFFSET + COLOR_MAP.at(c));
    }

    void BeginColorBase(Color c)
    {
        std::printf("\x1b[%um", COLOR_OFFSET_BASE + COLOR_MAP.at(c));
    }

    void BeginColorLightBase(Color c)
    {
        std::printf("\x1b[%um", COLOR_OFFSET + UNSATTURATED_OFFSET + COLOR_MAP.at(c));
    }

    void BeginColorRGB(ColorRGB c)
    {
        std::printf("\x1B[38;2;%u;%u;%um", c.r, c.g, c.b);
    }

    void BeginColorBaseRGB(ColorRGB c)
    {
        std::printf("\x1B[48;2;%u;%u;%um", c.r, c.g, c.b);
    }

	//style
    void ResetStyles()
    {
        std::printf("\x1b[0m");
    }

    void SetStyle(Style s)
    {
        std::printf("\x1b[%um", STYLE_MAP.at(s));
    }

    //erase
    void ClearLine()
    {
        std::printf("\x1b[2K");
    }

    void ClearDisplay()
    {
        std::printf("\x1b[3J");
    }

    void ClearDisplayCursorToEnd()
    {
        std::printf("\x1b[0J");
    }

    void ClearDisplayCursorToBegin()
    {
        std::printf("\x1b[1J");
    }

    //Cursor
    void BeginPrevLine()
    {
        std::printf("\x1b[1F");
    }

    void CursorPos(uint32_t line, uint32_t column)
    {
        std::printf("\x1b[%u;%uH", line, column);
    }

    void MoveCursorDir(Direction dir, uint32_t n)
    {
        std::printf("\x1b[%u%c", n, DIRECTION_MAP.at(dir));
    }

	#endif
}
