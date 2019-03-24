
E:/Program/Vulkan/Bin32/glslangValidator.exe -V vertex.vert
E:/Program/Vulkan/Bin32/glslangValidator.exe -V fragment.frag
E:/Program/Vulkan/Bin32/glslangValidator.exe -o computeMesh.spv -V computeMesh.comp
E:/Program/Vulkan/Bin32/glslangValidator.exe -o heightmap.spv -V heightmap.comp
E:/Program/Vulkan/Bin32/glslangValidator.exe -o test.spv -V test.comp

%VULKAN_SDK%/Bin32/glslangValidator.exe -V vertex.vert
%VULKAN_SDK%/Bin32/glslangValidator.exe -V fragment.frag
%VULKAN_SDK%/Bin32/glslangValidator.exe -o computeMesh.spv -V computeMesh.comp
%VULKAN_SDK%/Bin32/glslangValidator.exe -o heightmap.spv -V heightmap.comp
%VULKAN_SDK%/Bin32/glslangValidator.exe -o test.spv -V test.comp

pause