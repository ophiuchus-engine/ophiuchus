#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.hpp>

int main(int argc, char **argv)
{
    auto instanceVersion{vk::enumerateInstanceVersion()};

    std::cout << "Vulkan instance version v" << VK_API_VERSION_MAJOR(instanceVersion) << '.'
              << VK_API_VERSION_MINOR(instanceVersion) << '.' << VK_API_VERSION_PATCH(instanceVersion) << " - "
              << "variant " << VK_API_VERSION_VARIANT(instanceVersion) << '\n';

    vk::ApplicationInfo appInfo{"Hello Vulkan", VK_MAKE_API_VERSION(0, 0, 1, 0), "Ophiucus", VK_API_VERSION_1_3};

    vk::InstanceCreateInfo instanceInfo{{}, &appInfo, {}, {}, nullptr};

    auto instance = vk::createInstance(instanceInfo);

    instance.destroy();

    std::cout << "Hello Vulkan!!!\n";

    return 0;
}
