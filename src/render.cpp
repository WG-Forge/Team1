#include "render.h"

void Render::setWindow(sf::RenderWindow *renderWindow) {
    Render::window = renderWindow;
}

void Render::setCamera(Camera *pCamera) {
    Render::camera = pCamera;
}

bool Render::isTarget() const {
    return target != -1;
}

void Render::loadFont(const std::string& name, const std::string& path) {
    sf::Font font_;
    if (!font_.loadFromFile(path)) {
        exit(228);
    }
    Render::font[name] = font_;
}

void Render::draw(State &state) {
    for (auto l : state.GetLines()) {
        sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(l[0].position.x - (float)camera->getCameraX(),
                                        l[0].position.y - (float)camera->getCameraY())),
                sf::Vertex(sf::Vector2f(l[1].position.x - (float)camera->getCameraX(),
                                        l[1].position.y - (float)camera->getCameraY()))
        };
        window->draw(line, 2, sf::Lines);
    }
    int picked = -1;
    auto circles = state.GetCircles();
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    if (target == -1) {
        float dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < circles.size(); ++i) {
            float cur_dist = State::GetLen(
                    GraphState::Point(circles[i].getPosition().x + circles[i].getRadius(),
                                      circles[i].getPosition().y + circles[i].getRadius(), 0),
                    GraphState::Point((float)(mousePos.x + camera->getCameraX()),
                                      (float)(mousePos.y + camera->getCameraY()), 0));
            if (cur_dist <= circles[i].getRadius() && cur_dist < dist) {
                dist = cur_dist;
                picked = i;
            }
        }
    } else {
        picked = target;
    }
    if (target == -1) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && picked != -1) {
            target = picked;
        }
    } else {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            target = -1;
        }
    }
    if (target != -1) {
        state.changePointLocation(picked, mousePos.x + camera->getCameraX(),
                mousePos.y + camera->getCameraY());
    }
    for (size_t i = 0; i < circles.size(); ++i) {
        sf::CircleShape circle(circles[i].getRadius());
        circle.setPosition(circles[i].getPosition().x - (float)camera->getCameraX(),
                           circles[i].getPosition().y - (float)camera->getCameraY());
        if (picked == i) {
            circle.setFillColor(sf::Color::Blue);
        }
        window->draw(circle);
    }
    if (!hide) {
        for (auto [text, fontName]: state.GetInformation()) {
            text.setPosition(text.getPosition().x - (float)camera->getCameraX(),
                             text.getPosition().y - (float)camera->getCameraY());
            text.setFont(font[fontName]);
            window->draw(text);
        }
    }
    for (auto [text, fontName]: state.GetTexts()) {
        text.setFont(font[fontName]);
        window->draw(text);
    }
}
