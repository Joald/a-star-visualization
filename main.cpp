#include "Window.h"
#include "Menu.h"
#include "FieldSelector.h"


void handleEscape(sf::Event event) {
    if ((event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) or
        event.type == sf::Event::Closed){
        window().close();
    }

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
    text.setFillColor(sf::Color::Black);
//    text.setOutlineColor(sf::Color::Black);
//    text.setOutlineThickness(1);
    text.setCharacterSize(static_cast<unsigned int>(scaleY(0.05)));
    std::clog << "Text Label character size is " << text.getCharacterSize() << std::endl;

    return text;
}

void drawFieldMatrix(const FieldMatrix& matrix, const FieldSelector& fieldSelector) {
    const float PADDING_X = 1.f / 8.f;
    const float PADDING_Y = 1.f / 6.f;
    const float X_SIZE = 1 - 2 * PADDING_X;
    const float Y_SIZE = 1 - 2 * PADDING_Y;

    static Displayer displayer = initDisplayer();
    static sf::Text text = initLabel();
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
            if (weight > width * height * 2) {
                text.setString("Inf");
            } else {
                text.setString(std::to_string(weight).substr(0, 3));
            }
//            std::clog << "Position (" << i << ", " << j << "), text is \"" << text.getString().toAnsiString() << "\".\n";
            text.setPosition(pos);
            window().draw(text);
        }
    }
}

void runVisualization(Algorithm algorithm) {
    FieldMatrix matrix = load_fields();
    FieldSelector fieldSelector(algorithm, matrix);
    while (window().isOpen()) {
        sf::Event event{};
        while (window().pollEvent(event)) {
            handleEscape(event);
            if (event.type == sf::Event::KeyPressed) {
                if (!fieldSelector.isFinished()) {
                    fieldSelector.visitNext();
                }
            }
        }
        window().clear(sf::Color::White);
        drawFieldMatrix(matrix, fieldSelector);
        window().display();
    }
}

int main() {
    window().create(sf::VideoMode(800, 600), "A* visualization");
    Menu* currentMenu;
    Algorithm algorithm = UNSPECIFIED;
    Menu menu{{"Dijkstra", [&] { algorithm = DIJKSTRA; }},
//              {"A*",       [&] { algorithm = A_STAR; }}
    };
    currentMenu = &menu;
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
    return 0;
}