runevents = 1000
runevents = -1;

import FWCore.ParameterSet.Config as cms
process = cms.Process('L1analysis')

process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuGMTScalesConfig_cff")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

#theInputFiles = ['file:/u/user/jlee/scratch/L1data0417/out_L1_cust_2019NewTF.root']
#out_L1muon2023.root
#out_L1muon2023GE11.root
#out_L1muon2023GE21.root
fileOutputName = "SingleMu_SLHC12_PU0"
theInputFiles = ['file:SingleMu_SLHC12_PU0.root']

#fileOutputName = "out_L1muon2023GE21"
#theInputFiles = ['file:out_L1muon2023GE21.root']

#fileOutputName = "out_L1muon2023GE11"
#theInputFiles = ['file:out_L1muon2023GE11.root']

#fileOutputName = "out_L1_cust_2019NewTF"
#theInputFiles = ['file:/u/user/jlee/scratch/L1data0423/out_L1_cust_2019NewTF.root']
#fileOutputName = "out_L1_cust_2019PtMethod32NewTF"
#theInputFiles = ['file:L1data0423/out_L1_cust_2019PtMethod32NewTF.root']
#fileOutputName = "out_L1_cust_2019UnFlipHsInLutWithGemNewTF"
#theInputFiles = ['file:L1data0423/out_L1_cust_2019UnFlipHsInLutWithGemNewTF.root']
#fileOutputName = "out_L1_cust_2019WithGemNewTF"
#theInputFiles = ['file:L1data0423/out_L1_cust_2019WithGemNewTF.root']

histofileName= "histo_"+fileOutputName+".root"

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(runevents)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    #fileNames = cms.untracked.vstring("file:~/cmsrun/out_digi.root")
    fileNames = cms.untracked.vstring(*theInputFiles)
)
print "fileNames: ", process.source.fileNames
process.options = cms.untracked.PSet()

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('SingleMuPt100_cfi nevts:100'),
    name = cms.untracked.string('Applications')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(
    histofileName
))

process.L1TAnalyser = cms.EDAnalyzer('L1TAnalyser',
    #lctsTag = cms.InputTag("muonCSCDigis","MuonCSCCorrelatedLCTDigi"),
    lctsTag = cms.InputTag('simCscTriggerPrimitiveDigis', 'MPCSORTED'),
    vertexColTag = cms.InputTag("offlinePrimaryVertices"),
    outTreeFileName = cms.untracked.string("lt1analysis.root"),
    haveRECO = cms.untracked.bool(False),
    singleSectorNum = cms.untracked.int32(-1) #-1 for sum over all sectors
    )
process.pL1TAnalyser = cms.Path(process.L1TAnalyser)

process.schedule = cms.Schedule(process.pL1TAnalyser)
