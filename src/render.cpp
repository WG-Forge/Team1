#include "render.h"

void Render::setWindow(sf::RenderWindow *renderWindow) {
    Render::window = renderWindow;
}

void Render::setCamera(Camera *pCamera) {
    Render::camera = pCamera;
}

void Render::draw(State &state) {
    for (auto l : state.lines) {
        sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(l[0].position.x - (float)camera->getCameraX(),
                                    l[0].position.y - (float)camera->getCameraY())),
                sf::Vertex(sf::Vector2f(l[1].position.x - (float)camera->getCameraX(),
                                    l[1].position.y - (float)camera->getCameraY()))
        };
        window->draw(line, 2, sf::Lines);
    }
    for (auto circle : state.circles) {
        circle.setPosition(circle.getPosition().x - (float)camera->getCameraX(),
                           circle.getPosition().y - (float)camera->getCameraY());
        window->draw(circle);
    }
    for (auto [text, fontPath]: state.texts) {
        sf::Font font;
        if (!font.loadFromFile(fontPath)) {
            exit(228);
        }
        text.setFont(font);
        window->draw(text);
    }
}
