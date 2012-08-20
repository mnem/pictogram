//
//  PGContext.h
//
//  Created by David Wagner on 23/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PGContext_h
#define PGContext_h

#ifdef __cplusplus
extern "C" {
#endif

PGResult pgContextCreate(PGContext *context);
void pgContextDestroy(PGContext *context);

#ifdef __cplusplus
}
#endif

#endif
