//------------------------------------------------------------------------------
//
// File Name:	ParticleContainer.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project X
// Course:		CS230SU24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "DGL.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {	// Assume C declarations for C++.
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Mesh Mesh;
typedef struct ParticleContainer ParticleContainer;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new ParticleContainer object.
// (Hint: Internally, allocate memory for the specified number of Particle objects).
// Params:
//	 particleMax = The number of Particle objects to be allocated.
// Returns:
//	 If the ParticleContainer object was allocated successfully,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
ParticleContainer* ParticleContainerCreate(unsigned particleMax, bool areParticlesRecyclable);

// Free the memory associated with a ParticleContainer.
// (NOTE: The array of Particle objects must be freed.)
// (NOTE: The ParticleContainer pointer must be set to NULL.)
// Params:
//	 container = Pointer to the ParticleContainer pointer.
void ParticleContainerFree(ParticleContainer** container);

// Determines if the container is empty (no active particles).
// Params:
//	 container = Pointer to the ParticleContainer pointer.
// Returns:
//	 True if the container is empty, otherwise false.
bool ParticleContainerIsEmpty(ParticleContainer* container);

// Determines if the container is full (no free particles).
// Params:
//	 container = Pointer to the ParticleContainer pointer.
// Returns:
//	 True if the container is full, otherwise false.
bool ParticleContainerIsFull(ParticleContainer* container);

// Allocate a particle in the ParticleContainer.
// Params:
//	 container = Pointer to the ParticleContainer.
// Returns:
//   If the container was not full,
//	   then return a pointer to the particle allocated,
Particle* ParticleContainerAllocateParticle(ParticleContainer* container);

// Kill a particle in the ParticleContainer.
// (Hint: Update the Active and Free variables.)
// (Hint: Set the particle's lifetime variable based upon isRecyclable.)
// Params:
//	 container = Pointer to the ParticleContainer.
//	 particle = Pointer to the particle to be "killed".
void ParticleContainerKillParticle(ParticleContainer* container, Particle* particle);

// Update all Particles in the ParticleContainer.
// (NOTE: You must call ParticleUpdate for all Particle objects in the container.)
// (HINT: For simplicity, do this for all particles, both active and inactive.)
// Params:
//   container = Pointer to the ParticleContainer.
//	 dt = Change in time (in seconds) since the last game loop.
void ParticleContainerUpdate(ParticleContainer* container, float dt);

// Render all Particles in the ParticleContainer.
// (NOTE: You must call ParticleRender for all Particle objects in the container.)
// (HINT: For simplicity, do this for all particles, both active and inactive.)
// Params:
//   container = Pointer to the ParticleContainer.
//   mesh = Pointer to the mesh used to render the particle.
void ParticleContainerRender(const ParticleContainer* container, const Mesh* mesh);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       // End of extern "C" {
#endif

