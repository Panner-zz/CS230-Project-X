//------------------------------------------------------------------------------
//
// File Name:	BehaviorCursor.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project X
// Course:		CS230SU24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "DGL.h"

#include "Behavior.h"
#include "BehaviorCursor.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Physics.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct BehaviorCursor
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add asteroid-specific behavior variables.
	float velocityMax;	// Units: pixels/second

} BehaviorCursor;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorCursorRead(Component* component, Stream stream);
static void BehaviorCursorUpdatePosition(Behavior* behavior, float dt);
static void BehaviorCursorSpawnExplosion(Behavior* behavior);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new behavior (Spaceship) component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorCursorCreate(void)
{
	Behavior* component = calloc(1, sizeof(BehaviorCursor));

	if (component)
	{
		// Initialize the base Component variables.
		component->base.memorySize = sizeof(BehaviorCursor);
		component->base.type = cBehavior;
		component->base.read = BehaviorCursorRead;
		component->base.update = BehaviorUpdate;
		component->base.free = ComponentFree;
		
		// Initialize the base Behavior variables.
		component->onInit = BehaviorCursorInit;
		component->onUpdate = BehaviorCursorUpdate;
		component->onExit = BehaviorCursorExit;
	}

	// Cast the derived behavior to a base behavior component.
	return (Behavior*)component;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Read the properties of a Behavior component from a file.
// (NOTE: Read the base Behavior values using BehaviorRead.)
// (NOTE: Read the cursor's velocity using StreamReadFloat.)
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
static void BehaviorCursorRead(Component* component, Stream stream)
{
	// Read the base Behavior values.
	BehaviorRead(component, stream);

	// Read the cursor's velocity.
	BehaviorCursor* cursor = (BehaviorCursor*)component;
	cursor->velocityMax = StreamReadFloat(stream);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
static void BehaviorCursorInit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
static void BehaviorCursorUpdate(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(behavior);

	// Update the cursor's position any time one of the WASD keys is held down.
	BehaviorCursorUpdatePosition(behavior, dt);

	if (DGL_Input_KeyTriggered(' '))
	{
		BehaviorCursorSpawnExplosion(behavior);
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
static void BehaviorCursorExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorCursorUpdatePosition(Behavior* behavior, float dt)
{
	// This function should update the cursor's position, either using Physics velocity or Transform translation.

	// Get the cursor's Physics component.
	Physics* physics = EntityHas(behavior->base.parent, Physics);

	// Cast the behavior pointer to a new pointer of type "BehaviorCursor*".
	BehaviorCursor* cursor = (BehaviorCursor*)behavior;

	Vector2D velocity = (Vector2D){ 0, 0 };

	if (DGL_Input_KeyDown('W'))
	{
		velocity.y = cursor->velocityMax;
	}
	else if (DGL_Input_KeyDown('S'))
	{
		velocity.y = -cursor->velocityMax;
	}

	if (DGL_Input_KeyDown('A'))
	{
		velocity.x = -cursor->velocityMax;
	}
	else if (DGL_Input_KeyDown('D'))
	{
		velocity.x = cursor->velocityMax;
	}

	PhysicsSetVelocity(physics, &velocity);
}

static void BehaviorCursorSpawnExplosion(Behavior* behavior)
{
	// Get a cloned entity from the entity factory.
	Entity* entity = EntityFactoryBuild("Explosion");

	// Check for a valid Entity pointer.
	if (entity)
	{
		// Get the cursor's position.
		Transform* transform = EntityHas(behavior->base.parent, Transform);
		const Vector2D* position = TransformGetTranslation(transform);

		// Set the cloned entity's position.
		transform = EntityHas(entity, Transform);
		TransformSetTranslation(transform, position);

		// Add the cloned entity to the Scene.
		SceneAddEntity(entity);
	}
}
