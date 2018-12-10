THIRD_PARTY_PATH="../OpenBoard-ThirdParty"

linux {
    CONFIG += link_pkgconfig
    PKGCONFIG += poppler
    PKGCONFIG += freetype2

    # Find different versions of quazip
    packagesExist(quazip) {
        PKGCONFIG += quazip
        message("using quazip >= 0.7.4 with pkgconfig support")
    } else {
        exists(/usr/include/quazip5/quazip.h) {
            INCLUDEPATH += "/usr/include/quazip5"
            LIBS        += "-lquazip5"
            message("using quazip =< 0.7.4 without pkgconfig support")
        } else {
            DEPENDPATH += $$THIRD_PARTY_PATH/quazip/
            INCLUDEPATH += $$THIRD_PARTY_PATH/quazip/
            include($$THIRD_PARTY_PATH/quazip/quazip.pri)
            message("quazip not found on the system, falling back to ThirdParty")
        }
    }
}

!linux {
    include($$THIRD_PARTY_PATH/libs.pri)
}
