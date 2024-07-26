/*
 * cpkg
 * Author: bonsall2004
 * Description: Information about the program and utils
 */
#include "../ProgramInfo.h"
#include "definitions.h"
#include "cache.h"
#include <windows.h>
#include <ctime>
#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include <terminal_character_help.h>
#include <vector>
#include <thread>
#include <string>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ole32.lib")

namespace cpkg::info
{

  const char* last_update_time_key_path = "Software\\Bonsall2004\\cpkg";
  const char* last_update_time_key_name = "last_update_time";

  const wchar_t* repository_list_name = L"repositories.cpkg";

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

  bool is_valid_line(const std::wstring& line)
  {
    std::wregex pattern(LR"(^[^\s]+:https://[^\s]+$)");
    return std::regex_match(line, pattern);
  }

  bool download_file(const std::wstring& url, const std::wstring& file_path)
  {
    HINTERNET hInternet = nullptr;
    HINTERNET hConnect = nullptr;
    bool result = false;

    // Initialize WinINet
    hInternet = InternetOpen("File Downloader", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if(hInternet == nullptr)
    {
      std::wcerr << L"InternetOpen failed: " << GetLastError() << std::endl;
      return false;
    }

    // Open the URL
    hConnect = InternetOpenUrlW(hInternet, url.c_str(), nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if(hConnect == nullptr)
    {
      std::wcerr << L"InternetOpenUrlW failed: " << GetLastError() << std::endl;
      InternetCloseHandle(hInternet);
      return false;
    }

    // Check for HTTP response status code
    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    if(!HttpQueryInfoW(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, nullptr))
    {
      std::wcerr << L"HttpQueryInfo failed: " << GetLastError() << std::endl;
      InternetCloseHandle(hConnect);
      InternetCloseHandle(hInternet);
      return false;
    }

    if(statusCode != 200)
    { // Check for HTTP status 200 (OK)
      InternetCloseHandle(hConnect);
      InternetCloseHandle(hInternet);
      return false;
    }

    // Temporary file to hold downloaded content
    std::wstring tempFilePath = file_path + L".temp";
    std::ofstream tempFile(tempFilePath.c_str(), std::ios::binary);
    if(!tempFile.is_open())
    {
      std::wcerr << L"Failed to open temporary file: " << tempFilePath << std::endl;
      InternetCloseHandle(hConnect);
      InternetCloseHandle(hInternet);
      return false;
    }

    // Read data from the URL and write it to the temporary file
    char buffer[4096];
    DWORD bytesRead = 0;

    while(InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
      tempFile.write(buffer, bytesRead);
    }

    tempFile.close();
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    std::ifstream tempFileIn(tempFilePath.c_str());
    if(!tempFileIn.is_open())
    {
      std::wcerr << L"Failed to open temporary file for reading: " << tempFilePath << std::endl;
      return false;
    }

    std::ofstream outFile(file_path.c_str(), std::ios::binary | std::ios::app);
    if(!outFile.is_open())
    {
      std::wcerr << L"Failed to open file for appending: " << file_path << std::endl;
      return false;
    }

    std::string line;
    bool hasValidData = false;
    while(std::getline(tempFileIn, line))
    {
      if(is_valid_line(narrow_to_wide(line)))
      {
        outFile << line << '\n';  // Append valid line to the file
        hasValidData = true;
      }
    }

    tempFileIn.close();
    outFile.close();
    std::remove(wide_to_narrow(tempFilePath).c_str());

    result = hasValidData;
    return result;
  }

}