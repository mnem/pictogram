//
//  PGRenderer.c
//  MobilePoly
//
//  Created by David Wagner on 23/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include <assert.h>
#include "Pictogram.h"

const int NUM_FRAME_BUFFERS = 1;
const int NUM_RENDER_BUFFERS = 1;

typedef struct _PGRenderer {
    GLuint renderbuffer;
    GLuint framebuffer;
	// Currently active settings
	PGProgram activeProgram;
} _PGRenderer;

static void clearRendererContext(PGRenderer renderer)
{
	memset(renderer, 0, sizeof(_PGRenderer));
	
	renderer->activeProgram = NULL;
}

PGResult pgRendererCreate(PGRenderer *renderer)
{
	if (NULL == renderer)
	{
		return PGR_NullPointerBarf;
	}
	*renderer = NULL;
	
	PGRenderer r = malloc(sizeof(_PGRenderer));
	if (NULL == r) return PGR_OutOfMemory;
	*renderer = r;
	clearRendererContext(r);
	
    // Create & bind the color buffer so that the caller can allocate its space.
    glGenRenderbuffers(NUM_RENDER_BUFFERS, &r->renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, r->renderbuffer);
	
	return PGR_OK;
}

void pgRendererDestroy(PGRenderer *renderer)
{
	if (NULL != renderer && NULL != *renderer)
	{
		PGRenderer r = *renderer;
		
		glDeleteFramebuffers(1, &r->framebuffer);
		glDeleteRenderbuffers(1, &r->renderbuffer);
		
		memset(r, 0, sizeof(_PGRenderer));
		free(r);
		
		*renderer = NULL;
	}
}


PGResult pgRendererSetup(PGRenderer renderer, int width, int height)
{
    // Create the framebuffer object and attach the color buffer.
    glGenFramebuffers(NUM_FRAME_BUFFERS, &renderer->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER,
                              renderer->renderbuffer);
    
    glViewport(0, 0, width, height);
	
	return PGR_OK;
}

PGResult pgRendererUseProgram(PGRenderer renderer, PGProgram program)
{
	if(program != renderer->activeProgram)
	{
		glUseProgram(pgProgramGlHandle(program));
	}
	return PGR_OK;
}
