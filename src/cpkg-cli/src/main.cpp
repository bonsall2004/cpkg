/*
 * cpkg
 * Author: bonsall2004
 * Description: Main file for the CLI Portion of CPKG
 */
#include <iostream>
#include <CLI/CLI.hpp>
#include <CLIHelper.hpp>
#include "utils/version.h"
#include "program_info/ProgramInfo.h"

// all commands and flags for commands are registered via static initialisers
int main(int argc, char** argv)
{
  add_global_flags(app);

  bool version_flag = false;
  app.add_flag("--version", version_flag, "Show Version Information");

  bool update_flag = false;
  app.add_flag("--update", update_flag, "Check for updates");

  if (argc == 1) {
    std::cout << app.help() << "\n";
    return 0;
  }

  CLI11_PARSE(app, argc, argv);

  if(version_flag) cpkg::utils::print_version_box();

  if(update_flag)
  {
    cpkg::info::set_last_update_time();
    return 0;
  }
  return 0;
}