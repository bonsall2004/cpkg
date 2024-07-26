/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once
#include <corecrt.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace cpkg::info
{
  /**
   * @return WString Path roaming directory
   */
  std::wstring get_roaming_directory();

  /**
   *
   * @param narrow Regular std::string
   * @return std::wstring version of the narrow string
   */
  std::wstring narrow_to_wide(const std::string& narrow);

  /**
   *
   * @param wide Wide string
   * @return std::string version of the wide string
   */
  std::string wide_to_narrow(const std::wstring& wide);

  /**
   * @return path to the cpkg cache directory
   */
  std::wstring get_cache_directory();

  /**
   * @return path to the package cache
   */
  std::wstring get_package_cache_path();

  /**
   * @return path to the cpkg appdata directory
   */
  std::wstring get_cpkg_data_path();

  /**
   * @return Path to the repository list
   */
  std::wstring get_repository_list_path();

  /**
   *
   * @param packages passed by reference to allow for multiple return values
   * @return True if successful
   */
  bool get_cached_packages(std::unordered_map<std::string, std::string>& packages);
}