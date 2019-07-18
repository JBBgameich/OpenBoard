#!/usr/bin/env bash

if command -v sudo; then
        export SUDO=$(command -v sudo)
fi

# Set up ccache
export PATH=/usr/lib/ccache/:$PATH
export CCACHE_BASEDIR=${PWD}
export CCACHE_DIR=${PWD}/ccache
if [ -d $CCACHE_DIR ]; then
	$SUDO chown -R $(whoami):$(whoami) $CCACHE_DIR
else
	mkdir -p $CCACHE_DIR
fi

cmake .
make
