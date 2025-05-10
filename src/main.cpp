#include <exception>
#include <iostream>
#include <print>

import app;

auto main() -> int
{
    try {
        app::App vulkan_demo{ };
        vulkan_demo.run();
    } catch (const std::exception& err) {
        std::println(std::cerr, "PANIC: {}", err.what());
        return EXIT_FAILURE;
    } catch (...) {
        std::println(std::cerr, "PANIC: Unknown exception");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
