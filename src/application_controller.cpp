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
    if (!s.empty() && s[0] == '-') {
        s.erase(begin(s));
    }
    auto it = s.begin();
    while (it != s.end() && std::isdigit(*it))
    {
        ++it;
    }
    return !s.empty() && it == s.end();
}

void Application::HandleCommand(std::string command)
{
    consoleHistory.append(command + "\n");
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
        std::string result = client.Map(stoi(tokens.back())).data;
        consoleHistory.append(result + '\n');
        if (tokens.back() == "0")
        {
            while (!states.empty())
            {
                states.pop();
            }
            map = RailGraph::ParseMap0FromJson(result);
        }
        else if (tokens.back() == "1")
        {
            map.SetPosts(RailGraph::ParseMap1FromJson(result));
            auto trains = RailGraph::ParseTrainsFromJson(result);
            map.SetTrains(trains);
            if (!states.empty())
            {
                states.front().UpdateTrains(trains);
            }
        }
        else if (tokens.back() == "10")
        {
            map.SetPointsCoordinates(RailGraph::ParseMap10FromJson(result));
        }
    }
    else if (clientCommand == "move" && tokens.size() == 4 && IsNumber(tokens[1]) && IsNumber(tokens[2]) &&
             IsNumber(tokens[3]))
    {
        consoleHistory.append("Move successful\n");
        std::string str = client.Move(stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3])).data;
        consoleHistory.append(str + '\n');
    }
    else if (clientCommand == "upgrade" && tokens.size() == 2)
    {
        std::string parseStr = tokens.back();
        if (parseStr.front() == '{' && parseStr.back() == '}')
        {
            parseStr.back() = ',';
            std::vector<size_t> posts, trains;
            std::string number{};
            bool flag = false;
            for (size_t i = 1; i < parseStr.size(); ++i)
            {
                if (parseStr[i] == ',' || parseStr[i] == '|')
                {
                    if (!flag && !number.empty())
                    {
                        posts.emplace_back(std::stoi(number));
                    }
                    else if (flag && !number.empty())
                    {
                        trains.emplace_back(std::stoi(number));
                    }
                    if (parseStr[i] == '|')
                    {
                        flag = true;
                    }
                    number.clear();
                }
                else if (parseStr[i] >= '0' && parseStr[i] <= '9')
                {
                    number += parseStr[i];
                }
                else
                {
                    consoleHistory.append("Wrong arguments '" + command + "'\n");
                    return;
                }
            }
            consoleHistory.append("Upgrade successful\n");
            std::string result = client.Upgrade(posts, trains).data;
            consoleHistory.append(result + '\n');
        }
        else
        {
            consoleHistory.append("Unknown command '" + command + "'\n");
        }
    }
    else if (clientCommand == "turn" && tokens.size() == 1)
    {
        consoleHistory.append("Turn successful\n");
        std::string str = client.Turn().data;
        consoleHistory.append(str + '\n');
    }
    else if (clientCommand == "games" && tokens.size() == 1)
    {
        consoleHistory.append("Games successful\n");
        std::string str = client.Games().data;
        consoleHistory.append(str + '\n');
    }
    else if (clientCommand == "login" && tokens.size() == 2)
    {
        consoleHistory.append("Login to " + client.GetSocket().remote_endpoint().address().to_string() + "\n");
        std::string result = client.Login(tokens.back()).data;
        auto parsingResult = RailGraph::ParseLoginFromJson(result);
//        homeIdx = parsingResult.first, homePostIdx = parsingResult.second;
        consoleHistory.append(result + '\n');
    }
    else if (clientCommand == "clear")
    {
        consoleHistory.clear();
    }
    else if (clientCommand == "logout" && tokens.size() == 1)
    {
        consoleHistory.append("Logout from " + client.GetSocket().remote_endpoint().address().to_string() + "\n");
    }
    else if (clientCommand == "point" && tokens.size() == 2 && IsNumber(tokens[1]))
    {
        std::string str = map.GetPointInfo(stoi(tokens[1]));
        consoleHistory.append(str + '\n');
    }
    else if (clientCommand == "train" && tokens.size() == 2 && IsNumber(tokens[1]))
    {
        std::string str = map.GetTrainInfo(stoi(tokens[1]));
        consoleHistory.append(str + '\n');
    }
    else if (clientCommand == "adjacent" && tokens.size() == 2 && IsNumber(tokens[1]))
    {
        std::string result = "[";
        for (const auto &line : map.GetLines())
        {
            if (line.points.first == stoi(tokens[1]) || line.points.second == stoi(tokens[1]))
            {
                result += std::to_string(line.idx) + ",\n";
            }
        }
        consoleHistory.append(result + "]\n");
    }
    else
    {
        consoleHistory.append("Unknown command '" + command + "'\n");
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
        render.BackUp(states.front());
        touched[sf::Keyboard::Z] = true;
    }
    else
    {
        touched[sf::Keyboard::Z] = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !touched[sf::Keyboard::X])
    {
        states.front().ResetPointInformation();
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !render.IsTarget() && !render.IsPicked(states.front()))
    {
        render.canTarget = false;
        if (mouseX == -1 && mouseY == -1)
        {
            mouseX = sf::Mouse::getPosition().x, mouseY = sf::Mouse::getPosition().y;
            cameraX = camera.GetCameraX(), cameraY = camera.GetCameraY();
        }
        else
        {
            camera.SetCameraX(cameraX + mouseX - sf::Mouse::getPosition().x);
            camera.SetCameraY(cameraY + mouseY - sf::Mouse::getPosition().y);
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