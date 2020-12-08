#include "application.h"
#include "libs/imgui-sfml/imgui-SFML.h"
#include "libs/imgui/imgui.h"

#include <utility>

Application::Application(Config config, io_service &service) : config(std::move(config)), client(service)
{
}

int Application::Run()
{
    Init();

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
            PollEvent(event);
        }

        if (!states.empty())
        {
            window.clear();
            render.Draw(states.front());
        }
        else
        {
            mouseX = mouseY = cameraX = cameraY = -1;
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        if (firstRender)
        {
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::SetNextWindowSize(ImVec2(450, 600));
            firstRender = false;
        }
        ImGui::Begin("Console");
        ImGui::InputText("input", console, sizeof(console));
        char *history = const_cast<char *>(consoleHistory.c_str());
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
                                  ImVec2(-1, ImGui::GetWindowContentRegionMax().y - 100), ImGuiInputTextFlags_ReadOnly);
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

    HandleCommand("login " + config.teamName);
    ImGui::SFML::Init(window);

    std::vector<std::pair<sf::Text, std::string>> texts;
    texts.emplace_back(SfmlTool::GetText((float)window.getSize().x / 2, (float)window.getSize().y / 2,
                                         "No data to show", 15, sf::Color(82, 73, 73)), "8-bit-pusab");
    State state(GraphState(), texts);
    states.push(state);

    camera.SetCameraX(0);
    camera.SetCameraY(0);
}