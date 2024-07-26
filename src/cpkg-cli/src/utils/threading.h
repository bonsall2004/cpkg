/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */

#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <optional>
#include <regex>

/**
 * @param packages Package to search through
 * @param key Key we're searching for
 * @param numThreads How many threads to use, default is 8, (min = 0, max = 128)
 * @return std::optional<std::unordered_map<std::string, std::string>> of each result of key and link
 */
std::optional<std::unordered_map<std::string, std::string>> find_value_threaded(const std::unordered_map<std::string, std::string>& packages, const std::string& key, int numThreads = 8);