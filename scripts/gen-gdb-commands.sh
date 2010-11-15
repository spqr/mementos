#!/bin/sh
SELF=`basename "$0"`

if [ $# -ne 1 ]; then
	>&2 echo "Usage: ${SELF} executable-file"
	exit 1
fi
EXEFILE="$1"

ALWAYS_INTERESTING="\<main\>|_old_main"
MEMENTOS_INTERESTING="(${ALWAYS_INTERESTING}|${MEMENTOS_INTERESTING:-__mementos})"

TMPFILE=`mktemp -t ${SELF}` || exit 2

msp430-objdump -x "${EXEFILE}" | egrep "${MEMENTOS_INTERESTING}" > $TMPFILE
>&2 echo "Will create gdb breakpoints for these symbols in ${EXEFILE}:"
>&2 cat $TMPFILE
ADDRS=`< $TMPFILE cut -d' ' -f1 | sed -e 's/^0*//'`
for a in $ADDRS; do
	echo "break *0x${a}"
done

rm $TMPFILE
