#include <iostream>
#include "Window.h"

sf::RenderWindow& window() {
    static sf::RenderWindow ret;
    return ret;
}

float scaleX(float x) {
    if (x < 0 or 1 < x) {
        throw std::invalid_argument("Window positions must be between 0 and 1");
    }
    return x * window().getSize().x;
}

float scaleY(float y) {
    if (y < 0 or 1 < y) {
        throw std::invalid_argument("Window positions must be between 0 and 1");
    }
    return y * window().getSize().y;
}

sf::Vector2f scale(sf::Vector2f toScale) {
    return sf::Vector2f(scaleX(toScale.x), scaleY(toScale.y));
}

sf::Font& arial() {
    static sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Cannot load arial\n";
        exit(-1);
    }
    return font;
}

