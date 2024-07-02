//------------------------------------------------------------------------------
//
// File Name:	Behavior.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project X
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Entity Entity;
typedef struct Behavior Behavior;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*BehaviorFunctionPtr)(Behavior* behavior);
typedef void(*BehaviorFunctionPtrDt)(Behavior* behavior, float dt);

// This structure is being declared publicly, as it will be used to implement
// pseudo-inheritance in Project 5.
// NOTE: You are not allowed to change the contents of this structure, as it is
// part of the public interface.
typedef struct Behavior
{
	Component base;

	// Behavior Finite-State Machine (FSM) state variables.
	int stateCurr;
	int stateNext;

		// Behavior Finite-State Machine (FSM) function pointers.
	BehaviorFunctionPtr		onInit;
	BehaviorFunctionPtrDt	onUpdate;
	BehaviorFunctionPtr		onExit;

	// Additional variables shared by all behaviors.
	// NOTE: Variables that are unique to a specific behavior should not be placed here.

	// Generic timer - may be used as a life timer or a weapon cooldown timer.
	// (Default = 0, means infinite amount of time remaining or weapon can be fired.)
	float	timer;

} Behavior;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Read the properties of a Behavior component from a file.
// [NOTE: Read the stateCurr and stateNext values using StreamReadInt.]
// [NOTE: Read the timer value using StreamReadFloat.]
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
void BehaviorRead(Component* component, Stream stream);

// Update the Behavior component.
// (Hint: Refer to the Word document for detailed instructions regarding this function.)
// Params:
//	 behavior = Pointer to the Behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUpdate(Component* component, float dt);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

