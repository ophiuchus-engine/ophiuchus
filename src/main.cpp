#include <iostream>
#include <vulkan/vulkan.hpp>

int main()
{
    try
    {
        const auto instanceVersion{vk::enumerateInstanceVersion()};
        std::cout << "Vulkan instance version v" << VK_API_VERSION_MAJOR(instanceVersion) << '.'
                  << VK_API_VERSION_MINOR(instanceVersion) << '.' << VK_API_VERSION_PATCH(instanceVersion) << " - "
                  << "variant " << VK_API_VERSION_VARIANT(instanceVersion) << '\n';
        const vk::ApplicationInfo appInfo{"Hello Vulkan", VK_MAKE_API_VERSION(0, 0, 1, 0), "Ophiucus",
                                          VK_API_VERSION_1_3};

        const std::array<const char *const, 1> layers{"VK_LAYER_KHRONOS_validation"};
        const vk::InstanceCreateInfo instanceInfo{
            {}, &appInfo, layers, {}, {},
        };

        const auto instance{vk::createInstance(instanceInfo)};

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

        const auto MAGIC_PHYSICAL_DEVICE{physicalDevices[0]};

        const auto queueFamilyProperties{MAGIC_PHYSICAL_DEVICE.getQueueFamilyProperties()};

        std::cout << "Available queue families in MAGIC_PHYSICAL_DEVICE:\n";
        for (const auto &properties : queueFamilyProperties)
        {
            std::cout << "-  " << vk::to_string(properties.queueFlags) << '\n';
        }

        const uint32_t MAGIC_QUEUE_FAMILY_INDEX{0};
        const std::array<const float, 1> priorities{1.0F};
        const vk::DeviceQueueCreateInfo queueInfo{{}, MAGIC_QUEUE_FAMILY_INDEX, priorities, {}};

        const vk::DeviceCreateInfo deviceInfo{{}, queueInfo, {}, {}, {}, {}};

        const auto device{MAGIC_PHYSICAL_DEVICE.createDevice(deviceInfo)};

        const auto queue = device.getQueue(MAGIC_QUEUE_FAMILY_INDEX, 0);

        const int renderWidth = 1920;
        const int renderHeight = 1080;

        const auto renderImageFormat{vk::Format::eR8G8B8A8Unorm};
        const vk::ImageCreateInfo renderImageInfo{{},
                                                  vk::ImageType::e2D,
                                                  renderImageFormat,
                                                  vk::Extent3D{{renderWidth, renderHeight}, 1},
                                                  1,
                                                  1,
                                                  vk::SampleCountFlagBits::e1,
                                                  vk::ImageTiling::eOptimal,
                                                  vk::ImageUsageFlagBits::eColorAttachment |
                                                      vk::ImageUsageFlagBits::eTransferSrc,
                                                  vk::SharingMode::eExclusive,
                                                  {},
                                                  vk::ImageLayout::eUndefined};

        const auto renderImage{device.createImage(renderImageInfo)};

        const auto memoryRequirements{device.getImageMemoryRequirements(renderImage)};

        // search for a valid memory index
        const auto memoryProperties = MAGIC_PHYSICAL_DEVICE.getMemoryProperties();
        auto memoryTypeIndex{std::numeric_limits<uint32_t>::max()};
        for (uint32_t i{0U}; i < memoryProperties.memoryTypeCount; ++i)
        {
            if ((memoryRequirements.memoryTypeBits & (1U << i)) == 1 &&
                (memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) ==
                    vk::MemoryPropertyFlagBits::eDeviceLocal)
            {
                memoryTypeIndex = i;
                break;
            }
        }

        if (memoryTypeIndex == std::numeric_limits<uint32_t>::max())
        {
            throw std::runtime_error("No suitable memory type");
        }

        const vk::MemoryAllocateInfo allocateInfo{memoryRequirements.size, memoryTypeIndex, {}};

        const auto imageMemory{device.allocateMemory(allocateInfo)};
        device.bindImageMemory(renderImage, imageMemory, {});

        vk::ImageViewCreateInfo imageViewInfo{{},
                                              renderImage,
                                              vk::ImageViewType::e2D,
                                              renderImageFormat,
                                              {},
                                              vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1},
                                              {}};
        const auto renderImageView{device.createImageView(imageViewInfo)};

        vk::AttachmentDescription attachmentDescription{{},
                                                        renderImageFormat,
                                                        vk::SampleCountFlagBits::e1,
                                                        vk::AttachmentLoadOp::eClear,
                                                        vk::AttachmentStoreOp::eStore,
                                                        vk::AttachmentLoadOp::eDontCare,
                                                        vk::AttachmentStoreOp::eDontCare,
                                                        vk::ImageLayout::eUndefined,
                                                        vk::ImageLayout::eTransferSrcOptimal};

        vk::AttachmentReference attachmentReference{0U, vk::ImageLayout::eColorAttachmentOptimal};

        vk::SubpassDescription subpassDescription{{}, vk::PipelineBindPoint::eGraphics, {}, attachmentReference, {}, {},
                                                  {}};

        vk::RenderPassCreateInfo renderPassInfo{{}, attachmentDescription, subpassDescription};

        const auto rederPass{device.createRenderPass(renderPassInfo)};

        vk::FramebufferCreateInfo framebufferInfo{{}, rederPass, renderImageView, renderWidth, renderHeight, 1U, {}};

        const auto frameBuffer{device.createFramebuffer(framebufferInfo)};

        device.destroyFramebuffer(frameBuffer);
        device.destroyRenderPass(rederPass);
        device.destroyImageView(renderImageView);
        device.freeMemory(imageMemory);
        device.destroyImage(renderImage);
        device.destroy();
        instance.destroy();

        std::cout << "Hello Vulkan!!!\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
