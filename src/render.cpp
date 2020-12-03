#include "render.h"

void Render::setWindow(sf::RenderWindow *renderWindow)
{
    Render::window = renderWindow;
}

void Render::setCamera(Camera *pCamera)
{
    Render::camera = pCamera;
}

bool Render::isTarget() const
{
    return target != -1;
}

void Render::loadFont(const std::string &name, const std::string &path)
{
    sf::Font font_;
    if (!font_.loadFromFile(path))
    {
        exit(228);
    }
    Render::font[name] = font_;
}

void Render::draw(State &state)
{
    for (auto l : state.GetLines())
    {
        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(l[0].position.x - (float)camera->GetCameraX(),
                                                     l[0].position.y - (float)camera->GetCameraY())),
                             sf::Vertex(sf::Vector2f(l[1].position.x - (float)camera->GetCameraX(),
                                                     l[1].position.y - (float)camera->GetCameraY()))};
        window->draw(line, 2, sf::Lines);
    }
    int picked = -1;
    auto circles = state.GetCircles();
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    if (target == -1)
    {
        float dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < circles.size(); ++i)
        {
            float cur_dist = GraphState::GetDist(GraphState::Point(circles[i].getPosition().x + circles[i].getRadius(),
                                                             circles[i].getPosition().y + circles[i].getRadius(), 0),
                                           GraphState::Point((float)(mousePos.x + camera->GetCameraX()),
                                                             (float)(mousePos.y + camera->GetCameraY()), 0));
            if (cur_dist <= circles[i].getRadius() && cur_dist < dist)
            {
                dist = cur_dist;
                picked = i;
            }
        }
    }
    else
    {
        picked = target;
    }
    if (target == -1 && canTarget)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && picked != -1)
        {
            target = picked;
            backups.push(GraphState::Point(circles[picked].getPosition().x, circles[picked].getPosition().y, picked));
        }
    }
    else
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            target = -1;
        }
    }
    if (target != -1)
    {
        state.ChangePointLocation(picked, mousePos.x + camera->GetCameraX(), mousePos.y + camera->GetCameraY());
    }
    for (size_t i = 0; i < circles.size(); ++i)
    {
        sf::CircleShape circle(circles[i].getRadius());
        circle.setPosition(circles[i].getPosition().x - (float)camera->GetCameraX(),
                           circles[i].getPosition().y - (float)camera->GetCameraY());
        if (picked == i)
        {
            circle.setFillColor(sf::Color::Blue);
        }
        window->draw(circle);
    }
    for (auto [text, fontName] : state.GetTexts())
    {
        text.setPosition(text.getPosition().x - (float)camera->GetCameraX(),
                         text.getPosition().y - (float)camera->GetCameraY());
        text.setFont(font[fontName]);
        window->draw(text);
    }
}

bool Render::isPicked(State &state) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    for (const auto &circle : state.GetCircles())
    {
        float cur_dist = GraphState::GetDist(GraphState::Point(circle.getPosition().x + circle.getRadius(),
                                                         circle.getPosition().y + circle.getRadius(), 0),
                                       GraphState::Point((float)(mousePos.x + camera->GetCameraX()),
                                                         (float)(mousePos.y + camera->GetCameraY()), 0));
        if (cur_dist <= circle.getRadius())
        {
            return true;
        }
    }
    return false;
}

void Render::backUp(State &state)
{
    if (!backups.empty())
    {
        state.ChangePointLocation(backups.top().idx, backups.top().x, backups.top().y);
        backups.pop();
    }
}
