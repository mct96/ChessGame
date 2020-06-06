cmake_minimum_required(VERSION 3.12)

project(Chess)

set(MODEL_SRC
    ./src/model/game.cpp
)

set(MODEL_INC
    ./include/model/game.hpp
)

set(TEST_SRC
    ./test/test_logic.cpp
)

enable_testing()

add_executable(game_chess ${MODEL_SRC} ${TEST_SRC})
target_compile_definitions(game_chess
    PRIVATE $<$<CONFIG:release>:NDEBUB>
)
target_include_directories(game_chess
    PUBLIC ${MODEL_INC}
)

set_target_properties(game_chess PROPERTIES CXX_STANDARD 20)
set_target_properties(game_chess PROPERTIES CXX_STANDARD_REQUIRED YES)
set_target_properties(game_chess PROPERTIES CXX_EXTENSIONS NO)
