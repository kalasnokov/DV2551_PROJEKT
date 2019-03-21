
%VULKAN_SDK%/Bin32/glslangValidator.exe -V vertex.vert
%VULKAN_SDK%/Bin32/glslangValidator.exe -V fragment.frag
%VULKAN_SDK%/Bin32/glslangValidator.exe -o heightmap.spv -V heightmap.comp

pause