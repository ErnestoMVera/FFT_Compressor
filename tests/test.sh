#! /bin/sh
#
# test.sh
# Copyright (C) 2023  <@Unidad01>
#
# Distributed under terms of the MIT license.
#
../fft -f < test_input3.txt > out.txt
OUTPUT=$(diff -s out.txt gold_std1.txt)
EXPECTED="Files out.txt and gold_std1.txt are identical"
if [ "$OUTPUT" = "$EXPECTED" ]; then
	echo Test 1 Passed
else
	echo Test 1 Failed
fi
../fft -i < input_inverse.txt > out.txt
OUTPUT=$(diff -s out.txt gold_std2.txt)
EXPECTED="Files out.txt and gold_std2.txt are identical"
if [ "$OUTPUT" = "$EXPECTED" ]; then
	echo Test 2 Passed
else
	echo Test 2 Failed
fi
rm out.txt
