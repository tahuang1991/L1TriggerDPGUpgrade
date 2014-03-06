#ifndef __L1TMUON_PTREFINEMENTUNIT_H__
#define __L1TMUON_PTREFINEMENTUNIT_H__
// 
// Class: L1TMuon::PtRefinementUnit
//
// Info: This is a base class for any algorithm that takes a found track
//       with assigned pT and applies a refinement to that estimate.
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrackFwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

namespace L1TMuon {
  
  class PtRefinementUnit {
  public:
    PtRefinementUnit(const edm::ParameterSet&);
    virtual ~PtRefinementUnit() {}

    virtual void updateEventSetup(const edm::EventSetup&);

    virtual void refinePt(InternalTrack&) = 0;
  protected:
    std::string _name;    
    edm::ESHandle<L1MuTriggerScales> position_scales;
    edm::ESHandle<L1MuTriggerPtScale> pt_scale;
  private:
    unsigned long long position_scalesCacheID;
    unsigned long long pt_scaleCacheID;
  };
}

#endif
