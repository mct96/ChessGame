cmake_minimum_required(VERSION 3.12)

project(Chess)

set(MODEL_SRC
    ./src/model/color.cpp
    ./src/model/game.cpp
    ./src/model/history.cpp
    ./src/model/location.cpp
    ./src/model/move_path.cpp
    ./src/model/path.cpp
    ./src/model/piece.cpp
    ./src/model/type.cpp
    ./src/model/specialized_pieces.cpp
)

set(
    CONTROLLER_SRC
    ./src/controller/boardController.cpp
)

set(
    VIEW_SRC
    ./src/view/board.cpp
)

add_executable(chess main.cpp ${MODEL_SRC} ${CONTROLLER_SRC} ${VIEW_SRC})
set_target_properties(chess PROPERTIES CMAKE_CXX_STANDARD 20)
set_target_properties(chess PROPERTIES CMAKE_CXX_STANDARD_REQUIRED YES)
set_target_properties(chess PROPERTIES CMAKE_CXX_EXTENSIONS NO)

target_link_libraries(chess PRIVATE sfml-graphics sfml-window sfml-system)