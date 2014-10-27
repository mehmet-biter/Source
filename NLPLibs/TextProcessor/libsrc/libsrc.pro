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
    libTargomanTextProcessor/Private/Normalizer.h \
    libTargomanTextProcessor/TextProcessor.h \
    libTargomanTextProcessor/Private/Unicode.hpp \
    libTargomanTextProcessor/Private/SpellCheckers/clsPersianSpellCorrector.h \
    libTargomanTextProcessor/Private/IXMLWriter.h \
    libTargomanTextProcessor/Private/SpellCorrector.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanTextProcessor/Private/Normalizer.cpp \
    libTargomanTextProcessor/TextProcessor.cpp \
    libTargomanTextProcessor/Private/SpellCheckers/clsPersianSpellCorrector.cpp \
    libTargomanTextProcessor/Private/IXMLWriter.cpp \
    libTargomanTextProcessor/Private/SpellCorrector.cpp

OTHER_FILES += \
    conf/Abbreviations.tbl \
    conf/Normalization.conf \
    conf/PersianVerbStepPresent.tbl \
    conf/PersianVerbStemPast.tbl \
    conf/PersianStartWithNa.tbl \
    conf/PersianStartWithBi_Ba.tbl \
    conf/PersianSpace2ZWNJ.tbl \
    conf/PersianNouns.tbl \
    conf/PersianAutoCorrectTerms.tbl \
    conf/PersianHamzeOrMadAllowed.tbl \
    conf/SpellCorrectors/Persian/VerbStemPast.tbl \
    conf/SpellCorrectors/Persian/StartWithNa.tbl \
    conf/SpellCorrectors/Persian/StartWithBi_Ba.tbl \
    conf/SpellCorrectors/Persian/Space2ZWNJ.tbl \
    conf/SpellCorrectors/Persian/Nouns.tbl \
    conf/SpellCorrectors/Persian/HamzeOrMadAllowed.tbl \
    conf/SpellCorrectors/Persian/AutoCorrectTerms.tbl \
    conf/SpellCorrectors/Persian/Adjectives.tbl \
    conf/SpellCorrectors/Persian/VerbStemPast.tbl \
    conf/SpellCorrectors/Persian/StartWithNa.tbl \
    conf/SpellCorrectors/Persian/StartWithBi_Ba.tbl \
    conf/SpellCorrectors/Persian/Space2ZWNJ.tbl \
    conf/SpellCorrectors/Persian/Nouns.tbl \
    conf/SpellCorrectors/Persian/HamzeOrMadAllowed.tbl \
    conf/SpellCorrectors/Persian/AutoCorrectTerms.tbl \
    conf/SpellCorrectors/Persian/Adjectives.tbl \
    conf/SpellCorrectors/Persian/VerbStemPresent.tbl \
    conf/SpellCorrectors/Persian/StartWith_Na.tbl \
    conf/SpellCorrectors/Persian/StartWith_Bi_Ba.tbl

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


