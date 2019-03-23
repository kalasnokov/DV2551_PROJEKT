#pragma once
#include <vulkan/vulkan.hpp>
#include "dataObjects.hpp"
#include "vulkanBufferWrapper.hpp"
const int32_t chunk_size = 64;
const size_t gpuMemoryRequired = ((chunk_size * chunk_size) * (sizeof(float)) * 9) / 1000;
/*
	NOTES:

	On the fly generering av verticer kommer vara v�ldigt redundant, men sparar p� minne. Borde inte vara speciellt sv�rt, bara att hitta 
	en bra ratio p� hur m�nga verticer och distansen mellan dem och d�rmed generera en mesh

	Vi m�ste skicka med 5 chunks till renderingPipelinen och sedan INNAN vi skapar v�ra verticer, culla bort saker som spelaren inte kan se
	d� annars blir det v�ldigt m�nga verticer per frame. Also, detta �r reduntant af d� vi kommer ber�kna samma verticer om och om igen. 

	Chunksen m�ste numreras p� n�got fint s�tt

*/
class terrainGenerator
{
private:
	float seed;
	struct {
		BufferW storageBuffer;					// (Shader) storage buffer object containing the chunks
		BufferW uniformBuffer;					// Uniform buffer object containing particle system parameters
		VkQueue queue;								// Separate queue for compute commands (queue family may differ from the one used for graphics)
		VkCommandPool commandPool;					// Use a separate command pool (queue family may differ from the one used for graphics)
		VkCommandBuffer commandBuffer;				// Command buffer storing the dispatch commands and barriers
		VkFence fence;								// Synchronization fence to avoid rewriting compute CB if still in use
		VkDescriptorSetLayout descriptorSetLayout;	// Compute shader binding layout
		VkDescriptorSet descriptorSet;				// Compute shader bindings
		VkPipelineLayout pipelineLayout;			// Layout of the compute pipeline
		VkPipeline pipeline;						// Compute pipeline for updating particle positions
		struct computeUBO {							// Compute shader uniform block object
			float seed;							//		Frame delta time
			float chunkX;							//		x position of the attractor
			float chunkY;							//		y position of the attractor
			int32_t chunkSize = chunk_size;
		} ubo;
	} compute;
	struct chunk
	{
		float height[chunk_size][chunk_size];
	};
	dataObjects *dataObjectptr;
public:
	void setUp(dataObjects *dataObjects);
	terrainGenerator();
	~terrainGenerator();
};

