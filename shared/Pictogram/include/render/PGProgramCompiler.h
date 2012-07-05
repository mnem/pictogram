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

void pgCompileShaderFile(GLuint *shader, GLenum type, char *file, GLchar *log);
void pgCompileShaderString(GLuint *shader, GLenum type, char *source, GLchar *log);

#ifdef __cplusplus
}
#endif

#endif
