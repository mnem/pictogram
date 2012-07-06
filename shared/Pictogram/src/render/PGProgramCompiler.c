//
//  PGProgramCompiler.c
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/stat.h>
#include "Pictogram.h"

static char* mallocStringFromFile(const char *file)
{
	struct stat fileStat = { 0 };
	char *string = NULL;
	
	pgLog(PGL_Info, "stat'ing %s", file);
	if (0 == stat(file, &fileStat))
	{
		pgLog(PGL_Info, "%s size: %iB", file, fileStat.st_size);
		
		FILE *f = fopen(file, "r");
		if (NULL != f) 
		{
			string = (char*)malloc(fileStat.st_size + 1);
			fread(string, 1, fileStat.st_size, f);
			string[fileStat.st_size] = 0;
		}
	}
	
	return string;
}

PGResult pgCompileShaderFile(GLuint *shader, GLenum type, const char *file, GLchar *log)
{
	char *source = mallocStringFromFile(file);
	
	if (NULL == source)
	{
		return PGR_LazyGenericError;
	}
	
	pgLog(PGL_Info, "Shader source:\n%s", source);
	
	free(source);
	
	return PGR_OK;
}

PGResult pgompileShaderString(GLuint *shader, GLenum type, const char *source, GLchar *log)
{
	return PGR_OK;
}
