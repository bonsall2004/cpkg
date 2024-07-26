/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */

#include <CLI/App.hpp>
#include "CLIHelper.hpp"
#include "terminal_character_help.h"
#include "utils/threading.h"
#include <cache.h>

namespace cpkg::cli::registry::search
{
  std::string package_name;
  int thread_count = 4;
  void search_execute(CLI::App& app)
  {
    std::cout << FG_YELLOW "Searching...\n";
    std::unordered_map<std::string, std::string> packages;
    if(!cpkg::info::get_cached_packages(packages))
    {
      std::cerr << CURSOR_UP(1) << FG_RED << "Something wrong whilst trying to get package cache, try running update\n" << RESET;
      return;
    }
    if(package_name.empty())
    {
      std::cerr << CURSOR_UP(1) << FG_RED << "Package not found\n" << RESET;
      return;
    }
    auto results = find_value_threaded(packages, package_name, std::clamp(thread_count, 1, 128));
    if(!results.has_value())
    {
      std::cerr << CURSOR_UP(1) << FG_RED << "Package not found\n" << RESET;
      return;
    }
    std::cout << CURSOR_UP(1) << ERASE_LINE;

    for(const auto& str : *results)
    {
      std::cout << FG_GREEN << "[" << str.first << "]: " << FG_YELLOW << str.second << '\n' << RESET;
    }
  }

  CLI::App* search_setup(CLI::App& app)
  {
    auto subcommand = app.add_subcommand("search", "Search the cache for information about a package");

    subcommand->add_option("package", package_name, "Specify the package to search for")
      ->required(true);
    subcommand->add_option("-j,--jobs", thread_count, "How many threads should be used to search");
    subcommand->callback([&]()
    {
      search_execute(app);
    });

    return subcommand;
  }
}

REGISTER_SUBCOMMAND_SETUP(cpkg::cli::registry::search::search_setup);