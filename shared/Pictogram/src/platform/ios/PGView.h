//
//  PGView.h
//  MobilePoly
//
//  Created by David Wagner on 23/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PGView;

@protocol PGViewDelegate <NSObject>

- (void)renderPGView:(PGView*)pgView;

@end

@interface PGView : UIView

@property (unsafe_unretained, nonatomic)id<PGViewDelegate> delegate;

- (void)makeContextCurrent;

@end
