import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.TFileService = cms.Service(
    "TFileService",
    fileName=cms.string('L1ITMu_deltas.root')
    )

process.L1ITMuPlotter = cms.EDAnalyzer(
    'L1ITMuInternalTrackPlotter',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    trackSrcs = cms.VInputTag(
    cms.InputTag('L1ITMuSimpleDeltaEtaHitMatcher')
    )
)

infile = 'file:L1ITMU.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1ITMUSequence = cms.Sequence(process.L1ITMuPlotter)

process.L1ITMUPath = cms.Path(process.L1ITMUSequence)

process.schedule = cms.Schedule(process.L1ITMUPath)
