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
    fileNames = cms.untracked.vstring("file:out_L1.root")
#    fileNames = cms.untracked.vstring(*theInputFiles)
)

#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_SortByGEMDPhi_v4/']
#InputDir = ['/eos/uscms/store/user/lpcgem/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC12_100k_L1_PU0_Pt0_2023All_delta_fixdt_fixEven/daa57b56d56d80b9a22ba3c5323a8c8a/']
#InputDir = ['/eos/uscms/store/user/lpcgem/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC12_100k_L1_Jason_PU140_Pt0_2023All_delta_fixdt_fixeven/23dce495d98b9a3acc530862a80705c8/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_combined/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC13_100k_L1_PU140_Pt0_2023All_combined/5a549a4810b5acc2831e5eae6e69c904/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_lowQstubandrpc/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_FixCopad_v3/']
InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_MatchingWindow_v4/']
## input
from GEMCode.SimMuL1.GEMCSCTriggerSamplesLib import *
from GEMCode.GEMValidation.InputFileHelpers import *
process = useInputDir(process, InputDir, True)
#process = useInputDir(process, files['_gem98_pt2-50_PU0_pt0_new'], False)

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
"""
process.L1TAnalyser = cms.EDAnalyzer('L1TAnalyser',
    #lctsTag = cms.InputTag("muonCSCDigis","MuonCSCCorrelatedLCTDigi"),
    lctsTag = cms.InputTag('simCscTriggerPrimitiveDigis', 'MPCSORTED'),
    vertexColTag = cms.InputTag("offlinePrimaryVertices"),
    outTreeFileName = cms.untracked.string("lt1analysis.root"),
    haveRECO = cms.untracked.bool(False),
    singleSectorNum = cms.untracked.int32(-1) #-1 for sum over all sectors
    )
process.pL1TAnalyser = cms.Path(process.L1TAnalyser)
"""

process.L1TAnalyser = cms.EDAnalyzer('L1TAnalyser',
        minPt = cms.double(2.0),
        maxPt = cms.double(100.0),
        minEta = cms.double(1.6),
        maxEta = cms.double(2.4),
        SRLUT = cms.PSet(
			Binary = cms.untracked.bool(False),
			ReadLUTs = cms.untracked.bool(False),
			LUTPath = cms.untracked.string('./'),
			UseMiniLUTs = cms.untracked.bool(True)
		),
        debugTF = cms.bool(False)
 )
process.pL1TAnalyser = cms.Path(process.L1TAnalyser)

process.schedule = cms.Schedule(process.pL1TAnalyser)
