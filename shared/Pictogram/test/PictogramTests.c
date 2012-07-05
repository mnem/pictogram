//
//  PictogramTests.c
//  MobilePoly
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdio.h>
#include "CUnit/Basic.h"

#define ADD_TEST(suite, fn) CU_add_test((suite), (#fn), (fn))

void test_create_fragment_shader_from_file()
{
	
}

void test_create_fragment_shader_from_string()
{
	
}

void test_create_vertex_shader_from_file()
{
	
}

void test_create_vertex_shader_from_string()
{
	
}


int pgRunAllTests()
{
	CU_pSuite compilerSuite = NULL;
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();
	
	/* add a suite to the registry */
	compilerSuite = CU_add_suite("Program Compiler Suite", NULL, NULL);
	if (NULL == compilerSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == ADD_TEST(compilerSuite, test_create_fragment_shader_from_file)) ||
		(NULL == ADD_TEST(compilerSuite, test_create_fragment_shader_from_string)) ||
		(NULL == ADD_TEST(compilerSuite, test_create_vertex_shader_from_file)) ||
		(NULL == ADD_TEST(compilerSuite, test_create_vertex_shader_from_string)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
