cmake_minimum_required(VERSION 3.3)
project(openboard)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include(FeatureSummary)


#
# Version
#

set(VERSION_MAJ 1)
set(VERSION_MIN 5)
set(VERSION_PATCH 3)
set(VERSION_TYPE r) # a = alpha, b = beta, rc = release candidate, r = release, other => error
set(VERSION_BUILD 0)

set(VERSION "${VERSION_MAJ}.${VERSION_MIN}.${VERSION_PATCH}-${VERSION_TYPE}.${VERSION_BUILD}")

if(VERSION_TYPE STREQUAL "r")
    set(VERSION "${VERSION_MAJ}.${VERSION_MIN}.${VERSION_PATCH}")
endif()

set(VERSION_RC "${VERSION_MAJ},${VERSION_MIN},${VERSION_PATCH},${VERSION_TYPE},${VERSION_BUILD}")
string(REPLACE "a" "160" VERSION_RC ${VERSION_RC}) # 0xA0
string(REPLACE "b" "176" VERSION_RC ${VERSION_RC}) # 0xB0
string(REPLACE "rc" "192" VERSION_RC ${VERSION_RC}) # 0xC0
string(REPLACE "r" "240" VERSION_RC ${VERSION_RC}) # 0xF0

#
# Dependencies
#
find_package(Qt5 REQUIRED COMPONENTS
    Multimedia
    MultimediaWidgets
    Network
    PrintSupport
    Script
    Svg
    UiTools
    Xml
    XmlPatterns
    WebKit
    WebKitWidgets
    LinguistTools
)

