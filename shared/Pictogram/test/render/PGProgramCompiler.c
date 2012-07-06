//
//  PGProgramCompiler.c
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include "CUnit/Basic.h"
#include "../TestHelpers.h"
#include "Pictogram.h"

static void test_create_fragment_shader_from_file(void)
{
	GLuint shader = 0;
	const char testFile[] = "test_01.fsh";
	const size_t rootLen = strlen(TestAssetsRoot + 1 /* '/' */);
	const size_t nameLen = strlen(testFile);
	const size_t pathLen =  rootLen + nameLen;
	char path[pathLen + 1];
	
	strncpy(path, TestAssetsRoot, pathLen);
	strncat(path, "/", pathLen);
	strncat(path, testFile, pathLen);
	
	pgCompileShaderFile(&shader, GL_FRAGMENT_SHADER, path, NULL);
	
	CU_ASSERT_NOT_EQUAL(shader, 0);
}

static void test_create_fragment_shader_from_string(void)
{
	CU_FAIL(__FUNCTION__ " not yet implemented");
}

static void test_create_vertex_shader_from_file(void)
{
	CU_FAIL(__FUNCTION__ " not yet implemented");
}

static void test_create_vertex_shader_from_string(void)
{
	CU_FAIL(__FUNCTION__ " not yet implemented");
}

CU_pSuite PGProgramCompilerTestSetup(void)
{
	CREATE_SUITE("Program Compiler", NULL, NULL);
	
	if(!suite ||
	   !ADD_TEST(suite, test_create_fragment_shader_from_file) ||
	   !ADD_TEST(suite, test_create_fragment_shader_from_string) ||
	   !ADD_TEST(suite, test_create_vertex_shader_from_file) ||
	   !ADD_TEST(suite, test_create_vertex_shader_from_string))
	{
		return NULL;
	}
	
	return suite;
}
