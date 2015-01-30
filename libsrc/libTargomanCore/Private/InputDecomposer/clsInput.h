/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H
#define TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H

#include <QList>
#include <QSet>

#include "Private/GlobalConfigs.h"
#include "libTargomanCommon/Types.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"
#include "Private/InputDecomposer/clsToken.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace InputDecomposer {

TARGOMAN_ADD_EXCEPTION_HANDLER(exInput, exTargomanCore);

class clsInput
{
public:
    clsInput();
    ~clsInput();

    static void init();

    void parsePlain(const QString& _inputStr, const QString& _lang = "");
    void parseRichIXML(const QString& _inputIXML);
    void parseRichIXML(const QString& _inputIXML, bool _normalize, const QString &_lang = "");

    void clear();
    inline const QList<clsToken*>& tokens(){return this->Tokens;}

private:
    inline bool isSpace(const QChar& _ch){
        return _ch == ' ';// || _ch == '\n' || _ch == '\t';
    }

private:
    QList<clsToken*> Tokens;
    static QSet<QString>    SpecialTags;

    //Configuration
    static Targoman::Common::Configuration::tmplConfigurable<QString> UserDefinedTags;
    static LanguageModel::intfLMSentenceScorer*                       LMScorer;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H