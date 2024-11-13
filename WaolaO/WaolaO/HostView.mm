//
//  HostView.m
//  TableView
//
//  Created by e on 02.11.2024.
//

#import "HostView.h"

@implementation HostView

@dynamic humanReadableId;
@dynamic displayName;
@dynamic hostname;
@dynamic ipAddress;
@dynamic macAddress;
@dynamic lastSeenOnline;
@dynamic wakeupResult;

-(instancetype) initWithHostView: (IHostView*) hostView {
	if (!hostView) {
		[NSException raise:@"Waola exception" format:@"hostView == NULL has passed to %s", __func__];
	}
	
	self->_hostView = hostView;
	hostView->SetExtraData((const void*)&self, sizeof(&self));
	
	return self;
}

-(void) dealloc {
  delete _hostView;
}

+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)theKey {
	return NO;
}

-(void) processChanges {
	const auto dirtyFields = _hostView->GetHostState();
	
	if (cf_display_name & dirtyFields) {
		[self didChangeValueForKey:@"displayName"];
	}
	
	if (cf_host_name & dirtyFields) {
		[self didChangeValueForKey:@"hostname"];
	}
	
	if (cf_ip_address & dirtyFields) {
		[self didChangeValueForKey:@"ipAddress"];
	}
	
	if (cf_mac_address & dirtyFields) {
		[self didChangeValueForKey:@"macAddress"];
	}
	
	if (cf_last_seen_online & dirtyFields) {
		[self didChangeValueForKey:@"lastSeenOnline"];
	}
	
	if (cf_op_result & dirtyFields) {
		[self didChangeValueForKey:@"wakeupResult"];
	}
	
	_hostView->SetFieldChangeProcessed(dirtyFields);
}

-(NSString*) humanReadableId {
	return @(_hostView->GetHumanReadableId().c_str());
}

-(void) setDisplayName:(NSString*)displayName {
	[self willChangeValueForKey:@"displayName"];
	_hostView->SetDisplayName([displayName UTF8String]);
	[self didChangeValueForKey:@"displayName"];
}

-(NSString*) displayName {
	_hostView->SetFieldChangeProcessed(Waola::cf_display_name);
	return @(_hostView->GetDisplayName().c_str());
}

-(void) setHostname:(NSString*)hostname {
	[self willChangeValueForKey:@"hostname"];
	_hostView->SetHostname([hostname UTF8String]);
	[self didChangeValueForKey:@"hostname"];
}

-(NSString*) hostname {
	_hostView->SetFieldChangeProcessed(Waola::cf_host_name);
	return @(_hostView->GetHostname().c_str());
}

-(void) setIpAddress:(NSString*)ipAddress {
	[self willChangeValueForKey:@"ipAddress"];
	_hostView->SetIpAddressString([ipAddress UTF8String]);
	[self didChangeValueForKey:@"ipAddress"];
}

-(NSString*) ipAddress {
	_hostView->SetFieldChangeProcessed(Waola::cf_ip_address);
	return @(_hostView->GetIpAddressString().c_str());
}

-(void) setMacAddress:(NSString*)macAddress {
	[self willChangeValueForKey:@"macAddress"];
	_hostView->SetMacAddressString([macAddress UTF8String]);
	[self didChangeValueForKey:@"macAddress"];
}

-(NSString*) macAddress {
	_hostView->SetFieldChangeProcessed(Waola::cf_mac_address);
	return @(_hostView->GetMacAddressString().c_str());
}

-(NSString*) lastSeenOnline {
	_hostView->SetFieldChangeProcessed(Waola::cf_last_seen_online);
	return @(_hostView->GetLastSeenOnlineString().c_str());
}

-(NSString*) wakeupResult {
	_hostView->SetFieldChangeProcessed(Waola::cf_op_result);
	NSString* result;

	Waola::OpResult opResult = _hostView->GetOpResult();
	switch (opResult) {
			
		case Waola::wor_undefined:
			result = nil;
			break;
		case Waola::wor_success:
			result = @"Success";
			break;
		case Waola::wor_fail:
			result = @"Fail";
			break;
		default:
			[NSException raise:@"Waola exception" format:@"OpResult == %d has passed to %s",
				opResult, __func__];
	}
	
	return result;
}

@end
