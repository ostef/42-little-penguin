#!/bin/sh

/lib/modules/$(uname -r)/build/scripts/checkpatch.pl -f */*.c */*.h
