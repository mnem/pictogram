//
//  PGContext.c
//  MobilePoly
//
//  Created by David Wagner on 23/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include <assert.h>
#include "Pictogram.h"

typedef struct _PGContext {
	int nothing;
} _PGContext;

PGResult pgContextCreate(PGContext *context)
{
	if (NULL == context)
	{
		return PGR_NullPointerBarf;
	}
	*context = NULL;
	
	PGContext c = malloc(sizeof(_PGContext));
	if (NULL == c) return PGR_OutOfMemory;
	*context = c;
	memset(c, 0, sizeof(_PGContext));
	
	return PGR_OK;
}

void pgContextDestroy(PGContext *context)
{
	if (NULL != context && NULL != *context)
	{
		PGContext c = *context;
		
		memset(c, 0, sizeof(_PGContext));
		free(c);
		
		*context = NULL;
	}
}
