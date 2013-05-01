#ifndef __L1TMUON_DTTWOSTATIONBDTPTASSIGNMENT_H__
#define __L1TMUON_DTTWOSTATIONBDTPTASSIGNMENT_H__
// 
// Class: L1TMuon::DTTwoStationBDTPtAssignment
//
// Info: An implementation of the two station BDT pt assignment 
//       used in the TDR studies.
//       Note: this only works for DT tracks!
//
// Author: L. Gray (FNAL), B. Scurlock (UF)
//
#include <vector>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1TMuon {
  
  class DTTwoStationBDTPtAssignment : public PtAssignmentUnit {
  public:
    DTTwoStationBDTPtAssignment(const edm::ParameterSet&);
    ~DTTwoStationBDTPtAssignment() {}

    virtual void updateEventSetup(const edm::EventSetup&); 
    
    virtual void assignPt(InternalTrack&) const;
  private:    
  };
}

#endif
