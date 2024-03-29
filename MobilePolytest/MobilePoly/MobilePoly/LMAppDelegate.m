//
//  LMAppDelegate.m
//  MobilePoly
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#import "LMAppDelegate.h"

#import "LMViewController.h"

//#define RUN_PG_TESTS
#ifdef RUN_PG_TESTS
#import "PictogramTests.h"
#endif

#import "Pictogram.h"
#import "PGView.h"

@implementation LMAppDelegate

@synthesize window = _window;
@synthesize view = _view;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	pgLogSetLevel(PGL_Debug);

	CGRect screenBounds = [[UIScreen mainScreen] bounds];
    self.window = [[UIWindow alloc] initWithFrame:screenBounds];

//    CGRect screenBounds = [[UIScreen mainScreen] bounds];
//
//    m_window = [[UIWindow alloc] initWithFrame: screenBounds];
	self.view = [[PGView alloc] initWithFrame: screenBounds];
	[self.window addSubview:self.view];
	[self.window makeKeyAndVisible];
//
//    [m_window addSubview: m_view];
//    [m_window makeKeyAndVisible];

    // Override point for customization after application launch.
//	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
//	    self.viewController = [[LMViewController alloc] initWithNibName:@"LMViewController_iPhone" bundle:nil];
//	} else {
//	    self.viewController = [[LMViewController alloc] initWithNibName:@"LMViewController_iPad" bundle:nil];
//	}
//	self.window.rootViewController = self.viewController;
//    [self.window makeKeyAndVisible];

	
#ifdef RUN_PG_TESTS
	NSString *testRoot = [[NSBundle mainBundle] pathForResource:@"test_01" ofType:@"fsh" inDirectory:@"test_assets"];
	pgTestRunAll([[testRoot stringByDeletingLastPathComponent] cStringUsingEncoding:NSUTF8StringEncoding]);
#endif
	
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
