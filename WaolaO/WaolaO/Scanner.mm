//
//  Scanner.m
//  WaolaO
//
//  Created by e on 15.11.2024.
//

#import "WaolaHost+CoreDataClass.h"

#import "Scanner.h"

using namespace Waola;

@implementation Scanner {
	IScanner* _scanner;
	Waola::StateUnsubscribeToken _stateUnsubscribeToken;
	Waola::VaultUnsubscribeToken _vaultUnsubscribeToken;
}

@dynamic empty;

- (instancetype) init {
	self = [super init];
	
	if (self) {
		_scanner = IScanner::Create();
		
		if (!_scanner) {
			[NSException raise:@"Waola exception" format:@"Cannot create Scanner in %s", __func__];
		}
		
	}
	
	return self;
}

-(void) dealloc {
	IScanner::Destroy(_scanner);
}

- (BOOL) isEmpty {
	return _scanner->IsVaultEmpty();
}

- (void)discoverNetwork {
	_scanner->DiscoverAsync();
}

- (void)refresh {
	_scanner->Refresh();
}

- (void) batchAdd:(NSArray*)hosts {
	const NSUInteger hostCount = [hosts count];
	std::vector<const HostData*> hostData(hostCount);
	
	for (NSUInteger i = hostCount; i --> 0;) {
		WaolaHost* host = hosts[i];
		hostData[i] = new HostData([host.displayName UTF8String],
								   [host.hostname UTF8String], [host.ipAddress UTF8String],
								   [host.macAddress UTF8String], [host.lastSeenOnline timeIntervalSince1970]);
	}
	
	_scanner->BatchAdd(hostData);
	
	for (NSUInteger i = hostCount; i --> 0;) {
		delete hostData[i];
	}
}

- (void) addHost:(NSString*) displayName hostname:(NSString*)hostname ipAddress:(NSString*)ipAddress macAddress:(NSString*)macAddress {
	HostData hostData([displayName UTF8String], [hostname UTF8String], [ipAddress UTF8String], [macAddress UTF8String], 0);
	_scanner->Add(hostData);
}

- (void) deleteHost:(HostView*)hostView {
	_scanner->DeleteHost([hostView hostView]);
}

- (void) subscribeForVaultEvents:(id)subscriber callback:(VaultCallback)callback {
	VaultEventCallbackData vaultCbData((__bridge const void*)subscriber, callback);
	_vaultUnsubscribeToken = _scanner->SubscribeForVaultEvents(vaultCbData);
}

- (void) unsubscribeFromVaultEvents {
	_scanner->UnsubscribeFromVaultEvents(_vaultUnsubscribeToken);
}

- (void) subscribeForStateEvents:(id)subscriber callback:(StateCallback)callback {
	StateEventCallbackData stateCbData((__bridge const void*)subscriber, callback);
	_stateUnsubscribeToken = _scanner->SubscribeForStateEvents(stateCbData);
}

- (void) unsubscribeFromStateEvents {
	_scanner->UnsubscribeFromStateEvents(_stateUnsubscribeToken);
}

- (const void*) getStatus:(Module*)module opCode:(int* const)opCode {
	return _scanner->GetStatus(module, opCode);
}

@end
