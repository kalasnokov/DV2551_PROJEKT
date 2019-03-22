#pragma once
#include <vulkan/vulkan.hpp>
#include "dataObjects.hpp"
const size_t chunk_size = 64;
const size_t max_chunks = 250;
const size_t gpuMemoryRequired = (chunk_size * chunk_size) * (sizeof(float) * 2);
const size_t maxRamRequired = gpuMemoryRequired * max_chunks;
/*
	NOTES:

	Generera ett sjukt stort block av minne på GPUn, något sätt måste de vara numrerade så att vi kan veta vilka chunks vi kan ladda över till ram

	Eventuellt för att spara på minne, ha bara 3 chunks bakom spelaren laddat i VRAM istället för 6.

	On the fly generering av verticer kommer vara väldigt redundant, men sparar på minne. Borde inte vara speciellt svårt, bara att hitta 
	en bra ratio på hur många verticer och distansen mellan dem och därmed generera en mesh

	Om vi verkligen vill att världen skall vara oändlig måste vi även tänka på vanligt ram och inte bara VRAM
	det betyder att vi måste använda samma seed för chunksen så att vi kan ladda ur dem ur ram och sedan återskapa dem när det behövs. Antingen det eller ha
	ett max-chunk värde 

	Vi måste skicka med 5 chunks till renderingPipelinen och sedan INNAN vi skapar våra verticer, culla bort saker som spelaren inte kan se
	då annars blir det väldigt många verticer per frame. Also, detta är reduntant af då vi kommer beräkna samma verticer om och om igen. 

	Chunksen måste numreras på något fint sätt

*/
class terrainGenerator
{
private:

	dataObjects *dataObjectptr;
public:
	void setUp(dataObjects *dataObjects);
	terrainGenerator();
	~terrainGenerator();
};

