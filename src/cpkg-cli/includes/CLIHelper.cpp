#include "CLIHelper.hpp"

CLI::App app;
bool verbose = false;

void add_global_flags(CLI::App& app_)
{
  app_.add_flag("-v, --verbose", verbose, "Get an extended output including debug information");
}

void add_global_flags(CLI::App* app_)
{
  app_->add_flag("-v, --verbose", verbose, "Get an extended output including debug information");
}

void verbose_printf(const char* format, ...)
{
  if(verbose)
  {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}

bool string_to_int(const std::string& str, int& result)
{
  std::istringstream iss(str);
  iss >> result;
  return !iss.fail() && iss.eof();
}