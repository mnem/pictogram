//
//  PGLog.c
//  MobilePoly
//
//  Created by David Wagner on 06/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdio.h>
#include <stdarg.h>
#include "Pictogram.h"

static PGLogLevel MinimumLogLevel = PGL_Fatal;

void _pgLogv(PGLogLevel level, const char *fmt, ...)
{
	if (level >= MinimumLogLevel) 
	{
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
}

void pgSetLoggingLevel(PGLogLevel level)
{
	MinimumLogLevel = level;
}

void _pgLogAnyGlErrorsv(const char *fmt, ...)
{
	GLenum error = glGetError();
	if (GL_NO_ERROR != error)
	{
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
		
		switch (error) 
		{
			case GL_INVALID_ENUM:     
				printf("  Invalid enum.\n");
				break;
			case GL_INVALID_VALUE:    
				printf("  Invalid value.\n");
				break;
			case GL_INVALID_OPERATION:
				printf("  Invalid operation.\n");
				break;
			case GL_OUT_OF_MEMORY:    
				printf("  Out of memory.\n");
				break;
				
			default:
				printf("  Unrecognised gl error code: %d\n", error);
				break;
		}
	}
}
