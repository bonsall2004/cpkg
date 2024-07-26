#pragma once

#include "CLI/App.hpp"
#include <iostream>
#include <cstdarg>

#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)
#define UNIQUE_NAME(base) CONCATENATE(base, __LINE__)

#define REGISTER_SUBCOMMAND_SETUP(x) inline bool UNIQUE_NAME(REGISTERED_COMMAND_) = register_command(x);

extern CLI::App app;
extern bool verbose;

void add_global_flags(CLI::App& app_);
void add_global_flags(CLI::App* app_);

/**
 * @brief Printf that only runs if the verbose flag has been enabled
 */
void verbose_printf(const char* format, ...);

template <class F>
bool register_command(F f)
{
  CLI::App* app_ = f(app);
  app_->ignore_case();
  add_global_flags(app_);
  return true;
}

/**
 * @param str Input String
 * @param result Output Integer
 * @return True if sucessful
 */
bool string_to_int(const std::string& str, int& result);