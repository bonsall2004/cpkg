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
#include "repositories/repositories.h"
#include "terminal_character_help.h"
#include <cache.h>

// all commands and flags for commands are registered via static initialisers
int main(int argc, char** argv)
{
  add_global_flags(app);

  bool version_flag = false;
  app.add_flag("--version", version_flag, "Show Version Information");

  bool update_flag = false;
  app.add_flag("--check-for-updates", update_flag, "Check for updates");

  std::vector<std::string> new_repositories;
  app.add_option("--append-repositories", new_repositories, "Append new repositories to the end of your repository list");

  std::vector<std::string> repositories_to_remove;
  app.add_option("--remove-repositories", repositories_to_remove, "Removes a list of repositories");

  bool list_repositories = false;
  app.add_flag("--list-repositories", list_repositories, "List all the current saved search repositories");

  bool show_cache = false;
  app.add_flag("--show-cache", show_cache, "Show all the cached Repositories");

  CLI11_PARSE(app, argc, argv);

  if(argc == 1)
  {
    std::cout << app.help() << "\n";
    return 0;
  }

  if(update_flag)
  {
    cpkg::info::set_last_update_time();
    return 0;
  }

  if(version_flag)
  {
    cpkg::utils::print_version_box();
    return 0;
  };

  if(!repositories_to_remove.empty())
  {
    std::vector<std::string> names_to_remove;
    std::vector<int> indices_to_remove;

    for(const auto& item : repositories_to_remove)
    {
      int index;
      if(string_to_int(item, index))
      {
        indices_to_remove.push_back(index);
      }
      else
      {
        names_to_remove.push_back(item);
      }
    }

    if(!indices_to_remove.empty())
    {
      for(int index : indices_to_remove)
      {
        if(cpkg::repos::remove_repository_from_list(index))
        {
        }
        else
        {
          std::cout << "Couldn't remove repository at index " << index << std::endl;
        }
      }
    }

    if(!names_to_remove.empty())
    {
      if(cpkg::repos::remove_repositories_from_list(names_to_remove))
      {
      }
      else
      {
        std::cerr << "Something went wrong" << std::endl;
      }
    }
    std::cout << "Removed repositories!" << std::endl;
  }

  if(!new_repositories.empty())
  {
    if(!cpkg::repos::append_to_repository_list(new_repositories)) std::cerr << "Something went wrong trying to append repositories\n";
  }

  if(list_repositories)
  {
    std::vector<std::string> repositories;
    cpkg::repos::get_repository_list(repositories);
    if(repositories.empty())
    {
      std::cout << "There are no saved repositories...\n";
      return 0;
    }

    int i = 0;
    for(const auto& str : repositories)
    {
      std::cout << "[" << i++ << "] " << str << "\n";
    }

    return 0;

  }

  if(show_cache)
  {
    std::unordered_map<std::string, std::string> cache;
    if(!cpkg::info::get_cached_packages(cache))
    {
      std::cerr << FG_RED << "Something went wrong whilst getting the cache\n" << RESET;
      return 0;
    }

    for(const auto& pair : cache)
    {
      std::cout << FG_GREEN << pair.first << ": " << FG_YELLOW << pair.second << '\n' << RESET;
    }
  }

  return 0;
}