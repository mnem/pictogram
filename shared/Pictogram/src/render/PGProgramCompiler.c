//
//  PGProgramCompiler.c
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include "Pictogram.h"

PGResult pgCompileShaderFile(GLuint *outShader, GLenum type, const char *file, GLchar **outLog)
{
	char *source = NULL;
	if(PGR_OK != pgMallocStringFromFile(&source, file))
	{
		pgLog(PGL_Error, "Could not load shader file %s", file);
		free(source);
		return PGR_CouldNotReadFile;
	}
	
	PGResult result = pgCompileShaderString(outShader, type, source, outLog);
	
	free(source);
	
	return result;
}

PGResult pgCompileShaderString(GLuint *outShader, GLenum type, const char *source, GLchar **outLog)
{
	if (NULL == outShader)
	{
		return PGR_NullPointerBarf;
	}
	
	pgLogAnyGlErrors("Preparing to create and compile shader.");
    *outShader = glCreateShader(type);
	if (0 == *outShader)
	{
		pgLogAnyGlErrors("Could not create shader.");
		return PGR_CouldNotCreateShader;
	}
    glShaderSource(*outShader, 1, &source, NULL);
    glCompileShader(*outShader);
	pgLogAnyGlErrors("Compile shader.");

	if (NULL != outLog)
	{
		GLint logLength;
		GLchar *log = NULL;
		glGetShaderiv(*outShader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) 
		{
			log = (GLchar *)malloc(logLength * sizeof(GLchar));
			glGetShaderInfoLog(*outShader, logLength, &logLength, log);
		}
		else 
		{
			log = (GLchar *)malloc(sizeof(GLchar));
			log[0] = 0;
		}
		*outLog = log;
	}
    
    GLint status;
    glGetShaderiv(*outShader, GL_COMPILE_STATUS, &status);
    if (GL_FALSE == status) 
	{
        glDeleteShader(*outShader);
        return PGR_CouldNotCompileShader;
    }
	pgLogAnyGlErrors("Check shader log.");

	return PGR_OK;
}

PGResult pgCreateAndLinkProgram(GLuint *outProgram, GLuint vertexShader, GLuint fragmentShader, GLchar **outLog)
{
	if (NULL == outProgram)
	{
		return PGR_NullPointerBarf;
	}
	
    *outProgram = glCreateProgram();
	if (0 == *outProgram)
	{
		pgLogAnyGlErrors("Could not create new program.");
		return PGR_CouldNotCreateNewProgram;
	}
	
	PGResult result = pgLinkProgram(*outProgram, vertexShader, fragmentShader, outLog);
	
	if (PGR_OK != result)
	{
		glDeleteProgram(*outProgram);
		*outProgram = 0;
	}
	
	return result;
}

PGResult pgLinkProgram(GLuint program, GLuint vertexShader, GLuint fragmentShader, GLchar **outLog)
{
	if (0 == program)
	{
		return PGR_InvalidProgram;
	}

	if (0 != vertexShader)
	{
		glAttachShader(program, vertexShader);
	}
	if (0 != fragmentShader)
	{
		glAttachShader(program, fragmentShader);
	}
	
	// TODO: Somehow, we should be able to specify attrib locations. here.
	// I suspect what I'll do is pass in a hash of string names to GLints.
	// If it has a value of -1, it'll be fetched after the program is linked
	// with glGetAttribLocation, otherwise it will be bound with 
	// glBindAttribLocation
	
	pgLogAnyGlErrors("About to link program.");
	glLinkProgram(program);
	pgLogAnyGlErrors("Linked program.");
	
	if (NULL != outLog)
	{
		GLint logLength;
		GLchar *log = NULL;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) 
		{
			log = (GLchar *)malloc(logLength * sizeof(GLchar));
			glGetProgramInfoLog(program, logLength, &logLength, log);
		}
		else 
		{
			log = (GLchar *)malloc(sizeof(GLchar));
			log[0] = 0;
		}
		*outLog = log;
	}
    
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (GL_FALSE == status) 
	{
        return PGR_CouldNotLinkProgram;
    }
	
	// TODO: Should I be doing this here?
	if (0 != vertexShader)
	{
		glDetachShader(program, vertexShader);
	}
	if (0 != fragmentShader)
	{
		glDetachShader(program, fragmentShader);
	}
	
	return PGR_OK;
}
