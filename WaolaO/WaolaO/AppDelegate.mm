//
//  AppDelegate.m
//  WaolaO
//
//  Created by e on 01.11.2024.
//

#import "HostView.h"
#import "WaolaHost+CoreDataClass.h"
#import "AddEditHost.h"

#import "AppDelegate.h"

@interface AppDelegate () {
	Waola::IScanner* _scanner;
	Waola::IWaola* _waola;
	Waola::StateUnsubscribeToken _stateUnsubscribeToken;
	Waola::VaultUnsubscribeToken _vaultUnsubscribeToken;
}

@property (weak) IBOutlet NSWindow* window;
@property (weak) IBOutlet AddEditHost* addEditHost;
@property (weak) IBOutlet NSArrayController* hostsController;
@property (weak) IBOutlet NSProgressIndicator* progressSpinner;
@property (strong) NSMutableArray* hosts;

- (void)save;

@end

@implementation AppDelegate

@synthesize addEditHost = _addEditHost;

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)application {
	return YES;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
	_scanner = Waola::IScanner::Create();
	if (!_scanner) {
		NSLog(@"Scanner creation failed -> terminating");
		[NSApp terminate:self];
		return;
	}
	
	_waola = Waola::IWaola::Create();
	if (!_waola) {
		NSLog(@"Waola creation failed -> terminating");
		[NSApp terminate:self];
		return;
	}
	
	self.hosts = [NSMutableArray array];
	
	Waola::StateEventCallbackData stateCbData((__bridge const void*)self, OnScannerStateChanged);
	_stateUnsubscribeToken = _scanner->SubscribeForStateEvents(stateCbData);
	
	Waola::VaultEventCallbackData vaultCbData((__bridge const void*)self, OnVaultEvent);
	_vaultUnsubscribeToken = _scanner->SubscribeForVaultEvents(vaultCbData);
	
	NSManagedObjectContext* viewContext = self.persistentContainer.viewContext;
	NSError* fetchError = nil;
	NSArray* savedHosts = [viewContext executeFetchRequest:[WaolaHost fetchRequest] error:&fetchError];
	
	if (fetchError) {
		NSLog(@"%@", fetchError.localizedDescription);
	}
	
	const NSUInteger savedHostCount = [savedHosts count];
	std::vector<const HostData*> hostData(savedHostCount);
	
	for (NSUInteger i = savedHostCount; i --> 0;) {
		WaolaHost* host = savedHosts[i];
		hostData[i] = new HostData([host.displayName UTF8String],
			[host.hostname UTF8String], [host.ipAddress UTF8String],
			[host.macAddress UTF8String], [host.lastSeenOnline timeIntervalSince1970]);
	}
	
	_scanner->BatchAdd(hostData);
	
	for (NSUInteger i = savedHostCount; i --> 0;) {
		delete hostData[i];
	}
	
	if (_scanner->IsVaultEmpty()) {
		[self.progressSpinner startAnimation:self];
		_scanner->DiscoverAsync();
	}
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	_scanner->UnubscribeFromVaultEvents(_vaultUnsubscribeToken);
	_scanner->UnubscribeFromStateEvents(_stateUnsubscribeToken);
	[self persistData];
	Waola::IWaola::Destroy(_waola);
	Waola::IScanner::Destroy(_scanner);
}

- (void) persistData {
	NSFetchRequest* fetchRequest = [WaolaHost fetchRequest];
	[fetchRequest setIncludesPropertyValues:NO];
	NSManagedObjectContext* viewContext = self.persistentContainer.viewContext;
	NSError* error;
	NSArray* hosts = [viewContext executeFetchRequest:fetchRequest error:&error];
	
	if (error) {
		NSLog(@"%@", error.localizedDescription);
	}
	
	for (NSManagedObject* host in hosts)
	{
		[viewContext deleteObject:host];
	}
	
	for (HostView* hostView in self.hosts) {
		WaolaHost* host = [NSEntityDescription insertNewObjectForEntityForName:@"WaolaHost" inManagedObjectContext:viewContext];
		host.displayName = hostView.exactDisplayName;
		host.hostname = hostView.hostname;
		host.ipAddress = hostView.ipAddress;
		host.macAddress = hostView.macAddress;
		host.lastSeenOnline = hostView.lastSeenOnline;
	}
	
	[self save];
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
	return YES;
}

- (void) onScannerStateChanged:(const Waola::StateEvent*) stateEvent {
	if (stateEvent->Tasks == Waola::wt_idle) {
		dispatch_async(dispatch_get_main_queue(), ^{
			//[self.hostsController rearrangeObjects];
			[self.progressSpinner stopAnimation:self];
		});
	}
}

