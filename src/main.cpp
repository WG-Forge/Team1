#include "application.h"
#include "client/client.h"

const Config config;

int main()
{
    Application app(config);
    return app.Run();
}