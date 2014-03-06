import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.L1TMuonText = cms.EDAnalyzer(
    'L1TMuonTextDumper',
    doGen = cms.untracked.bool(True),
    genSrc = cms.untracked.InputTag("genParticles"),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1TMuonTriggerPrimitives','CSC'),
    cms.InputTag('L1TMuonTriggerPrimitives','DT'),
    cms.InputTag('L1TMuonTriggerPrimitives','RPC'),
    cms.InputTag('L1TMuonTriggerPrimitives','GEM'),
    cms.InputTag('L1TMuonTriggerPrimitives','HCAL')
    ),
    converterSrcs = cms.VInputTag(
    cms.InputTag('L1CSCTFTrackConverter'),
    cms.InputTag('L1DTTFTrackConverter'),
    cms.InputTag('L1RPCbTFTrackConverter'),
    cms.InputTag('L1RPCfTFTrackConverter'),
    cms.InputTag('L1TMuonSimpleDeltaEtaHitMatcher')
    )
)

infile = 'file:L1TMuon.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1TMuonSequence = cms.Sequence(process.L1TMuonText)

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

process.schedule = cms.Schedule(process.L1TMuonPath)