- (void) onVaultEvent:(const Waola::VaultEvent*) vaultEvent {
	switch (vaultEvent->OpCode) {
		case Waola::wva_undefined:
			[NSException raise:@"Waola exception" format:@"VaultEvent == wva_undefined has passed to %s", __func__];
			break;
		case Waola::wva_added:
		{
			Waola::IHostView* hostView = vaultEvent->HostView.Clone();
			hostView->SetFieldChangeProcessed(Waola::cf_all);
			HostView* hostViewView = [[HostView alloc] initWithHostView:hostView];
			
			if (!hostViewView) {
				[NSException raise:@"Waola exception" format:@"Cannot alloc HostView in %s", __func__];
			}
			
			[self.hosts addObject:hostViewView];
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.hostsController rearrangeObjects];
			});
		}
			break;
		case Waola::wva_modified:
		{
			const void* bridge = vaultEvent->HostView.GetExtraData();
			HostView* __weak hostViewView = *(HostView* __weak *)bridge;
			if (hostViewView) {
				[hostViewView processChanges];
			}
		}
			break;
		case Waola::wva_deleted:
			const void* bridge = vaultEvent->HostView.GetExtraData();
			HostView* __weak hostViewView = *(HostView* __weak *)bridge;
			[self.hosts removeObject:hostViewView];
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.hostsController rearrangeObjects];
			});
			break;
	}
}

- (IBAction)onTableViewDoubleAction:(id)sender {
	[self WakeUpSelection];
}

- (IBAction)onWakeUp:(id)sender {
	[self WakeUpSelection];
}

- (IBAction)onRescan:(id)sender {
	[self.progressSpinner startAnimation:self];
	_scanner->DiscoverAsync();
}

- (IBAction)onRefresh:(id)sender {
	[self.progressSpinner startAnimation:self];
	_scanner->Refresh();
}

- (IBAction)onCopy:(id)sender {
}

- (IBAction)onAdd:(id)sender {
	if (!self.addEditHost) {
		NSArray* topLevelObjects;
		[[NSBundle mainBundle] loadNibNamed:@"AddEditHost" owner:self topLevelObjects:&topLevelObjects];
	}
	
	[self.window beginSheet:self.addEditHost completionHandler:^(NSModalResponse returnCode) {
		if (returnCode == YES) {
			[self.progressSpinner startAnimation:self];
			
			HostData hostData([self.addEditHost.displayName UTF8String], [self.addEditHost.hostname UTF8String], [self.addEditHost.ipAddress UTF8String], [self.addEditHost.macAddress UTF8String], 0);
			self->_scanner->Add(hostData);
			
			[self.progressSpinner stopAnimation:nil];
		}
		self.addEditHost = nil;
	}];
}

- (IBAction)onEdit:(id)sender {
	
	NSArray* selectedHosts = [self.hostsController selectedObjects];
	if ([selectedHosts count] > 0) {
		
		if (!self.addEditHost) {
			NSArray* topLevelObjects;
			[[NSBundle mainBundle] loadNibNamed:@"AddEditHost" owner:self topLevelObjects:&topLevelObjects];
		}
		
		HostView* selectedHost = selectedHosts[0];
		
		self.addEditHost.displayName = selectedHost.exactDisplayName;
		self.addEditHost.hostname = selectedHost.hostname;
		self.addEditHost.ipAddress = selectedHost.ipAddress;
		self.addEditHost.macAddress = selectedHost.macAddress;

		[self.window beginSheet:self.addEditHost completionHandler:^(NSModalResponse returnCode) {
			if (returnCode == YES) {
				selectedHost.displayName = self.addEditHost.displayName;
				selectedHost.hostname = self.addEditHost.hostname;
				selectedHost.ipAddress = self.addEditHost.ipAddress;
				selectedHost.macAddress = self.addEditHost.macAddress;
			}
			self.addEditHost = nil;
		}];
	}
}

- (IBAction)onDelete:(id)sender {
	NSArray* selectedHosts = [self.hostsController selectedObjects];
	NSUInteger n = [selectedHosts count];
	
	if (n > 0) {
		NSAlert* alert = [[NSAlert alloc] init];
		NSString* messageText = n > 1
		? @"Delete selected host?"
		: [NSString stringWithFormat:@"%@%@%@", @"Delete '", [selectedHosts[0] humanReadableId], @"'?"];		
		
		[alert setMessageText:messageText];
		[alert addButtonWithTitle:@"Yes"];
		[alert addButtonWithTitle:@"No"];
		[alert setAlertStyle:NSAlertStyleWarning];
		
		[alert beginSheetModalForWindow:self.window completionHandler:^(NSModalResponse returnCode) {
			if (returnCode == NSAlertFirstButtonReturn) {
				[self deleteSelection];
			}
		}];
	}
}

- (void) deleteSelection {
	[self.progressSpinner startAnimation:nil];
	
	auto hostList = [self selection];
	for (auto& hostView : hostList) {
		_scanner->DeleteHost(hostView);
	}
	
	[self.progressSpinner stopAnimation:nil];
}

- (IBAction)onAbout:(id)sender {
}

- (IBAction)onAddSheetCancel:(id)sender {
	[NSApp endSheet:self.addEditHost returnCode:NO];
}

