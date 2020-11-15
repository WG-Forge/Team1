#include "application.h"

const Config config;

int main()
{
    Application app(config);
    return app.run();
}