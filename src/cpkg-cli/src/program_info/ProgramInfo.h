/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once
#include <corecrt.h>
#include <string>
#include <vector>

#ifdef WIN32

#else

#endif

namespace cpkg::info
{
  /**
   * @return last update time from windows registry
   */
  time_t get_last_update_time();

  /**
   * @brief sets the current time to the last_update_time registry key
   * @return True if sucessful
   */
  bool set_last_update_time();

  /**
   * @param url URL online for where the resource is located
   * @param file_path Where the file should be stored
   * @return True if successful
   */
  bool download_file(const std::wstring& url, const std::wstring& file_path);
}
