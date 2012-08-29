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

struct PGContextPrivate {
	int nothing;
};

PGResult pgContextCreate(PGContext *context)
{
	if (NULL == context)
	{
		return PGR_NullPointerBarf;
	}
	*context = NULL;
	
	PGContext c = malloc(sizeof(struct PGContextPrivate));
	if (NULL == c) return PGR_OutOfMemory;
	*context = c;
	memset(c, 0, sizeof(struct PGContextPrivate));
	
	return PGR_OK;
}

void pgContextDestroy(PGContext *context)
{
	if (NULL != context && NULL != *context)
	{
		PGContext c = *context;
		
		memset(c, 0, sizeof(struct PGContextPrivate));
		free(c);
		
		*context = NULL;
	}
}
