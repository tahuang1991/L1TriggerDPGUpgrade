import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.L1ITMuText = cms.EDAnalyzer(
    'L1ITMuTextDumper',
    doGen = cms.untracked.bool(True),
    genSrc = cms.untracked.InputTag("genParticles"),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1ITMuTriggerPrimitives','CSC'),
    cms.InputTag('L1ITMuTriggerPrimitives','DT'),
    cms.InputTag('L1ITMuTriggerPrimitives','RPCb'),
    cms.InputTag('L1ITMuTriggerPrimitives','RPCf')
    )
)

process.L1CSCTrackConverter = cms.EDProducer(
    'L1CSCTFTrackConverter',
    CSCTrackSrc = cms.InputTag('simCsctfTrackDigis',''),
    TriggerPrimitiveSrc = cms.InputTag('L1ITMuTriggerPrimitives','')
    )

infile = 'file:L1ITMU.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1ITMUSequence = cms.Sequence(process.L1ITMuText+
                                      process.L1CSCTrackConverter)

process.L1ITMUPath = cms.Path(process.L1ITMUSequence)

process.schedule = cms.Schedule(process.L1ITMUPath)
