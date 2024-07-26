/*
 * cpkg
 * Author: bonsall2004
 * Description: Information about the program and utils
 */
#include "../ProgramInfo.h"
#include <windows.h>
#include <ctime>
#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <filesystem>

#pragma comment(lib, "ole32.lib")

namespace cpkg::info
{

  const char* last_update_time_key_path = "Software\\Bonsall2004\\cpkg";
  const char* last_update_time_key_name = "last_update_time";

  const wchar_t* repository_list_name = L"cpkg_repo.dat";

  time_t get_last_update_time()
  {
    HKEY hKey;
    LONG result;
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);
    time_t lastUpdateTime = 0;

    result = RegOpenKeyEx(HKEY_CURRENT_USER, last_update_time_key_path, 0, KEY_QUERY_VALUE, &hKey);

    if(result != ERROR_SUCCESS) return 0;
    result = RegQueryValueEx(hKey, last_update_time_key_name, nullptr, nullptr, (LPBYTE)buffer, &bufferSize);

    if(result == ERROR_SUCCESS)
    {
      lastUpdateTime = std::stoll(buffer);
    }
    RegCloseKey(hKey);
    return lastUpdateTime;
  }

  bool set_last_update_time()
  {
    HKEY hKey;
    LONG result;

    std::time_t currentTime = std::time(nullptr);
    std::string timestamp = std::to_string(currentTime);

    result = RegCreateKeyEx(HKEY_CURRENT_USER, last_update_time_key_path, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hKey, nullptr);

    if(result != ERROR_SUCCESS)
      return false;

    result = RegSetValueEx(hKey, last_update_time_key_name, 0, REG_SZ, (const BYTE*)timestamp.c_str(), (DWORD)(timestamp
      .size() + 1));

    if(result != ERROR_SUCCESS)
    {
      RegCloseKey(hKey);
      return false;
    }

    RegCloseKey(hKey);
    return true;
  }

  std::string wide_to_narrow(const std::wstring& wide)
  {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide);
  }

  std::wstring narrow_to_wide(const std::string& narrow)
  {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(narrow);
  }

  std::wstring get_roaming_directory()
  {
    wchar_t path[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, path)))
    {
      return { path };
    }
    return {};
  }
  std::string wstring_to_string(const std::wstring& wstr)
  {
    if(wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);

    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);

    return str;
  }

  bool save_repository_list(const std::vector<std::string>& vec)
  {
    std::wstring roamingDir = get_roaming_directory();
    if(roamingDir.empty())
    {
      std::cerr << "Error: Roaming directory is empty." << std::endl;
      return false;
    }

    roamingDir += L"\\.cpkg\\";
    std::string roamingDirStr = wstring_to_string(roamingDir);

    if(!std::filesystem::exists(roamingDirStr))
    {
      if(!std::filesystem::create_directories(roamingDirStr))
      {
        std::cerr << "Error: Failed to create directories. Path: " << roamingDirStr << std::endl;
        return false;
      }
    }

    std::wstring filePath = roamingDir + repository_list_name;
    std::string filePathStr = wstring_to_string(filePath);

    std::ofstream file(filePathStr.c_str(), std::ios::binary);
    if(!file.is_open())
    {
      std::cerr << "Error: Failed to open file for writing. Path: " << filePathStr << std::endl;
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

  bool get_repository_list(std::vector<std::string>& vec)
  {
    std::wstring roamingDir = get_roaming_directory();
    if(roamingDir.empty())
    {
      return false;
    }

    roamingDir += L"\\.cpkg\\";
    roamingDir += repository_list_name;

    std::ifstream file(roamingDir.c_str(), std::ios::binary);
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

  bool append_to_repository_list(const std::vector<std::string>& vec)
  {
    std::vector<std::string> current_repository_list;
    get_repository_list(current_repository_list);
    for(const auto& str : vec)
    {
      current_repository_list.push_back(str);
    }
    return save_repository_list(current_repository_list);
  }

  bool remove_repositories_from_list(const std::vector<std::string>& repositories_to_remove)
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

  bool remove_repository_from_list(const std::string& repository)
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
  bool remove_repository_from_list(int index)
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
}