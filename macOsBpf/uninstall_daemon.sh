#!/bin/sh

CHMOD_BPF_SRC=ChmodBpf
CHMOD_BPF_DST_BASE="/Library/Application Support"
CHMOD_BPF_DST="$CHMOD_BPF_DST_BASE"/Waola

PLIST_SRC=com.asuscomm.smartnclever.ChmodBpf.plist
PLIST_DST="/Library/LaunchDaemons"

if [ -d "$CHMOD_BPF_DST" ]; then
	rm -rf "$CHMOD_BPF_DST" || { echo "Cannot delete '$CHMOD_BPF_DST'" 1>&2; } 
fi

if [ -f "$PLIST_DST/$PLIST_SRC" ]; then
	rm "$PLIST_DST/$PLIST_SRC" || { echo "Cannot delete '$PLIST_DST/$PLIST_DST'" 1>&2; }
fi

chmod g-rw /dev/bpf*   || { echo "Cannot chmod g-rw /dev/bpf*" 1>&2; }
chgrp wheel /dev/bpf* || { echo "Cannot chgrp wheel /dev/bpf*" 1>&2; }

if [ "$EUID" -ne 0 ]; then
	echo 'Also try with root permissions (i. e. sudo ...)'
fi
