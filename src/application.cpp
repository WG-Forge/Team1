#include "application.h"

Application::Application(const Config config) : config(config) {
}

int Application::run() {
    init();
    sf::Clock dtTimer;

    int mouseX = -1, mouseY = -1, cameraX = -1, cameraY = -1, delta;
    bool touchedX = false;
    while (window.isOpen()) {
        if (dtTimer.getElapsedTime().asMilliseconds() * config.fps <= 1000) {
            continue;
        }
        dtTimer.restart();
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !touchedX) {
                render.hideInformationSwitch();
                touchedX = true;
            } else {
                touchedX = false;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !render.isTarget()) {
                if (mouseX == -1 && mouseY == -1) {
                    mouseX = sf::Mouse::getPosition().x, mouseY = sf::Mouse::getPosition().y;
                    cameraX = camera.getCameraX(), cameraY = camera.getCameraY();
                } else {
                    camera.setCameraX(cameraX + mouseX - sf::Mouse::getPosition().x);
                    camera.setCameraY(cameraY + mouseY - sf::Mouse::getPosition().y);
                }
            } else {
                mouseX = mouseY = cameraX = cameraY = -1;
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                delta = event.mouseWheel.delta;
                states.front().Resize(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y, delta * 10);
            }
        }

        if (!states.empty()) {
            window.clear();
            render.draw(states.front());
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

void Application::init() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    if (config.isFullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "Rail-simulator",
                      sf::Style::Fullscreen, settings);
    } else {
        sf::VideoMode winMode(config.windowX, config.windowY);
        window.create(winMode, "Rail-simulator", sf::Style::Close,
                      settings);
    }

    if (config.hideCursor) {
        window.setMouseCursorVisible(false);
    }

    render.setCamera(&camera);
    render.setWindow(&window);
    render.loadFont("8-bit-pusab", "../resourses/8-bit-pusab.ttf");

  //test
    {
        std::ifstream fin(config.pathToJson);
        std::stringstream ss;
        ss << fin.rdbuf();
        GraphParser graphParser(ss.str());
        GraphState graphState = CreateCircleGraphStateFromGraph(graphParser.GetGraph());
        std::vector<std::pair<sf::Text, std::string>> texts;
        State state(graphState, texts);
        state.AddText(window.getSize().x / 2.f - 150, window.getSize().y - 50.f,
                      "Press esc to exit program", "8-bit-pusab", 10, sf::Color::White);
        state.AddText(window.getSize().x - 270.f, 10.f,
                      "left click on the point to move", "8-bit-pusab", 10, sf::Color::White);
        state.AddText(window.getSize().x - 270.f, 30.f,
                      "right click to release", "8-bit-pusab", 10, sf::Color::White);
        state.AddText(window.getSize().x - 220.f, 60.f,
                      "Press 'X' to show ", "8-bit-pusab", 14, sf::Color::White);
        state.AddText(window.getSize().x - 220.f, 80.f,
                      "vertex numbering", "8-bit-pusab", 13, sf::Color::White);
        state.AddText(window.getSize().x - 190.f,  110.f,
                      "Scroll wheel", "8-bit-pusab", 14, sf::Color::White);
        state.AddText(window.getSize().x - 170.f, 130.f,
                      "to resize", "8-bit-pusab", 14, sf::Color::White);
        states.push(state);
    }

    auto center = states.front().GetCenter();
    camera.setCameraX((int)(center.x - window.getSize().x / 2));
    camera.setCameraY((int)(center.y - window.getSize().y / 2));
}
