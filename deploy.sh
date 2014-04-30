#!/bin/bash
echo 'Deploying Sonetta...'

SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DEBUGDIR="$SCRIPTDIR/debug"
RELEASEDIR="$SCRIPTDIR/release"

# Copy assets to debug/release folder and copy 
# output to root folder

if [ -d $DEBUGDIR ]; then
	cp -R -f -t $DEBUGDIR $SCRIPTDIR/core/assets/*
fi

if [ -d $RELEASEDIR ]; then
	cp -R -f -t $RELEASEDIR $SCRIPTDIR/core/assets/*
fi

echo 'Deployment finished.'



