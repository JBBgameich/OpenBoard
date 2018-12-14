#!/usr/bin/env bash

if command -v sudo; then
        export SUDO=$(command -v sudo)
fi

if command -v qmake; then
        export QMAKE=$(command -v qmake)
elif command -v qmake-qt5; then
        export QMAKE=$(command -v qmake-qt5)
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

QT_SELECT=5 $QMAKE CONFIG+=ccache
make
