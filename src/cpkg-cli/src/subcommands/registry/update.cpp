/*
 * cpkg
 * Author: bonsall2004
 * Description: The Command to update the package registry
 */

#include "CLIHelper.hpp"
#include "terminal_character_help.h"
#include "definitions.h"
#include "repositories/repositories.h"
#include <CLI/App.hpp>
#include <thread>
#include <string>
#include <program_info/ProgramInfo.h>
#include <cache.h>
#include <regex>

namespace cpkg::cli::registry::update
{
  int thread_count = 1;

  bool remove_duplicates_from_file(const std::string& file_path)
  {
    std::ifstream inputFile(file_path);
    if(!inputFile.is_open())
    {
      return false;
    }

    std::set<std::string> uniqueLines;
    std::string line;

    while(std::getline(inputFile, line))
    {
      uniqueLines.insert(line);
    }

    inputFile.close();

    std::ofstream outputFile(file_path, std::ios::trunc);
    if(!outputFile.is_open())
    {
      return false;
    }

    for(const auto& uniqueLine : uniqueLines)
    {
      outputFile << uniqueLine << std::endl;
    }

    outputFile.close();
    return true;
  }

  void update_repository_execute(CLI::App& app)
  {
    using namespace cpkg::info;

    std::vector<std::string> repositories;

    if(!cpkg::repos::get_repository_list(repositories))
    {
      std::cerr << "No repository lists found, try adding some with the --append-repositories repo1 repo2 repo3\n";
      return;
    }

    if(thread_count <= 0) thread_count = 1;
    size_t repository_list_size = repositories.size();
    std::vector<std::thread> threads;

    std::remove(cpkg::info::wide_to_narrow(get_package_cache_path()).c_str());

    size_t chunk_size = (repository_list_size + thread_count - 1) / thread_count;
    for(size_t thread_index = 0; thread_index < thread_count; thread_index++)
    {
      size_t start_index = thread_index * chunk_size;
      size_t end_index = std::min(start_index + chunk_size, repository_list_size);

      threads.emplace_back([&, start_index, end_index]()
      {
        for(size_t i = start_index; i < end_index; i++)
        {
          if(std::regex_match(repositories[i], repo_list_regex))
          {
            if(!cpkg::info::download_file(cpkg::info::narrow_to_wide(repositories[i]), get_package_cache_path()))
            {
              std::cerr << FG_RED << "Invalid URL: " << repositories[i] << " | IGNORED\n" << RESET;
            }
            else
            {
              std::cout << FG_GREEN << "Valid URL: " << repositories[i] << " | ADDED\n" << RESET;
            }
            continue;
          }
          std::cout << FG_RED << "Invalid URL: " << repositories[i] << " | IGNORED\n" << RESET;
        }
      });
    }

    for(std::thread& thread : threads) if(thread.joinable()) thread.join();

    std::cout << FG_YELLOW << "Removing duplicates...\n";

    if(remove_duplicates_from_file(cpkg::info::wide_to_narrow(get_package_cache_path())))
    {
      std::cout << CURSOR_UP(1) << FG_GREEN << "Duplicates Removed!       \n";
    }
    else
    {
      std::cout << CURSOR_UP(1) << FG_RED << "Failed to remove duplicates!\n";
    }
  }

  CLI::App* update_registry_setup(CLI::App& app)
  {
    auto subcommand = app.add_subcommand("update", "Update the repository list cache from sources");
    subcommand->add_option("-j,--jobs", thread_count, "How many concurrent jobs should run")
      ->expected(1, 128);

    subcommand->callback([&]()
    {
      update_repository_execute(app);
    });

    return subcommand;
  }
}

REGISTER_SUBCOMMAND_SETUP(cpkg::cli::registry::update::update_registry_setup);