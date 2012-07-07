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

enum
{
	PGR_OK = 0 // The only one with a guaranteed value.
,	PGR_LazyGenericError
,	PGR_CouldNotReadFile
,	PGR_CouldNotCompileShader
,	PGR_NullPointerBarf
}
PGResultEnum;

enum
{
	PGL_Debug,
	PGL_Info,
	PGL_Warn,
	PGL_Error,
	PGL_Fatal
}
PGLogLevelEnum;

typedef int PGResult;
typedef int PGLogLevel;
typedef struct LMPContext LMPContext;
	
#ifdef __cplusplus
}
#endif

#endif
