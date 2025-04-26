#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "io.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "utils.hpp"

namespace config {
    class config {
    private:
        std::filesystem::path file_path;

    public:
        config(std::filesystem::path _file_path) : file_path(_file_path) {}

        bool save(toml::table config_data_table) {
            std::ofstream file(file_path, std::ios::app);
            if (file) {
                file << config_data_table << std::endl;
                return true;
            }
            else {
                return false;
            }
        }

        std::string get_content_string() {
            std::ifstream file(file_path);
            if (file) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                return buffer.str();
            }
            else {
                io::error("Failed to open config file for reading!");
                return "";
            }
        }

        toml::table get_content_toml() {
            auto content = this->get_content_string();
            if (content.empty()) {
                io::error("Read config failed or config is empty!");
                bool gen_config = io::y_or_n("Would you want to generate a new config now?", false);
                if (gen_config) {
                    io::question("Where do you want to save your game data?", true);
                    std::string game_data;
                    std::cin >> game_data;

                    toml::table root_node, new_config_data;
                    utils::table_add_node<std::string>(root_node, "game_data", game_data);

                    new_config_data.insert_or_assign("global", root_node);
                    bool save_status = this->save(new_config_data);

                    if (save_status) { io::done("Config has been generated!"); }
                    else { io::error("There is something wrong when saving config!"); }
                }
                else {
                    io::info("There is nothing to do.");
                }
            }
            else {
                try {
                    return toml::parse(content);
                }
                catch (const toml::parse_error& e) {
                    io::error(e.what());
                }
            }
            return toml::table();
        }
    };
}

#endif
