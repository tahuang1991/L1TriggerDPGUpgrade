import FWCore.ParameterSet.Config as cms

#trigger primitives, etc.
from L1TriggerDPGUpgrade.L1TMuon.L1TMuonTriggerPrimitiveProducer_cfi import *

HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")

# old L1 track converters
from L1TriggerDPGUpgrade.L1TMuon.L1CSCTFTrackConverter_cfi import *
from L1TriggerDPGUpgrade.L1TMuon.L1DTTFTrackConverter_cfi import *
from L1TriggerDPGUpgrade.L1TMuon.L1RPCTFTrackConverter_cfi import *
from L1TriggerDPGUpgrade.L1TMuon.L1TMuonSimpleDeltaEtaHitMatcher_cfi import *

#pt assignment and refinement
from L1TriggerDPGUpgrade.L1TMuon.L1TMuonAssignedInternalTrackProducer_cfi \
     import *
# Bobby's DT pt BDT for two stations
DTTwoStationAssignedTrackProducer = \
                                  L1TMuonAssignedInternalTrackProducer.clone()
DTTwoStationAssignedTrackProducer.src = cms.InputTag('L1DTTFTrackConverter')
DTTwoStationAssignedTrackProducer.PtAssignmentUnit = DTTwoStationBDT
DTTwoStationAssignedTrackProducer.PtRefinementUnit = DTTwoStationCorridor

#subsequence and final sequence definitions
L1TMuonConverterSequence = cms.Sequence( L1CSCTFTrackConverter    +
                                         L1DTTFTrackConverter     +
                                         L1RPCTFTrackConverters    )

L1TMuonTrackBuilderSequence = cms.Sequence( L1TMuonSimpleDeltaEtaHitMatcher )

L1TMuonPtAssignmentSequence = cms.Sequence( DTTwoStationAssignedTrackProducer )

L1TMuonSequence = cms.Sequence( L1TMuonTriggerPrimitives    +
                                L1TMuonConverterSequence    + 
                                L1TMuonTrackBuilderSequence +
                                L1TMuonPtAssignmentSequence  )
