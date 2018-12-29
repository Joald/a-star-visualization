#pragma once

#include <string>
#include <initializer_list>
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include <iostream>



class Menu {
    using MenuAction = std::function<void()>;
    using Option = std::pair<std::string, MenuAction>;
    std::vector<Option> options;
    std::vector<sf::Text> optionDisplays;
public:
    Menu(std::initializer_list<Option> list)
      : options(list), optionDisplays() {
        updateOptions();
    }

    void addOption(const Option& option) {
        options.push_back(option);
        updateOptions();
    }

    void updateOptions();

    void displayMenu();

    void handleEvent(sf::Event event);
};