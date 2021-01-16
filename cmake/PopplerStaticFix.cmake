# Poppler builds a static library on MSVC, but doesn't tell us which libraries it used
# Link against them ourselves
if (WIN32)
    find_package(JPEG REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(TIFF REQUIRED)
    find_package(PNG REQUIRED)
    find_package(OpenJPEG REQUIRED)
    find_package(LCMS2 REQUIRED)

    set(
        Poppler_LIBRARIES
        JPEG::JPEG
        TIFF::TIFF
        PNG::PNG
        LCMS2::LCMS2
        ${OPENJPEG_LIBRARIES}
        ${FREETYPE_LIBRARIES}
    )
endif()
