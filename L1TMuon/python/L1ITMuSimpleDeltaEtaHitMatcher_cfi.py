import FWCore.ParameterSet.Config as cms

L1ITMuSimpleDeltaEtaHitMatcher = cms.EDProducer(
    'L1ITMuSimpleDeltaEtaHitMatcher',
    TriggerPrimitiveSrc = cms.InputTag('L1ITMuTriggerPrimitives'),
    genSrc = cms.InputTag('genParticles'),
    DetaWindowSize = cms.double(0.3)
    )
