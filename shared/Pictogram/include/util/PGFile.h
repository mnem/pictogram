//
//  PGFile.h
//
//  Created by David Wagner on 06/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PGFile_h
#define PGFile_h

#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * Reads the full contents of a file into a newly
 * allocated string.
 *
 * You must free() the string when you are finished
 * with it.
 */
PGResult pgMallocStringFromFile(char **outString, const char *file);
	
#ifdef __cplusplus
}
#endif


#endif
