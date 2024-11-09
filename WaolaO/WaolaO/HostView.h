//
//  HostView.h
//  TableView
//
//  Created by e on 02.11.2024.
//

using namespace Waola;

@interface HostView : NSObject

@property (weak) NSString* displayName;
@property (weak) NSString* hostname;
@property (weak) NSString* ipAddress;
@property (weak) NSString* macAddress;
@property (weak, readonly) NSString* lastSeenOnline;
@property (weak, readonly) NSString* wakeupResult;
@property (readonly, nonnull) Waola::IHostView* hostView;

-(instancetype _Nonnull) initWithHostView: (IHostView* _Nonnull) hostView;
-(void) processChanges;

@end