import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.TFileService = cms.Service(
    "TFileService",
    fileName=cms.string('L1ITMu_caloInspector.root')
    )

process.L1TMuonCaloInsp = cms.EDAnalyzer(
    'L1TMuonCaloInspector',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    bestSrc = cms.InputTag("standAloneMuons"),
    worstSrc = cms.InputTag("L1TMuonSimpleDeltaEtaHitMatcher")
)

infile = 'file:L1TMuon.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1TMuonSequence = cms.Sequence(process.L1TMuonCaloInsp)

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

process.schedule = cms.Schedule(process.L1TMuonPath)
