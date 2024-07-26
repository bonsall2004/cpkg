/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#include <cache.h>
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <shlobj.h>
#include <fstream>
#include "terminal_character_help.h"

std::wstring cpkg::info::get_roaming_directory()
{
  wchar_t path[MAX_PATH];
  if(SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, path)))
  {
    return { path };
  }
  return {};
}

std::wstring cpkg::info::get_cache_directory()
{
  std::wstring file_path = cpkg::info::get_cpkg_data_path() + L"\\cache";
  if(!std::filesystem::exists(file_path))
  {
    if(!std::filesystem::create_directory(file_path))
    {
      std::cerr << FG_RED << "Failed to create cache directory\n" << RESET;
    }
  }

  return file_path;
}

std::wstring cpkg::info::get_package_cache_path()
{
  return get_cache_directory() + L"\\repository_lists.cpkg-cache";
}

std::wstring cpkg::info::get_cpkg_data_path()
{
  std::wstring file_path = cpkg::info::get_roaming_directory() + L"\\.cpkg";
  if(!std::filesystem::exists(file_path))
  {
    if(!std::filesystem::create_directory(file_path))
    {
      std::cerr << FG_RED << "Failed to create .cpkg directory\n" << RESET;
    }
  }

  return file_path;
}

std::wstring cpkg::info::get_repository_list_path()
{
  return get_cpkg_data_path() + L"\\repositories.cpkg";
}

std::string cpkg::info::wide_to_narrow(const std::wstring& wide)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wide);
}

std::wstring cpkg::info::narrow_to_wide(const std::string& narrow)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(narrow);
}

bool cpkg::info::get_cached_packages(std::unordered_map<std::string, std::string>& packages)
{
  std::ifstream cache_file(get_package_cache_path().c_str());
  if(!cache_file.is_open())
  {
    std::cerr << "Failed to open repository cache \n";
    return false;
  }

  std::string line;
  while(std::getline(cache_file, line))
  {
    std::istringstream line_stream(line);
    std::string key, value;

    if(std::getline(line_stream, key, ':') && std::getline(line_stream, value))
    {
      packages[key] = value;
    }
    else
    {
      std::cerr << "Invalid line format: " << line << std::endl;
    }
  }

  cache_file.close();
  return true;
}