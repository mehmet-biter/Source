################################################################################
# Copyright © 2012-2014, Targoman.com
#
# Published under the terms of TCRL(Targoman Community Research License)
# You can find a copy of the license file with distributed source or
# download it from http://targoman.com/License.txt
#
################################################################################
BasePath = ".."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    libLanguageModel/clsLanguageModel.h \
    libLanguageModel/Definitions.h \
    libLanguageModel/Private/clsLanguageModel_p.h \
    libLanguageModel/Private/ARPAManager.h \
    libLanguageModel/clsLMSentenceScorer.h \
    libLanguageModel/Private/clsLMSentenceScorer_p.h \
    libLanguageModel/Private/clsProbingModel.h \
    libLanguageModel/Private/clsBaseModel.hpp \
    libLanguageModel/Private/WeightStructs.hpp \
    libLanguageModel/Private/NGram.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libLanguageModel/clsLanguageModel.cpp \
    libLanguageModel/Private/ARPAManager.cpp \
    libLanguageModel/clsLMSentenceScorer.cpp \
    libLanguageModel/Private/clsProbingModel.cpp \
    libLanguageModel/Private/NGram.cpp

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = lib
TARGET = $$ProjectName
DESTDIR = $$BaseLibraryFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC
INCLUDEPATH+=lib$$ProjectName

build_static {
    DEFINES += TARGOMAN_BUILD_STATIC
    CONFIG+= staticlib
}

QMAKE_POST_LINK += mkdir -p $$BaseLibraryIncludeFolder/lib$$ProjectName;
QMAKE_POST_LINK += cp -vf lib$$ProjectName/*.h lib$$ProjectName/*.hpp $$BaseLibraryIncludeFolder/lib$$ProjectName 2>/dev/null|| : ;
QMAKE_POST_LINK += mkdir -p $$BaseConfigFolder/  ;
QMAKE_POST_LINK += cp -rvf conf/* $$BaseConfigFolder/ 2>/dev/null|| : ;

