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
#include <memory>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

namespace L1TMuon {
  
  class DTTwoStationBDTPtAssignment : public PtAssignmentUnit {
    typedef std::unique_ptr<TMVA::Reader> pTMVAReader;
  public:
    DTTwoStationBDTPtAssignment(const edm::ParameterSet&);
    ~DTTwoStationBDTPtAssignment() {}

    virtual void updateEventSetup(const edm::EventSetup&); 
    
    virtual void assignPt(InternalTrack&) const;
  private:
    pTMVAReader _bdt_readers[3][4]; //[sta 1][sta 2]
    Float_t _DTeta[3][4],_dPhi[3][4],_phiB_one[3], _phiB_two[4];
  };
}

#endif
