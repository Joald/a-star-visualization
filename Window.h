#pragma once

#include <SFML/Graphics.hpp>

sf::RenderWindow& window();

float scaleX(float);
float scaleY(float);

sf::Vector2f scale(sf::Vector2f);

sf::Font& arial();