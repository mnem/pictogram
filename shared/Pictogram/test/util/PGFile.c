//
//  PGFile.c
//
//  Created by David Wagner on 07/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdlib.h>
#include "../PictogramTests_private.h"

static void test_string_from_non_existant_file(void)
{
	const char testFile[] = "file_i_really_dont_exist.bleh";
	char *path = pgTestsCreateAssetPath(testFile);
	char *string = (char *)0xdeadbeef;
	
	PGResult result = pgCreateStringFromFile(&string, path);
	
	CU_ASSERT_EQUAL(result, PGR_CouldNotReadFile);
	CU_ASSERT_PTR_NULL(string);
	
	free(path);
}

static void test_string_from_empty_file(void)
{
	const char testFile[] = "file_no_contents.txt";
	char *path = pgTestsCreateAssetPath(testFile);
	char *string = (char *)0xdeadbeef;
	
	PGResult result = pgCreateStringFromFile(&string, path);
	
	CU_ASSERT_EQUAL(result, PGR_OK);
	CU_ASSERT_PTR_NOT_NULL(string);
	CU_ASSERT_PTR_NOT_EQUAL(string, (char *)0xdeadbeef)
	CU_ASSERT_STRING_EQUAL(string, "");
	
	free(path);
	free(string);
}

static void test_string_from_one_byte_file(void)
{
	const char testFile[] = "file_one_byte.txt";
	char *path = pgTestsCreateAssetPath(testFile);
	char *string = (char *)0xdeadbeef;
	
	PGResult result = pgCreateStringFromFile(&string, path);
	
	CU_ASSERT_EQUAL(result, PGR_OK);
	CU_ASSERT_PTR_NOT_NULL(string);
	CU_ASSERT_PTR_NOT_EQUAL(string, (char *)0xdeadbeef)
	CU_ASSERT_STRING_EQUAL(string, "a");
	
	free(path);
	free(string);
}

static void test_string_null_file_name(void)
{
	char *path = NULL;
	char *string = (char *)0xdeadbeef;
	
	PGResult result = pgCreateStringFromFile(&string, path);
	
	CU_ASSERT_EQUAL(result, PGR_NullPointerBarf);
	CU_ASSERT_PTR_NULL(string);
}

CU_pSuite pgTestSuiteCreateFile(void)
{
	CREATE_SUITE("File Utils", NULL, NULL);
	
	if(!suite ||
	   !ADD_TEST(suite, test_string_from_non_existant_file) ||
	   !ADD_TEST(suite, test_string_from_empty_file) ||
	   !ADD_TEST(suite, test_string_null_file_name) ||
	   !ADD_TEST(suite, test_string_from_one_byte_file))
	{
		return NULL;
	}
	
	return suite;
}
