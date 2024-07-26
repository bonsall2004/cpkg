/*
 * cpkg
 * Author: bonsall2004
 * Description: The Command to update the package registry
 */

#include "CLIHelper.hpp"
#include <CLI/App.hpp>

namespace cpkg::cli::registry::update
{
  bool ignore_cached_repositories = false;
  bool get_all_repositories = false;
  CLI::App* update_registry_setup(CLI::App& app)
  {
    auto subcommand = app.add_subcommand("update", "Update the package registry from sources");

    subcommand->add_option("--ignore-cache", ignore_cached_repositories, "Ignore cached repositories and get them fresh from the web");
    subcommand->add_option("-a, --all", get_all_repositories, "Ignore cached repositories and get them fresh from the web");

    subcommand->callback([&]()
    {
      update_registry_setup(app);
    });

    return subcommand;
  }

  void update_repository_execute(CLI::App& app)
  {

  }
}

REGISTER_SUBCOMMAND_SETUP(cpkg::cli::registry::update::update_registry_setup);