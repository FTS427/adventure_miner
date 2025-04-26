#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <toml++/toml.hpp>

#include "include/config.hpp"
#include "include/game.hpp"
#include "include/io.hpp"

int
main(int argc, char* args[ ]) {
    config::config c(CONFIG_PATH);
    toml::table config_data = c.get_content_toml( );

    if ( argc > 1 ) {
        if ( io::args_parse(args [1], "-h", "--help") ) {
            io::a_help( );
            return 0;
            }
        else if ( io::args_parse(args [1], "-v", "--version") ) {
            printf("%s\n", PROJECT_VERSION);
            return 0;
            }
        else if ( io::args_parse(args [1], "-i", "-a") ) {
            io::a_about( );
            return 0;
            }
        else {
            io::error("Too less arguments or wrong arguments");
            return 1;
            }
        }

    game::Player player =
        game::login(config_data ["global"]["game_data"].as_string( )->get( ));

    return 0;
    }
