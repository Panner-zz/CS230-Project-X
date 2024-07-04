//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitter.c
// Author(s):	Doug Schilling (dschilling), <YourNameHere>
// Project:		Project X
// Course:		CS230SU24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "DGL.h"

#include "Component.h"
#include "Entity.h"
#include "Mesh.h"
#include "MeshLibrary.h"
#include "Particle.h"
#include "ParticleContainer.h"
#include "ParticleEmitter.h"
#include "Random.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ParticleEmitter
{
	Component base;

	//--------------------------------------------------------------------------
	// General Emitter Properties:
	//--------------------------------------------------------------------------

	// The maximum number of active particles that can be emitted.
	// (Hint: This is used to determine the container size.)
	unsigned emitMax;

	// The rate at which particles are emitted (in units "particles/second").
	float emitRate;

	// Boolean to indicate if particles are "recyclable".
	bool areParticlesRecyclable;

	// The particle lifetime range values.
	// Particle lifetime = RandomRangeFloat(lifetimeMin, lifetimeMax);
	float lifetimeMin;
	float lifetimeMax;

	// The particle speed range values.
	// Particle speed = RandomRangeFloat(speedMin, speedMax);
	float speedMin;
	float speedMax;

	// The size of each particle (used to determine the particle's transformation).
	Vector2D scale;

	// The mesh used to draw the particles.
	// Read the mesh name from the stream and call MeshLibraryBuild.
	const Mesh* mesh;

	// The tint color to be applied to the mesh vertex colors (all are white).
	DGL_Color tintColor;

	//--------------------------------------------------------------------------
	// Additional Emitter Variables:
	//--------------------------------------------------------------------------

	// An accumulator value for the number of particles to be emitted.
	float emitAccumulator;

	// A container of particles (the maximium size is determined by "emitMax").
	ParticleContainer* container;

} ParticleEmitter;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

// Free the memory associated with an ParticleEmitter component.
// (NOTE: The ParticleEmitter pointer must be set to NULL.)
// Params:
//	 ParticleEmitter = Pointer to the ParticleEmitter pointer.
void ParticleEmitterFree(Component** ParticleEmitter);

// Read the properties of an ParticleEmitter component from a file.
// (Hint: The order in the struct declaration matches the order in the file.)
// Params:
//	 ParticleEmitter = Pointer to the ParticleEmitter component.
//	 stream = The data stream used for reading.
static void ParticleEmitterRead(Component* component, Stream stream);

