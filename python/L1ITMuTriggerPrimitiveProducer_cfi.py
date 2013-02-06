import FWCore.ParameterSet.Config as cms

L1ITMuTriggerPrimitives = cms.EDProducer(
    'L1ITMuTriggerPrimitiveProducer',
    DT   = cms.PSet( collectorType = cms.string('DTCollector'),
                     src = cms.InputTag('dtTriggerPrimitiveDigis') ),
    RPCb = cms.PSet( collectorType = cms.string('RPCCollector'),
                     src = cms.InputTag('simRpcTriggerDigis', 'RPCb') ),
    CSC  = cms.PSet( collectorType = cms.string('CSCCollector'),
                     src = cms.InputTag('simCscTriggerPrimitiveDigis',
                                        'MPCSORTED') ),
    RPCf = cms.PSet( collectorType = cms.string('RPCCollector'),
                     src = cms.InputTag('simRpcTriggerDigis', 'RPCf') )    
    )
