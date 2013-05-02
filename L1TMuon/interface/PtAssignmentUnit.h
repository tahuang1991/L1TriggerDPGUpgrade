#ifndef __L1TMUON_PTASSIGNMENTUNIT_H__
#define __L1TMUON_PTASSIGNMENTUNIT_H__
// 
// Class: L1TMuon::PtAssignmentUnit
//
// Info: This is a base class for any algorithm that takes a found track
//       and uses the available information to assign the track's pT.
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
  
  class PtAssignmentUnit {
  public:
    PtAssignmentUnit(const edm::ParameterSet&);
    virtual ~PtAssignmentUnit() {}

    virtual void updateEventSetup(const edm::EventSetup&);

    virtual void assignPt(InternalTrack&) = 0;
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
