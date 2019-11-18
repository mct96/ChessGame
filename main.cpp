#include <SFML/Graphics.hpp>

#include "include/view/board.hpp"



int main() {
    const auto width = 800, height = 600;

    // sf::View boardView{sf::FloatRect{{0, 0}, {8 * 200, 8 * 200}}};
    // boardView.setViewport({0, 0, .75, 1});

    sf::RenderWindow window{sf::VideoMode{width, height}, "Awesome Chess"};
    // window.setView(boardView);
    // window.setFramerateLimit(30);

    ch::CBoardView gameView{
        {0, 0, 800, 600},
        "assets/sprite.png",
        "assets/board.png"
    };

    sf::CircleShape d{13};
    d.setPosition({700, 500});
    d.setFillColor({255, 0, 0});
    sf::Time time;
    while (window.isOpen()) {


        sf::Event event{};
        if (window.pollEvent(event)) {
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                gameView.onClick(event);
                break;

            default:
                ;
            }
        }

        window.clear(sf::Color{255, 255, 255});

        if (window.hasFocus())
            window.draw(gameView);

        window.display();

    }



    return 0;
}