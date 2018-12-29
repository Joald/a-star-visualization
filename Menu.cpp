#include "Menu.h"
#include "Window.h"

namespace {

const float FONT_SIZE = 0.05;

} // namespace

void Menu::updateOptions() {
    if (options.size() > 9) {
        throw std::invalid_argument("Can't have more than 9 options!");
    }
    optionDisplays.clear();
    int i = 0;
    for (auto&[textString, action] : options) {
        auto& text = optionDisplays.emplace_back();
        text.setFont(arial());
        text.setString(std::to_string(i + 1) + ". " + textString);
        text.setCharacterSize(static_cast<unsigned int>(scaleY(FONT_SIZE)));
        text.setPosition(scale(sf::Vector2f(1.f / 5.f, 1.f / 5.f + i * 3.f * FONT_SIZE)));
        std::clog << "Text " << i << " position is (" << text.getPosition().x << ", " << text.getPosition().y << ")" << std::endl;
        std::clog << "Text " << i << " character size is " << text.getCharacterSize() << std::endl;
        text.setFillColor(sf::Color::Black);
        i++;
    }
}



void Menu::displayMenu() {
    for (const auto& optionDisplay : optionDisplays) {
        window().draw(optionDisplay);
    }
}

void Menu::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyReleased) {
        for (int i = 0; i < options.size(); ++i) {
            int key = sf::Keyboard::Num0 + i + 1;
            if (event.key.code == key) {
                options[i].second();
            }
        }
    }
}
