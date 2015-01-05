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

#ifndef TARGOMAN_CORE_PRIVATE_INPUT_CLSINPUT_H
#define TARGOMAN_CORE_PRIVATE_INPUT_CLSINPUT_H

#include <QList>
#include "Configurations.h"
#include "clsToken.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace Input {

TARGOMAN_ADD_EXCEPTION_HANDLER(exInput, exTargomanCore);

class clsInput
{
public:
    clsInput();

    void parsePlain(const QString& _inputStr, const QString& _lang = "");
    void parseRichIXML(const QString& _inputIXML);

private:
    QList<clsToken> Tokens;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_INPUT_CLSINPUT_H