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
    *outShader = glCreateShader(type);
    glShaderSource(*outShader, 1, &source, NULL);
    glCompileShader(*outShader);

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

	return PGR_OK;
}
