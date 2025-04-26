#ifndef IO_HPP_
#define IO_HPP_

#include <fmt/base.h>
#include <iostream>
#include <toml++/impl/parse_error.hpp>
#include <toml++/impl/parser.hpp>
#include <toml++/impl/table.hpp>
#pragma once

#include <cstring>

#include "color.h"

#define CONFIG_PATH "config.toml"

namespace io {

    inline void a_help() { fmt::print("This is a help tip\n"); }
    inline void a_about() {
        fmt::println("---- {} ----\nAuthor: {}\nDescription: {}\nVersion: {}\nLicense: {}\nRepo: {}",
                   PROJECT_NAME, PROJECT_AUTHOR, PROJECT_DESCRIPTION, PROJECT_VERSION, PROJECT_LICENSE, PROJECT_REPO);
    }

    inline void error(const std::string& error_info) { fmt::println("{}[ERROR]{} {}", RED, RESET, error_info); }
    inline void done(const std::string& done_info) { fmt::println("{}[DONE]{} {}", GREEN, RESET, done_info); }
    inline void info(const std::string& info_info) { fmt::println("{}[INFO]{} {}", BLUE, RESET, info_info); }
    inline void warn(const std::string& warn_info) { fmt::println("{}[WARN]{} {}", YELLOW, RESET, warn_info); }

    inline void question(const std::string& question_text, bool nl) {
        if ( nl ) {
            fmt::println("{}", question_text);
            printf("> ");
        }
        else { fmt::print("{} ", question_text); }
    }

    inline bool args_parse( char *args, const char  *cmd1, const char *cmd2) {
        if ( std::strcmp(args, cmd1) == 0 || std::strcmp(args, cmd2) ) { return true; }
        else { return false; }
    }

    inline bool y_or_n(const std::string& question_text, bool nl) {
        std::string input;
        while (true) {
            io::question(question_text + " [y/n]", nl);
            std::cin >> input;
            
            auto left = std::find_if_not(input.begin(), input.end(), ::isspace);
            auto right = std::find_if_not(input.rbegin(), input.rend(), ::isspace).base();
            std::string input_resault(left, right);

            if ( input_resault == "Y" || input_resault == "y" ) { return true; }
            else if ( input_resault == "N" || input_resault == "n" ) { return false; }
            else { io::error("Wrong input, enter again!"); }
        }
    }
} // namespace io

#endif
