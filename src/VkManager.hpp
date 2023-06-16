#pragma once

#include <vulkan/vulkan.hpp>

struct ApplicationInfo
{
    const std::string_view name;
    const unsigned int majorVersion;
    const unsigned int minorVersion;
};

struct EngineInfo
{
    const std::string_view name;
    const unsigned int majorVersion;
    const unsigned int minorVersion;
};

struct GraphicsInfo
{
    const std::string_view apiName;
    const unsigned int majorVersion;
    const unsigned int minorVersion;
};

class VkManager
{
  public:
    VkManager(const GraphicsInfo &graphicsInfo);

    void Startup(const ApplicationInfo &appInfo, const EngineInfo &engineInfo);

    void Shutdown();

  private:
    const vk::Instance initializeInstance(const ApplicationInfo &appInfo, const EngineInfo &engineInfo) const;

    const std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;

    vk::PhysicalDevice choosePhysicalDevice(const std::vector<vk::PhysicalDevice> &physicalDevices) const;

    const GraphicsInfo vulkanInfo;

    bool canStartup{false};

    bool isInitialized{false};

    vk::Instance instance;

    vk::PhysicalDevice physicalDevice;
};
