import FWCore.ParameterSet.Config as cms

#available Pt Assignment modules
from L1TriggerDPGUpgrade.L1TMuon.pt_assignment.Null_cfi \
     import Null as NullAssign
from L1TriggerDPGUpgrade.L1TMuon.pt_assignment.DTTwoStationBDT_cfi \
     import DTTwoStationBDT 

#available Pt Refinement modules
from L1TriggerDPGUpgrade.L1TMuon.pt_refinement.Null_cfi \
     import Null as NullRefine
from L1TriggerDPGUpgrade.L1TMuon.pt_refinement.DTTwoStationCorridor_cfi \
     import DTTwoStationCorridor 

L1TMuonAssignedInternalTrackProducer = cms.EDProducer(
    "L1TMuonAssignedInternalTrackProducer",
    src = cms.InputTag('fixme'),
    PtAssignmentUnit = NullAssign,
    PtRefinementUnit = NullAssign
    )
