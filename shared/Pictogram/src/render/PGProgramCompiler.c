//
//  PGProgramCompiler.c
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include <assert.h>
#include "Pictogram.h"

#define UNKNOWN (1)
typedef struct {
	UT_hash_handle hh;  /* makes this structure hashable     */
	GLint location;     /* variable's location               */
	char name[UNKNOWN]; /* variable's name. Key for the hash. Actual size made by magic */
} PGProgramVariable;
#undef UNKNOWN

typedef struct _PGProgram {
	GLuint program; /* This must always be the first member so we can masquerade as a GLuint pointer */
	GLuint vertexShader;
	GLuint fragmentShader;
	
	GLchar *programLinkLog;
	GLchar *vertexShaderCompileLog;
	GLchar *fragmentShaderCompileLog;
	
	PGProgramVariable *attributesHash;
	PGProgramVariable *uniformsHash;
} _PGProgram;

static PGResult pgCompileShaderString(GLuint *outShader, GLenum type, const char *source, GLchar **outLog)
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
		*outShader = 0;
        return PGR_CouldNotCompileShader;
    }
	pgLogAnyGlErrors("Check shader log.");
	
	return PGR_OK;
}

static PGResult pgCompileShaderFile(GLuint *outShader, GLenum type, const char *file, GLchar **outLog)
{
	char *source = NULL;
	if(PGR_OK != pgCreateStringFromFile(&source, file))
	{
		pgLog(PGL_Error, "Could not load shader file %s", file);
		free(source);
		return PGR_CouldNotReadFile;
	}
	
	PGResult result = pgCompileShaderString(outShader, type, source, outLog);
	
	free(source);
	
	return result;
}

static PGResult pgLinkProgram(GLuint program, GLuint vertexShader, GLuint fragmentShader, GLchar **outLog)
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
		*outLog = log;
	}
    
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (GL_FALSE == status) 
	{
        return PGR_CouldNotLinkProgram;
    }
	
	return PGR_OK;
}

static GLint pgProgramVariableLocation(PGProgramVariable *hash, const char* name)
{
	if (NULL == hash || NULL == name) return -1;
	PGProgramVariable *var = NULL;
	
	HASH_FIND_STR(hash, name, var);
	
	if (NULL == var )
	{
		return -1;
	}
	else 
	{
		return var->location;
	}
}

PGResult pgProgramCreateAndBuild(PGProgram *program, const char *vertexSource, const char *fragmentSource)
{
	// Sanitise the params /////////////////////////////////////////////////
	if (NULL == program) return PGR_NullPointerBarf;
	*program = NULL;
	
	if (NULL == vertexSource || NULL == fragmentSource) return PGR_NullPointerBarf;
	
	// Create the structure to hold the details ////////////////////////////
	PGProgram p = malloc(sizeof(_PGProgram));
	memset(p, 0, sizeof(_PGProgram));
	if (NULL == p) return PGR_OutOfMemory;
	*program = p;
	
	// Load the shaders ////////////////////////////////////////////////////
	PGResult result;
	result = pgCompileShaderFile(&p->vertexShader, 
								 GL_VERTEX_SHADER, 
								 vertexSource, 
								 &p->vertexShaderCompileLog);
	if (PGR_OK != result) return result;

	result = pgCompileShaderFile(&p->fragmentShader, 
								 GL_FRAGMENT_SHADER, 
								 fragmentSource, 
								 &p->fragmentShaderCompileLog);
	if (PGR_OK != result) return result;

	// Link the program ////////////////////////////////////////////////////
    p->program = glCreateProgram();
	if (0 == p->program)
	{
		pgLogAnyGlErrors("Could not create new program.");
		return PGR_CouldNotCreateNewProgram;
	}
	
	return pgLinkProgram(p->program,
						 p->vertexShader,
						 p->fragmentShader,
						 &p->programLinkLog);	
}

void pgProgramDestroy(PGProgram *program)
{
	if (NULL != program)
	{
		PGProgram p = *program;
		
		// Free the shaders and program ////////////////////////////////////
		if (0 != p->vertexShader) glDeleteShader(p->vertexShader);
		if (0 != p->fragmentShader) glDeleteShader(p->fragmentShader);
		if (0 != p->program)
		{
			if (0 != p->vertexShader) glDetachShader(p->program, p->vertexShader);
			if (0 != p->fragmentShader) glDetachShader(p->program, p->fragmentShader);
			glDeleteProgram(p->program);
		}
		
		// Free the logs
		free(p->programLinkLog);
		free(p->vertexShaderCompileLog);
		free(p->fragmentShaderCompileLog);

		// Free the program variables hash /////////////////////////////////
		PGProgramVariable *currentAttrib, *tmpAttrib;
		HASH_ITER(hh, p->attributesHash, currentAttrib, tmpAttrib) 
		{
			HASH_DEL(p->attributesHash, currentAttrib);
			free(currentAttrib);
		}
		assert(p->attributesHash == NULL);

		PGProgramVariable *currentUniform, *tmpUniform;
		HASH_ITER(hh, p->uniformsHash, currentUniform, tmpUniform) 
		{
			HASH_DEL(p->uniformsHash, currentUniform);
			free(currentUniform);
		}
		assert(p->uniformsHash == NULL);

		// Get all the program variables ///////////////////////////////////
		// TODO

		// Free the PGProgram //////////////////////////////////////////////
		free(p);
		
		*program = NULL;
	}
}

GLuint pgProgramVertexShader(PGProgram program)
{
	if (NULL == program) return 0;
	
	return program->vertexShader;
}

GLuint pgProgramFragmentShader(PGProgram program)
{
	if (NULL == program) return 0;
	
	return program->fragmentShader;
}

const GLchar* pgProgramVertexShaderCompileLog(PGProgram program)
{
	if (NULL == program || NULL == program->vertexShaderCompileLog) return "";
	
	return program->vertexShaderCompileLog;
}

const GLchar* pgProgramFragmentShaderCompileLog(PGProgram program)
{
	if (NULL == program || NULL == program->fragmentShaderCompileLog) return "";

	return program->fragmentShaderCompileLog;
}

const GLchar* pgProgramLinkLog(PGProgram program)
{
	if (NULL == program || NULL == program->programLinkLog) return "";
	
	return program->programLinkLog;
}

GLint pgProgramAttribLocation(PGProgram program, const char* name)
{
	if (NULL == program) return -1;
	
	return pgProgramVariableLocation(program->attributesHash, name);
}

GLint pgProgramUniformLocation(PGProgram program, const char* name)
{
	if (NULL == program) return -1;
	
	return pgProgramVariableLocation(program->uniformsHash, name);
}
