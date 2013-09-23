#!/bin/sh
echo 'Deploying Sonata.';
echo 'Copying QML...';

if [ -d "debug" ]; then
	cp -R -f -t debug assets/*
fi

if [ -d "release" ]; then
	cp -R -f -t release assets/*
fi

