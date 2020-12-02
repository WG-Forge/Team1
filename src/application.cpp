#include "application.h"
#include "libs/imgui-sfml/imgui-SFML.h"
#include "libs/imgui/imgui.h"

#include <utility>

Application::Application(Config config, io_service &service) : config(std::move(config)), client(service)
{
}

std::string ParseMessage(std::string &message)
{
    rapidjson::Document document;
    document.Parse(message.c_str());
    rapidjson::StringBuffer buffer{};
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
    document.Accept(writer);
    return buffer.GetString();
}

int Application::run()
{
    init();

    ImGui::SFML::Init(window);
    sf::Color bgColor;
    char console[255] = "";
    sf::Clock deltaClock;

    sf::Clock dtTimer;

    int mouseX = -1, mouseY = -1, cameraX = -1, cameraY = -1, delta;
    while (window.isOpen())
    {
        if (dtTimer.getElapsedTime().asMilliseconds() * config.fps <= 1000)
        {
            continue;
        }
        dtTimer.restart();
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && Application::focusedConsole &&
                !touched[sf::Keyboard::Enter])
            {
                std::string command(console);
                PollCommand(command);
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
                continue;
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

        if (!states.empty())
        {
            window.clear();
            render.draw(states.front());
        }
        else
        {
            mouseX = mouseY = cameraX = cameraY = -1;
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            delta = event.mouseWheel.delta;
            states.front().Resize(delta * 10);
        }
        if (!states.empty())
        {
            window.clear();
            render.draw(states.front());
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Console");
        ImGui::BeginChild("Output");
        ImGui::InputText("input", console, sizeof(console));
        char *history = const_cast<char *>(consoleHistory.c_str());
        ImGui::InputTextMultiline("output", history, consoleHistory.size(), ImVec2(-1, ImGui::GetWindowContentRegionMax().y - 100));
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            Application::focusedConsole = true;
        }
        else
        {
            Application::focusedConsole = false;
        }
        ImGui::EndChild();
        ImGui::End();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;
}

void Application::init()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    if (config.isFullscreen)
    {
        window.create(sf::VideoMode::getDesktopMode(), "Rail-simulator", sf::Style::Fullscreen, settings);
    }
    else
    {
        sf::VideoMode winMode(config.windowX, config.windowY);
        window.create(winMode, "Rail-simulator", sf::Style::Close, settings);
    }

    if (config.hideCursor)
    {
        window.setMouseCursorVisible(false);
    }

    render.setCamera(&camera);
    render.setWindow(&window);
    render.loadFont("8-bit-pusab", "../resourses/8-bit-pusab.ttf");

    PollCommand("login " + config.teamName);

    // test
    {
        std::ifstream fin(config.pathToJson);
        std::stringstream ss;
        ss << fin.rdbuf();
        RailGraph::GraphParser graphParser(ss.str());
        GraphState graphState = CreateReingoldGraphStateFromGraph(graphParser.GetGraph());
        std::vector<std::pair<sf::Text, std::string>> texts;
        State state(graphState, texts);
        state.AddText(window.getSize().x / 2.f - 150, window.getSize().y - 50.f, "Press esc to exit program",
                      "8-bit-pusab", 10, sf::Color::White);
        state.AddText(window.getSize().x - 270.f, 10.f, "left click on the point to move", "8-bit-pusab", 10,
                      sf::Color::White);
        state.AddText(window.getSize().x - 270.f, 30.f, "right click to release", "8-bit-pusab", 10, sf::Color::White);
        state.AddText(window.getSize().x - 220.f, 60.f, "Press 'X' to show ", "8-bit-pusab", 14, sf::Color::White);
        state.AddText(window.getSize().x - 220.f, 80.f, "vertex numbering", "8-bit-pusab", 13, sf::Color::White);
        state.AddText(window.getSize().x - 190.f, 110.f, "Scroll wheel", "8-bit-pusab", 14, sf::Color::White);
        state.AddText(window.getSize().x - 170.f, 130.f, "to resize", "8-bit-pusab", 14, sf::Color::White);
        states.push(state);
    }

    auto center = states.front().GetCenter();
    camera.setCameraX((int)(center.x - (float)window.getSize().x / 2));
    camera.setCameraY((int)(center.y - (float)window.getSize().y / 2));
}

void Application::PollCommand(std::string command)
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
    std::string result = "Unknown command\n";
    if (clientCommand == "map")
    {
        if (tokens.size() == 2)
        {
            result = "Map successful\n";
            consoleHistory.append(result);
            std::string str = client.Map(stoi(tokens.back())).data + "\n";
            ParseMessage(str);
            consoleHistory.append(str);
            return;
        }
    }
    else if (clientCommand == "move")
    {
        if (tokens.size() == 4)
        {
            result = "Move successful\n";
            consoleHistory.append(result);
            std::string str = client.Move(stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3])).data + "\n";
            ParseMessage(str);
            consoleHistory.append(str);
            return;
        }
    }
    else if (clientCommand == "upgrade")
    {
    }
    else if (clientCommand == "turn")
    {
        if (tokens.size() == 1)
        {
            result = "Turn successful\n";
            consoleHistory.append(result);
            std::string str = client.Turn().data + "\n";
            ParseMessage(str);
            consoleHistory.append(str);
            return;
        }
    }
    else if (clientCommand == "games")
    {
        if (tokens.size() == 1)
        {
            result = "Games successful\n";
            consoleHistory.append(result);
            std::string str = client.Games().data + "\n";
            ParseMessage(str);
            consoleHistory.append(str);
            return;
        }
    }
    else if (clientCommand == "login")
    {
        if (tokens.size() == 2)
        {
            result = "Login to " + client.GetSocket().remote_endpoint().address().to_string() + "\n";
            consoleHistory.append(result);
            std::string str = client.Login(tokens.back()).data + "\n";
            ParseMessage(str);
            consoleHistory.append(str);
            return;
        }
    }
    else if (clientCommand == "clear")
    {
        consoleHistory.clear();
        return;
    }
    else if (clientCommand == "logout")
    {
        if (tokens.size() == 1)
        {
            std::string str = "Logout from " + client.GetSocket().remote_endpoint().address().to_string() + "\n";
            consoleHistory.append(str);
            return;
        }
    }
    consoleHistory.append(result);
}