find_package(ECM REQUIRED) # Required for finding poppler
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(QuaZip 0.6 REQUIRED)
find_package(Poppler REQUIRED COMPONENTS Core Cpp)
find_package(X11 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

#
# Main sources
#

set(OPENBOARD_SOURCES
    plugins/cffadaptor/src/UBCFFAdaptor.cpp
    src/adaptors/UBCFFSubsetAdaptor.cpp
    src/adaptors/UBExportAdaptor.cpp
    src/adaptors/UBExportCFF.cpp
    src/adaptors/UBExportDocument.cpp
    src/adaptors/UBExportDocumentSetAdaptor.cpp
    src/adaptors/UBExportFullPDF.cpp
    src/adaptors/UBExportPDF.cpp
    src/adaptors/UBExportWeb.cpp
    src/adaptors/UBImportAdaptor.cpp
    src/adaptors/UBImportCFF.cpp
    src/adaptors/UBImportDocument.cpp
    src/adaptors/UBImportDocumentSetAdaptor.cpp
    src/adaptors/UBImportImage.cpp
    src/adaptors/UBImportPDF.cpp
    src/adaptors/UBMetadataDcSubsetAdaptor.cpp
    src/adaptors/UBSvgSubsetAdaptor.cpp
    src/adaptors/UBThumbnailAdaptor.cpp
    src/api/UBLibraryAPI.cpp
    src/api/UBW3CWidgetAPI.cpp
    src/api/UBWidgetMessageAPI.cpp
    src/api/UBWidgetUniboardAPI.cpp
    src/board/UBBoardController.cpp
    src/board/UBBoardPaletteManager.cpp
    src/board/UBBoardView.cpp
    src/board/UBDrawingController.cpp
    src/board/UBFeaturesController.cpp
    src/core/UBApplication.cpp
    src/core/UBApplicationController.cpp
    src/core/UBDisplayManager.cpp
    src/core/UBDocumentManager.cpp
    src/core/UBDownloadManager.cpp
    src/core/UBDownloadThread.cpp
    src/core/UBForeignObjectsHandler.cpp
    src/core/UBIdleTimer.cpp
    src/core/UBMimeData.cpp
    src/core/UBOpenSankoreImporter.cpp
    src/core/UBPersistenceManager.cpp
    src/core/UBPersistenceWorker.cpp
    src/core/UBPreferencesController.cpp
    src/core/UBSceneCache.cpp
    src/core/UBSetting.cpp
    src/core/UBSettings.cpp
    src/core/UBTextTools.cpp
    src/core/main.cpp
    src/desktop/UBCustomCaptureWindow.cpp
    src/desktop/UBDesktopAnnotationController.cpp
    src/desktop/UBDesktopPalette.cpp
    src/desktop/UBDesktopPropertyPalette.cpp
    src/desktop/UBWindowCapture.h
    src/document/UBDocumentContainer.cpp
    src/document/UBDocumentController.cpp
    src/document/UBDocumentProxy.cpp
    src/document/UBSortFilterProxyModel.cpp
    src/domain/UBGraphicsDelegateFrame.cpp
    src/domain/UBGraphicsGroupContainerItem.cpp
    src/domain/UBGraphicsGroupContainerItemDelegate.cpp
    src/domain/UBGraphicsItemDelegate.cpp
    src/domain/UBGraphicsItemGroupUndoCommand.cpp
    src/domain/UBGraphicsItemTransformUndoCommand.cpp
    src/domain/UBGraphicsItemUndoCommand.cpp
    src/domain/UBGraphicsItemZLevelUndoCommand.cpp
    src/domain/UBGraphicsMediaItem.cpp
    src/domain/UBGraphicsMediaItemDelegate.cpp
    src/domain/UBGraphicsPDFItem.cpp
    src/domain/UBGraphicsPixmapItem.cpp
    src/domain/UBGraphicsPolygonItem.cpp
    src/domain/UBGraphicsProxyWidget.cpp
    src/domain/UBGraphicsScene.cpp
    src/domain/UBGraphicsStroke.cpp
    src/domain/UBGraphicsStrokesGroup.cpp
    src/domain/UBGraphicsSvgItem.cpp
    src/domain/UBGraphicsTextItem.cpp
    src/domain/UBGraphicsTextItemDelegate.cpp
    src/domain/UBGraphicsTextItemUndoCommand.cpp
    src/domain/UBGraphicsWidgetItem.cpp
    src/domain/UBGraphicsWidgetItemDelegate.cpp
    src/domain/UBItem.cpp
    src/domain/UBPageSizeUndoCommand.cpp
    src/domain/UBResizableGraphicsItem.cpp
    src/domain/UBSelectionFrame.cpp
    src/domain/UBUndoCommand.cpp
    src/frameworks/UBBase32.cpp
    src/frameworks/UBCoreGraphicsScene.cpp
    src/frameworks/UBCryptoUtils.cpp
    src/frameworks/UBFileSystemUtils.cpp
    src/frameworks/UBGeometryUtils.cpp
    src/frameworks/UBPlatformUtils.cpp
    src/frameworks/UBStringUtils.cpp
    src/frameworks/UBVersion.cpp
    src/gui/UBActionPalette.cpp
    src/gui/UBBackgroundPalette.cpp
    src/gui/UBBlackoutWidget.cpp
    src/gui/UBBoardThumbnailsView.cpp
    src/gui/UBCachePropertiesWidget.cpp
    src/gui/UBCircleFrame.cpp
    src/gui/UBColorPicker.cpp
    src/gui/UBDockDownloadWidget.cpp
    src/gui/UBDockPalette.cpp
    src/gui/UBDockPaletteWidget.cpp
    src/gui/UBDocumentNavigator.cpp
    src/gui/UBDocumentThumbnailWidget.cpp
    src/gui/UBDocumentToolsPalette.cpp
    src/gui/UBDownloadWidget.cpp
    src/gui/UBFavoriteToolPalette.cpp
    src/gui/UBFeaturesActionBar.cpp
    src/gui/UBFeaturesWidget.cpp
    src/gui/UBFloatingPalette.cpp
    src/gui/UBIconButton.cpp
    src/gui/UBKeyboardPalette.cpp
    src/gui/UBLeftPalette.cpp
    src/gui/UBMagnifer.cpp
    src/gui/UBMainWindow.cpp
    src/gui/UBMessageWindow.cpp
    src/gui/UBMessagesDialog.cpp
    src/gui/UBMousePressFilter.cpp
    src/gui/UBOpenSankoreImporterWidget.cpp
    src/gui/UBPageNavigationWidget.cpp
    src/gui/UBPropertyPalette.cpp
    src/gui/UBResources.cpp
    src/gui/UBRightPalette.cpp
    src/gui/UBRubberBand.cpp
    src/gui/UBScreenMirror.cpp
    src/gui/UBSpinningWheel.cpp
    src/gui/UBStartupHintsPalette.cpp
    src/gui/UBStylusPalette.cpp
    src/gui/UBThumbnailView.cpp
    src/gui/UBThumbnailWidget.cpp
    src/gui/UBToolWidget.cpp
    src/gui/UBToolbarButtonGroup.cpp
    src/gui/UBUpdateDlg.cpp
    src/gui/UBWebToolsPalette.cpp
    src/gui/UBWidgetMirror.cpp
    src/gui/UBZoomPalette.cpp
    src/network/UBAutoSaver.cpp
    src/network/UBCookieJar.cpp
    src/network/UBHttpFileDownloader.cpp
    src/network/UBHttpGet.cpp
    src/network/UBNetworkAccessManager.cpp
    src/network/UBServerXMLHttpRequest.cpp
    src/pdf-merger/ASCII85Decode.cpp
    src/pdf-merger/ASCIIHexDecode.cpp
    src/pdf-merger/AnnotsHandler.cpp
    src/pdf-merger/CCITTFaxDecode.cpp
    src/pdf-merger/ContentHandler.cpp
    src/pdf-merger/DCTDecode.cpp
    src/pdf-merger/Document.cpp
    src/pdf-merger/Filter.cpp
    src/pdf-merger/FilterPredictor.cpp
    src/pdf-merger/FlateDecode.cpp
    src/pdf-merger/JBIG2Decode.cpp
    src/pdf-merger/LZWDecode.cpp
    src/pdf-merger/Merger.cpp
    src/pdf-merger/Object.cpp
    src/pdf-merger/OverlayDocumentParser.cpp
    src/pdf-merger/Page.cpp
    src/pdf-merger/PageElementHandler.cpp
    src/pdf-merger/Parser.cpp
    src/pdf-merger/Rectangle.cpp
    src/pdf-merger/RemoveHimselfHandler.cpp
    src/pdf-merger/RunLengthDecode.cpp
    src/pdf-merger/Utils.cpp
    src/pdf/GraphicsPDFItem.cpp
    src/pdf/PDFRenderer.cpp
    src/pdf/UBWebPluginPDFWidget.cpp
    src/pdf/XPDFRenderer.cpp
    src/podcast/UBAbstractVideoEncoder.cpp
    src/podcast/UBPodcastController.cpp
    src/podcast/UBPodcastRecordingPalette.cpp
    src/podcast/intranet/UBIntranetPodcastPublisher.cpp
    src/podcast/youtube/UBYouTubePublisher.cpp
    src/tools/UBAbstractDrawRuler.cpp
    src/tools/UBGraphicsCache.cpp
    src/tools/UBGraphicsCompass.cpp
    src/tools/UBGraphicsCurtainItem.cpp
    src/tools/UBGraphicsCurtainItemDelegate.cpp
    src/tools/UBGraphicsProtractor.cpp
    src/tools/UBGraphicsRuler.cpp
    src/tools/UBGraphicsTriangle.cpp
    src/tools/UBToolsManager.cpp
    src/web/UBOEmbedParser.cpp
    src/web/UBTrapFlashController.cpp
    src/web/UBWebController.cpp
    src/web/UBWebKitUtils.cpp
    src/web/UBWebPage.cpp
    src/web/UBWebPluginWidget.cpp
    src/web/browser/WBBrowserWindow.cpp
    src/web/browser/WBChaseWidget.cpp
    src/web/browser/WBDownloadManager.cpp
    src/web/browser/WBEditTableView.cpp
    src/web/browser/WBHistory.cpp
    src/web/browser/WBModelMenu.cpp
    src/web/browser/WBSearchLineEdit.cpp
    src/web/browser/WBSqueezeLabel.cpp
    src/web/browser/WBTabWidget.cpp
    src/web/browser/WBToolBarSearch.cpp
    src/web/browser/WBUrlLineEdit.cpp
    src/web/browser/WBWebTrapWebView.cpp
    src/web/browser/WBWebView.cpp
)

# QtSingleApplication
set(OPENBOARD_SOURCES
    ${OPENBOARD_SOURCES}
    src/qtsingleapplication/src/qtlocalpeer.cpp
    src/qtsingleapplication/src/qtlocalpeer.h
    src/qtsingleapplication/src/qtsingleapplication.cpp
    src/qtsingleapplication/src/qtsingleapplication.h
)

#
# Platform specific sources
#
if(UNIX AND NOT APPLE)
    set(OPENBOARD_SOURCES
        ${OPENBOARD_SOURCES}
        src/desktop/UBWindowCapture_linux.cpp
        src/frameworks/UBPlatformUtils_linux.cpp
        src/gui/UBKeyboardPalette_linux.cpp
        src/podcast/ffmpeg/UBFFmpegVideoEncoder.cpp
        src/podcast/ffmpeg/UBMicrophoneInput.cpp
    )
endif()

if(WIN32)
    set(OPENBOARD_SOURCES
        ${OPENBOARD_SOURCES}
        src/desktop/UBWindowCaptureDelegate_win.cpp src/desktop/UBWindowCaptureDelegate_win.h
        src/desktop/UBWindowCapture_win.cpp
        src/frameworks/UBPlatformUtils_win.cpp
        src/gui/UBKeyboardPalette_win.cpp
        src/podcast/windowsmedia/UBWaveRecorder.cpp src/podcast/windowsmedia/UBWaveRecorder.h
        src/podcast/windowsmedia/UBWindowsMediaFile.cpp src/podcast/windowsmedia/UBWindowsMediaFile.h
        src/podcast/windowsmedia/UBWindowsMediaVideoEncoder.cpp src/podcast/windowsmedia/UBWindowsMediaVideoEncoder.h
    )
endif()

if(APPLE)
    set(OPENBOARD_SOURCES
        ${OPENBOARD_SOURCES}
        src/desktop/UBWindowCapture_mac.mm
        src/frameworks/UBPlatformUtils_mac.mm
        src/gui/UBKeyboardPalette_mac.mm
        src/podcast/ffmpeg/UBFFmpegVideoEncoder.cpp src/podcast/ffmpeg/UBFFmpegVideoEncoder.h
        src/podcast/ffmpeg/UBMicrophoneInput.cpp src/podcast/ffmpeg/UBMicrophoneInput.h
    )
endif()

#
# UI files
#
qt5_wrap_ui(OPENBOARD_SOURCES
    resources/forms/blackoutWidget.ui
    resources/forms/brushProperties.ui
    resources/forms/capturePublishing.ui
    resources/forms/documents.ui
    resources/forms/intranetPodcastPublishingDialog.ui
    resources/forms/mainWindow.ui
    resources/forms/preferences.ui
    resources/forms/trapFlash.ui
    resources/forms/youTubePublishingDialog.ui
    src/web/browser/downloaditem.ui
    src/web/browser/downloads.ui
    src/web/browser/passworddialog.ui
    src/web/browser/proxy.ui
)

#
# Resources
#
qt5_add_resources(OPENBOARD_SOURCES
    resources/OpenBoard.qrc
)

#
# Translations
#
set(TS_FILES
   resources/i18n/OpenBoard_en_UK.ts
   resources/i18n/OpenBoard_fr.ts
   resources/i18n/OpenBoard_fr_CH.ts
   resources/i18n/OpenBoard_de.ts
   resources/i18n/OpenBoard_nl.ts
   resources/i18n/OpenBoard_es.ts
   resources/i18n/OpenBoard_it.ts
   resources/i18n/OpenBoard_pl.ts
   resources/i18n/OpenBoard_ru.ts
   resources/i18n/OpenBoard_da.ts
   resources/i18n/OpenBoard_nb.ts
   resources/i18n/OpenBoard_sv.ts
   resources/i18n/OpenBoard_ja.ts
   resources/i18n/OpenBoard_ko.ts
   resources/i18n/OpenBoard_zh.ts
   resources/i18n/OpenBoard_zh_CN.ts
   resources/i18n/OpenBoard_zh_TW.ts
   resources/i18n/OpenBoard_ro.ts
   resources/i18n/OpenBoard_ar.ts
   resources/i18n/OpenBoard_iw.ts
   resources/i18n/OpenBoard_pt.ts
   resources/i18n/OpenBoard_sk.ts
   resources/i18n/OpenBoard_bg.ts
   resources/i18n/OpenBoard_ca.ts
   resources/i18n/OpenBoard_el.ts
   resources/i18n/OpenBoard_tr.ts
   resources/i18n/OpenBoard_cs.ts
   resources/i18n/OpenBoard_gl.ts
   resources/i18n/OpenBoard_uk.ts
   resources/i18n/OpenBoard_hu.ts
   resources/i18n/OpenBoard_mg.ts
)

qt5_create_translation(QM_FILES
    ${CMAKE_SOURCE_DIR}
    ${TS_FILES}
)

qt5_add_translation(QM_FILES
    ${TS_FILES}
)

add_custom_target(translations DEPENDS ${QM_FILES})

#
# Executable
#
add_executable(openboard
    ${OPENBOARD_SOURCES}
)

add_definitions(
    -DNO_THIRD_PARTY_WARNINGS
    -DUBVERSION="${VERSION}"
    -DUBVERSION_RC="${VERSION_RC}"
)

target_include_directories(openboard PUBLIC
    plugins/cffadaptor/src
    src
    src/pdf-merger
    src/qtsingleapplication/src
    src/web/browser

    ${QUAZIP_INCLUDE_DIRS}
)

target_link_libraries(openboard
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::Network
    Qt5::PrintSupport
    Qt5::Script
    Qt5::Svg
    Qt5::UiTools
    Qt5::Xml
    Qt5::XmlPatterns
    Qt5::WebKit
    Qt5::WebKitWidgets
    Poppler::Core
    Poppler::Cpp
    OpenSSL::Crypto
    X11
    ${QUAZIP_LIBRARIES}
    ZLIB::ZLIB
)

if(UNIX)
    target_link_libraries(openboard
        swresample swscale avutil avcodec avformat
    )
endif()

#
# Installing
#
if(UNIX)
    install(TARGETS openboard DESTINATION bin/)
    install(DIRECTORY resources/ DESTINATION share/OpenBoard)
    install(FILES ${QM_FILES} DESTINATION share/OpenBoard/i18n)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
