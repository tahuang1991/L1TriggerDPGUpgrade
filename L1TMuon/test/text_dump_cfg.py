import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.L1ITMuText = cms.EDAnalyzer(
    'L1ITMuTextDumper',
    doGen = cms.untracked.bool(True),
    genSrc = cms.untracked.InputTag("genParticles"),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1ITMuTriggerPrimitives','CSC'),
    cms.InputTag('L1ITMuTriggerPrimitives','DT'),
    cms.InputTag('L1ITMuTriggerPrimitives','RPC')
    ),
    converterSrcs = cms.VInputTag(
    cms.InputTag('L1CSCTFTrackConverter'),
    cms.InputTag('L1DTTFTrackConverter'),
    cms.InputTag('L1RPCbTFTrackConverter'),
    cms.InputTag('L1RPCfTFTrackConverter'),
    cms.InputTag('L1ITMuSimpleDeltaEtaHitMatcher')
    )
)

infile = 'file:L1ITMU.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1ITMUSequence = cms.Sequence(process.L1ITMuText)

process.L1ITMUPath = cms.Path(process.L1ITMUSequence)

process.schedule = cms.Schedule(process.L1ITMUPath)
