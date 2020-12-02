#include "application.h"

std::string ParseMessage(std::string &message)
{
    rapidjson::Document document;
    document.Parse(message.c_str());
    rapidjson::StringBuffer buffer{};
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
    document.Accept(writer);
    return buffer.GetString();
}

bool IsNumber(std::string s)
{
    auto it = s.begin();
    while (it != s.end() && std::isdigit(*it))
    {
        ++it;
    }
    return !s.empty() && it == s.end();
}

void Application::HandleCommand(std::string command)
{
    std::stringstream ss(command);
    std::string current;
    std::vector<std::string> tokens;
    while (ss >> current)
    {
        tokens.emplace_back(current);
    }
    if (tokens.empty())
    {
        return;
    }
    auto clientCommand = tokens.front();

    if (clientCommand == "map" && tokens.size() == 2 && IsNumber(tokens.back()))
    {
        consoleHistory.append("Map successful\n");
        std::string str = client.Map(stoi(tokens.back())).data + "\n";
        ParseMessage(str);
        consoleHistory.append(str);
    }
    else if (clientCommand == "move" && tokens.size() == 4 && IsNumber(tokens[1]) && IsNumber(tokens[2]) &&
             IsNumber(tokens[3]))
    {
        consoleHistory.append("Move successful\n");
        std::string str = client.Move(stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3])).data + "\n";
        ParseMessage(str);
        consoleHistory.append(str);
    }
    else if (clientCommand == "upgrade")
    {
    }
    else if (clientCommand == "turn" && tokens.size() == 1)
    {
        consoleHistory.append("Turn successful\n");
        std::string str = client.Turn().data + "\n";
        ParseMessage(str);
        consoleHistory.append(str);
    }
    else if (clientCommand == "games" && tokens.size() == 1)
    {
        consoleHistory.append("Games successful\n");
        std::string str = client.Games().data + "\n";
        ParseMessage(str);
        consoleHistory.append(str);
    }
    else if (clientCommand == "login" && tokens.size() == 2)
    {
        consoleHistory.append("Login to " + client.GetSocket().remote_endpoint().address().to_string() + "\n");
        std::string str = client.Login(tokens.back()).data + "\n";
        ParseMessage(str);
        consoleHistory.append(str);
    }
    else if (clientCommand == "clear")
    {
        consoleHistory.clear();
    }
    else if (clientCommand == "logout" && tokens.size() == 1)
    {
        consoleHistory.append("Logout from " + client.GetSocket().remote_endpoint().address().to_string() + "\n");
    }
    else
    {
        consoleHistory.append("Unknown command " + command + "\n");
    }
}

void Application::PollEvent(sf::Event &event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && Application::focusedConsole && !touched[sf::Keyboard::Enter])
    {
        std::string command(console);
        HandleCommand(command);
        memset(console, 0, sizeof console);
        touched[sf::Keyboard::Enter] = true;
    }
    else
    {
        touched[sf::Keyboard::Enter] = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        window.close();
    }
    if (Application::focusedConsole)
    {
        return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        !touched[sf::Keyboard::Z])
    {
        render.backUp(states.front());
        touched[sf::Keyboard::Z] = true;
    }
    else
    {
        touched[sf::Keyboard::Z] = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !touched[sf::Keyboard::X])
    {
        render.hideInformationSwitch();
        touched[sf::Keyboard::X] = true;
    }
    else
    {
        touched[sf::Keyboard::X] = false;
    }
    if (event.type == sf::Event::Closed)
    {
        window.close();
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !render.isTarget() && !render.isPicked(states.front()))
    {
        render.canTarget = false;
        if (mouseX == -1 && mouseY == -1)
        {
            mouseX = sf::Mouse::getPosition().x, mouseY = sf::Mouse::getPosition().y;
            cameraX = camera.getCameraX(), cameraY = camera.getCameraY();
        }
        else
        {
            camera.setCameraX(cameraX + mouseX - sf::Mouse::getPosition().x);
            camera.setCameraY(cameraY + mouseY - sf::Mouse::getPosition().y);
        }
    }
    else
    {
        render.canTarget = true;
        mouseX = mouseY = cameraX = cameraY = -1;
    }
    if (event.type == sf::Event::MouseWheelMoved)
    {
        delta = event.mouseWheel.delta;
        states.front().Resize(delta * 10);
    }
}