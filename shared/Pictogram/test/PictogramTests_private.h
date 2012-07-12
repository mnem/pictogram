//
//  TestHelpers.h
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PictogramTests_private_h
#define PictogramTests_private_h

#ifdef __cplusplus
extern "C" {
#endif

#include "CUnit/Basic.h"
#include "Pictogram.h"

/**
 * Test suite creators
 */
extern CU_pSuite pgTestSuiteCreateProgramCompiler(void);
extern CU_pSuite pgTestSuiteCreateFile(void);
	
/**
 * You must free() the returned string when finished.
 */
extern char *pgTestsCreateAssetPath(const char *file);

#define CREATE_SUITE(name, initFn, cleanFn) CU_pSuite suite = CU_add_suite((name " suite"), (initFn), (cleanFn))
#define ADD_TEST(suite, fn) CU_add_test((suite), (#fn), (fn))

#ifdef __cplusplus
}
#endif

#endif
