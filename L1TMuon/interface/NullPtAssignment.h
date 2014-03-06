#ifndef __L1TMUON_NULLPTASSIGNMENT_H__
#define __L1TMUON_NULLPTASSIGNMENT_H__
// 
// Class: L1TMuon::NullPtAssignment
//
// Info: Pass-thru module
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1TMuon {
  
  class NullPtAssignment: public PtAssignmentUnit {
  public:
    NullPtAssignment(const edm::ParameterSet&);
    ~NullPtAssignment() {}

    virtual void updateEventSetup(const edm::EventSetup&) {}
    
    virtual void assignPt(InternalTrack&) {}
  private:    
  };
}

#endif
