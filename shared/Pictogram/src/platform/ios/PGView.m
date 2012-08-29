//
//  PGView.m
//  MobilePoly
//
//  Created by David Wagner on 23/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "PGView.h"
#import "Pictogram.h"



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


@interface PGView ()
{
    GLuint _vertexArray;
    GLuint _vertexBuffer;
	PGProgram _pgprogram;
}
- (void)setupGL;
- (void)tearDownGL;
- (BOOL)loadShaders;
- (void) hacky;

- (void) drawView: (CADisplayLink*) displayLink;

@property (strong, nonatomic) EAGLContext *eaglContext;
@property (strong, nonatomic) CADisplayLink *displayLink;

@property (unsafe_unretained, nonatomic) PGRenderer renderer;

@end

@implementation PGView

@synthesize displayLink = _displayLink;
@synthesize delegate = _delegate;
@synthesize eaglContext = _eaglContext;
@synthesize renderer = _renderer;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void)dealloc
{
    pgRendererDestroy(&_renderer);
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
	if (self)
	{
		if (![self setupEAGLContextAndRunLoop])
		{
			self = nil;
		}
	}
	return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
	{
		if (![self setupEAGLContextAndRunLoop])
		{
			self = nil;
		}
		else
		{
			[self hacky];
		}
    }
    return self;
}

- (BOOL)setupEAGLContextAndRunLoop
{
	CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
	eaglLayer.opaque = YES;
	
	self.eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (!_eaglContext || ![self makeContextCurrent])
	{
		pgLog(PGL_Error, "Could not create EAGLEContext");
		return NO;
	}
	
	pgLogAnyGlErrors("About to create PGRenderer");
	pgRendererCreate(&_renderer);
	
	pgLogAnyGlErrors("About to bind renderBufferStorage");
	[_eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];
	
	pgLogAnyGlErrors("About to setup PGRenderer");
	pgRendererSetup(_renderer, CGRectGetWidth(self.bounds), CGRectGetHeight(self.bounds));
	pgLogAnyGlErrors("Setup PGRenderer");

	self.displayLink = [CADisplayLink displayLinkWithTarget:self
												   selector:@selector(drawView:)];

	
	[_displayLink addToRunLoop:[NSRunLoop currentRunLoop]
					   forMode:NSDefaultRunLoopMode];
	
	//[self drawView: nil];
	
	return YES;
}

- (BOOL)makeContextCurrent
{
	return [EAGLContext setCurrentContext:_eaglContext];
}

- (void) drawView:(CADisplayLink*) displayLink
{
	// TODO: I wonder if I should call [self makeContextCurrent];
	if (nil != _delegate)
	{
		[_delegate renderPGView:self];
	}
	
	[self renderPGView:self];
	
    [_eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)hacky
{
	[self setupGL];
}

- (void)setupGL
{
//	PGView *pgv = (PGView *)self.view;
//	pgv.delegate = self;
	
	[self makeContextCurrent];
	
    [self loadShaders];
	
	pgRendererUseProgram(self.renderer, _pgprogram);
	
	ApplyOrtho(_pgprogram, 2, 3);
}

- (void)tearDownGL
{
	pgProgramDestroy(&_pgprogram);
}

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
