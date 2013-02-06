import FWCore.ParameterSet.Config as cms

process = cms.Process('L1ITMU')

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('L1Trigger.L1IntegratedMuonTrigger.L1ITMuTriggerPrimitiveProducer_cfi')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V7A::All', '')

infile = 'file:/home/lgray/L1TriggerData/SingleMuFlatPt_5GeVto200GeV_GEN_SIM_DIGI_L1.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1ITMUSequence = cms.Sequence(process.L1ITMuTriggerPrimitives)

process.L1ITMUPath = cms.Path(process.L1ITMUSequence)

process.FEVTDEBUGoutput = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('L1ITMU.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

process.outPath = cms.EndPath(process.FEVTDEBUGoutput)

process.schedule = cms.Schedule(process.L1ITMUPath,
                                process.outPath)
