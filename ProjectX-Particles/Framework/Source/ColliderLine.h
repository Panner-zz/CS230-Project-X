//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 6
// Course:		CS230S24
//
// Copyright � 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Collider Collider;
typedef struct DGL_Vec2 Vector2D;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structures to be defined in ColliderLine.c.
#if 0
typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;

typedef struct ColliderLine
{
	// Inherit the base collider structure.
	Collider	base;

	// The number of line segments in the list.
	unsigned int		lineCount;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[cLineSegmentMax];

} ColliderLine;
#endif

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (line) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
Collider* ColliderLineCreate(void);

// Check for collision between a line collider and a circle collider.
// Params:
//	 collider1 = Pointer to the line collider component.
//	 collider2 = Pointer to the circle collider component.
// Returns:
//	 If the pointers are valid,
//	   then return the results of a line-circle collision check,
//	   else return false.
bool ColliderLineIsCollidingWithCircle(const Collider* collider, const Collider* other);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

