#include <format>
#include <iostream>

#include "VkManager.hpp"

VkManager::VkManager(const GraphicsInfo &graphicsInfo) : vulkanInfo{graphicsInfo}
{
    const auto majorVersion{vulkanInfo.majorVersion};
    const auto minorVersion{vulkanInfo.minorVersion};

    const auto instanceVersion{vk::enumerateInstanceVersion()};
    const auto instanceMajorVersion{VK_API_VERSION_MAJOR(instanceVersion)};
    const auto instanceMinorVersion{VK_API_VERSION_MINOR(instanceVersion)};

    if (instanceMajorVersion > majorVersion)
    {
        std::cout << std::format("Current instance major version is higher than requested.\n v{}.{} > v{}.{}\n It "
                                 "may introduce breaking changes.",
                                 instanceMajorVersion, instanceMinorVersion, majorVersion, minorVersion);
    }
    else if (instanceMajorVersion == majorVersion)
    {
        if (instanceMinorVersion >= minorVersion)
        {
            std::cout << "Compatible instance version requested.\n";
            canStartup = true;
        }
        else
        {
            std::cout << std::format("Incompatible instance minor version.\n Requested v{}.{} found v{}.{}\n",
                                     majorVersion, minorVersion, instanceMajorVersion, instanceMinorVersion);
        }
    }
    else
    {
        std::cout << std::format("Incompatible instance major version.\n Requested v{}.{} found v{}.{}\n", majorVersion,
                                 minorVersion, instanceMajorVersion, instanceMinorVersion);
    }
}

void VkManager::Startup(const ApplicationInfo &appInfo, const EngineInfo &engineInfo)
{
    if (canStartup)
    {
        instance = initializeInstance(appInfo, engineInfo);

        const auto physicalDevices{enumeratePhysicalDevices()};
        physicalDevice = choosePhysicalDevice(physicalDevices);

        // physicalDevice.createDevice()

        isInitialized = true;
    }
}

void VkManager::Shutdown()
{
    if (isInitialized)
    {
        instance.destroy();
    }
}

const vk::Instance VkManager::initializeInstance(const ApplicationInfo &appInfo, const EngineInfo &engineInfo) const
{
    const vk::ApplicationInfo appInfo{appInfo.name,
                                      VK_MAKE_API_VERSION(0U, appInfo.majorVersion, appInfo.minorVersion, 0U),
                                      engineInfo.name,
                                      VK_MAKE_API_VERSION(0U, engineInfo.majorVersion, engineInfo.minorVersion, 0U),
                                      VK_MAKE_API_VERSION(0U, vulkanInfo.majorVersion, vulkanInfo.minorVersion, 0U, 0U),
                                      nullptr};

    const vk::InstanceCreateInfo instanceInfo{{}, &appInfo, {}, {}, nullptr};

    return vk::createInstance(instanceInfo);
}

const std::vector<vk::PhysicalDevice> VkManager::enumeratePhysicalDevices() const
{
    const auto physicalDevices{instance.enumeratePhysicalDevices()};

    std::cout << "Available physical devices:\n";
    for (const auto &device : physicalDevices)
    {
        const auto properties{device.getProperties()};
        std::cout << "-  " << properties.deviceName << '\n';
    }

    if (physicalDevices.empty())
    {
        throw std::runtime_error("No available physical devices");
    }

    return physicalDevices;
}

vk::PhysicalDevice VkManager::choosePhysicalDevice(const std::vector<vk::PhysicalDevice> &physicalDevices) const
{
    return physicalDevices[0];
}
