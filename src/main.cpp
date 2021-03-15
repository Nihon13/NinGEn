#include "Core/core.h"
#include "Core/window.h"

int main()
{
    using namespace ningen;

    Window window;

    glClearColor(0.0f, 0.3f, 0.4f, 1.0f);

    while (!window.windowShouldClose())
    {
        window.clearWindow();
        window.updateWindow();
    }

    return 0;
}