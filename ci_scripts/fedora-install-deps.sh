#!/usr/bin/env bash
dnf install \
    https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm -y

dnf install \
    git \
    qt5 \
    qt5-qtsvg-devel \
    qt5-qtwebkit-devel \
    qt5-qtxmlpatterns-devel \
    qt5-qtscript-devel \
    openssl-devel \
    libpaper-devel \
    motif-devel \
    qt5-qtmultimedia-devel \
    qt5-qttools-devel \
    redhat-lsb-core \
    poppler-devel \
    quazip-qt5-devel \
    ffmpeg-devel \
    qt5-qttools-static \
    poppler-cpp-devel \
    ccache \
    -y

