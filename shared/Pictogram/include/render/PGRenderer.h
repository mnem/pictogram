//
//  PGRenderer.h
//
//  Created by David Wagner on 23/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PGRenderer_h
#define PGRenderer_h

#ifdef __cplusplus
extern "C" {
#endif
	
	PGResult pgRendererCreate(PGRenderer *renderer);
	void pgRendererDestroy(PGRenderer *renderer);
	
	PGResult pgRendererSetup(PGRenderer renderer, int width, int height);
	
	PGResult pgRendererUseProgram(PGRenderer renderer, PGProgram program);
	
	
#ifdef __cplusplus
}
#endif

#endif
