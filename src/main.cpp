#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <toml++/toml.hpp>

#include "include/io.hpp"
#include "include/game.hpp"
#include "utils.hpp"

int main(int argc, char* args[]) {
    io::Config c(CONFIG_PATH);
    std::string c_data = c.get_content();

    if ( c_data.empty() ) {
        io::error("Read config failed or config is empty!");
        bool gen_config = io::y_or_n("Would you want to general a new config now?");
        if ( gen_config ) {
            io::info("Where do you want to save your game data?");
            std::string game_data;
            printf("> ");
            std::cin >> game_data;
            
            toml::table root_node, new_config_data;
            utils::table_add_node<std::string>(root_node, "game_data", game_data);

            new_config_data.insert_or_assign("global", root_node);
            bool save_status = c.save(new_config_data);
            
            if ( save_status ) { io::done("Config has generaled!"); return 0; }
            else { io::error("There is something wrong!"); return 1; }
        }
        else { io::info("Bye~"); return 1; }
    }
    else {
        toml::table config_data;

        try { config_data = toml::parse(c_data); }
        catch ( const toml::parse_error& err ) { io::error(err.what()); return 1; }

        if ( argc > 1 ) {
            if ( io::args_parse(args[1], "-h") || io::args_parse(args[1], "--help") ) { io::a_help(); return 0; }
            else if ( io::args_parse(args[1], "-v") || io::args_parse(args[1], "--version") ) { printf("%s\n", PROJECT_VERSION); return 0; }
            else if ( io::args_parse(args[1], "--about") ) { io::a_about();return 0; }
            else { io::error("Too less arguments or wrong arguments"); return 1; }
        }
        
        game::Player player = game::login(config_data["global"]["game_data"].as_string()->get());
        
        return 0;
    }
}
