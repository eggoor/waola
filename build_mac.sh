#!/bin/sh

EXPORT_PATH=./Release
ARCHIVE_PATH=$EXPORT_PATH

SCHEMES=("waolac" "WaolaS")

if [ -d $EXPORT_PATH ]; then
	rm -rf $EXPORT_PATH/*
else
	mkdir $EXPORT_PATH
fi

for scheme in "${SCHEMES[@]}"
do
echo Building $scheme … 
xcodebuild -scheme $scheme -archivePath "$ARCHIVE_PATH/$scheme" archive || { echo "archive failed" 1>&2; exit 1; }

if [[ "WaolaS" == "$scheme" ]]; then
	xcodebuild -exportArchive -archivePath "$ARCHIVE_PATH/$scheme".xcarchive -exportPath $EXPORT_PATH -exportOptionsPlist ./ExportOptions.plist || { echo "export failed" 1>&2; exit 1; }
else
	cp "$ARCHIVE_PATH/$scheme".xcarchive/Products/usr/local/bin/* $EXPORT_PATH
fi
done
