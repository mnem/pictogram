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

@interface PGView ()

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
    }
    return self;
}

- (BOOL)setupEAGLContextAndRunLoop
{
	CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
	eaglLayer.opaque = YES;
	
	self.eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (nil == _eaglContext)
	{
		pgLog(PGL_Error, "Could not create EAGLEContext");
		return NO;
	}
	[self makeContextCurrent];
	
	pgRendererCreate(&_renderer);
	[_eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];
	pgRendererSetup(_renderer, CGRectGetWidth(self.bounds), CGRectGetHeight(self.bounds));
	
	self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
	[_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	[self drawView: nil];
	
	return YES;
}

- (void)makeContextCurrent
{
	[EAGLContext setCurrentContext:_eaglContext];
}

- (void) drawView: (CADisplayLink*) displayLink
{
	// TODO: I wonder if I should call [self makeContextCurrent];
	if (nil != _delegate)
	{
		[_delegate renderPGView:self];
	}
	
    [_eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

@end
