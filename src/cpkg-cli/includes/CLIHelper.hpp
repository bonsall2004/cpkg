#pragma once

#include "CLI/App.hpp"
#include <iostream>
#include <cstdarg>

// Macros for unique naming
#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)
#define UNIQUE_NAME(base) CONCATENATE(base, __LINE__)

// Function to register a subcommand setup
#define REGISTER_SUBCOMMAND_SETUP(x) inline bool UNIQUE_NAME(REGISTERED_COMMAND_) = register_command(x);

// Global variables
extern CLI::App app;
extern bool verbose;

// Function declarations
void add_global_flags(CLI::App& app_);
void add_global_flags(CLI::App* app_);
void verbose_printf(const char* format, ...);

template<class F>
bool register_command(F f)
{
  CLI::App* app_ = f(app);
  app_->ignore_case();
  add_global_flags(app_);
  return true;
}

bool string_to_int(const std::string& str, int& result);