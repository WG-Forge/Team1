#include "application.h"

const Config config;

int main()
{
    io_service ioService;
    Application app(config, ioService);
    return app.run();
}