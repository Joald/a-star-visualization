#include <cassert>
#include "Window.h"
#include "Menu.h"
#include "FieldSelector.h"


void handleEscape(sf::Event event) {
    if ((event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) or
        event.type == sf::Event::Closed) {
        window().close();
    }
}

void showMouseCoords() {
    sf::Text text;
    text.setFont(arial());
    auto pos = sf::Mouse::getPosition(window());
    text.setString(
      std::to_string(pos.x) + ", " +
      std::to_string(pos.y)
    );
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(100);
    window().draw(text);
}

using Displayer = std::map<Field, sf::RectangleShape>;

Displayer initDisplayer() {
    Displayer map;
    map[Field::VISITED].setFillColor(sf::Color::Cyan);
    map[Field::TARGET].setFillColor(sf::Color::Blue);
    map[Field::ENQUEUED].setFillColor(sf::Color::Yellow);
    map[Field::START].setFillColor(sf::Color::Green);
    map[Field::BLOCKING].setFillColor(sf::Color::Black);
    map[Field::NON_BLOCKING].setFillColor(sf::Color::White);
    map[Field::PATH].setFillColor(sf::Color::Red);
    return map;
}

sf::Text initLabel() {
    sf::Text text;
    text.setFont(arial());
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
    std::clog << "Text Label character size is " << text.getCharacterSize() << std::endl;

    return text;
}

void drawFieldMatrix(const FieldMatrix& matrix, const FieldSelector& fieldSelector) {
    float PADDING_X = 1.f / 8.f;
    float PADDING_Y = 1.f / 6.f;
    float X_SIZE = 1 - 2 * PADDING_X;
    float Y_SIZE = 1 - 2 * PADDING_Y;

    static Displayer displayer = initDisplayer();
    static sf::Text text = initLabel();
    text.setCharacterSize(static_cast<unsigned int>(scaleY(0.05)));
    float width = matrix.size(), height = matrix.front().size();
    float tileX = X_SIZE / width, tileY = Y_SIZE / height;
    for (auto&[_, value] : displayer) {
        value.setSize(scale(sf::Vector2f(tileX, tileY)));
        value.setOutlineThickness(1);
        value.setOutlineColor(sf::Color::Black);
    }
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            auto& rect = displayer[matrix[i][j]];
            auto pos = scale(sf::Vector2f(PADDING_X + i * tileX, PADDING_Y + j * tileY));
            rect.setPosition(pos);

            window().draw(rect);
            auto weight = fieldSelector.getWeight(i, j);
            if (weight > width * height * 200) {
                text.setString("Inf");
            } else {
                text.setString(std::to_string(weight).substr(0, std::log10(weight) + 3));
            }
            text.setPosition(pos);
            window().draw(text);
        }
    }
}

const char* getAlgorithmName(Algorithm algorithm) {
    switch (algorithm) {
        case UNSPECIFIED:
            throw std::invalid_argument("XD!");
        case DIJKSTRA:
            return "Dijkstra's algorithm";
        case A_STAR:
            return "A* with Euclidean distance";
        case A_STAR_SQUARE_DISTANCE:
            return "A* with squared Euclidean distance";
        case A_STAR_MANHATTAN:
            return "A* with the Manhattan metric";
    }
}

void drawAlgorithmName(Algorithm algorithm) {
    static sf::Text text = initLabel();
    const float SIZE = 0.1;
    text.setCharacterSize(scaleY(SIZE));
    text.setOutlineThickness(2);
    text.setString(getAlgorithmName(algorithm));
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(scaleX(0.5), scaleY(SIZE)));
    window().draw(text);
}

void runVisualization(Algorithm algorithm) {
    FieldMatrix matrix = load_fields();
    FieldSelector fieldSelector(algorithm, matrix);
    while (window().isOpen()) {
        sf::Event event{};
        while (window().pollEvent(event)) {
            handleEscape(event);
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    return;
                }
                if (!fieldSelector.isFinished()) {
                    fieldSelector.visitNext();
                }
            }
        }
        window().clear(sf::Color::White);
        drawFieldMatrix(matrix, fieldSelector);
        drawAlgorithmName(algorithm);
        window().display();
    }
}

int main() {
    window().create(sf::VideoMode::getDesktopMode(), "A* visualization");
    Menu* currentMenu;
    Algorithm algorithm = UNSPECIFIED;
    Menu menu{{"Dijkstra",                           [&] { algorithm = DIJKSTRA; }},
              {"A* with Euclidean distance",         [&] { algorithm = A_STAR; }},
              {"A* with squared Euclidean distance", [&] { algorithm = A_STAR_SQUARE_DISTANCE; }},
              {"A* with Manhattan distance",         [&] { algorithm = A_STAR_MANHATTAN; }},
    };
    currentMenu = &menu;
    while (window().isOpen()) {
        while (window().isOpen() && algorithm == UNSPECIFIED) {
            sf::Event event{};
            while (window().pollEvent(event)) {
                currentMenu->handleEvent(event);
                handleEscape(event);
            }
            window().clear(sf::Color::White);
            currentMenu->displayMenu();
            window().display();
        }
        if (window().isOpen()) {
            runVisualization(algorithm);
        }
        algorithm = UNSPECIFIED;
    }
    return 0;
}