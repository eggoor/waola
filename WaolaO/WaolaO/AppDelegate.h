//
//  AppDelegate.h
//  WaolaO
//
//  Created by e on 01.11.2024.
//

#import "Scanner.h"

@interface AppDelegate : NSObject<NSApplicationDelegate>
@property (readonly, strong) NSPersistentContainer* persistentContainer;
@property (weak) NSString* status;
@property (strong) Scanner* scanner;
@end
