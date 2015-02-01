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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_NLPLIBS_PRIVATE_WEIGHTS_HPP
#define TARGOMAN_NLPLIBS_PRIVATE_WEIGHTS_HPP

namespace Targoman {
namespace NLPLibs {
namespace Private {

struct stuProb {
    float Prob;
};

struct stuProbAndBackoffWeights {
  quint64 ID;
  float Prob;
  float Backoff;
  inline stuProbAndBackoffWeights(quint64 _id = 0, float _prob = 0, float _backoff = 0){
      this->Prob = _prob;
      this->Backoff = _backoff;
      this->ID = _id;
  }
};

struct stuProbBackoffAndRestWeights {
  float Prob;
  float Backoff;
  float Rest;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_WEIGHTS_HPP