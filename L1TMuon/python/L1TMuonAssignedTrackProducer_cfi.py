import FWCore.ParameterSet.Config as cms

#available Pt Assignment modules
from L1TriggerDPGUpgrade.L1TMuon.pt_assignment.Null_cfi \
     import Null as NullAssign

#available Pt Refinement modules
from L1TriggerDPGUpgrade.L1TMuon.pt_refinement.Null_cfi \
     import Null as NullRefine
from L1TriggerDPGUpgrade.L1TMuon.pt_refinement.DTTwoStationCorridor_cfi \
     import DTTwoStationCorridor 

L1TMuonAssignedTrackProducer = cms.EDProduct(
    "L1TMuonAssignedTrackProducer",
    src = cms.InputTag('L1DTTFTrackConverter'),
    PtAssignmentUnit = NullAssign,
    PtRefinementUnit = DTTwoStationCorridor
    )
