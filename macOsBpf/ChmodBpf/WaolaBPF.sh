#! /bin/zsh
# shellcheck shell=bash

# Pre-create /dev/bpf* devices. Set to 0 to disable.
BPF_COUNT=4

SYSCTL_MAX=$( sysctl -n debug.bpf_maxdevices )
if [ "$BPF_COUNT" -gt "$SYSCTL_MAX" ]; then
	BPF_COUNT=$SYSCTL_MAX
fi

syslog -s -l notice "WaolaBPF: Ensuring $BPF_COUNT /dev/bpf* devices and setting permissions"

i=0
while [ "$i" -lt "$BPF_COUNT" ]; do
	read -n 0 < /dev/bpf$i > /dev/null 2>&1
	i=$(( $i + 1 ))
done
	
chgrp admin /dev/bpf*
chmod g+rw /dev/bpf*
