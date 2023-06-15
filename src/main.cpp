#include <iostream>
#include <vulkan/vulkan.hpp>

int main()
{
    try
    {
        auto instanceVersion{vk::enumerateInstanceVersion()};
        std::cout << "Vulkan instance version v" << VK_API_VERSION_MAJOR(instanceVersion) << '.'
                  << VK_API_VERSION_MINOR(instanceVersion) << '.' << VK_API_VERSION_PATCH(instanceVersion) << " - "
                  << "variant " << VK_API_VERSION_VARIANT(instanceVersion) << '\n';
        const vk::ApplicationInfo appInfo{"Hello Vulkan", VK_MAKE_API_VERSION(0, 0, 1, 0), "Ophiucus",
                                          VK_API_VERSION_1_3};

        const vk::InstanceCreateInfo instanceInfo{{}, &appInfo, {}, {}, nullptr};

        auto instance = vk::createInstance(instanceInfo);

        instance.destroy();

        std::cout << "Hello Vulkan!!!\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
