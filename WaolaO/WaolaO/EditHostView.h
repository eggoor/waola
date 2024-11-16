//
//  EditHostView.h
//  WaolaO
//
//  Created by e on 14.11.2024.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface EditHostView : NSWindow

@property (weak) NSString* displayName;
@property (weak) NSString* hostname;
@property (weak) NSString* ipAddress;
@property (weak) NSString* macAddress;

@end

NS_ASSUME_NONNULL_END
