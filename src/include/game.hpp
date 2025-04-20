#ifndef GAME_HPP_
#define GAME_HPP_

#pragma once

#include <string>
#include <vector>
#include <toml++/toml.hpp>
#include <filesystem>

#include "utils.hpp"

namespace game {

    class Item {
        private:
            std::string name, description;
        public:
            Item(std::string _name):
                name(_name) {}
    };

    class Container {
        private:
            int size;
            std::vector<Item> items;
        public:
            Container(int _size):
                size(_size), items() {}
            
            void add() {
                if (items.size() < static_cast<size_t>(size)) {
                    items.emplace_back("default");
                    size = size - 1;
                }
            }
            void reduce() {
                if (!items.empty()) {
                    items.pop_back();
                    size = size + 1;
                }
            }
    };
    
    class Player {
        private:
            std::string name, passwd;
            int level, health = 20;
            bool live = true;

        public:
            Player(std::string _name, std::string _passwd, int _level):
                name(_name), passwd(_passwd), level(_level){}

            void change_name(std::string new_name) { name = new_name; }
            void change_passwd(std::string new_passwd) { passwd = new_passwd; }

            void level_up(int up) { level = level + up; }
            void level_down(int down) { level = level - down; }

            int get_level() const { return level; }
            std::string get_name() const { return name; }
            int get_health() const { return health; }

            bool is_died() {
                if ( health <= 0) { live = false; }
                return live;
            }

            bool check_passwd(const std::string& new_passwd) {
                if ( new_passwd == passwd ) { return false; }
                else { return true; }
            }

            void print_data() {
                printf("Name: %s\n", name.c_str());
                printf("Password: %s\n", passwd.c_str());
                printf("Level: %d\n", level);
            }

            toml::table to_toml() const {
                toml::table root_node{};
                utils::table_add_node<std::string>(root_node, "passwd", passwd);
                utils::table_add_node<int64_t>(root_node, "level", level);
                utils::table_add_node<bool>(root_node, "live", live);
                utils::table_add_node<int64_t>(root_node, "health", health);

                toml::table toml_data{};
                toml_data.insert_or_assign(name, root_node);
                return toml_data;
            }

            bool save_data(std::filesystem::path data_path) {
                std::fstream data_file;
                data_file.open(data_path, std::ios::app);
    
                if ( data_file ) {
                    
                    if ( !utils::search_file(data_path, "[" + name + "]") ) {
                        data_file << this->to_toml() << std::endl;
                        data_file.close();
                        return true;
                    }
                    else {
                        data_file.close();
                        io::error(name + " has already been in " + data_path.string());
                        return false;
                    }
                }
                else {
                    data_file.close();
                    io::error("Can't write player's data!");
                    return false;
                }
            }

            int load_data(std::string_view data_path) {
                toml::table data;
                try {
                    data = toml::parse_file(data_path);

                    if ( !data.empty() ) {
                        if ( data.get(name) ) {
                            if ( passwd == data[name]["passwd"] ) {
                                level = data[name]["level"].value<int>().value();
                                health = data[name]["health"].value<int>().value();
                                live = data[name]["live"].as_boolean();
                                return 0;
                            }
                            else { return 4; }
                        }
                        else { return 3; }
                    }
                    else { return 2; }
                }
                catch ( const toml::parse_error& err ) { return 1; }
            }
    };

    inline Player login(std::filesystem::path player_data_path) {
        printf("Enter your game name: ");
        char pl_name[128], pl_passwd[128];
        scanf("%s", pl_name);
        printf("Enter password: ");
        scanf("%s", pl_passwd);

        game::Player pl(pl_name, pl_passwd, 0);

        int load_status = pl.load_data(player_data_path.string());
        if ( load_status == 0 ) {
            io::done("Welcome back! " + pl.get_name());
            return pl;
        }
        else if ( load_status == 4 ) {
            io::error("Wrong password!");
            
            for (scanf("Please enter your password again: %s", pl_passwd); pl.check_passwd(pl_passwd); scanf("Please enter your password again: %s", pl_passwd));
            return pl;
        }
        else {
            if ( load_status == 1) { io::error("Can't load player's data!"); }
            else if ( load_status == 2 ) { io::error("Empty player's data file!"); }
            else { io::error("Can't find " + pl.get_name() + "'s data!"); }

                bool input_status = io::y_or_n("You are new, would you want to save your data?");
                if ( input_status ) {
                    if ( pl.save_data(player_data_path) ) {
                        io::done("Data saved!");
                        return pl;
                    }
                }
                else {
                    io::warn("Ok, but your data will be lost after you left game");
                    return pl;
                }
                return pl;
        }
    }
    
    /*
    inline void start(Player pl) {

    }
    */
}

#endif