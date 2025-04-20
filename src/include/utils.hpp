#ifndef UTILS_HPP_
#define UTILS_HPP_

#pragma once

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

#include "io.hpp"

namespace utils {
    template<typename T>
    static void table_add_node(toml::table& table, const std::string& key, const T& value ) {
        table.insert_or_assign( key, toml::value<T>(value) );
    }

    inline bool search_file(std::filesystem::path file_path, const std::string& target) {
        std::vector<std::string> content;
        std::string tmp;
        std::ifstream file(file_path, std::ios::in);

        if ( file ) {
            while (std::getline(file, tmp)) { content.push_back(tmp); }
            file.close();

            if ( std::find(content.begin(), content.end(), target) == content.end() ) { return false; }
            else { return true; }
        }
        else {
            file.close();
            io::error("Can't open " + file_path.string());
            return false;
        }
    }
}

#endif