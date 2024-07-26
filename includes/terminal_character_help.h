/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once

// Cursor movement macros
#define CURSOR_UP(n) "\033[" #n "A"
#define CURSOR_DOWN(n) "\033[" #n "B"
#define CURSOR_FORWARD(n) "\033[" #n "C"
#define CURSOR_BACKWARD(n) "\033[" #n "D"
#define CURSOR_NEXT_LINE(n) "\033[" #n "E"
#define CURSOR_PREVIOUS_LINE(n) "\033[" #n "F"
#define CURSOR_HORIZONTAL_ABS(n) "\033[" #n "G"
#define CURSOR_POSITION(row, col) "\033[" #row ";" #col "H"

// Erase functions
#define ERASE_DISPLAY "\033[2J"
#define ERASE_LINE "\033[2K"
#define ERASE_LINE_FROM_CURSOR "\033[K"

// Text styles
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"
#define STRIKETHROUGH "\033[9m"

// Foreground colors
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_BLUE "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"
#define FG_DEFAULT "\033[39m"

// Background colors
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BG_DEFAULT "\033[49m"

// Bright Foreground colors
#define FG_BRIGHT_BLACK "\033[90m"
#define FG_BRIGHT_RED "\033[91m"
#define FG_BRIGHT_GREEN "\033[92m"
#define FG_BRIGHT_YELLOW "\033[93m"
#define FG_BRIGHT_BLUE "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN "\033[96m"
#define FG_BRIGHT_WHITE "\033[97m"

// Bright Background colors
#define BG_BRIGHT_BLACK "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"