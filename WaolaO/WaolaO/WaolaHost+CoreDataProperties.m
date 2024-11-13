//
//  WaolaHost+CoreDataProperties.m
//  WaolaO
//
//  Created by e on 13.11.2024.
//
//

#import "WaolaHost+CoreDataProperties.h"

@implementation WaolaHost (CoreDataProperties)

+ (NSFetchRequest<WaolaHost *> *)fetchRequest {
	return [NSFetchRequest fetchRequestWithEntityName:@"WaolaHost"];
}

@dynamic displayName;
@dynamic hostname;
@dynamic ipAddress;
@dynamic lastSeenOnline;
@dynamic macAddress;

@end
