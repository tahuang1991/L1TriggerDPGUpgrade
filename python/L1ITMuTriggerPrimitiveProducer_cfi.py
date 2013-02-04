import FWCore.ParameterSet.Config as cms

L1ITMuTriggerPrimitives = cms.EDProducer(
    'L1ITMuTriggerPrimitiveProducer',
    DT   = cms.PSet( src = cms.InputTag('dtTriggerPrimitiveDigis') ),
    RPCb = cms.PSet( src = cms.InputTag('simRpcTriggerDigis', 'RPCb') ),
    CSC  = cms.PSet( src = cms.InputTag('cscTriggerPrimitiveDigis',
                                        'MPCSORTED') ),
    RPCf = cms.PSet( src = cms.InputTag('simRpcTriggerDigis', 'RPCf') )    
    )
