cmake_minimum_required(VERSION 3.12)

project(Chess)

set(SRC
    src/game.cpp
    src/game_types.cpp
    src/parser.cpp
)

set(INCLUDE
    src/game.hpp
    src/game_types.hpp
    src/parser.hpp
)

enable_testing()

add_executable(game_chess ${SRC})
target_compile_definitions(game_chess
    PRIVATE $<$<CONFIG:release>:NDEBUB>
)
target_include_directories(game_chess
    PUBLIC ${INCLUDE}
)

set_target_properties(game_chess PROPERTIES CXX_STANDARD 20)
set_target_properties(game_chess PROPERTIES CXX_STANDARD_REQUIRED YES)
set_target_properties(game_chess PROPERTIES CXX_EXTENSIONS NO)
