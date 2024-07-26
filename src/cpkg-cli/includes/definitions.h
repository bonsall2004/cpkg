/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once
#include <regex>

/**
 * @brief Regex to find out if something is a valid repository list
 */
const std::regex repo_list_regex(R"(\b(?:https?|ftp):\/\/[\w.-]+(?:\.[a-zA-Z]{2,})+(?:\/[\w.-]*)*\b)");
