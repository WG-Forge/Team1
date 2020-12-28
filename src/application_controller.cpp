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
    if (!s.empty() && s[0] == '-')
    {
        s.erase(begin(s));
    }
    auto it = s.begin();
    while (it != s.end() && std::isdigit(*it))
    {
        ++it;
    }
    return !s.empty() && it == s.end();
}

void Application::HandleCommand(std::string command, bool display, int clientIndex)
{
    if (display)
    {
        consoleHistory.append(command + "\n");
    }
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
    std::string result;
    if (clientCommand == "map" && tokens.size() == 2 && IsNumber(tokens.back()))
    {
        clientMutexes[clientIndex].lock();
        auto clientResult = clients[clientIndex].Map(stoi(tokens.back())).data;
        clientMutexes[clientIndex].unlock();
        if (tokens.back() == "0")
        {
            mapMutex.lock();
            map = RailGraph::ParseMap0FromJson(clientResult);
            mapMutex.unlock();
        }
        else if (tokens.back() == "1")
        {
            mapMutex.lock();
            map.SetPosts(RailGraph::ParseMap1FromJson(clientResult));
            auto posts = map.GetPosts();
            mapMutex.unlock();
            auto trains = RailGraph::ParseTrainsFromJson(clientResult);
            std::vector<std::string> players;
            for (const auto &i : posts)
            {
                if (i.postInfo.index() == RailGraph::Graph::Post::CityIndex)
                {
                    auto info = std::get<RailGraph::Graph::CityInfo>(i.postInfo).playerIdx;
                    if (info.has_value())
                    {
                        players.emplace_back(info.value());
                    }
                }
            }
            auto ratings = RailGraph::ParseRatingFromJson(clientResult, players);
            mapMutex.lock();
            map.SetTrains(trains);
            map.SetRatings(ratings);
            mapMutex.unlock();
            stateMutex.lock();
            state.UpdateTrains(trains);
            state.UpdateRatings(ratings, window.getSize().x);
            stateMutex.unlock();
        }
        else if (tokens.back() == "10")
        {
            mapMutex.lock();
            map.SetPointsCoordinates(RailGraph::ParseMap10FromJson(clientResult));
            mapMutex.unlock();
        }
        result = clientResult + "\n";
    }
    else if (clientCommand == "move" && tokens.size() == 4 && IsNumber(tokens[1]) && IsNumber(tokens[2]) &&
             IsNumber(tokens[3]))
    {
        clientMutexes[clientIndex].lock();
        auto clientResult = clients[clientIndex].Move(stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3])).data;
        clientMutexes[clientIndex].unlock();
        result = clientResult + "\n";
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
                    std::cout << "Wrong upgrade arguments\n";
                }
            }
            clientMutexes[clientIndex].lock();
            auto clientResult = clients[clientIndex].Upgrade(posts, trains).data;
            clientMutexes[clientIndex].unlock();
            result = clientResult + "\n";
        }
        else
        {
            result = "Unknown command '" + command + "'\n";
        }
    }
    else if (clientCommand == "turn" && tokens.size() == 1)
    {
        clientMutexes[clientIndex].lock();
        auto clientResult = clients[clientIndex].Turn().data;
        clientMutexes[clientIndex].unlock();
        result = clientResult + "\n";
    }
    else if (clientCommand == "games" && tokens.size() == 1)
    {
        clientMutexes[clientIndex].lock();
        auto clientResult = clients[clientIndex].Games().data;
        clientMutexes[clientIndex].unlock();
        result = clientResult + "\n";
    }
    else if (clientCommand == "login" && tokens.size() == 2)
    {
        clientMutexes[clientIndex].lock();
        result = "Login to " + clients[clientIndex].GetSocket().remote_endpoint().address().to_string() + "\n";
        auto clientResult = clients[clientIndex].Login(tokens.back()).data;
        clientMutexes[clientIndex].unlock();
        auto parsingResult = RailGraph::ParseLoginFromJson(clientResult);
        brain.SetHomeIdx(std::get<0>(parsingResult)), brain.SetHomePostIdx(std::get<1>(parsingResult));
        brain.SetIdx(std::get<2>(parsingResult));
        std::vector<int> trainIdx;
        for (const auto &i : RailGraph::ParseTrainsFromJson(clientResult))
        {
            trainIdx.emplace_back(i.info.idx);
        }
        brain.SetTrains(trainIdx);
        result += clientResult + "\n";
    }
    else if (clientCommand == "login" && tokens.size() == 6)
    {
        clientMutexes[clientIndex].lock();
        result = "Login to " + clients[clientIndex].GetSocket().remote_endpoint().address().to_string() + "\n";
        auto clientResult = clients[clientIndex]
                                .Login(tokens[1], tokens[2], tokens[3], std::stoi(tokens[4]), std::stoi(tokens[5]))
                                .data;
        clientMutexes[clientIndex].unlock();
        auto parsingResult = RailGraph::ParseLoginFromJson(clientResult);
        brain.SetHomeIdx(std::get<0>(parsingResult)), brain.SetHomePostIdx(std::get<1>(parsingResult));
        brain.SetIdx(std::get<2>(parsingResult));
        std::vector<int> trainIdx;
        for (const auto &i : RailGraph::ParseTrainsFromJson(clientResult))
        {
            trainIdx.emplace_back(i.info.idx);
        }
        brain.SetTrains(trainIdx);
        result += clientResult + "\n";
    }
    else if (clientCommand == "player")
    {
        clientMutexes[clientIndex].lock();
        result = clients[clientIndex].Player().data;
        clientMutexes[clientIndex].unlock();
    }
    else if (clientCommand == "clear")
    {
        consoleHistory.clear();
    }
    else if (clientCommand == "logout" && tokens.size() == 1)
    {
        result = "Logout from " + clients[clientIndex].GetSocket().remote_endpoint().address().to_string() + "\n";
    }
    else if (clientCommand == "point" && tokens.size() == 2 && IsNumber(tokens[1]))
    {
        mapMutex.lock();
        result = map.GetPointInfo(stoi(tokens[1])) + "\n";
        mapMutex.unlock();
    }
    else if (clientCommand == "train" && tokens.size() == 2 && IsNumber(tokens[1]))
    {
        mapMutex.lock();
        result = map.GetTrainInfo(stoi(tokens[1])) + "\n";
        mapMutex.unlock();
    }
    else if (clientCommand == "pause")
    {
        pause = true;
    }
    else if (clientCommand == "unpause")
    {
        pause = false;
    }
    else
    {
        result = "Unknown command '" + command + "'\n";
    }
    if (display)
    {
        consoleHistory += result;
    }
}

void Application::PollEvent(sf::Event &event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && Application::focusedConsole && !touched[sf::Keyboard::Enter])
    {
        std::string command(console);
        HandleCommand(command, true, 2);
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
        render.BackUp(state);
        touched[sf::Keyboard::Z] = true;
    }
    else
    {
        touched[sf::Keyboard::Z] = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !touched[sf::Keyboard::X])
    {
        state.ResetPointInformation();
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !render.IsTarget() && !render.IsPicked(state))
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
        state.Resize(delta * 10);
    }
}