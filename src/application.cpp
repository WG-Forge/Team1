#include "application.h"
#include "libs/imgui-sfml/imgui-SFML.h"
#include "libs/imgui/imgui.h"

#include <atomic>
#include <utility>

Application::Application(Config config) : config(std::move(config))
{
}

int Application::Run()
{
    Init();

    std::atomic<int> tick = 0;

    std::thread brainThread([this, &tick] {
        while (window.isOpen())
        {
            clientMutexes[0].lock();
            auto data = clients[0].Games().data;
            clientMutexes[0].unlock();
            if (!config.isSoloGame)
            {
                if (RailGraph::ParseStatusOfGameFromJson(data, config.gameName) != 2)
                {
                    continue;
                }
            }
            else
            {
                if (RailGraph::ParseStatusOfGameFromJson(data, "Game of " + config.teamName) != 2)
                {
                    continue;
                }
            }
            if (!pause)
            {
                sf::Clock debug;
                std::vector<std::thread> workers;
                mapMutex.lock();
                auto commands = brain.GetTurn(tick);
                mapMutex.unlock();
                std::cerr << debug.getElapsedTime().asMilliseconds() << ' ';
                debug.restart();
                for (int i = 0; i < commands.size(); ++i)
                {
                    workers.emplace_back([this, commands, i] { HandleCommand(commands[i], false, i % clientsNum); });
                }
                for (auto &worker : workers)
                {
                    worker.join();
                }
                HandleCommand("turn", false, 0);
                HandleCommand("map 1", false, 0);
                tick++;
                std::cerr << debug.getElapsedTime().asMilliseconds() << '\n';
            }
        }
    });

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
            stateMutex.lock();
            PollEvent(event);
            stateMutex.unlock();
        }

        window.clear();
        stateMutex.lock();
        render.Draw(state);
        stateMutex.unlock();

        ImGui::SFML::Update(window, deltaClock.restart());
        if (firstRender)
        {
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::SetNextWindowSize(ImVec2(450, 900));
            firstRender = false;
        }
        ImGui::Begin("Console");
        ImGui::InputText("input", console, sizeof(console));
        char *history = const_cast<char *>(consoleHistory.c_str());
        if (consoleHistory.size() > 10'000)
        {
            consoleHistory.clear();
        }
        if (ImGui::TreeNode("Help"))
        {
            ImGui::Indent();
            ImGui::Text("left click on the point to move");
            ImGui::Text("right click to release");
            ImGui::Text("press 'X' to show indices of vertices");
            ImGui::Text("press 'Escape' to exit");
            ImGui::Text("scroll wheel to resize");
            ImGui::Text("press 'Ctrl + Z' to backup");
            ImGui::Unindent();
            ImGui::TreePop();
        }
        ImGui::InputTextMultiline("output", history, consoleHistory.size(),
                                  ImVec2(-1, ImGui::GetWindowContentRegionMax().y / 3 * 2 - 50),
                                  ImGuiInputTextFlags_ReadOnly);
        ImGui::Spacing(), ImGui::Spacing();
        mapMutex.lock();
        consoleInformation = map.GetPointInfo(render.GetPicked(state));
        mapMutex.unlock();
        consoleInformation += "\n" + std::to_string(tick);
        char *information = const_cast<char *>(consoleInformation.c_str());
        ImGui::InputTextMultiline("info", information, consoleInformation.size(),
                                  ImVec2(-1, ImGui::GetWindowContentRegionMax().y / 3 - 50),
                                  ImGuiInputTextFlags_ReadOnly);
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            Application::focusedConsole = true;
        }
        else
        {
            Application::focusedConsole = false;
        }
        ImGui::End();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    brainThread.join();
    return EXIT_SUCCESS;
}

void Application::Init()
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

    render.SetCamera(&camera);
    render.SetWindow(&window);
    render.LoadFont("8-bit-pusab", "../resourses/8-bit-pusab.ttf");
    render.LoadTexture("town", "../resourses/town.png");
    render.LoadTexture("market", "../resourses/market.png");
    render.LoadTexture("storage", "../resourses/storage.png");
    render.LoadTexture("default", "../resourses/default.png");
    render.LoadTexture("train", "../resourses/train.png");

    for (int i = 0; i < clientsNum; ++i)
    {
        if (!config.isSoloGame)
        {
            HandleCommand("login " + config.teamName + " " + config.password + " " + config.gameName + " " +
                              std::to_string(config.numTurns) + " " + std::to_string(config.numPlayers),
                          false, i);
        }
        else
        {
            HandleCommand("login " + config.teamName, false, i);
        }
    }
    HandleCommand("map 0", false, 0);
    HandleCommand("map 1", false, 0);
    HandleCommand("map 10", false, 0);
    brain.SetMap(map);
    ImGui::SFML::Init(window);

    state = State(map, std::vector<std::pair<sf::Text, std::string>>{});
    auto center = state.GetCenter();
    camera.SetCameraX(center.renderX - window.getSize().x / 2);
    camera.SetCameraY(center.renderY - window.getSize().y / 2);
}