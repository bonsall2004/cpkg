/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#include "repositories/repositories.h"
#include "terminal_character_help.h"
#include "definitions.h"
#include <iostream>
#include <cache.h>
#include <fstream>
#include <regex>

bool cpkg::repos::save_repository_list(const std::vector<std::string>& vec)
{
  std::ofstream file(cpkg::info::get_repository_list_path().c_str(), std::ios::binary);
  if(!file.is_open())
  {
    std::cerr << FG_RED << "Failed to open repository list \n" << RESET;
    return false;
  }

  size_t vecSize = vec.size();
  file.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

  for(const auto& str : vec)
  {
    size_t length = str.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(str.c_str(), length);
  }

  if(file.fail())
  {
    std::cerr << "Error: Writing to file failed." << std::endl;
    return false;
  }

  file.close();
  return true;
}

bool cpkg::repos::get_repository_list(std::vector<std::string>& vec)
{
  std::ifstream file(cpkg::info::get_repository_list_path().c_str(), std::ios::binary);
  if(!file)
  {
    return false;
  }

  size_t vecSize;
  file.read(reinterpret_cast<char*>(&vecSize), sizeof(vecSize));
  vec.resize(vecSize);

  for(auto& str : vec)
  {
    size_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    str.resize(length);
    file.read(&str[0], length);
  }

  file.close();
  return true;
}

bool cpkg::repos::append_to_repository_list(const std::vector<std::string>& vec)
{
  std::vector<std::string> current_repository_list;
  get_repository_list(current_repository_list);
  for(const auto& str : vec)
  {
    if(!std::regex_match(str, repo_list_regex))
    {
      std::cerr << FG_RED << "Repository: " << str << " is not valid, not adding it.\n" << RESET;
      continue;
    }
    current_repository_list.push_back(str);
    std::cout << "Repository: " << str << " added to list\n";
  }
  return save_repository_list(current_repository_list);
}

bool cpkg::repos::remove_repositories_from_list(const std::vector<std::string>& repositories_to_remove)
{
  std::vector<std::string> current_repository_list;
  if(!get_repository_list(current_repository_list))
  {
    std::cerr << "Error: Failed to retrieve repository list." << std::endl;
    return false;
  }

  for(const auto& repository : repositories_to_remove)
  {
    auto endPos = std::remove(current_repository_list.begin(), current_repository_list.end(), repository);
    if(endPos == current_repository_list.end())
    {
      std::cerr << "Warning: Repository \"" << repository << "\" not found in the list." << std::endl;
    }
    else
    {
      current_repository_list.erase(endPos, current_repository_list.end());
    }
  }

  if(!save_repository_list(current_repository_list))
  {
    std::cerr << "Error: Failed to save repository list." << std::endl;
    return false;
  }

  return true;
}

bool cpkg::repos::remove_repository_from_list(const std::string& repository)
{
  std::vector<std::string> current_repository_list;
  if(!get_repository_list(current_repository_list))
  {
    std::cerr << "Error: Failed to retrieve repository list." << std::endl;
    return false;
  }

  auto endPos = std::remove(current_repository_list.begin(), current_repository_list.end(), repository);
  if(endPos == current_repository_list.end())
  {
    std::cerr << "Error: Repository not found." << std::endl;
    return false;
  }
  current_repository_list.erase(endPos, current_repository_list.end());

  if(!save_repository_list(current_repository_list))
  {
    std::cerr << "Error: Failed to save repository list." << std::endl;
    return false;
  }
  return true;
}

bool cpkg::repos::remove_repository_from_list(int index)
{
  std::vector<std::string> current_repository_list;
  if(!get_repository_list(current_repository_list))
  {
    std::cerr << "Error: Failed to retrieve repository list." << std::endl;
    return false;
  }

  if(index >= 0 && index < static_cast<int>(current_repository_list.size()))
  {

    current_repository_list.erase(current_repository_list.begin() + index);
  }
  else
  {

    std::cerr << "Error: Index out of bounds." << std::endl;
    return false;
  }

  if(!save_repository_list(current_repository_list))
  {
    std::cerr << "Error: Failed to save repository list." << std::endl;
    return false;
  }

  return true;
}