#include "L1TriggerDPGUpgrade/L1TMuon/interface/DTTwoStationBDTPtAssignment.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "TString.h"

using namespace L1TMuon;

DTTwoStationBDTPtAssignment::
DTTwoStationBDTPtAssignment( const edm::ParameterSet& ps ):
  PtAssignmentUnit(ps) {
  for( int sta1 = 0; sta1 < 3; ++sta1 ) {
    for( int sta2 = sta1+1; sta2 < 4; ++sta2) {
      edm::FileInPath weightsfile = 
	ps.getParameter<edm::FileInPath>(Form("weights_MB%i_MB%i",
					      sta1+1,sta2+1));
      std::string mvaname = 
	ps.getParameter<std::string>(Form("mvaname_MB%i_MB%i",sta1+1,sta2+1));
      _bdt_readers[sta1][sta2].reset(new TMVA::Reader("!Color:!Silent"));
      _bdt_readers[sta1][sta2]->AddVariable("DTeta",
					    &_DTeta[sta1][sta2]);
      _bdt_readers[sta1][sta2]->AddVariable(Form("dPhi%i%i",sta1+1,sta2+1),
					    &_dPhi[sta1][sta2]);
      if( sta1 != 2 ) {
	_bdt_readers[sta1][sta2]->AddVariable(Form("Phib%i",sta1+1),
					      &_phiB_one[sta1]);
      }
      if( sta2 != 2 ) {
	_bdt_readers[sta1][sta2]->AddVariable(Form("Phib%i",sta2+1),
					      &_phiB_two[sta2]);
      }
      _bdt_readers[sta1][sta2]->BookMVA(mvaname.c_str(), 
					weightsfile.fullPath().c_str());
    }
  }
}

void DTTwoStationBDTPtAssignment::
updateEventSetup(const edm::EventSetup& es) {
  PtAssignmentUnit::updateEventSetup(es);
}

void DTTwoStationBDTPtAssignment::assignPt(InternalTrack& trk) const {
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnitFactory.h"
DEFINE_EDM_PLUGIN( PtAssignmentUnitFactory, 
		   DTTwoStationBDTPtAssignment, 
		   "DTTwoStationBDTPtAssignment");
