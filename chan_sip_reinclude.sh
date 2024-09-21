#!/bin/sh

# chan_sip reinclude script
# Readd chan_sip to the Asterisk source tree

# Abort if anything fails
set -e

printf "Reincluding chan_sip...\n"

SIPSRCDIR=/usr/src/chan_sip

# Do a simple sanity check: we must be in the Asterisk source dir
if [ ! -d channels ]; then
	printf "Not in the Asterisk source directory?\n"
	exit 2
fi

# If chan_sip is already present, abort
if [ -f channels/chan_sip.c ]; then
	printf "Detected chan_sip already present?\n"
	exit 2
fi

if [ ! -d $SIPSRCDIR ]; then
	printf "$SIPSRCDIR does not already exist... cloning\n"
	git clone https://github.com/InterLinked1/chan_sip $SIPSRCDIR
else
	# Run in a subshell, so we don't need to worry about returning
	# to the current directory afterwards
	printf "Detected $SIPSRCDIR already present...\n"
	if [ -f $SIPSRCDIR/.git ]; then
		(cd $SIPSRCDIR; git checkout master; git pull --ff-only)
	fi
fi
cp -r $SIPSRCDIR/channels/sip channels
cp $SIPSRCDIR/channels/chan_sip.c channels
git apply $SIPSRCDIR/channels/Makefile.diff
cp $SIPSRCDIR/configs/samples/*.sample configs/samples

printf "chan_sip has been successfully reincorporated into Asterisk\n"
