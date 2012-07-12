//
//  PGFile.c
//
//  Created by David Wagner on 06/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#include "Pictogram.h"

PGResult pgCreateStringFromFile(char **outString, const char *file)
{
	if (NULL == outString) 
	{
		return PGR_NullPointerBarf;
	}
	
	// So that the out value is always safe to free()
	*outString = NULL;
	
	if (NULL == file) 
	{
		return PGR_NullPointerBarf;
	}
	
	struct stat status = { 0 };
	if (0 != stat(file, &status))
	{
		pgLog(PGL_Error, "Could not get status for %s: %s", 
			  file, strerror(errno));
		return PGR_CouldNotReadFile;
	}
	
	FILE *filePointer = fopen(file, "r");
	if (NULL == filePointer)
	{
		pgLog(PGL_Error, "Could not open %s: %s", 
			  file, strerror(errno));
		return PGR_CouldNotReadFile;
	}
	
	char *string = (char*)malloc(status.st_size + 1);
	size_t read = fread(string, 1, status.st_size, filePointer);
	int closeErr = fclose(filePointer);
	string[read] = 0;
	*outString = string;
	
	// Treat size mis-matches and failures to close as warnings.
	// I may live to regret this decision.
	if (read != status.st_size)
	{
		pgLog(PGL_Warn, "Did not read all bytes in %s. Expected to read %d, but read %d", 
			  file, read, status.st_size);
	}
	if (0 != closeErr)
	{
		pgLog(PGL_Warn, "Failed to close %s: %s", 
			  file, strerror(errno));
	}
	
	return PGR_OK;
}
