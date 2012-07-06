//
//  TestHelpers.h
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef TestHelpers_h
#define TestHelpers_h

#ifdef __cplusplus
extern "C" {
#endif
	
extern const char *TestAssetsRoot;

#define CREATE_SUITE(name, initFn, cleanFn) CU_pSuite suite = CU_add_suite((name " suite"), (initFn), (cleanFn))
#define ADD_TEST(suite, fn) CU_add_test((suite), (#fn), (fn))

#ifdef __cplusplus
}
#endif

#endif
