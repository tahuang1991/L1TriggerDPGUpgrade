import FWCore.ParameterSet.Config as cms

from L1Trigger.DTTrackFinder.dttfDigis_cfi import dttfDigis

L1ITMuTriggerPrimitives = cms.EDProducer(
    'L1ITMuTriggerPrimitiveProducer',
    DT   = cms.PSet( collectorType = cms.string('DTCollector'),
                     src = cms.InputTag('simDtTriggerPrimitiveDigis'),
                     BX_min = cms.int32(dttfDigis.BX_min.value()),
                     BX_max = cms.int32(dttfDigis.BX_max.value()) ) ,
    RPC = cms.PSet( collectorType = cms.string('RPCCollector'),
                    src = cms.InputTag('simRpcDigis') ),
    CSC  = cms.PSet( collectorType = cms.string('CSCCollector'),
                     src = cms.InputTag('simCscTriggerPrimitiveDigis',
                                        'MPCSORTED') )
    )
