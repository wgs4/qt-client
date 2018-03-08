include( ../global.pri )

TARGET   = scriptapitest
TEMPLATE = app
CONFIG   += qt warn_on

QT += core printsupport script serialport sql webchannel websockets \
      widgets xml xmlpatterns

equals(QT_MAJOR_VERSION, 5) {
  lessThan (QT_MINOR_VERSION, 9) {
    QT += webkit webkitwidgets
  }
}

INCLUDEPATH += ../scriptapi \
               ../common \
               ../../xtuple-build-desktop/scriptapi \
               ../../xtuple-build-desktop/common

DEPENDPATH  += $${INCLUDEPATH} ../lib

win32-msvc* {
  PRE_TARGETDEPS += ../lib/xtuplescriptapi.lib          \
                    ../lib/xtuplecommon.$${XTLIBEXT}    \
                    $${OPENRPT_LIBDIR}/qzint.$${OPENRPTLIBEXT}    \
                    $${OPENRPT_LIBDIR}/MetaSQL.$${OPENRPTLIBEXT}  \
                    $${OPENRPT_LIBDIR}/renderer.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/wrtembed.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/openrptcommon.$${OPENRPTLIBEXT}
} else {
  PRE_TARGETDEPS += ../lib/libxtuplecommon.$${XTLIBEXT} \
                    ../lib/libxtuplescriptapi.a         \
                    $${OPENRPT_LIBDIR}/libqzint.$${OPENRPTLIBEXT}    \
                    $${OPENRPT_LIBDIR}/libMetaSQL.$${OPENRPTLIBEXT}  \
                    $${OPENRPT_LIBDIR}/librenderer.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/libwrtembed.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/libopenrptcommon.$${OPENRPTLIBEXT}
}

QMAKE_LIBDIR = ../lib $${OPENRPT_LIBDIR} $$QMAKE_LIBDIR
LIBS        += -lqzint $${DMTXLIB} -lxtuplescriptapi -lxtuplecommon -lopenrptcommon
LIBS        += -lrenderer -lMetaSQL

OBJECTS_DIR = tmp
win32 {
  win32-msvc*:LIBS += -lshell32
}

DESTDIR     = .
MOC_DIR     = moc
UI_DIR      = ui

FORMS = \
    scriptapitestresults.ui

HEADERS = \ 
    scriptapitestresults.h

SOURCES = scriptapitest.cpp \
    scriptapitestresults.cpp
