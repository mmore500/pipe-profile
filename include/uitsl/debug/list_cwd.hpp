#pragma once
#ifndef UITSL_DEBUG_LIST_CWD_HPP_INCLUDE
#define UITSL_DEBUG_LIST_CWD_HPP_INCLUDE

#include <filesystem>
#include <iostream>

namespace uitsl {

void list_cwd() {
  std::cout << std::filesystem::absolute(".") << std::endl;
  for (auto& p: std::filesystem::directory_iterator(".")) {
    std::cout << p.path() << std::endl;
  }
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_LIST_CWD_HPP_INCLUDE
