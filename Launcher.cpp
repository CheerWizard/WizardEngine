#include "test/TestApplication.h"

int main() {
    auto app = new TestApplication();
    app->run();
    delete app;
}
