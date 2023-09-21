C:/VulkanSDK/1.3.231.1/Bin/glslc.exe shader.vert -o shader.vert.spv
C:/VulkanSDK/1.3.231.1/Bin/glslc.exe shader.frag -o shader.frag.spv

C:/VulkanSDK/1.3.231.1/Bin/glslc.exe debug.vert -o debug.vert.spv
C:/VulkanSDK/1.3.231.1/Bin/glslc.exe debug.frag -o debug.frag.spv

@REM C:/VulkanSDK/1.3.231.1/Bin/glslc.exe particle.vert -o particle.vert.spv
@REM C:/VulkanSDK/1.3.231.1/Bin/glslc.exe particle.frag -o particle.frag.spv
@REM C:/VulkanSDK/1.3.231.1/Bin/glslc.exe particle.comp -o particle.comp.spv
C:/VulkanSDK/1.3.231.1/Bin/glslc.exe clusteredGrid.comp -o clusteredGrid.comp.spv
C:/VulkanSDK/1.3.231.1/Bin/glslc.exe clusteredCullLight.comp -o clusteredCullLight.comp.spv
pause