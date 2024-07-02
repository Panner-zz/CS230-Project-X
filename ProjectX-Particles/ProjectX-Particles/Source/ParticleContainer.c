//------------------------------------------------------------------------------
//
// File Name:	ParticleContainer.c
// Author(s):	Doug Schilling (dschilling), <YourNameHere>
// Project:		Project X
// Course:		CS230SU24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <assert.h>

#include "Particle.h"
#include "ParticleContainer.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ParticleContainer
{
	// The maximum number of Particle objects allowed.
	unsigned particleMax;

	// The current number of Particle objects free.
	unsigned particleFree;

	// The current number of Particle objects active.
	unsigned particleActive;

	// If true, then particles are "recycled" after dying.
	// (Hint: This results in a continuous effect.)
	bool areParticlesRecyclable;

	// The dynamically-allocated array of Particle objects.
	Particle* particles;

} ParticleContainer;


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new ParticleContainer object.
// (NOTE: Internally, allocate memory for the specified number of Particle objects.)
// (HINT: The structure's particleMax value should be 0 if the array was not allocated.)
// Params:
//	 particleMax = The number of Particle objects to be allocated.
// Returns:
//	 If the ParticleContainer object was allocated successfully,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
ParticleContainer* ParticleContainerCreate(unsigned particleMax, bool areParticlesRecyclable)
{
	// @@@TEMPORARY:
	UNREFERENCED_PARAMETER(particleMax);
	UNREFERENCED_PARAMETER(areParticlesRecyclable);

	// Allocate memory for the ParticleContainer object.
	ParticleContainer* container = calloc(1, sizeof(ParticleContainer));

	// If the ParticleContainer was allocated successfully, then:
	//   Store pointer to a dynamically allocated array of Particle objects.
	//   Store the number of allocated Particle objects (particleMax).
	if (container)
	{
		container->particles = calloc(particleMax, sizeof(Particle));
		if (container->particles)
		{
			container->particleMax = particleMax;
			container->particleFree = particleMax;
			container->areParticlesRecyclable = areParticlesRecyclable;
		}
	}

	// Return the pointer to the particleContainer object.
	return container;
}

// Free the memory associated with a ParticleContainer.
// (NOTE: The array of Particle objects must be freed.)
// (NOTE: The ParticleContainer pointer must be set to NULL.)
// Params:
//	 container = Pointer to the ParticleContainer pointer.
void ParticleContainerFree(ParticleContainer** container)
{
	// @@@TEMPORARY:
	UNREFERENCED_PARAMETER(container);

	// Verify that a valid particleContainer object was passed.
	assert(container);

	// Free the dynamically-allocated array of Particle objects.
	free((*container)->particles);

	// Free the dynamically-allocated particleContainer object.
	free(*container);

	// Set the pointer to memory to NULL.
	*container = NULL;
}

// Determines if the container is empty (no active particles).
// Params:
//	 container = Pointer to the ParticleContainer pointer.
// Returns:
//	 True if the container is empty, otherwise false.
bool ParticleContainerIsEmpty(ParticleContainer* container)
{
	assert(container && "ParticleContainerIsEmpty was passed a NULL pointer.");

	if (container->particleActive > 0) {
		return true;
	}

	return false;
}

// Determines if the container is full (no free particles).
// Params:
//	 container = Pointer to the ParticleContainer pointer.
// Returns:
//	 True if the container is full, otherwise false.
bool ParticleContainerIsFull(ParticleContainer* container)
{
	assert(container && "ParticleContainerIsFull was passed a NULL pointer.");

	if (container->particleFree == 0) {
		return true;
	}

	return false;
}

// Allocate a particle in the ParticleContainer.
// Params:
//	 container = Pointer to the ParticleContainer.
// Returns:
//   If the container was not full,
//	   then return a pointer to the particle allocated,
//     else return NULL.
Particle* ParticleContainerAllocateParticle(ParticleContainer* container)
{
	assert(container);

	// Check that the container is NOT full (no free particles).
	if (!ParticleContainerIsFull(container))
	{
		// For each Particle object in the particles array:
		for (unsigned i = 0; i < container->particleMax; ++i)
		{
			// If the particle's lifetime == 0, the return a reference to the particle.
			if (container->particles[i].lifetime == 0)
			{
				// Decrement the number of free particles.
				--container->particleFree;

				// Increment the number of active particles.
				++container->particleActive;

				// Return a reference to the current particle.
				return &(container->particles[i]);
			}
		}
	}
	return NULL;
}

// Kill a particle in the ParticleContainer.
// (Hint: Update the Active and Free variables.)
// (Hint: Set the particle's lifetime variable based upon isRecyclable.)
// Params:
//	 container = Pointer to the ParticleContainer.
//	 particle = Pointer to the particle to be "killed".
void ParticleContainerKillParticle(ParticleContainer* container, Particle* particle)
{
	assert(container);
	assert(particle);

	if (container->areParticlesRecyclable) {
		particle->lifetime = 0.0f;
		container->particleFree++;
	}
	else {
		particle->lifetime = -1.0f;
	}

	container->particleActive--;

}

// Update all Particles in the ParticleContainer.
// (NOTE: You must call ParticleUpdate for all Particle objects in the container.)
// (HINT: For simplicity, do this for all particles, both active and inactive.)
// Params:
//   container = Pointer to the ParticleContainer.
//	 dt = Change in time (in seconds) since the last game loop.
void ParticleContainerUpdate(ParticleContainer* container, float dt)
{
	assert(container);

	// For each Particle object in the particles array (active or otherwise):
	//   Call ParticleUpdate on every Particle object in the array.
	for (unsigned i = 0; i < container->particleMax; ++i)
	{
		ParticleUpdate(&(container->particles[i]), container, dt);
	}
}

// Render all Particles in the ParticleContainer.
// (NOTE: You must call ParticleRender for all Particle objects in the container.)
// (HINT: For simplicity, do this for all particles, both active and inactive.)
// Params:
//   container = Pointer to the ParticleContainer.
//   mesh = Pointer to the mesh used to render the particle.
void ParticleContainerRender(const ParticleContainer* container, const Mesh* mesh)
{
	assert(container);
	assert(mesh);

	// For each Particle object in the particles array (active or otherwise):
	//   Call ParticleRender, passing a reference to particle and the mesh.
	for (unsigned i = 0; i < container->particleMax; ++i)
	{
		ParticleRender(&(container->particles[i]), mesh);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

