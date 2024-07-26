/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#include "../ProgramInfo.h"
#include <windows.h>
#include <string>
#include <ctime>

namespace cpkg::info
{

  const char* last_update_time_key_path = "Software\\Bonsall2004\\cpkg";
  const char* last_update_time_key_name = "last_update_time";

  time_t get_last_update_time() {
    HKEY hKey;
    LONG result;
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);
    time_t lastUpdateTime = 0;

    result = RegOpenKeyEx(
      HKEY_CURRENT_USER,
      last_update_time_key_path,
      0,
      KEY_QUERY_VALUE,
      &hKey
    );

    if (result != ERROR_SUCCESS) return 0;
    result = RegQueryValueEx(
      hKey,
      last_update_time_key_name,
      NULL,
      NULL,
      (LPBYTE)buffer,
      &bufferSize
    );

    if (result == ERROR_SUCCESS) {
      lastUpdateTime = std::stoll(buffer);
    }
    RegCloseKey(hKey);
    return lastUpdateTime;
  }


  bool set_last_update_time()
  {
    HKEY hKey;
    LONG result;

    const char* valueName = "last_update_time";

    std::time_t currentTime = std::time(nullptr);
    std::string timestamp = std::to_string(currentTime);

    result = RegCreateKeyEx(
      HKEY_CURRENT_USER,
      last_update_time_key_path,
      0,
      NULL,
      REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS,
      NULL,
      &hKey,
      NULL
    );

    if (result != ERROR_SUCCESS)
    {
      return false;
    };

    result = RegSetValueEx(
      hKey,
      last_update_time_key_name,
      0,
      REG_SZ,
      (const BYTE*)timestamp.c_str(),
      (DWORD)(timestamp.size() + 1)
    );

    if (result != ERROR_SUCCESS) {
      RegCloseKey(hKey);
      return false;
    }

    RegCloseKey(hKey);
    return true;
  }

  bool is_update_available()
  {
    return true;
  }
}