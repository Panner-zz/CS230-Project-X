//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project X
// Course:		CS230SU23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {	// Assume C declarations for C++.
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Component Component;
typedef struct Entity Entity;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

typedef enum ComponentType
{
	cTransform,
	cAnimation,
	cBehavior,
	cCollider,
	cPhysics,
	cSprite,
	cParticleEmitter,

} ComponentType;

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

typedef void(*ComponentFunctionPtr)(const Component* component);
typedef void(*ComponentFunctionPtrDt)(Component* component, float dt);
typedef void(*ComponentFunctionPtrStream)(Component* component, Stream stream);
typedef void(*ComponentFunctionFree)(Component** component);

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Component
{
	ComponentType type;

	// Pointer to the parent Entity associated with this component.
	Entity* parent;

	// Component Finite-State Machine (FSM) function pointers.

	ComponentFunctionPtrStream read;
	ComponentFunctionPtrDt	update;
	ComponentFunctionPtr	render;
	ComponentFunctionFree	free;

	Component* (*clone)(const Component* component);

	// The following variable is used to allocate the correct amount of memory
	//   when cloning a component.  Cloning of "derived" components will
	//   fail if insufficient memory is allocated.
	unsigned memorySize;

} Component;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Component* ComponentClone(const Component* other);
void ComponentFree(Component** component);

Entity* ComponentGetParent(const Component* component);
void ComponentSetParent(Component* component, Entity* entity);

void ComponentRead(Component* component, Stream stream);
void ComponentUpdate(Component* component, float dt);
void ComponentRender(const Component* component);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       // End of extern "C" {
#endif

