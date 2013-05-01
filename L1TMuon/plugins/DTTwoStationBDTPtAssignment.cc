#include "L1TriggerDPGUpgrade/L1TMuon/interface/DTTwoStationBDTPtAssignment.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
 
using namespace L1TMuon;

DTTwoStationBDTPtAssignment::
DTTwoStationBDTPtAssignment( const edm::ParameterSet& ps ):
  PtAssignmentUnit(ps) {
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
