import FWCore.ParameterSet.Config as cms

process = cms.Process('L1TMUON')


process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1TriggerDPGUpgrade.L1TMuon.L1TMuon_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')

infile = ['file:SingleMuFlatPt_5GeVto200GeV_GEN_SIM_DIGI_L1_RECO.root']

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

#process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(100)
#)

process.L1TMuonText = cms.EDAnalyzer(
    'L1TMuonTextDumper',
    doGen = cms.untracked.bool(True),
    genSrc = cms.untracked.InputTag("genParticles"),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1TMuonTriggerPrimitives','CSC'),
    cms.InputTag('L1TMuonTriggerPrimitives','DT'),
    cms.InputTag('L1TMuonTriggerPrimitives','HCAL')
    ),
    converterSrcs = cms.VInputTag(    
    cms.InputTag('L1DTTFTrackConverter')
    )
)

process.L1TMuonConverterSequence += process.L1TMuonText

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

outCommands = cms.untracked.vstring('drop *')
outCommands.append('keep *_genParticles_*_*')
outCommands.append('keep *_simCsctfDigis_*_*')
outCommands.append('keep *_simDttfDigis_*_*')
outCommands.append('keep *_simRpcTriggerDigis_*_*')
outCommands.append('keep *_simMuonRPCDigis_*_*')
outCommands.append('keep *_simDtTriggerPrimitiveDigis_*_*')
outCommands.append('keep *_simCscTriggerPrimitiveDigis_*_*')
outCommands.append('keep *_simHcalTriggerPrimitiveDigis_*_*')
outCommands.append('keep *_L1TMuonTriggerPrimitives_*_*')
outCommands.append('keep *_*Converter_*_*')
outCommands.append('keep *_*AssignedTrackProducer_*_*')
outCommands.append('keep *_*Matcher_*_*')

outCommands.append('keep *_standAloneMuons_*_*')
outCommands.append('keep *_globalMuons_*_*')
outCommands.append('keep *_towerMaker_*_*')
outCommands.append('keep *_simHcalDigis_*_*')
outCommands.append('keep *_horeco_*_*')
outCommands.append('keep *_hbhereco_*_*')

process.FEVTDEBUGoutput = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = outCommands,
    fileName = cms.untracked.string('L1TMuon.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

process.outPath = cms.EndPath(process.FEVTDEBUGoutput)

process.schedule = cms.Schedule(process.L1TMuonPath,
                                process.outPath)


