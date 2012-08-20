//
//  PGDataTypes.h
//
//  Created by David Wagner on 04/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PGDataTypes_h
#define PGDataTypes_h

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum
	{
		PGR_OK = 0 // The only one with a guaranteed value.
	,	PGR_LazyGenericError
	,	PGR_CouldNotReadFile
	,	PGR_CouldNotCreateShader
	,	PGR_CouldNotCompileShader
	,	PGR_NullPointerBarf
	,	PGR_CouldNotCreateNewProgram
	,	PGR_InvalidProgram
	,	PGR_CouldNotLinkProgram
	,   PGR_OutOfMemory
	}
	PGResult;

	typedef enum
	{
		PGL_Debug,
		PGL_Info,
		PGL_Warn,
		PGL_Error,
		PGL_Fatal
	}
	PGLogLevel;

	typedef struct _PGContext* PGContext;
	typedef struct _PGProgram* PGProgram;
	typedef struct _PGRenderer* PGRenderer;
	
#ifdef __cplusplus
}
#endif

#endif
