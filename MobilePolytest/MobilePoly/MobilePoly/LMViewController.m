//
//  LMViewController.m
//  MobilePoly
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#import "LMViewController.h"
#import "Pictogram.h"
#import "PGView.h"

static void ApplyOrtho(PGProgram program, float maxX, float maxY)
{
    float a = 1.0f / maxX;
    float b = 1.0f / maxY;
    float ortho[16] = {
        a, 0,  0, 0,
        0, b,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    };
    
    GLint projectionUniform = pgProgramUniformLocation(program, "projectionMatrix");//  glGetUniformLocation(m_simpleProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
}

static void ApplyRotation(PGProgram program, float degrees)
{
    float radians = degrees * 3.14159f / 180.0f;
    float s = sin(radians);
    float c = cos(radians);
    float zRotation[16] = {
        c, s, 0, 0,
		-s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    GLint modelviewUniform = pgProgramUniformLocation(program, "modelViewMatrix");
    glUniformMatrix4fv(modelviewUniform, 1, 0, &zRotation[0]);
}

@interface LMViewController () <PGViewDelegate>
{
    GLuint _vertexArray;
    GLuint _vertexBuffer;
	PGProgram _pgprogram;
}

- (void)setupGL;
- (void)tearDownGL;

- (BOOL)loadShaders;
@end

@implementation LMViewController

- (void)viewDidLoad
{
    [super viewDidLoad];    
    [self setupGL];
}

- (void)dealloc
{
    [self tearDownGL];
    [super viewDidUnload];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
	
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) 
	{
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } 
	else 
	{
        return YES;
    }
}

- (void)setupGL
{
	PGView *pgv = (PGView *)self.view;
	pgv.delegate = self;
	
	[pgv makeContextCurrent];
	
    [self loadShaders];
	
	pgRendererUseProgram(pgv.renderer, _pgprogram);
	
	ApplyOrtho(_pgprogram, 2, 3);
}

- (void)tearDownGL
{
	pgProgramDestroy(&_pgprogram);
}

#pragma mark - GLKView and GLKViewController delegate methods
typedef struct {
	float Position[2];
	float Color[4];
} Vertex;

// Define the positions and colors of two triangles.
const static Vertex Vertices[] = {
    {{-0.5, -0.866}, {1, 1, 0.5f, 1}},
    {{0.5, -0.866},  {1, 1, 0.5f, 1}},
    {{0, 1},         {1, 1, 0.5f, 1}},
    {{-0.5, -0.866}, {0.5f, 0.5f, 0.5f}},
    {{0.5, -0.866},  {0.5f, 0.5f, 0.5f}},
    {{0, -0.4f},     {0.5f, 0.5f, 0.5f}},
};



- (void)renderPGView:(PGView *)pgView
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	ApplyRotation(_pgprogram, 0);
	
	GLuint positionSlot = pgProgramAttribLocation(_pgprogram, "position"); 
	GLuint colorSlot = pgProgramAttribLocation(_pgprogram, "color");
	
	glEnableVertexAttribArray(positionSlot);
	glEnableVertexAttribArray(colorSlot);
	
	GLsizei stride = sizeof(Vertex);
	const GLvoid* pCoords = &Vertices[0].Position[0];
	const GLvoid* pColors = &Vertices[0].Color[0];
	
	glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, stride, pCoords);
	glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
	
	GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
	glDisableVertexAttribArray(positionSlot);
	glDisableVertexAttribArray(colorSlot);
}

#pragma mark -  OpenGL ES 2 shader compilation

- (BOOL)loadShaders
{
    NSString *vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"mvp_col" ofType:@"vsh"];
    NSString *fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"mvp_col" ofType:@"fsh"];
	const char *vertShader = [vertShaderPathname cStringUsingEncoding:NSUTF8StringEncoding];
	const char *fragShader = [fragShaderPathname cStringUsingEncoding:NSUTF8StringEncoding];
	pgLogAnyGlErrors("In loadShaders");
	PGResult result = pgProgramCreateAndBuild(&_pgprogram, vertShader, fragShader);
	if (PGR_OK != result)
	{
		pgLog(PGL_Error, "Failed to build program (%d), logs:\n-- Vertex --\n%s\n-- Fragment --\n%s\n-- Link --\n%s",
			  result,
			  pgProgramVertexShaderCompileLog(_pgprogram),
			  pgProgramFragmentShaderCompileLog(_pgprogram),
			  pgProgramLinkLog(_pgprogram));
	}
	else
	{
		pgLog(PGL_Info, "Built program successfully (%d), logs:\n-- Vertex --\n%s\n-- Fragment --\n%s\n-- Link --\n%s",
			  result,
			  pgProgramVertexShaderCompileLog(_pgprogram),
			  pgProgramFragmentShaderCompileLog(_pgprogram),
			  pgProgramLinkLog(_pgprogram));
	}
	return result == PGR_OK;
}

@end
