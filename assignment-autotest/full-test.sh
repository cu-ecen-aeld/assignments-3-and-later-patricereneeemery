#!/bin/bash

echo "starting test with SKIP_BUILD=\"$SKIP_BUILD\" and DO_VALIDATE=\"$DO_VALIDATE\""

if [ -z "$SKIP_BUILD" ]; then
    make -C ../finder-app clean all
fi

./unit-test.sh
if [ $? -ne 0 ]; then
    echo "Unit test failed"
    exit 1
fi

if [ ! -f conf/assignment.txt ]; then
    echo "Missing conf/assignment.txt, no assignment to run"
    exit 1
fi

echo "All tests passed"
exit 0
