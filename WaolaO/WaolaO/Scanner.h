//
//  Scanner.h
//  WaolaO
//
//  Created by e on 15.11.2024.
//

#import "HostView.h"

NS_ASSUME_NONNULL_BEGIN

typedef void (*StateCallback)(const StateEvent&);
typedef void(*VaultCallback)(const VaultEvent&);

@interface Scanner : NSObject
@property (readonly, getter=isEmpty) BOOL empty;
- (void) discoverNetwork;
- (void) refresh;
- (void) batchAdd:(NSArray*)hosts;
- (void) addHost:(NSString*)displayName hostname:(NSString*)hostname ipAddress:(NSString*)ipAddress macAddress:(NSString*)macAddress;
- (void) deleteHost:(HostView*)hostView;
- (void) subscribeForVaultEvents:(id)subscriber callback:(VaultCallback)callback;
- (void) unsubscribeFromVaultEvents;
- (void) subscribeForStateEvents:(id)subscriber callback:(StateCallback)callback;
- (void) unsubscribeFromStateEvents;
- (const void*) getStatus:(Module*)module opCode:(int* const)opCode;
@end

NS_ASSUME_NONNULL_END
