//
//  WaolaHost+CoreDataProperties.h
//  WaolaO
//
//  Created by e on 13.11.2024.
//
//

#import "WaolaHost+CoreDataClass.h"


NS_ASSUME_NONNULL_BEGIN

@interface WaolaHost (CoreDataProperties)

+ (NSFetchRequest<WaolaHost *> *)fetchRequest NS_SWIFT_NAME(fetchRequest());

@property (nullable, nonatomic, copy) NSString *displayName;
@property (nullable, nonatomic, copy) NSString *hostname;
@property (nullable, nonatomic, copy) NSString *ipAddress;
@property (nullable, nonatomic, copy) NSDate *lastSeenOnline;
@property (nullable, nonatomic, copy) NSString *macAddress;

@end

NS_ASSUME_NONNULL_END
