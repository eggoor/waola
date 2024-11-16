//
//  StatusMonitor.m
//  WaolaO
//
//  Created by e on 15.11.2024.
//

#import "AppDelegate.h"

#import "StatusMonitor.h"

using namespace Waola;

@interface StatusMonitor ()
- (void) processUpdate:(const void* const)statusData;
- (NSString*) resolverHostnameOpCode2Status:(const void*)statusData;
- (NSString*) resolverIpAddessOpCode2Status:(const void*)statusData;
- (void) setStatus:(NSString*)status;
@end

@implementation StatusMonitor {
	AppDelegate* _appDelegate;
	NSTimer* _timer;
	Module _lastModule;
	int _lastOpCode;
	const void* _lastStatusData;
	BOOL _isUpdated;
}

- (instancetype)initWithAppDelegate:(AppDelegate*)appDelegate {
	self = [super init];
	
	if (self) {
		_appDelegate = appDelegate;
		_lastModule = wm_undefined;
		_lastOpCode = 0;
		_lastStatusData = NULL;
		_isUpdated = NO;
	}
	
	return self;
}

- (void)start {
	dispatch_async(dispatch_get_main_queue(), ^{
		if (!self->_timer) {
			self->_timer = [NSTimer scheduledTimerWithTimeInterval:0.08 target:self selector:@selector(onTimerTick:) userInfo:nil repeats:YES];
		}
	});
}

- (void)stop {
	dispatch_async(dispatch_get_main_queue(), ^{
		if (self->_timer) {
			[self->_timer invalidate];
			self->_timer = nil;
		}
	});
}

- (void) onTimerTick:(NSTimer *)timer {
	Module module = wm_undefined;
	int opCode = 0;
	const void* statusData = [_appDelegate.scanner getStatus:&module opCode:&opCode];
	[self processStatus:module opCode:opCode statusData:statusData];
}

- (void) processStatus:(Module)module opCode:(uint32_t)opCode statusData:(const void*)statusData {
	if (module == wm_undefined) {
		dispatch_async(dispatch_get_main_queue(), ^{
			self->_appDelegate.status = @"Undefined module in processStatus";
		});
		NSLog(@"%@", _appDelegate.status);
		return;
	}
	
	if (_lastModule != module)	{
		_isUpdated = YES;
		_lastModule = module;
	}
	
	if (_lastOpCode != opCode)	{
		if (!_isUpdated) {
			_isUpdated = YES;
		}
		_lastOpCode = opCode;
	}
	
	if (_isUpdated || module != wm_scanner) {
		[self processUpdate:statusData];
	}
}

- (void) processUpdate:(const void*)statusData {
	switch (_lastModule) {
		case wm_scanner:
			[self processScannerUpdate];
			break;
		case wm_discoverer:
			[self processDiscovererOrResolverUpdate:statusData];
			break;
		case wm_resolver:
			[self processDiscovererOrResolverUpdate: statusData];
			break;
		default:
			NSLog(@"Invalid module identifier '%d' is passed to processUpdate.", _lastModule);
	}
	
	if (_isUpdated) {
		_isUpdated = NO;
	}
}

- (void) processScannerUpdate {
	NSString* status;
	
	switch (_lastOpCode) {
		case wco_idle:
			status = @"Idle";
			break;
		case wco_discovering:
			status = @"Discovering";
			break;
		case wco_resolving:
			status = @"Refreshing";
			break;
		case wco_cancelling:
			status = @"Cancelling";
			break;
		default:
			status = [NSString stringWithFormat:@"Invalid operation code '%d' is passed to processScannerUpdate", _lastOpCode];
			NSLog(@"%@", status);
	}
	
	dispatch_async(dispatch_get_main_queue(), ^{
		self->_appDelegate.status = status;
	});
}

- (void) processDiscovererOrResolverUpdate:(const void*)statusData {
	if (_lastStatusData != statusData) {
		_lastStatusData = statusData;
		if (!_isUpdated) {
			_isUpdated = true;
		}
	}
	
	if (!_isUpdated) {
		return;
	}
	
	switch (_lastModule) {
		case wm_discoverer:
			[self processDiscovererUpdate:_lastStatusData];
			break;
		case wm_resolver:
			[self processResolverUpdate:_lastStatusData];
			break;
		default:
			NSLog(@"Invalid module identifier '%d' is passed to processDiscovererOrResolverUpdate", _lastModule);
	}
}

