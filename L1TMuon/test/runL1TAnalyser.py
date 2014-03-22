import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ""
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       #'file:/pnfs/user/trackfinder/out_L1_cust_2019NewTF.root'
       'file:/pnfs/user/trackfinder/out_L1_cust_2019WithGemNewTF.root'
    )
)
# Event Setup
##############
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

# LCT Occupancies Analyzer
process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('ProductNotFound'))

process.L1TAnalyser = cms.EDAnalyzer('L1TAnalyser',
	 #lctsTag = cms.InputTag("muonCSCDigis","MuonCSCCorrelatedLCTDigi"),
     lctsTag = cms.InputTag('simCscTriggerPrimitiveDigis', 'MPCSORTED'),
	 vertexColTag = cms.InputTag("offlinePrimaryVertices"),
	 outTreeFileName = cms.untracked.string("LctOccTree.root"),
	 haveRECO = cms.untracked.bool(False),
	 singleSectorNum = cms.untracked.int32(-1) #-1 for sum over all sectors
)
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(
		"LCTOccupanciesOutput_data.root"
))

#This path is for Real Data
process.p = cms.Path(process.L1TAnalyser)
