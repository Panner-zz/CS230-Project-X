//------------------------------------------------------------------------------
//
// File Name:	Particle.h
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

#include "Vector2D.h"

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

typedef struct Particle
{
	// The remaining "lifetime" of the particle.
	//   Three "states":
	//	   > 0.0f: Alive (Currently in use)
	//	   = 0.0f: Free (Can be allocated)
	//	   < 0.0f: Dead (Cannot be allocated)
	float lifetime;

	// The 3 "transform" values.
	Vector2D position;
	Vector2D scale;
	float rotation;

	// The "physics" value(s).
	Vector2D velocity;

} Particle;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Update the state of a Particle object.
// (Hint: Only update active particles (lifeTime > 0.0f).)
// (Hint: Use the Semi-Implicit Euler method to update position.)
// Params:
//	 particle = Pointer to the Particle object.
//	 dt = Change in time (in seconds) since the last game loop.
void ParticleUpdate(Particle* particle, ParticleContainer* container, float dt);

// Render a Particle object.
// (Hint: Only render active particles (lifeTime > 0.0f).)
// Params:
//	 particle = Pointer to the Particle object.
void ParticleRender(const Particle* particle, const Mesh* mesh);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       // End of extern "C" {
#endif

