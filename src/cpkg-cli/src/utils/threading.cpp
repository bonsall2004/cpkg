/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#include "threading.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>

double calculate_similarity(const std::string& key1, const std::string& key2)
{
  size_t minLength = std::min(key1.length(), key2.length());
  size_t maxLength = std::max(key1.length(), key2.length());

  size_t matchingChars = 0;
  for(size_t i = 0; i < minLength; ++i)
  {
    if(key1[i] == key2[i])
    {
      ++matchingChars;
    }
  }
  return static_cast<double>(matchingChars) / maxLength;
}

std::unordered_map<std::string, std::string> find_in_chunk(const std::unordered_map<std::string, std::string>::const_iterator& start, const std::unordered_map<std::string, std::string>::const_iterator& end, const std::regex& keyPattern)
{
  std::unordered_map<std::string, std::string> results;
  for(auto it = start; it != end; ++it)
  {
    if(std::regex_search(it->first, keyPattern))
    {
      results[it->first] = it->second;
    }
  }
  return results;
}

std::optional<std::unordered_map<std::string, std::string>> find_value_threaded(const std::unordered_map<std::string, std::string>& packages, const std::string& key, int numThreads)
{
  if(numThreads <= 0)
  {
    throw std::invalid_argument("Number of threads must be positive.");
  }

  size_t mapSize = packages.size();
  size_t chunkSize = mapSize / numThreads;
  size_t remainder = mapSize % numThreads;

  std::vector<std::thread> threads;
  std::vector<std::unordered_map<std::string, std::string>> results(numThreads);
  std::vector<std::unordered_map<std::string, std::string>::const_iterator> starts(numThreads + 1);

  auto it = packages.begin();
  for(int i = 0; i < numThreads; ++i)
  {
    starts[i] = it;
    std::advance(it, chunkSize + (i < remainder ? 1 : 0));
  }
  starts[numThreads] = packages.end();

  std::regex keyPattern(key);

  std::mutex resultMutex;

  for(int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back([&, i]
    {
      auto chunkResults = find_in_chunk(starts[i], starts[i + 1], keyPattern);
      std::lock_guard<std::mutex> lock(resultMutex);
      results[i] = std::move(chunkResults);
    });
  }

  for(auto& t : threads)
  {
    t.join();
  }

  std::unordered_map<std::string, std::string> combinedResults;
  for(const auto& result : results)
  {
    combinedResults.insert(result.begin(), result.end());
  }

  if(combinedResults.empty())
  {
    return std::nullopt;
  }

  std::vector<std::pair<std::string, std::string>> sortedResults(combinedResults.begin(), combinedResults
    .end());
  std::sort(sortedResults.begin(), sortedResults.end(), [&key](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b)
  {
    return calculate_similarity(a.first, key) > calculate_similarity(b.first, key);
  });

  std::unordered_map<std::string, std::string> sortedMap(sortedResults.begin(), sortedResults.end());
  return sortedMap;
}