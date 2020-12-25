#include "render.h"

void Render::SetWindow(sf::RenderWindow *renderWindow)
{
    Render::window = renderWindow;
}
void Render::SetCamera(Camera *pCamera)
{
    Render::camera = pCamera;
}
bool Render::IsTarget() const
{
    return target != -1;
}
void Render::LoadFont(const std::string &name, const std::string &path)
{
    sf::Font font_;
    if (!font_.loadFromFile(path))
    {
        exit(228);
    }
    Render::font[name] = font_;
}
void Render::LoadTexture(const std::string &name, const std::string &path)
{
    sf::Texture texture_;
    if (!texture_.loadFromFile(path))
    {
        exit(228);
    }
    Render::texture[name] = texture_;
}
void Render::Draw(State &state)
{
    for (auto l : state.GetLines())
    {
        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(l[0].position.x - (float)camera->GetCameraX(),
                                                     l[0].position.y - (float)camera->GetCameraY())),
                             sf::Vertex(sf::Vector2f(l[1].position.x - (float)camera->GetCameraX(),
                                                     l[1].position.y - (float)camera->GetCameraY()))};
        line[0].color = sf::Color(82, 73, 73);
        line[1].color = sf::Color(82, 73, 73);
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
            float cur_dist = RailGraph::Graph::GetDist(
                RailGraph::Graph::Point(circles[i].first.getPosition().x + circles[i].first.getRadius(),
                                        circles[i].first.getPosition().y + circles[i].first.getRadius(), 0, 0),
                RailGraph::Graph::Point(mousePos.x + camera->GetCameraX(), mousePos.y + camera->GetCameraY(), 0, 0));
            if (cur_dist <= circles[i].first.getRadius() && cur_dist < dist)
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
            RailGraph::Graph::Point oldPoint = RailGraph::Graph::Point(0, 0, circles[picked].first.getPosition().x,
                                                                       circles[picked].first.getPosition().y);
            oldPoint.idx = picked;
            backups.push(oldPoint);
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
        sf::CircleShape circle(circles[i].first.getRadius());
        circle.setPosition(circles[i].first.getPosition().x - (float)camera->GetCameraX(),
                           circles[i].first.getPosition().y - (float)camera->GetCameraY());
        if (picked == i)
        {
            circle.setFillColor(sf::Color::Blue);
        }
        if (texture.count(circles[i].second))
        {
            SfmlTool::SetTextureOnCircleShape(circle, &texture[circles[i].second]);
            circle.setOutlineThickness(1);
            circle.setOutlineColor(sf::Color(82, 73, 73));
        }
        window->draw(circle);
    }
    for (auto [text, fontName] : state.GetNonStaticTexts())
    {
        text.setPosition(text.getPosition().x - (float)camera->GetCameraX(),
                         text.getPosition().y - (float)camera->GetCameraY());
        text.setFont(font[fontName]);
        window->draw(text);
    }
    for (auto [text, fontName] : state.GetStaticTexts())
    {
        text.setPosition(text.getPosition().x, text.getPosition().y);
        text.setFont(font[fontName]);
        window->draw(text);
    }
    for (auto [circle, textureName] : state.GetTrains())
    {
        circle.setPosition(circle.getPosition().x - (float)camera->GetCameraX(),
                           circle.getPosition().y - (float)camera->GetCameraY());
        if (texture.count(textureName))
        {
            SfmlTool::SetTextureOnCircleShape(circle, &texture[textureName]);
            circle.setOutlineThickness(1);
            circle.setOutlineColor(sf::Color(82, 73, 73));
        }
        window->draw(circle);
    }
}

bool Render::IsPicked(State &state) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    for (const auto &circle : state.GetCircles())
    {
        float cur_dist = RailGraph::Graph::GetDist(
            RailGraph::Graph::Point(circle.first.getPosition().x + circle.first.getRadius(),
                                    circle.first.getPosition().y + circle.first.getRadius(), 0, 0),
            RailGraph::Graph::Point(mousePos.x + camera->GetCameraX(), mousePos.y + camera->GetCameraY(), 0, 0));
        if (cur_dist <= circle.first.getRadius())
        {
            return true;
        }
    }
    return false;
}

void Render::BackUp(State &state)
{
    if (!backups.empty())
    {
        state.ChangePointLocation(backups.top().idx, backups.top().renderX, backups.top().renderY);
        backups.pop();
    }
}

int Render::GetPicked(State &state) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    for (const auto &circle : state.GetCircles())
    {
        float cur_dist = RailGraph::Graph::GetDist(
            RailGraph::Graph::Point(circle.first.getPosition().x + circle.first.getRadius(),
                                    circle.first.getPosition().y + circle.first.getRadius(), 0, 0),
            RailGraph::Graph::Point(mousePos.x + camera->GetCameraX(), mousePos.y + camera->GetCameraY(), 0, 0));
        if (cur_dist <= circle.first.getRadius())
        {
            return state.GetCircleIndex(circle.first);
        }
    }
    return -1;
}