#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::CreatePyramidalBufferWraps()
    {
        size_t totalImages = appPyramidalHeightMaps.size();
        appPyramidalImageWraps.resize(totalImages);
        for (int i = 0; i < totalImages; ++i)
        {
            // try to pass through the pixel from texture
            //appPyramidalImageWraps[i] = CreateImageWrap();
        }
        /*
        for (auto texturePath : appTexturePaths)
        {
            auto textureImageWrap = CreateTextureImageWrap(texturePath);
            TransitionImageLayout(textureImageWrap.image,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            appTextureImageWraps.push_back(textureImageWrap);
        }
        */

    }

    void ReinkanApp::CreateComputeGeneratePyramidalDescriptorSetWrap()
    {
        // binding
        // 0 : textures[]
        // 1 : heightMapData[]
        // 2 : PyramidalMap[]
    }
}