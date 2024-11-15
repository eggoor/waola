//
//  StatusMonitor.h
//  WaolaO
//
//  Created by e on 15.11.2024.
//

#import "AppDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface StatusMonitor : NSObject
- (instancetype)initWithAppDelegate:(AppDelegate*)appDelegate;
- (void)start;
- (void)stop;
@end

NS_ASSUME_NONNULL_END
