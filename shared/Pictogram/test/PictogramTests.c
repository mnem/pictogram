//
//  PictogramTests.c
//  MobilePoly
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include "CUnit/Basic.h"

extern CU_pSuite PGProgramCompilerTestSetup(void);

const char *TestAssetsRoot = "";

char *pgMallocTestAssetPath(const char *file)
{
	const size_t rootLen = strlen(TestAssetsRoot + 1 /* '/' */);
	const size_t nameLen = strlen(file);
	const size_t pathLen =  rootLen + nameLen;
	char *path = malloc(pathLen + 1);
	strncpy(path, TestAssetsRoot, pathLen);
	strncat(path, "/", pathLen);
	strncat(path, file, pathLen);
	return path;
}

int pgRunAllTests(const char *testAssetsRoot)
{
	TestAssetsRoot = testAssetsRoot;
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	if (PGProgramCompilerTestSetup()) 
	{
		/* Run all tests using the CUnit Basic interface */
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
	}

	CU_cleanup_registry();
	return CU_get_error();
}
