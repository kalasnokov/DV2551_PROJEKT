#pragma once
#include <vulkan/vulkan.hpp>
#include "dataObjects.hpp"
const size_t chunk_size = 64;
const size_t max_chunks = 250;
const size_t gpuMemoryRequired = (chunk_size * chunk_size) * (sizeof(float) * 2);
const size_t maxRamRequired = gpuMemoryRequired * max_chunks;
/*
	NOTES:

	Generera ett sjukt stort block av minne p� GPUn, n�got s�tt m�ste de vara numrerade s� att vi kan veta vilka chunks vi kan ladda �ver till ram

	Eventuellt f�r att spara p� minne, ha bara 3 chunks bakom spelaren laddat i VRAM ist�llet f�r 6.

	On the fly generering av verticer kommer vara v�ldigt redundant, men sparar p� minne. Borde inte vara speciellt sv�rt, bara att hitta 
	en bra ratio p� hur m�nga verticer och distansen mellan dem och d�rmed generera en mesh

	Om vi verkligen vill att v�rlden skall vara o�ndlig m�ste vi �ven t�nka p� vanligt ram och inte bara VRAM
	det betyder att vi m�ste anv�nda samma seed f�r chunksen s� att vi kan ladda ur dem ur ram och sedan �terskapa dem n�r det beh�vs. Antingen det eller ha
	ett max-chunk v�rde 

	Vi m�ste skicka med 5 chunks till renderingPipelinen och sedan INNAN vi skapar v�ra verticer, culla bort saker som spelaren inte kan se
	d� annars blir det v�ldigt m�nga verticer per frame. Also, detta �r reduntant af d� vi kommer ber�kna samma verticer om och om igen. 

	Chunksen m�ste numreras p� n�got fint s�tt

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

