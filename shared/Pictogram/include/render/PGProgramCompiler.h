//
//  PGProgramCompiler.h
//
//  Created by David Wagner on 04/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PGProgramCompiler_h
#define PGProgramCompiler_h

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a program from the supplied source files.
 *
 * If any error occurs you must still call pgDestroyProgram() on the
 * PGProgram variable you passed. If program is not null, you may inspect
 * it for compile or link logs in order to work out where your error 
 * occured.
 */
PGResult pgProgramCreateAndBuild(PGProgram *program, const char *vertexSource, const char *fragmentSource);
void pgProgramDestroy(PGProgram *program);

#define pgProgramGlHandle(PGProgram) (*((GLuint*)(PGProgram)))
GLuint pgProgramVertexShader(PGProgram program);
GLuint pgProgramFragmentShader(PGProgram program);
GLint pgProgramAttribLocation(PGProgram program, const char* name);
GLint pgProgramUniformLocation(PGProgram program, const char* name);

const GLchar* pgProgramVertexShaderCompileLog(PGProgram program);
const GLchar* pgProgramFragmentShaderCompileLog(PGProgram program);
const GLchar* pgProgramLinkLog(PGProgram program);
	
#ifdef __cplusplus
}
#endif

#endif
