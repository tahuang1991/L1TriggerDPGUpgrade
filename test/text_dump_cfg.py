import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.L1ITMuText = cms.EDAnalyzer(
    'L1ITMuTextDumper',
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1ITMuTriggerPrimitives','CSC'),
    cms.InputTag('L1ITMuTriggerPrimitives','DT'),
    cms.InputTag('L1ITMuTriggerPrimitives','RPCb'),
    cms.InputTag('L1ITMuTriggerPrimitives','RPCf')
    )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V7A::All', '')

infile = 'file:L1ITMU.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1ITMUSequence = cms.Sequence(process.L1ITMuText)

process.L1ITMUPath = cms.Path(process.L1ITMUSequence)

process.schedule = cms.Schedule(process.L1ITMUPath)
