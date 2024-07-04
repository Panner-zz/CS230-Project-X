//------------------------------------------------------------------------------
//
// File Name:	Particle.c
// Author(s):	Doug Schilling (dschilling), <YourNameHere>
// Project:		Project X
// Course:		CS230SU24
//
// Copyright � 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DGL.h"

#include "Mesh.h"
#include "Particle.h"
#include "ParticleContainer.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

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

// Update the state of a Particle object.
// (Hint: Only update active particles (lifeTime > 0.0f).)
// (Hint: Use the Semi-Implicit Euler method to update position.)
// Params:
//	 particle = Pointer to the Particle object.
//	 dt = Change in time (in seconds) since the last game loop.
void ParticleUpdate(Particle* particle, ParticleContainer* container, float dt)
{
	if (particle->lifetime > 0.0f) {
		particle->lifetime -= dt;
		if (particle->lifetime > 0.0f) {
			Vector2DScaleAdd(&particle->position, &particle->velocity, dt, &particle->position);
		}
		else {
			ParticleContainerKillParticle(container, particle);
		}
	}

	// @@@TEMPORARY:
	UNREFERENCED_PARAMETER(particle);
	UNREFERENCED_PARAMETER(container);
	UNREFERENCED_PARAMETER(dt);
}

// Render a Particle object.
// (Hint: Only render active particles (lifeTime > 0.0f).)
// Params:
//	 particle = Pointer to the Particle object.
void ParticleRender(const Particle* particle, const Mesh* mesh)
{
	if (particle->lifetime > 0.0f) {
		DGL_Graphics_SetCB_TransformData(&particle->position, &particle->scale, particle->rotation);
		MeshRender(mesh);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
