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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_H
#define TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_H

#include <QHash>
#include <QVariantHash>
#include "ISO639.h" //From https://github.com/softnhard/ISO639

#include "libTargomanTextProcessor/TextProcessor.h"
#include "libTargomanTextProcessor/Private/Normalizer.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSpellCorrecter, exTextProcessor);

class intfSpellCorrector
{
protected:
    struct stuConfigStep{
        bool    IsKeyVal;
        union{
            QHash<QString, QString>* KeyValStorage;
            QSet<QString>*           ListStorage;
        };

        stuConfigStep(QHash<QString, QString>* _storage){
            this->IsKeyVal = true;
            this->KeyValStorage = _storage;
        }

        stuConfigStep(QSet<QString>* _storage){
            this->IsKeyVal = false;
            this->ListStorage = _storage;
        }

        stuConfigStep(){
            this->KeyValStorage = NULL;
        }
    };

public:
    intfSpellCorrector(){this->Active = false;}
    virtual ~intfSpellCorrector(){}

    inline bool active() const {return this->Active;}
    inline const QHash<QString, QString>&  autoCorrectTerms(){return this->AutoCorrectTerms;}
    inline size_t maxAutoCorrectTokens(){return this->MaxAutoCorrectTokens;}
    bool init(const QVariantHash _settings);

    virtual QString process(const QStringList& _tokens) = 0;
    virtual bool canBeCheckedInteractive(const QString& _inputWord) const = 0;
    virtual void storeAutoCorrectTerm(const QString& _from, const QString& _to) = 0;

    virtual QString process(const QString& _token){
        return this->AutoCorrectTerms.value(_token);
    }

protected:
    virtual bool postInit(const QVariantHash _settings) = 0;

protected:
    QHash<QString, QString>           AutoCorrectTerms;
    QHash<QString, stuConfigStep>     ConfigSteps;
    int  MaxAutoCorrectTokens;
    QString AutoCorrectFile;
    bool Active;
    QString Lang;
};

class SpellCorrector
{
public:
    static SpellCorrector& instance() {return Q_LIKELY(Instance) ? *Instance : *(Instance = new SpellCorrector);}

    QString process(const QString& _lang, const QString& _inputStr, bool _interactive);
    bool init(const QHash<QString, QVariantHash> &_settings);

private:
    SpellCorrector();

private:
    QHash<QString, intfSpellCorrector*> Processors;
    static SpellCorrector* Instance;
};

}
}
}
#endif // TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_H