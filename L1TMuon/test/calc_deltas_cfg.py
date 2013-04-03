import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.TFileService = cms.Service(
    "TFileService",
    fileName=cms.string('L1ITMu_deltas.root')
    )

process.L1TMuonPlotter = cms.EDAnalyzer(
    'L1TMuonInternalTrackPlotter',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    trackSrcs = cms.VInputTag(
    cms.InputTag('L1TMuonSimpleDeltaEtaHitMatcher')
    )
)

infile = 'file:L1TMuon.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1TMuonSequence = cms.Sequence(process.L1TMuonPlotter)

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

process.schedule = cms.Schedule(process.L1TMuonPath)
