//
//  PictogramTests.c
//  MobilePoly
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include "PictogramTests_private.h"

static char *TestAssetsRoot = NULL;

char *pgTestsCreateAssetPath(const char *file)
{
	const size_t pathLen =  strlen(TestAssetsRoot) + strlen(file) + 1;
	char *path = malloc(pathLen);

	strncpy(path, TestAssetsRoot, pathLen);
	strncat(path, file, pathLen);

	return path;
}

int pgTestRunAll(const char *testAssetsRoot)
{
	size_t pathLength = strlen(testAssetsRoot) + 2; /* '/', 0x00 */
	TestAssetsRoot = malloc(pathLength);
	strncpy(TestAssetsRoot, testAssetsRoot, pathLength);
	strncat(TestAssetsRoot, "/", pathLength);
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	if (pgTestSuiteCreateProgramCompiler()
		&& pgTestSuiteCreateFile()
		) 
	{
		/* Run all tests using the CUnit Basic interface */
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
	}

	CU_cleanup_registry();
	
	free(TestAssetsRoot);
	TestAssetsRoot = NULL;
	
	return CU_get_error();
}
