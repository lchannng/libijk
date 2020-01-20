/*
 * File  : os.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/12/12 16:10:53
 */

#pragma once
#ifndef OS_HPP_XZWSVKTR
#define OS_HPP_XZWSVKTR

#include <filesystem>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

namespace ijk {
namespace os {

inline std::string getcwd() {
    return fs::current_path().string();
}

inline void chdir(std::string_view path) {
    fs::current_path(path);
}

inline void mkdirs(const fs::path &path, std::error_code& ec) {
    fs::create_directories(path, ec);
}

inline fs::path executable_path() {
#if defined(_WIN32) || defined(_WIN64)
    char temp[MAX_PATH];
    auto len = GetModuleFileName(NULL, temp, MAX_PATH);
#else
    char temp[1024];
    auto len = readlink("/proc/self/exe", temp, 1024);
#endif
    return fs::path(std::string_view(temp, len));
}

inline std::string executable_full_name() {
    auto p = executable_path().filename();
    return p.string();
}

inline std::string executable_name() {
    auto p = executable_path().stem();
    return p.string();
}

}  // namespace os
}  // namespace ijk

#endif /* end of include guard: OS_HPP_XZWSVKTR */
