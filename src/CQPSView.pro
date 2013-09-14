TEMPLATE = app

TARGET = 

DEPENDPATH += .

CONFIG += debug

# Input
SOURCES += \
CQPSViewTest.cpp \
CQPSView.cpp \
CQPSViewRenderer.cpp \
CPSViewArray.cpp \
CPSViewArrayToken.cpp \
CPSViewBooleanToken.cpp \
CPSView.cpp \
CPSViewDictionary.cpp \
CPSViewDictionaryToken.cpp \
CPSViewError.cpp \
CPSViewFile.cpp \
CPSViewFileToken.cpp \
CPSViewFilterFile.cpp \
CPSViewFilterFileToken.cpp \
CPSViewFontIdToken.cpp \
CPSViewGState.cpp \
CPSViewGStateToken.cpp \
CPSViewIntegerToken.cpp \
CPSViewMarkToken.cpp \
CPSViewMemory.cpp \
CPSViewName.cpp \
CPSViewNameToken.cpp \
CPSViewNullToken.cpp \
CPSViewOperator.cpp \
CPSViewOperatorToken.cpp \
CPSViewPackedArray.cpp \
CPSViewPackedArrayToken.cpp \
CPSViewPath.cpp \
CPSViewRealToken.cpp \
CPSViewResource.cpp \
CPSViewSave.cpp \
CPSViewSaveToken.cpp \
CPSViewStack.cpp \
CPSViewString.cpp \
CPSViewStringFile.cpp \
CPSViewStringFileToken.cpp \
CPSViewStringToken.cpp \
CPSViewTextFile.cpp \
CPSViewTextFileToken.cpp \
CPSViewToken.cpp \
CFreeType.cpp \

HEADERS += \
CQPSView.h \
CQPSViewRenderer.h \
CPSViewArray.h \
CPSViewArrayToken.h \
CPSViewBooleanToken.h \
CPSViewDictionary.h \
CPSViewDictionaryToken.h \
CPSViewError.h \
CPSViewFile.h \
CPSViewFileToken.h \
CPSViewFilterFile.h \
CPSViewFilterFileToken.h \
CPSViewFontIdToken.h \
CPSViewGStateFont.h \
CPSViewGState.h \
CPSViewGStatePattern.h \
CPSViewGStateToken.h \
CPSView.h \
CPSViewI.h \
CPSViewIntegerToken.h \
CPSViewMarkToken.h \
CPSViewMemory.h \
CPSViewName.h \
CPSViewNameToken.h \
CPSViewNullToken.h \
CPSViewOperator.h \
CPSViewOperatorToken.h \
CPSViewPackedArray.h \
CPSViewPackedArrayToken.h \
CPSViewPath.h \
CPSViewRealToken.h \
CPSViewResource.h \
CPSViewSave.h \
CPSViewSaveToken.h \
CPSViewSharedMem.h \
CPSViewStack.h \
CPSViewStringFile.h \
CPSViewStringFileToken.h \
CPSViewString.h \
CPSViewStringToken.h \
CPSViewTextFile.h \
CPSViewTextFileToken.h \
CPSViewToken.h \
CPSViewTypes.h \
CFreeType.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
. \
../../CQUtil/include \
../../CImageLib/include \
../../CConfig/include \
../../CArgs/include \
../../CFile/include \
../../CFont/include \
../../CRegExp/include \
../../CGlob/include \
../../CStrUtil/include \
../../CReadLine/include \
../../COS/include \
../../CMath/include \
../../CUtil/include \
/usr/include/freetype2 \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CArgs/lib \
-L../../CConfig/lib \
-L../../CFont/lib \
-L../../CRegExp/lib \
-L../../CGlob/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../CReadLine/lib \
-L../../CStrUtil/lib \
-L../../COS/lib \
-lCQUtil \
-lCImageLib \
-lCFont \
-lCArgs \
-lCConfig \
-lCReadLine \
-lCFile \
-lCOS \
-lCRegExp \
-lCGlob \
-lCStrUtil \
-ljpeg -lpng -ltre -lreadline
