//------------------------------------------------------------------------------
//
// File Name:	Main.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project X
// Course:		CS230S24
//
// Copyright � 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DGL.h"

#include "CheatSystem.h"
#include "Engine.h"
#include "ParticleSystem.h"
#include "PlatformSystem.h"
#include "SceneSystem.h"

//------------------------------------------------------------------------------
// Main Function:
//------------------------------------------------------------------------------

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR command_line, _In_ int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	//--------------------------------------------------------------------------
	// Add all systems to the engine framework.
	//--------------------------------------------------------------------------

	// NOTE: Certain modules need to be initialized first.
	EngineAddSystem(PlatformSystemGetInstance(hInstance, show));
#ifdef _DEBUG
	EngineAddSystem(CheatSystemGetInstance());
#endif
	EngineAddSystem(SceneSystemGetInstance());
	EngineAddSystem(ParticleSystemGetInstance());

	// NOTE: Other systems can be initialized later and in any order.

	// Initialize the game engine.
	bool running = EngineInit();

	float dt = 0.0f;

	//--------------------------------------------------------------------------
	// MAIN LOOP: Do not exit until instructed to do so by the DGL or Scene System.
	//--------------------------------------------------------------------------
	while (running)
	{
		// Update the current frame.
		EngineUpdate(dt);

		// Render the current frame.
		EngineRender();

		if (!DGL_System_DoesWindowExist() || !SceneSystemIsRunning())
			running = FALSE;

		dt = (float)DGL_System_GetDeltaTime();

#if 0	// CLAMP DT
		dt = min(dt, 0.05f);
#endif
	}

	// Shutdown the game engine.
	EngineExit();

	// Return error code 0 ("no error").
	return 0;
}