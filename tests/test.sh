#! /bin/sh
#
# test.sh
# Copyright (C) 2023  <@Unidad01>
#
# Distributed under terms of the MIT license.
#
../fft.exe < test_input3.txt > out.txt
OUTPUT=$(diff -s out.txt gold_std1.txt)
EXPECTED="Files out.txt and gold_std1.txt are identical"
if [ "$OUTPUT" = "$EXPECTED" ]; then
	echo Passed
else
	echo Failed
fi
