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
 * Compiles the specified source file into a shader. You should
 * glDeleteShader() the shader when you are finished with it.
 *
 * If log is not null, the compilation log is returned. You must
 * free() the log when you are finished with it.
 */
PGResult pgCompileShaderFile(GLuint *outShader, GLenum type, const char *file, GLchar **outLog);

/**
 * Compiles the specified source string into a shader. You should
 * glDeleteShader() the shader when you are finished with it.
 *
 * If log is not null, the compilation log is returned. You must
 * free() the log when you are finished with it.
 */
PGResult pgCompileShaderString(GLuint *outShader, GLenum type, const char *source, GLchar **outLog);

PGResult pgLinkProgram(GLuint vertexShader, GLuint fragmentShader, GLchar **outLog);

#ifdef __cplusplus
}
#endif

#endif
