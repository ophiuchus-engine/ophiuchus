add_executable(main "main.cpp" "VkManager.hpp" "VkManager.cpp")

target_include_directories(main PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(main PRIVATE Vulkan::Vulkan)
