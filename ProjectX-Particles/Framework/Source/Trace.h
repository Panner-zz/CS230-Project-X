//------------------------------------------------------------------------------
//
// File Name:	Trace.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
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

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

typedef enum
{
	SeverityNone = 0,
	SeverityDebug,
	SeverityVerbose,
	SeverityInfo,
	SeverityWarning,
	SeverityError,

} TraceSeverity;

//------------------------------------------------------------------------------
// Public Macros:
//------------------------------------------------------------------------------

#define TraceDebug(formatString, ...) Trace(SeverityDebug, formatString, __VA_ARGS__);
#define TraceVerbose(formatString, ...) Trace(SeverityVerbose, formatString, __VA_ARGS__);
#define TraceInfo(formatString, ...) Trace(SeverityInfo, formatString, __VA_ARGS__);
#define TraceMessage(formatString, ...) Trace(SeverityInfo, formatString, __VA_ARGS__);
#define TraceWarning(formatString, ...) Trace(SeverityWarning, formatString, __VA_ARGS__);
#define TraceError(formatString, ...) Trace(SeverityError, formatString, __VA_ARGS__);

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

// Initialize the Tracing/Logging module.
// - Open "trace.log" for writing in text mode.
// - Perform error handling in the event that the file failed to open.
void TraceInit(TraceSeverity severityLevel);

// Set the minimum severity level for the tracing/logging.
TraceSeverity TraceSetSeverityLevel(TraceSeverity severityLevel);

// Output a message to the Tracing/Logging file.
// - Print the given message to the file if it was opened successfully.
// - Every message must be printed on its own line.
// - There must be no blank lines between messages.
void Trace(TraceSeverity level, const char * formatString, ...);

// Shutdown the Tracing/Logging module.
// - Close the file if-and-only-if the file was opened successfully
void TraceShutdown();

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

