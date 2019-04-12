#!/usr/bin/env bash

. ./ci_scripts/common.sh

$SUDO apt-get update
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
        libquazip5-headers \
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
        qtbase5-private-dev \
        bear
