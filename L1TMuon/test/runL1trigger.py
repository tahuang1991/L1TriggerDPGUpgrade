GE11 = False
GE21 = False
noGEM = False
GE11 = True
#GE21 = True
runevents = 10
#runevents = 10000;
fileOutputName = "out_L1muon2023"
## input
#directory = '/pnfs/user/trackfinder/tf/'
#directory = '/u/user/jlee/scratch/singleMu_pu0/'
#directory = '/u/user/jlee/scratch/CMSSW_6_2_0_SLHC11_PU0/'
#directory = '/u/user/jlee/scratch/SingleMu_SLHC12_PU0/'
directory = '/eos/uscms/store/user/lpcgem/dildick/SingleMuPt2-50_1M_SLHC11_2023Muon/SingleMuPt2-50_1M_SLHC11_2023Muon_DIGI_PU0/abb92f2d576c84bfcdd5da9b6637acf8/'


import FWCore.ParameterSet.Config as cms

process = cms.Process('L1')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023MuonReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023Muon_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

from CalibMuon.CSCCalibration.CSCIndexer_cfi import CSCIndexerESProducer
process.CSCIndexerESProducer= CSCIndexerESProducer
from CalibMuon.CSCCalibration.CSCChannelMapper_cfi import CSCChannelMapperESProducer
process.CSCChannelMapperESProducer= CSCChannelMapperESProducer

if GE11:
    fileOutputName = fileOutputName+"GE11"
if GE21:
    fileOutputName = fileOutputName+"GE21"

histofileName= "histo_"+fileOutputName+".root"
fileOutputName = "file:"+fileOutputName+".root"
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(runevents)
)

inputDir = [directory]
theInputFiles = []
import os
for d in range(len(inputDir)):
  my_dir = inputDir[d]
  if not os.path.isdir(my_dir):
    print "ERROR: This is not a valid directory: ", my_dir
    if d==len(inputDir)-1:
      print "ERROR: No input files were selected"
      exit()
    continue
  print "Proceed to next directory"
  ls = os.listdir(my_dir)
  ## this works only if you pass the location on pnfs - FIXME for files staring with store/user/...
#  theInputFiles.extend([my_dir[:16] + x for x in ls if x.endswith('root')])
#  theInputFiles.extend(['file:' + my_dir[:50] + x for x in ls if x.endswith('.root')])
  theInputFiles.extend(['file:' + my_dir[:] + x for x in ls if x.endswith('.root')])

theInputFiles = theInputFiles[:]
#theInputFiles = ['file:/u/user/jlee/scratch/SingleMuPt2-50Fwdv2_50k_DIGI_PU0_SLHC10_2023Muon/out_digi_10_1_UPn.root']
#theInputFiles = ['file:/u/user/jlee/scratch/SingleMuHighPt_SLHC10_GEN_SIM_DIGI.root']

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
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

# Output definition
#outCommands = cms.untracked.vstring('keep *')

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
#    outputCommands = outCommands,
    fileName = cms.untracked.string(fileOutputName),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('L1')
    )
)

# Additional output definition
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(histofileName)
    )

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)
process.L1simulation_step = cms.Path(process.SimL1Emulator)

process.L1TAnalyser = cms.EDAnalyzer('L1TAnalyser',
    #lctsTag = cms.InputTag("muonCSCDigis","MuonCSCCorrelatedLCTDigi"),
    lctsTag = cms.InputTag('simCscTriggerPrimitiveDigis', 'MPCSORTED'),
    vertexColTag = cms.InputTag("offlinePrimaryVertices"),
    outTreeFileName = cms.untracked.string("lt1analysis.root"),
    haveRECO = cms.untracked.bool(False),
    singleSectorNum = cms.untracked.int32(-1) #-1 for sum over all sectors
    )
process.pL1TAnalyser = cms.Path(process.L1TAnalyser)

process.schedule = cms.Schedule(process.L1simulation_step,process.endjob_step,process.FEVTDEBUGHLToutput_step,process.pL1TAnalyser)


# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
#from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2023Muon
#process = cust_2023Muon(process)
#from SLHCUpgradeSimulations.Configuration.combinedCustoms import * 
#process=customisePostLS1(process)
#process=customiseBE5DPixel10D(process)
#process=customise_HcalPhase2(process)
#process=customise_ev_BE5DPixel10D(process)
#process=customise_gem(process)
#process=customise_rpc(process)
#process=customise_me0(process)
from SLHCUpgradeSimulations.Configuration.combinedCustoms import *
if noGEM:
    print "using cust_2019"
    process = cust_2019(process)
if GE11:
    print "using cust_2019WithGE11only"
    process = cust_2019WithGem(process)
if GE21:
    print "using cust_2019WithGem"
    process = cust_2023(process)
#    process.simCscTriggerPrimitiveDigis.tmbSLHC.me21ILT.doGemMatching = cms.untracked.bool(False)
#process.simCscTriggerPrimitiveDigis.tmbSLHC.me3141ILT.runME3141ILT = cms.untracked.bool(False)

#process.simCscTriggerPrimitiveDigis.tmbSLHC.me21ILT.debugMatching = True
print process.simCsctfTrackDigis.SectorProcessor.PTLUT
print "firmwareSP", process.simCsctfTrackDigis.SectorProcessor.firmwareSP
#print "runME11ILT", process.simCscTriggerPrimitiveDigis.tmbSLHC.me11ILT.runME11ILT

print process.simCscTriggerPrimitiveDigis.clctSLHC.clctNplanesHitPattern

# End of customisation functions
#print process.l1extraParticles
#print "runME11ILT", process.simCscTriggerPrimitiveDigis.tmbSLHC.me11ILT

#print process.XMLIdealGeometryESSource.geomXMLFiles
