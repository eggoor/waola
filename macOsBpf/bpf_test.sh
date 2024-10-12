#!/bin/sh

success=0
i=0
while [ -c /dev/bpf$i ]; do
	if [ -w /dev/bpf$i ]; then
		success=1
		break
	fi
	i=$(( i + 1 ))
done

if [ "$success" == "1" ]; then
	echo 'Successfully opened '/dev/bpf$i' for both reading and writing ~> you'\''re all set'
else
	echo 'Unable to open any BPF device for both reading and writing ~> you need to adjust /dev/bpf* permissions'
fi
