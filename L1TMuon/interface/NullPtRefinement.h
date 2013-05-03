#ifndef __L1TMUON_NULLPTREFINEMENT_H__
#define __L1TMUON_NULLPTREFINEMENT_H__
// 
// Class: L1TMuon::NullPtRefinement
//
// Info: Pass-thru module
//
// Author: L. Gray (FNAL),
//
#include <memory>
#include <vector>
#include <unordered_map>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TH1F.h"

class TGraph;
class TH1F;

namespace L1TMuon {
  
  class NullPtRefinement : public PtRefinementUnit {    
  public:
    NullPtRefinement(const edm::ParameterSet&);
    ~NullPtRefinement() {}

    virtual void updateEventSetup(const edm::EventSetup&) {}

    virtual void refinePt(InternalTrack&) {}
  private:       
  };
}

#endif
