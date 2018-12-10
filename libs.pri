THIRD_PARTY_PATH="../OpenBoard-ThirdParty"

linux {
    CONFIG += link_pkgconfig
    PKGCONFIG += poppler
    PKGCONFIG += freetype2

    # Find different versions of quazip
    packagesExist(quazip) {
        PKGCONFIG += quazip
        message(using quazip >= 0.7.4 with pkgconfig support)
    }
    exists(/usr/include/quazip5/quazip.h) {
        INCLUDEPATH += "/usr/include/quazip5"
        LIBS        += "-lquazip5"
        message(using quazip =< 0.7.4 without pkgconfig support)
    }
}

!linux {
    include($$THIRD_PARTY_PATH/libs.pri)
}
