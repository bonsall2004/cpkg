/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#include "version.h"
#include <iostream>
#include <iomanip>
#include <updater/updater.h>
#include <atomic>
#include <thread>
#include "../program_info/ProgramInfo.h"
#include "core/versions/utils.h"

namespace cpkg::utils
{
  const int labelWidth = 24;
  const int valueWidth = 20;

  void print_row(const char* left, const char* right)
  {
    std::cout << "| " << std::left << std::setw(labelWidth) << left << " " << std::right << std::setw(valueWidth) << right << " |\n";
  }

  std::string update_string(bool v)
  {
    return v ? "   \033[32mUpdate Available!\033[0m" : "Up To Date!";
  }

  void print_version_box()
  {
    time_t last_update_time = cpkg::info::get_last_update_time();

    std::cout << "|---------------- Version Info -----------------|\n";
    print_row("CPKG CLI Version:", CPKG_VERSION);
    print_row("CPKG CORE Version:", CPKG_CORE_LIB_VERSION);
    std::cout << "|---------------- Update Info ------------------|\n";

    if(last_update_time != 0)
    {
      struct tm* timeInfo = std::localtime(&last_update_time);
      char buffer[80];
      std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
      print_row("Last Update Time:", buffer);
    }
    print_row("CPKG CLI:", "Checking for Updates");
    print_row("CPKG Core:", "Checking for Updates");

    std::cout << "|-----------------------------------------------|\n";

    std::string cpkg_cli_update_string = update_string(cpkg::updater::check_for_updates());
    std::cout << "\033[A" << "\033[A" << "\033[A" << "| " << std::left << std::setw(labelWidth) << "CPKG CLI:" << " " << std::right << std::setw(valueWidth) << cpkg_cli_update_string << " |       \n" << std::flush;

    std::string cpkg_core_update_string = update_string(cpkg::core::utils::is_update_available());
    std::cout << "| " << std::left << std::setw(labelWidth) << "CPKG Core:" << " " << std::right << std::setw(valueWidth) << cpkg_core_update_string << " |       \033[B\n" << std::flush;
  }
}