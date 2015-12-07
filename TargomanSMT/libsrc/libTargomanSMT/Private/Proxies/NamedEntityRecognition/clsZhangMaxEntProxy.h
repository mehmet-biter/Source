/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */

// There is no NER for anything but Statistical Machine Translation!
#ifndef SMT

#ifndef TARGOMAN_CORE_PRIVATE_PROXIES_NAMEDENTITYRECOGNITION_CLSZHANGMAXENTPROXY_H
#define TARGOMAN_CORE_PRIVATE_PROXIES_NAMEDENTITYRECOGNITION_CLSZHANGMAXENTPROXY_H

#include "Private/Proxies/NamedEntityRecognition/intfNamedEntityRecognizer.h"
#include "libMaxent/maxentmodel.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
namespace NamedEntityRecognition {

class clsZhangMaxEntProxy : public intfNamedEntityRecognizer
{
public:
    explicit clsZhangMaxEntProxy();

    ~clsZhangMaxEntProxy(){}

    void init();
    void tagNamedEntities(InputDecomposer::Sentence_t _sentence);

private:
    QVector<vector<string>> getMaxEntContexts(InputDecomposer::Sentence_t _sentence);

public:
    static Common::Configuration::tmplConfigurable<FilePath_t> FilePath;
    static bool ModelLoaded;
    static maxent::MaxentModel Model;

private:
    TARGOMAN_DEFINE_SINGLETONMODULE("ZhangMaxEntProxy", clsZhangMaxEntProxy);
};

}
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PROXIES_NAMEDENTITYRECOGNITION_CLSZHANGMAXENTPROXY_H

#endif
