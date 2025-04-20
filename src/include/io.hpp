#ifndef IO_HPP_
#define IO_HPP_

#pragma once

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <toml++/toml.hpp>
#include <fmt/core.h>

#define CONFIG_PATH "config.toml"

namespace io {
    constexpr auto ERROR_COLOR = "\033[31;1m";
    constexpr auto DONE_COLOR  = "\033[32;1m";
    constexpr auto INFO_COLOR  = "\033[34;1m";
    constexpr auto WARN_COLOR  = "\033[33;1m";
    constexpr auto RESET_COLOR = "\033[0m";

    class Config {
        private:
            std::filesystem::path file_path;

        public:
            Config(std::filesystem::path _file_path) : file_path(_file_path) {}

            bool save(toml::table config_data_table) {
                std::fstream file(file_path, std::ios::app);
                if ( file ) {
                    file << config_data_table << std::endl;
                    file.close();
                    return true;
                }
                else { return false; }
            }

            std::string get_content() {
                std::fstream file(file_path, std::ios::in);
                if ( file ) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    file.close();
                    return buffer.str();
                }
                else { file.close(); return ""; }
            }
    };

    inline void a_help() { fmt::print("This is a help tip\n"); }
    inline void a_about() {
        fmt::print("---- {} ----\nAuthor: {}\nDescription: {}\nVersion: {}\nLicense: {}\nRepo: {}\n",
                   PROJECT_NAME, PROJECT_AUTHOR, PROJECT_DESCRIPTION, PROJECT_VERSION, PROJECT_LICENSE, PROJECT_REPO);
    }

    inline void error(const std::string& error_info) { fmt::print("{}[ERROR]{} {}\n", ERROR_COLOR, RESET_COLOR, error_info); }
    inline void done(const std::string& done_info) { fmt::print("{}[DONE]{} {}\n", DONE_COLOR, RESET_COLOR, done_info); }
    inline void info(const std::string& info_info) { fmt::print("{}[INFO]{} {}\n", INFO_COLOR, RESET_COLOR, info_info); }
    inline void warn(const std::string& warn_info) { fmt::print("{}[WARN]{} {}\n", WARN_COLOR, RESET_COLOR, warn_info); }

    inline bool args_parse( char* args, const char* cmd) {
        if ( std::strcmp(args, cmd) == 0 ) { return true; }
        else { return false; }
    }

    inline bool y_or_n(const std::string& question) {
        std::string input;
        info(question + " [y/n] ");
        while (true) {
            printf("> ");
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