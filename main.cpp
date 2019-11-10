#include <SFML/Graphics.hpp>

#include "include/view/board.hpp"



int main() {
    const auto width = 600, height = 600;
    int x = 0, y = 0;



    sf::RenderWindow window{sf::VideoMode{width, height}, "Awesome Chess"};

    while (window.isOpen()) {
        sf::Texture tex{};
        tex.loadFromFile("assets/pieces.png",
            sf::IntRect{{205 * x + 25, 200 * y + 25}, {180, 170}});

        sf::Sprite piece{tex};
        piece.setColor(sf::Color{255, 0, 0});

        sf::Event event{};
        if (window.pollEvent(event)) {
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Up)
                    y = std::min(y + 1, 1);
                if (event.key.code == sf::Keyboard::Down)
                    y = std::max(y - 1, 0);
                if (event.key.code == sf::Keyboard::Right)
                    x = std::min(x + 1, 7);
                if (event.key.code == sf::Keyboard::Left)
                    x = std::max(x - 1, 0);

                break;

            default:
                ;
            }
        }

        window.clear(sf::Color{255, 255, 255});
        window.draw(piece);
        window.display();
    }



    return 0;
}