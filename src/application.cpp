#include "application.h"

Application::Application(const Config config) : config(config) {
}

int Application::run() {
    init();
    sf::Clock dtTimer;

    int mouseX = -1, mouseY = -1, cameraX = -1, cameraY = -1, delta = 0;
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
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
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
                delta += event.mouseWheel.delta;
            }
        }

        if (!states.empty()) {
            window.clear();
            states.front().resize(delta);
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
        window.create(winMode, "Rail-simulator", sf::Style::Close, settings);
    }

    if (config.hideCursor) {
        window.setMouseCursorVisible(false);
    }

    render.setCamera(&camera);
    render.setWindow(&window);

    //test
    {
        State state;
        state.addLine(10, 10, 30, 30);
        state.addLine(30, 30, 234, 123);
        state.addLine(123, 430, 234, 123);
        state.addCircle(10, 10, 10);
        state.addCircle(30, 30, 10);
        state.addCircle(234, 123, 10);
        state.addCircle(123, 430, 10);
        state.addText((int)(window.getSize().x / 2) - 150, (int)window.getSize().y - 50,
                "Press esc to exit program", "../resourses/8-bit-pusab.ttf");
        states.push(state);
    }
}