- (void) processDiscovererUpdate:(const void*)statusData {
	switch (_lastOpCode) {
		case wdi_scheduling_arp:
			[self onDiscovererIpAddress:statusData];
			break;
		case wdi_sending_arp:
			[self onDiscovererIpAddress:statusData];
			break;
		case wdi_got_arp:
			[self onDiscovererIpAddress:statusData];
			break;
		case wdi_arp_failed:
			[self onDiscovererIpAddress:statusData];
			break;
		default:
			NSLog(@"Invalid operation code '%d' is passed to %s", _lastOpCode, __func__);
	}
}

- (void) processResolverUpdate:(const void*)statusData {
	switch (_lastOpCode) {
		case wre_getaddrinfo:
			[self onResolverHostName:statusData];
			break;
		case wre_getaddrinfo_failed:
			[self onResolverHostName:statusData];
			break;
		case wre_gotnameinfo:
			[self onResolverHostName:statusData];
			break;
		case wre_gotaddrinfo:
			[self onResolverIpAddress:statusData];
			break;
		case wre_getnameinfo:
			[self onResolverIpAddress:statusData];
			break;
		case wre_getnameinfo_failed:
			[self onResolverIpAddress:statusData];
			break;
		case wre_sending_arp:
			[self onResolverIpAddress:statusData];
			break;
		case wre_got_arp:
			[self onResolverIpAddress:statusData];
			break;
		case wre_arp_failed:
			[self onResolverIpAddress:statusData];
			break;
		default:
			NSLog(@"Invalid operation code '%d' is passed to %s", _lastOpCode, __func__);
	}
}

- (void) onDiscovererIpAddress:(const void*)statusData {
	[self setStatus:[self discovererOpCode2Status:statusData]];
}

- (void) onResolverHostName:(const void*)statusData {
	[self setStatus:[self resolverHostnameOpCode2Status:statusData]];
}

- (void) onResolverIpAddress:(const void*)statusData {
	[self setStatus:[self resolverIpAddessOpCode2Status:statusData]];
}

- (NSString*) discovererOpCode2Status:(const void*)statusData {
	NSString* status;
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, statusData, ip_str, INET_ADDRSTRLEN);
	
	switch (_lastOpCode) {
		case wdi_scheduling_arp:
			status = [NSString stringWithFormat:@"Scheduling connection to %s", ip_str];
			break;
		case wdi_sending_arp:
			status = [NSString stringWithFormat:@"Trying to connect to %s", ip_str];
			break;
		case wdi_got_arp:
			status = [NSString stringWithFormat:@"%s connected", ip_str];
			break;
		case wdi_arp_failed:
			status = [NSString stringWithFormat:@"Failed to connect to %s", ip_str];
			break;
		default:
			status = [NSString stringWithFormat:@"Invalid operation code '%d' is passed to %s", _lastOpCode, __func__];
			NSLog(@"%@", status);
	}
	
	return status;
}

- (NSString*) resolverIpAddessOpCode2Status:(const void*)statusData {
	NSString* status;
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, statusData, ip_str, INET_ADDRSTRLEN);
	
	switch (_lastOpCode) {
		case wre_sending_arp:
			status = [NSString stringWithFormat:@"Trying to connect to %s", ip_str];
			break;
		case wre_got_arp:
			status = [NSString stringWithFormat:@"%s connected", ip_str];
			break;
		case wre_arp_failed:
			status = [NSString stringWithFormat:@"Failed to connect to %s", ip_str];
			break;
		case wre_gotaddrinfo:
			status = [NSString stringWithFormat:@"Resolved %s", ip_str];
			break;
		case wre_getnameinfo:
			status = [NSString stringWithFormat:@"Resolving %s", ip_str];
			break;
		case wre_getnameinfo_failed:
			status = [NSString stringWithFormat:@"Failed to resolve %s", ip_str];
			break;
		default:
			status = [NSString stringWithFormat:@"Invalid operation code '%d' is passed to %s", _lastOpCode, __func__];
			NSLog(@"%@", status);
	}
	
	return status;
}

- (NSString*) resolverHostnameOpCode2Status:(const void* const)statusData {
	NSString* status;
	const char* const hostname = (const char* const)statusData;
	
	switch (_lastOpCode) {
		case wre_getaddrinfo:
			status = [NSString stringWithFormat:@"Resolving %s", hostname];
			break;
		case wre_gotnameinfo:
			status = [NSString stringWithFormat:@"Resolved %s", hostname];
			break;
		case wre_getaddrinfo_failed:
			status = [NSString stringWithFormat:@"Failed to resolve %s", hostname];
			break;
		default:
			status = [NSString stringWithFormat:@"Invalid operation code '%d' is passed to %s", _lastOpCode, __func__];
			NSLog(@"%@", status);
	}
	
	return status;
}

- (void) setStatus:(NSString*)status {
	self->_appDelegate.status = status;
}

@end
