#ifndef __L1TMUON_CORRIDORPTREFINEMENT_H__
#define __L1TMUON_CORRIDORPTREFINEMENT_H__
// 
// Class: L1TMuon::CorridorPtRefinement
//
// Info: Implements the 'corridor' (confidence interval)
//       based tail-clipping from B. Scurlock.
//
// Author: L. Gray (FNAL), B. Scurlock (UF)
//
#include <vector>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1TMuon {
  
  class CorridorPtRefinement: public PtRefinementUnit {
  public:
    CorridorPtRefinement(const edm::ParameterSet&);
    ~CorridorPtRefinement() {}

    virtual void refinePt(const edm::EventSetup&, 
			  InternalTrack&) const;
  private:    
  };
}

#endif
