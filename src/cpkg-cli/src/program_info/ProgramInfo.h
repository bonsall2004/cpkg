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
  time_t get_last_update_time();
  bool set_last_update_time();

  bool get_repository_list(std::vector<std::string>& vec);
  bool save_repository_list(const std::vector<std::string>& vec);
  bool append_to_repository_list(const std::vector<std::string>& vec);
  bool remove_repository_from_list(int index);
  bool remove_repository_from_list(std::string& repository);
  bool remove_repositories_from_list(const std::vector<std::string>& repositories_to_remove);
}
