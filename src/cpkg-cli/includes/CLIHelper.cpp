#include "CLIHelper.hpp"

CLI::App app;
bool verbose = false;

void add_global_flags(CLI::App& app)
{
  app.add_flag("-v, --verbose", verbose, "Get an extended output including debug information");
}

void add_global_flags(CLI::App* app)
{
  app->add_flag("-v, --verbose", verbose, "Get an extended output including debug information");
}

void verbose_printf(const char* format, ...)
{
  if (verbose)
  {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}
