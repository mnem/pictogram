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
	GLint size;
	GLenum type;
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

/**
 * Used to pass the relevant Attrib or Uniform query function for extracting variables
 * from programs
 */
typedef void (*GetVariableFn)(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);

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

static GLenum pgProgramVariableType(PGProgramVariable *hash, const char* name)
{
	// TODO: Check if it's actually valid to return GL_INVALID_ENUM, that
	// is it won't clash with an enum. I'm not sure what context it's
	// used in (i.e. is it an error code or an enum?)
	if (NULL == hash || NULL == name) return GL_INVALID_ENUM;
	PGProgramVariable *var = NULL;
	
	HASH_FIND_STR(hash, name, var);
	
	if (NULL == var )
	{
		return GL_INVALID_ENUM;
	}
	else 
	{
		return var->type;
	}
}

static GLsizei pgProgramVariableSize(PGProgramVariable *hash, const char* name)
{
	if (NULL == hash || NULL == name) return 0;
	PGProgramVariable *var = NULL;
	
	HASH_FIND_STR(hash, name, var);
	
	if (NULL == var )
	{
		return 0;
	}
	else 
	{
		return var->size;
	}
}

static void destroyProgramVariablesHash(PGProgramVariable **hash_)
{
	if (NULL != hash_) 
	{
		PGProgramVariable *current, *temp, *hash = *hash_;
		HASH_ITER(hh, hash, current, temp) 
		{
			HASH_DEL(hash, current);
			free(current);
		}
		*hash_ = hash;
	}
}

static void extractVariables(GLuint program, PGProgramVariable **hash_, GetVariableFn get, GLint numVariables, GLint maxVariableLength)
{
	destroyProgramVariablesHash(hash_);
	
	if (NULL != hash_ && numVariables > 0 && maxVariableLength > 0)
	{		
		PGProgramVariable *hash = *hash_;
		assert(HASH_COUNT(hash) == 0);
		GLchar name[maxVariableLength];
		for (GLuint attrIndex = 0; attrIndex < numVariables; attrIndex++)
		{
			GLsizei nameLength = 0;
			GLint size = 0;
			GLenum type = 0;
			
			get(program, attrIndex, maxVariableLength, &nameLength, &size, &type, name);
			if (nameLength > 0)
			{
				size_t recordSize = offsetof(PGProgramVariable, name) + nameLength + 1; /* nul */
				PGProgramVariable *var = malloc(recordSize);
				memset(var, 0, recordSize);
				var->location = attrIndex;
				var->size = size;
				if (var->size > 1)
				{
					pgLog(PGL_Warn, "%s is an array or struct. Pictogram doesn't handle those well yet - you will have to use traditional GL functions to manipulate it.", name);
				}
				var->type = type;
				strncpy(var->name, name, nameLength + 1);
				HASH_ADD_STR(hash, name, var);
			}
			else 
			{
				pgLogAnyGlErrors("Failed to get active program variable");
			}
			
		}
		*hash_ = hash;
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
	if (NULL == p) return PGR_OutOfMemory;
	*program = p;
	memset(p, 0, sizeof(_PGProgram));
	
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
	
	result = pgLinkProgram(p->program,
						   p->vertexShader,
						   p->fragmentShader,
						   &p->programLinkLog);	
	if (PGR_OK != result) return result;
	
	// Fetch the attributes ////////////////////////////////////////////////
	GLint numVars;
	GLint varNameMax;
	
	glGetProgramiv(p->program, GL_ACTIVE_ATTRIBUTES, &numVars);
	glGetProgramiv(p->program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &varNameMax);
	extractVariables(p->program, &p->attributesHash, glGetActiveAttrib, numVars, varNameMax); 
	
	glGetProgramiv(p->program, GL_ACTIVE_UNIFORMS, &numVars);
	glGetProgramiv(p->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &varNameMax);
	extractVariables(p->program, &p->uniformsHash, glGetActiveUniform, numVars, varNameMax); 
	
	return PGR_OK;
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
		destroyProgramVariablesHash(&p->attributesHash);
		assert(p->attributesHash == NULL);

		destroyProgramVariablesHash(&p->uniformsHash);
		assert(p->uniformsHash == NULL);

		// Free the PGProgram //////////////////////////////////////////////
		memset(p, 0, sizeof(_PGProgram));
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

GLint pgProgramAttribCount(PGProgram program)
{
	if (NULL == program) return -1;
	
	return HASH_COUNT(program->attributesHash);
}

GLint pgProgramAttribLocation(PGProgram program, const char* name)
{
	if (NULL == program) return -1;
	
	return pgProgramVariableLocation(program->attributesHash, name);
}

GLenum pgProgramAttribType(PGProgram program, const char* name)
{
	if (NULL == program) return GL_INVALID_ENUM;
	
	return pgProgramVariableType(program->attributesHash, name);
}

GLsizei pgProgramAttribSize(PGProgram program, const char* name)
{
	if (NULL == program) return 0;
	
	return pgProgramVariableSize(program->attributesHash, name);
}

GLint pgProgramUniformCount(PGProgram program)
{
	if (NULL == program) return -1;
	
	return HASH_COUNT(program->uniformsHash);
}

GLint pgProgramUniformLocation(PGProgram program, const char* name)
{
	if (NULL == program) return -1;
	
	return pgProgramVariableLocation(program->uniformsHash, name);
}

GLenum pgProgramUniformType(PGProgram program, const char* name)
{
	if (NULL == program) return GL_INVALID_ENUM;
	
	return pgProgramVariableType(program->uniformsHash, name);
}

GLsizei pgProgramUniformSize(PGProgram program, const char* name)
{
	if (NULL == program) return 0;
	
	return pgProgramVariableSize(program->uniformsHash, name);
}
