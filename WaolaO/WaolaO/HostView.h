//
//  HostView.h
//  TableView
//
//  Created by e on 02.11.2024.
//

using namespace Waola;

@interface HostView : NSObject

@property (weak, readonly) NSString* _Nullable humanReadableId;
@property (weak) NSString* _Nullable displayName;
@property (weak, readonly) NSString* _Nullable exactDisplayName;
@property (weak) NSString* _Nullable hostname;
@property (weak) NSString* _Nullable ipAddress;
@property (weak) NSString* _Nullable macAddress;
@property (weak, readonly) NSDate* _Nullable lastSeenOnline;
@property (weak, readonly) NSString* _Nullable wakeupResult;
@property (readonly, nonnull) Waola::IHostView* hostView;

-(instancetype _Nonnull) initWithHostView: (IHostView* _Nonnull) hostView;
-(void) processChanges;
-(NSString*_Nonnull) toString;

@end
