#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
    ESTATUS="$1"
    ASTATUS="$2"

    # Make sure the program exited with the right exit status.
    if [ "$ASTATUS" -ne "$ESTATUS" ]; then
	fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
	return 1
    fi

    return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
    NAME="$1"
    EFILE="$2"
    AFILE="$3"

    # Make sure we're really expecting this file.
    if [ ! -f "$EFILE" ]; then
	return 0
    fi

    # Make sure the output matches the expected output.
    echo "   diff $EFILE $AFILE"
    diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
	fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
	return 1
    fi

    return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
    NAME="$1"
    EFILE="$2"
    AFILE="$3"

    # if the expected output file doesn't exist, the actual file should be
    # empty.
    if [ ! -f "$EFILE" ]; then
	if [ -s "$AFILE" ]; then
	    fail "FAILED - $NAME ($AFILE) should be empty."
	    return 1
	fi
	return 0
    fi

    # Make sure the output matches the expected output.
    echo "   diff $EFILE $AFILE"
    diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
	fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
	return 1
    fi

    return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should not exist.
checkFileOrDNE() {
    NAME="$1"
    EFILE="$2"
    AFILE="$3"
    
    if [ ! -f "$EFILE" ]; then
	# if the expected output file doesn't exist, the actual file
	# should not exist
	if [ -f "$AFILE" ]; then
	    fail "FAILED - $NAME ($AFILE) should not have been created."
	    return 1
	fi
	return 0
    else
	# If the expected output file does exist, so should the actual
	# file
	if [ ! -f "$AFILE" ]; then
	    fail "FAILED - $NAME ($AFILE) should have been created."
	    return 1
	fi
    fi

    # Make sure the output matches the expected output.
    echo "   diff $EFILE $AFILE"
    diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
	fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
	return 1
    fi

    return 0
}

# The given file should exist but should be empty.
checkEmpty() {
    NAME="$1"
    AFILE="$2"
    
    if [ -s "$AFILE" ]; then
	fail "FAILED - $NAME ($AFILE) should be empty."
	return 1
    fi

    return 0
}

# Test the ident program.
runTest() {
    TESTNO=$1
    ESTATUS=$2

    echo "Test $TESTNO"
    rm -f output.txt stdout.txt stderr.txt

    echo "   ./calc ${args[@]} > stdout.txt 2> stderr.txt"
    ./calc ${args[@]} > stdout.txt 2> stderr.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS" ||
       ! checkFileOrEmpty "Terminal output" "expected-stdout-$TESTNO.txt" "stdout.txt" ||
       ! checkFileOrDNE "Output file" "expected-$TESTNO.txt" "output.txt" ||
       ! checkFileOrEmpty "Stderr output" "expected-stderr-$TESTNO.txt" "stderr.txt"
    then
	return 1
    fi

    echo "Test $TESTNO PASS"
    return 0
}

# make a fresh copy of the target programs
make clean
make

if [ $? -ne 0 ]; then
    fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x calc ]; then
    args=(input-01.txt output.txt)
    runTest 01 0
    
    args=(input-02.txt output.txt)
    runTest 02 0
    
    args=(input-03.txt output.txt)
    runTest 03 0
    
    args=(input-04.txt)
    runTest 04 0
    
    args=(input-05.txt)
    runTest 05 0
    
    args=(input-06.txt output.txt)
    runTest 06 0
    
    args=(input-07.txt output.txt)
    runTest 07 0
    
    args=(input-08.txt)
    runTest 08 0
    
    args=(input-09.txt)
    runTest 09 0
    
    args=(input-10.txt)
    runTest 10 0
    
    args=(input-11.txt)
    runTest 11 0
    
    args=(input-12.txt output.txt)
    runTest 12 0
    
    args=(input-13.txt)
    runTest 13 0
    
    args=(input-14.txt output.txt)
    runTest 14 0
    
    args=(input-15.txt output.txt)
    runTest 15 1
    
    args=(input-16.txt output.txt)
    runTest 16 1
    
    args=(input-17.txt output.txt)
    runTest 17 1
    
    args=(input-18.txt output.txt)
    runTest 18 1
    
    args=(input-19.txt output.txt)
    runTest 19 1
    
    args=(input-20.txt output.txt)
    runTest 20 1
    
    args=(input-21.txt output.txt)
    runTest 21 1
    
    args=(input-22.txt output.txt)
    runTest 22 1
    
    args=(extra-argument.txt input-23.txt output.txt)
    runTest 23 1
    
    args=(missing-input-file.txt output.txt)
    runTest 24 1
else
    fail "Since your program didn't compile, no tests were run."
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi
