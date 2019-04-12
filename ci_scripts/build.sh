#!/usr/bin/env bash

. ./ci_scripts/common.sh

# Set up ccache
export PATH=/usr/lib/ccache/:$PATH
export CCACHE_BASEDIR=${PWD}
export CCACHE_DIR=${PWD}/ccache
if [ -d $CCACHE_DIR ]; then
	$SUDO chown -R $(whoami):$(whoami) $CCACHE_DIR
else
	mkdir -p $CCACHE_DIR
fi

mkdir -p QtWebKit && touch QtWebKit/QWebView
QT_SELECT=5 qmake CONFIG+=ccache
$BEAR make -j$(nproc)
