/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once
#include <vector>
#include <string>

namespace cpkg::repos
{
  /**
   * @param vec Loads the repository list into this variable, passed by reference
   * @return True is successful
   */
  bool get_repository_list(std::vector<std::string>& vec);

  /**
 * @param vec overrides the current repository list with this one
 * @return True is successful
 */
  bool save_repository_list(const std::vector<std::string>& vec);

  /**
  * @param vec appends to the current repository list
  * @return True is successful
  */
  bool append_to_repository_list(const std::vector<std::string>& vec);

  /**
  * @param index remove a repository list by index
  * @return True is successful
  */
  bool remove_repository_from_list(int index);

  /**
  * @param index remove a repository list by string literal
  * @return True is successful
  */
  bool remove_repository_from_list(const std::string& repository);

  /**
  * @param vec remove multiple repositories by string literal vector
  * @return True is successful
  */
  bool remove_repositories_from_list(const std::vector<std::string>& repositories_to_remove);
}