// Update the ParticleEmitter.
// Params:
//	 component = Pointer to the ParticleEmitter component.
//	 dt = Change in time (in seconds) since the last game loop.
static void ParticleEmitterUpdate(Component* component, float dt);
static void ParticleEmitterRender(const Component* component);
static void ParticleEmitterEmit(ParticleEmitter* emitter);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new ParticleEmitter component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
ParticleEmitter* ParticleEmitterCreate(void)
{
	// Allocate the memory for the ParticleEmitter.
	ParticleEmitter* component = calloc(1, sizeof(ParticleEmitter));

	if (component)
	{
		// Initialize the base Component variables.
		component->base.memorySize = sizeof(ParticleEmitter);
		component->base.type = cParticleEmitter;
		component->base.read = ParticleEmitterRead;
		component->base.update = ParticleEmitterUpdate;
		component->base.render = ParticleEmitterRender;
		component->base.free = ParticleEmitterFree;
	}

	// Return the resulting ParticleEmitter.
	return component;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Free the memory associated with an ParticleEmitter component.
// (NOTE: The ParticleEmitter pointer must be set to NULL.)
// Params:
//	 ParticleEmitter = Pointer to the ParticleEmitter pointer.
void ParticleEmitterFree(Component** component)
{
	// Verify that a valid ParticleEmitter was specified.
	if (component)
	{
		ParticleEmitter* emitter = (ParticleEmitter*)*component;
		if (emitter && emitter->container)
		{
			ParticleContainerFree(&(emitter->container));
		}

		ComponentFree(component);
	}
}

// Read the properties of an ParticleEmitter component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 ParticleEmitter = Pointer to the ParticleEmitter component.
//	 stream = The data stream used for reading.
static void ParticleEmitterRead(Component* component, Stream stream)
{
	if (component && stream)
	{
		// Cast the component pointer to a new pointer of type "ParticleEmitter*".       
		ParticleEmitter* emitter = (ParticleEmitter*)component;
		// //grab the max particles        
		emitter->emitMax = StreamReadInt(stream);
		//grab the emit rate        
		emitter->emitRate = StreamReadFloat(stream);
		//grad the recycle      
		emitter->areParticlesRecyclable = StreamReadBoolean(stream);
		emitter->lifetimeMin = StreamReadFloat(stream);
		emitter->lifetimeMax = StreamReadFloat(stream); 
		emitter->speedMin = StreamReadFloat(stream);
		emitter->speedMax = StreamReadFloat(stream);
		Vector2D scale;
		StreamReadVector2D(stream, &scale);
		emitter->scale = scale;
		emitter->mesh = MeshLibraryBuild(StreamReadToken(stream));
		DGL_Color tint;
		StreamReadColor(stream, &tint);
		emitter->tintColor = tint;
		// @@@TEMPORARY:       
		UNREFERENCED_PARAMETER(stream);
	}

	return;
}

// Update the ParticleEmitter.
// Params:
//	 ParticleEmitter = Pointer to the ParticleEmitter component.
//	 dt = Change in time (in seconds) since the last game loop.
static void ParticleEmitterUpdate(Component* component, float dt)
{
	// Verify that the ParticleEmitter pointer is valid.
	if (component)
	{
		ParticleEmitter* emitter = (ParticleEmitter*)component;

		// If a particle container doesn't exist, then create one.
		if (emitter->container == NULL)
		{
			// Create a ParticleContainer with a size of emitMax;
			emitter->container = ParticleContainerCreate(emitter->emitMax, emitter->areParticlesRecyclable);
		}

		// Check if there are any active particles.
		if (!ParticleContainerIsEmpty(emitter->container))
		{
			// Update all existing particles.
			ParticleContainerUpdate(emitter->container, dt);
		}

		//update the accumulator        
		emitter->emitAccumulator += emitter->emitRate * dt; 

		while (emitter->emitAccumulator >= 1) {            
			//SPAWN A PARTICLE            
			ParticleEmitterEmit(emitter);
			//DECRAMENT THE ACCUMULATOR       
			emitter->emitAccumulator--;    
		}

		if (ParticleContainerIsEmpty(emitter->container) && ParticleContainerIsFull(emitter->container))  
			EntityDestroy(component->parent);

		// @@@TEMPORARY:
		UNREFERENCED_PARAMETER(dt);
	}
}

static void ParticleEmitterRender(const Component* component)
{
	ParticleEmitter* emitter = (ParticleEmitter*)component;

	if (!ParticleContainerIsEmpty(emitter->container)) {
		DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT); 
		DGL_Graphics_SetCB_Alpha(1.0f);
		DGL_Graphics_SetCB_TintColor(&emitter->tintColor);
		ParticleContainerRender(emitter->container, emitter->mesh);
	}
	// @@@TEMPORARY:
}

// Attempt to emit a single, new particle.
// (Hint: No particle is emitted when the container is full.)
// (Hint: EVERY variable in the particle MUST be initialized.)
// Params:
//	 ParticleEmitter = Pointer to the ParticleEmitter component.
static void ParticleEmitterEmit(ParticleEmitter* emitter)
{
	if (!ParticleContainerIsFull(emitter->container)) {
		Particle* newParticle = ParticleContainerAllocateParticle(emitter->container);
		if (newParticle) {
			newParticle->lifetime = RandomRangeFloat(emitter->lifetimeMin, emitter->lifetimeMax);
			Entity* entity = ComponentGetParent(&emitter->base); 
			Transform* transform = EntityHas(entity, Transform);   
			Vector2DSet(&newParticle->position, TransformGetTranslation(transform)->x, TransformGetTranslation(transform)->y);
			Vector2DSet(&newParticle->scale, TransformGetScale(transform)->x, TransformGetScale(transform)->y);
			newParticle->rotation = 0.0f;
			float angle = RandomRangeFloat(0.0f, (float)M_PI * 2.0f);
			float speed = RandomRangeFloat(emitter->speedMin, emitter->speedMax);
			DGL_Vec2 velocity = { 0 };
			Vector2DFromAngleRad(&velocity, angle);
			Vector2DScale(&velocity, &velocity, speed);
			Vector2DSet(&newParticle->velocity, velocity.x, velocity.y);
		}
	}
}
