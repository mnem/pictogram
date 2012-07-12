//
//  PGProgramCompiler.c
//
//  Created by David Wagner on 05/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "../PictogramTests_private.h"

static void test_create_new_program(void)
{
	char *vertexShader = pgTestsCreateAssetPath("test_01.vsh");
	char *fragmentShader = pgTestsCreateAssetPath("test_01.fsh");
	PGProgram program = NULL;
	
	PGResult result = pgProgramCreateAndBuild(&program, vertexShader, fragmentShader);

	CU_ASSERT_EQUAL(result, PGR_OK);
	
	CU_ASSERT_NOT_EQUAL(pgProgramVertexShader(program), 0);
	CU_ASSERT_NOT_EQUAL(pgProgramFragmentShader(program), 0);
	CU_ASSERT_NOT_EQUAL(pgProgramGlHandle(program), 0);
	
	CU_ASSERT_PTR_NOT_NULL(pgProgramVertexShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramFragmentShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramLinkLog(program));
	
	CU_ASSERT_EQUAL(strlen(pgProgramVertexShaderCompileLog(program)), 0);
	CU_ASSERT_EQUAL(strlen(pgProgramFragmentShaderCompileLog(program)), 0);
	CU_ASSERT_EQUAL(strlen(pgProgramLinkLog(program)), 0);

	pgProgramDestroy(&program);
	
	CU_ASSERT_PTR_NULL(program);
	
	free(vertexShader);
	free(fragmentShader);
}

static void test_create_new_program_with_broken_vertex_shader(void)
{
	char *vertexShader = pgTestsCreateAssetPath("test_02_broken.vsh");
	char *fragmentShader = pgTestsCreateAssetPath("test_01.fsh");
	PGProgram program = NULL;
	
	PGResult result = pgProgramCreateAndBuild(&program, vertexShader, fragmentShader);
	
	CU_ASSERT_EQUAL(result, PGR_CouldNotCompileShader);
	
	CU_ASSERT_EQUAL(pgProgramVertexShader(program), 0);
	CU_ASSERT_EQUAL(pgProgramFragmentShader(program), 0);
	CU_ASSERT_EQUAL(pgProgramGlHandle(program), 0);
	
	CU_ASSERT_PTR_NOT_NULL(pgProgramVertexShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramFragmentShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramLinkLog(program));
	
	CU_ASSERT_NOT_EQUAL(strlen(pgProgramVertexShaderCompileLog(program)), 0);
	CU_ASSERT_EQUAL(strlen(pgProgramFragmentShaderCompileLog(program)), 0);
	CU_ASSERT_EQUAL(strlen(pgProgramLinkLog(program)), 0);

	pgProgramDestroy(&program);
	
	CU_ASSERT_PTR_NULL(program);
	
	free(vertexShader);
	free(fragmentShader);
}

static void test_create_new_program_with_broken_fragment_shader(void)
{
	char *vertexShader = pgTestsCreateAssetPath("test_01.vsh");
	char *fragmentShader = pgTestsCreateAssetPath("test_02_broken.fsh");
	PGProgram program = NULL;
	
	PGResult result = pgProgramCreateAndBuild(&program, vertexShader, fragmentShader);
	
	CU_ASSERT_EQUAL(result, PGR_CouldNotCompileShader);
	
	CU_ASSERT_NOT_EQUAL(pgProgramVertexShader(program), 0);
	CU_ASSERT_EQUAL(pgProgramFragmentShader(program), 0);
	CU_ASSERT_EQUAL(pgProgramGlHandle(program), 0);
	
	CU_ASSERT_PTR_NOT_NULL(pgProgramVertexShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramFragmentShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramLinkLog(program));
	
	CU_ASSERT_EQUAL(strlen(pgProgramVertexShaderCompileLog(program)), 0);
	CU_ASSERT_NOT_EQUAL(strlen(pgProgramFragmentShaderCompileLog(program)), 0);
	CU_ASSERT_EQUAL(strlen(pgProgramLinkLog(program)), 0);
	
	pgProgramDestroy(&program);
	
	CU_ASSERT_PTR_NULL(program);
	
	free(vertexShader);
	free(fragmentShader);
}

static void test_create_new_program_with_unlinkable_shaders(void)
{
	char *vertexShader = pgTestsCreateAssetPath("test_03_unlinkable.vsh");
	char *fragmentShader = pgTestsCreateAssetPath("test_03_unlinkable.fsh");
	PGProgram program = NULL;
	
	PGResult result = 0;
	result = pgProgramCreateAndBuild(&program, vertexShader, fragmentShader);

	CU_ASSERT_EQUAL(result, PGR_CouldNotLinkProgram);
	
	CU_ASSERT_NOT_EQUAL(pgProgramVertexShader(program), 0);
	CU_ASSERT_NOT_EQUAL(pgProgramFragmentShader(program), 0);
	CU_ASSERT_NOT_EQUAL(pgProgramGlHandle(program), 0);
	
	CU_ASSERT_PTR_NOT_NULL(pgProgramVertexShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramFragmentShaderCompileLog(program));
	CU_ASSERT_PTR_NOT_NULL(pgProgramLinkLog(program));
	
	CU_ASSERT_EQUAL(strlen(pgProgramVertexShaderCompileLog(program)), 0);
	CU_ASSERT_EQUAL(strlen(pgProgramFragmentShaderCompileLog(program)), 0);
	CU_ASSERT_NOT_EQUAL(strlen(pgProgramLinkLog(program)), 0);
	
	pgProgramDestroy(&program);
	
	CU_ASSERT_PTR_NULL(program);
	
	free(vertexShader);
	free(fragmentShader);
}

CU_pSuite pgTestSuiteCreateProgramCompiler(void)
{
	CREATE_SUITE("Program Compiler", NULL, NULL);
	
	if(!suite
	   || !ADD_TEST(suite, test_create_new_program)
	   || !ADD_TEST(suite, test_create_new_program_with_broken_vertex_shader)
	   || !ADD_TEST(suite, test_create_new_program_with_broken_fragment_shader)
	   || !ADD_TEST(suite, test_create_new_program_with_unlinkable_shaders)
	   )
	{
		return NULL;
	}
	
	return suite;
}