- (IBAction)onAddSheetSave:(id)sender {
	[NSApp endSheet:self.addEditHost returnCode:YES];
}

- (void) WakeUpSelection {
	NSArray* selectedHosts = [self.hostsController selectedObjects];
	NSUInteger n = [selectedHosts count];
	
	if (n > 0) {
		std::vector<IHostView*> hostList(n);
		
		for (NSUInteger i = n; i --> 0;) {
			HostView* hostViewView = selectedHosts[i];
			[hostViewView willChangeValueForKey:@"wakeupResult"];
			hostList[i] = ([hostViewView hostView]);
		}
		
		_waola->WakeUp(hostList);
		
		for (NSUInteger i = n; i --> 0;) {
			HostView* hostViewView = selectedHosts[i];
			[hostViewView didChangeValueForKey:@"wakeupResult"];
		}
	}
}

- (std::vector<IHostView*>) selection {
	NSArray* selectedHosts = [self.hostsController selectedObjects];
	NSUInteger n = [selectedHosts count];
	std::vector<IHostView*> hostList(n);
	for (NSUInteger i = n; i --> 0;) {
		hostList[i] = ([selectedHosts[i] hostView]);
	}

	return hostList;
}

void OnScannerStateChanged(const Waola::StateEvent& stateEvent) {
	AppDelegate* self = (__bridge AppDelegate*)stateEvent.GetSubscriber();
	[self onScannerStateChanged:&stateEvent];
}

void OnVaultEvent(const Waola::VaultEvent& vaultEvent) {
	AppDelegate* self = (__bridge AppDelegate*)vaultEvent.GetSubscriber();
	[self onVaultEvent:&vaultEvent];
}

#pragma mark - Core Data stack

@synthesize persistentContainer = _persistentContainer;

- (NSPersistentContainer *)persistentContainer {
    // The persistent container for the application. This implementation creates and returns a container, having loaded the store for the application to it.
    @synchronized (self) {
        if (_persistentContainer == nil) {
            _persistentContainer = [[NSPersistentContainer alloc] initWithName:@"WaolaO"];
            [_persistentContainer loadPersistentStoresWithCompletionHandler:^(NSPersistentStoreDescription *storeDescription, NSError *error) {
                if (error != nil) {
                    // Replace this implementation with code to handle the error appropriately.
                    // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
                    
                    /*
                     Typical reasons for an error here include:
                     * The parent directory does not exist, cannot be created, or disallows writing.
                     * The persistent store is not accessible, due to permissions or data protection when the device is locked.
                     * The device is out of space.
                     * The store could not be migrated to the current model version.
                     Check the error message to determine what the actual problem was.
                    */
                    NSLog(@"Unresolved error %@, %@", error, error.userInfo);
                    abort();
                }
            }];
        }
    }
    
    return _persistentContainer;
}

#pragma mark - Core Data Saving and Undo support

- (void)save {
    // Performs the save action for the application, which is to send the save: message to the application's managed object context. Any encountered errors are presented to the user.
    NSManagedObjectContext *context = self.persistentContainer.viewContext;

    if (![context commitEditing]) {
        NSLog(@"%@:%@ unable to commit editing before saving", [self class], NSStringFromSelector(_cmd));
    }
    
    NSError *error = nil;
    if (context.hasChanges && ![context save:&error]) {
        // Customize this code block to include application-specific recovery steps.              
        [[NSApplication sharedApplication] presentError:error];
    }
}

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)window {
    // Returns the NSUndoManager for the application. In this case, the manager returned is that of the managed object context for the application.
    return self.persistentContainer.viewContext.undoManager;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    // Save changes in the application's managed object context before the application terminates.
    NSManagedObjectContext *context = self.persistentContainer.viewContext;

    if (![context commitEditing]) {
        NSLog(@"%@:%@ unable to commit editing to terminate", [self class], NSStringFromSelector(_cmd));
        return NSTerminateCancel;
    }
    
    if (!context.hasChanges) {
        return NSTerminateNow;
    }
    
    NSError *error = nil;
    if (![context save:&error]) {

        // Customize this code block to include application-specific recovery steps.
        BOOL result = [sender presentError:error];
        if (result) {
            return NSTerminateCancel;
        }

        NSString *question = NSLocalizedString(@"Could not save changes while quitting. Quit anyway?", @"Quit without saves error question message");
        NSString *info = NSLocalizedString(@"Quitting now will lose any changes you have made since the last successful save", @"Quit without saves error question info");
        NSString *quitButton = NSLocalizedString(@"Quit anyway", @"Quit anyway button title");
        NSString *cancelButton = NSLocalizedString(@"Cancel", @"Cancel button title");
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:question];
        [alert setInformativeText:info];
        [alert addButtonWithTitle:quitButton];
        [alert addButtonWithTitle:cancelButton];

        NSInteger answer = [alert runModal];
        
        if (answer == NSAlertSecondButtonReturn) {
            return NSTerminateCancel;
        }
    }

    return NSTerminateNow;
}

@end
