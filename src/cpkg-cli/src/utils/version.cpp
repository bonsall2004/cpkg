/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#include "version.h"
#include <iostream>
#include <iomanip>
#include "../program_info/ProgramInfo.h"
#include "core/versions/utils.h"

namespace cpkg::utils
{

  void print_row(const char* left, const char* right)
  {
    const int labelWidth = 24;
    const int valueWidth = 20;

    std::cout << "| " << std::left << std::setw(labelWidth) << left << " "
      << std::right << std::setw(valueWidth) << right << " |\n";
  }

  void print_version_box()
  {
    time_t last_update_time = cpkg::info::get_last_update_time();

    // Print version info
    std::cout << "|---------------- Version Info -----------------|\n";
    print_row("CPKG CLI Version:", CPKG_VERSION);
    print_row("CPKG CORE Version:", CPKG_CORE_LIB_VERSION);
    std::cout << "|---------------- Update Info ------------------|\n";

    if (last_update_time != 0) {
      struct tm* timeInfo = std::localtime(&last_update_time);
      char buffer[80];
      std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
      print_row("Last Update Time:", buffer);
    }

    print_row("CPKG CLI:", cpkg::info::is_update_available() ? "   \033[32mUpdate Available!\033[0m" : "Up To Date!");
    print_row("CPKG Core:", cpkg::core::utils::is_update_available() ? "   \033[32mUpdate Available!\033[0m" : "Up To Date!");
    std::cout << "|-----------------------------------------------|\n";
  }
}