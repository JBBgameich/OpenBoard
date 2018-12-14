#!/usr/bin/env bash

if command -v sudo; then
	export SUDO=$(command -v sudo)
fi

$SUDO apt-get update
# Needed to detect dependencies in the next step
$SUDO apt-get install lsb-release -y
$SUDO apt-get install -y \
        ccache \
        build-essential \
        flex \
        libasound2-dev \
        libass-dev \
        libavcodec-dev \
        libavformat-dev \
        libbz2-dev \
        libfreetype6-dev \
        liblzma-dev \
        libmp3lame-dev \
        libogg-dev \
        libopus-dev \
        libpoppler-cpp-dev \
        libpoppler-dev \
        libpoppler-private-dev \
        libqt5svg5-dev \
        libqt5webkit5-dev \
        libqt5xmlpatterns5-dev \
        libquazip5-dev \
        $(if ! lsb_release -d | grep bullseye>/dev/null; then echo libquazip5-headers; fi) \
        libsdl-dev \
        libssl-dev \
        libswscale-dev \
        libtheora-dev \
        libva-dev \
        libvorbis-dev \
        libvpx-dev \
        libx264-dev \
        libxcb-shm0-dev \
        lzma-dev \
        qt5-qmake \
        qtbase5-dev \
        qtmultimedia5-dev \
        qtscript5-dev \
        qttools5-dev \
        qtbase5-private-dev
