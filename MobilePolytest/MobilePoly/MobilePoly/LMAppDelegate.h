//
//  LMAppDelegate.h
//  MobilePoly
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LMViewController, PGView;

@interface LMAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) PGView *view;
@property (strong, nonatomic) LMViewController *viewController;

@end
