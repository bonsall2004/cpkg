/*
 * cpkg-updater
 * Author: bonsall2004
 * Description: 
 */
#include "version_info.h"
#include <windows.h>
#include <wininet.h>
#include <string>
#include <sstream>
#include <vector>

namespace cpkg::updater
{
  std::vector<int> split_version(const std::string& version)
  {
    std::vector<int> components;
    std::string component;
    std::istringstream stream(version);

    while(std::getline(stream, component, '.'))
    {
      components.push_back(std::stoi(component));
    }

    return components;
  }

  bool is_version_greater(const std::string& v1, const std::string& v2)
  {
    auto components1 = split_version(v1);
    auto components2 = split_version(v2);

    size_t max_size = std::max(components1.size(), components2.size());

    components1.resize(max_size, 0);
    components2.resize(max_size, 0);

    for(size_t i = 0; i < max_size; ++i)
    {
      if(components1[i] > components2[i])
      {
        return true;
      }
      else if(components1[i] < components2[i])
      {
        return false;
      }
    }

    return false;
  }

  std::string get_latest_release_version(const std::string& owner, const std::string& repo)
  {
    std::string version;
    HINTERNET h_internet = InternetOpen(reinterpret_cast<LPCSTR>(L"GitHub Client"), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if(h_internet)
    {
      HINTERNET h_connect = InternetOpenUrlA(h_internet, ("https://api.github.com/repos/" + owner + "/" + repo + "/releases/latest")
        .c_str(), nullptr, 0, INTERNET_FLAG_RELOAD, 0);

      if(h_connect)
      {
        char buffer[4096];
        DWORD bytes_read;
        std::string response;

        while(InternetReadFile(h_connect, buffer, sizeof(buffer) - 1, &bytes_read) && bytes_read > 0)
        {
          buffer[bytes_read] = '\0';
          response.append(buffer);
        }

        size_t tag_pos = response.find(R"("tag_name":")");
        if(tag_pos != std::string::npos)
        {
          tag_pos += 12; // Length of the string "\"tag_name\":\""
          size_t end_pos = response.find('\"', tag_pos);
          if(end_pos != std::string::npos)
          {
            version = response.substr(tag_pos, end_pos - tag_pos);

            if(!version.empty() && version[0] == 'v')
            {
              version = version.substr(1);
            }
          }
        }
        InternetCloseHandle(h_connect);
      }
      InternetCloseHandle(h_internet);
    }

    return version;
  }

  bool check_for_updates()
  {
    std::string latest_version = get_latest_release_version("bonsall2004", "cpkg");

    if(latest_version.empty())
    {
      return false;
    }

    return is_version_greater(latest_version, "1.0.0");
  }
}
