#!/bin/sh

if [ "$EUID" -ne 0 ]; then
	echo 'Also try with root permissions (i. e. sudo ...)'
fi

CHMOD_BPF_SRC="ChmodBpf"
CHMOD_BPF_SCRIPT="WaolaBPF.sh"
CHMOD_BPF_DST_BASE="/Library/Application Support"
CHMOD_BPF_DST="$CHMOD_BPF_DST_BASE"/Waola

PLIST_SRC=com.asuscomm.smartnclever.ChmodBpf.plist
PLIST_DST="/Library/LaunchDaemons"

"$(dirname "$0")/$CHMOD_BPF_SRC/$CHMOD_BPF_SCRIPT" || { echo "Cannot run '$(dirname "$0")/$CHMOD_BPF_SRC/$CHMOD_BPF_SCRIPT'" 1>&2; exit 1; }

if [ -d "$CHMOD_BPF_DST" ]; then
	rm -rf "$CHMOD_BPF_DST"/*
else
	mkdir "$CHMOD_BPF_DST"
fi

cp -r "$(dirname "$0")/$CHMOD_BPF_SRC" "$CHMOD_BPF_DST"  || { echo "Cannot copy '$(dirname "$0")/$CHMOD_BPF_SRC' to '$CHMOD_BPF_DST'" 1>&2; exit 1; }
cp "$(dirname "$0")/$PLIST_SRC" "$PLIST_DST"  || { echo "Cannot copy '$(dirname "$0")/$PLIST_SRC' to '$PLIST_DST'" 1>&2; exit 1; }
