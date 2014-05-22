LPC = False
runevents = -1;
#runevents = 20000
#LPC = True
fileOutputName = ""

import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.register ('gem', 1,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "gem: 1  default")
options.register ('data', 1,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "data: 1  default")
import sys
if len(sys.argv) > 0:
    last = sys.argv.pop()
    sys.argv.extend(last.split(","))
    print sys.argv
    
if hasattr(sys, "argv") == True:
	options.parseArguments()
	gem = options.gem
	data = options.data

GE11 = False
GE21 = False
doRate = False
if gem == 1:
    GE11 = True
if gem == 2:
    GE21 = True
if data == 0:
    dataSetName = "SingleMuPt2-50_1M_SLHC11_2023Muon_DIGI_PU0"
if data == 1:
    dataSetName = "SingleMuPt2-50_1M_SLHC11_2023Muon_DIGI_PU140"
if data == 2:
    dataSetName = "SingleNu_SLHC12_2023Muon_DIGI_PU140"
    doRate = True
if data == 3:
    dataSetName = "SingleNu_SLHC12_2023Muon_DIGI_PU400"
    doRate = True
if data == 4:
    dataSetName = "SingleMu_SLHC12_PU0"
if data == 5:
    dataSetName = "SingleMu_SLHC12_PU140"
    
from os.path import expanduser
directory = expanduser("~")+'/scratch/'
saveDir = ''
if LPC:
    directory = '/uscms/home/jlee/cmsrun/'
    saveDir = '/eos/uscms/store/user/lpcgem/jlee/l1csctf/'
directory = directory + dataSetName + '/'
fileOutputName = dataSetName+fileOutputName

process = cms.Process('L1')
# import of standard configuration
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023MuonReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023Muon_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load("Configuration.StandardSequences.L1Extra_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

from CalibMuon.CSCCalibration.CSCIndexer_cfi import CSCIndexerESProducer
process.CSCIndexerESProducer= CSCIndexerESProducer
from CalibMuon.CSCCalibration.CSCChannelMapper_cfi import CSCChannelMapperESProducer
process.CSCChannelMapperESProducer= CSCChannelMapperESProducer

if GE11:
    fileOutputName = fileOutputName+"GE11"
if GE21:
    fileOutputName = fileOutputName+"GE21"

histofileName= saveDir+"histo_"+fileOutputName+".root"
fileOutputName = "file:"+saveDir+fileOutputName+".root"
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
  theInputFiles.extend(['file:' + my_dir[:] + x for x in ls if x.endswith('.root')])

theInputFiles = theInputFiles[:]

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(*theInputFiles)
)
print "fileNames:", process.source.fileNames
print "fileOutput", fileOutputName

process.options = cms.untracked.PSet()

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('SingleMuPt100_cfi nevts:100'),
    name = cms.untracked.string('Applications')
)

# Output definition
outCommands = cms.untracked.vstring('drop *')
outCommands.append('keep *_genParticles_*_*')
outCommands.append('keep *_simCsctfDigis_*_*')
outCommands.append('keep *_simCsctfTrackDigis_*_*')
outCommands.append('keep *_simCscTriggerPrimitiveDigis_*_*')
outCommands.append('keep *_g4SimHits_*_*')
outCommands.append('keep *_simMuonGEMCSCPadDigis_*_*')

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    #outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    outputCommands = outCommands,
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
process.L1Extra_step = cms.Path(process.L1Extra)

process.l1extraParticles.centralBxOnly = cms.bool(True)
process.l1extraParticles.produceMuonParticles = cms.bool(True)
process.l1extraParticles.produceCaloParticles = cms.bool(False)
process.l1extraParticles.ignoreHtMiss = cms.bool(False)

if not doRate:
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
if doRate:
    process.L1TTriggerRate = cms.EDAnalyzer('L1TTriggerRate',
        minPt = cms.double(2.0),
        maxPt = cms.double(100.0),
        minEta = cms.double(1.6),
        maxEta = cms.double(2.4),
    )
    process.pL1TAnalyser = cms.Path(process.L1TTriggerRate)
    
process.schedule = cms.Schedule(process.L1simulation_step,process.L1Extra_step,process.endjob_step,process.FEVTDEBUGHLToutput_step,process.pL1TAnalyser)

from SLHCUpgradeSimulations.Configuration.combinedCustoms import *
from GEMCode.SimMuL1.GEMCSCdPhiDict import *
pt = 'Pt10' # 'Pt10', 'Pt15', 'Pt20'
process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiOdd = dphi_lct_pad_ME11[pt]['odd']
process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiEven = dphi_lct_pad_ME11[pt]['even']

process.simCscTriggerPrimitiveDigis.me21tmbSLHCGEM.gemMatchDeltaPhiOdd = dphi_lct_pad_ME21[pt]['odd']
process.simCscTriggerPrimitiveDigis.me21tmbSLHCGEM.gemMatchDeltaPhiEven = dphi_lct_pad_ME21[pt]['even']

if GE21:
    process = cust_2023Muon(process)
    process.simCscTriggerPrimitiveDigis.commonParam.runME3141ILT = cms.bool(True)
    process.simCscTriggerPrimitiveDigis.me3141tmbSLHCRPC.debugMatching = cms.bool(False)
    process.simCscTriggerPrimitiveDigis.me21tmbSLHCGEM.gemMatchDeltaPhiOdd = cms.double(1)
    process.simCscTriggerPrimitiveDigis.me21tmbSLHCGEM.gemMatchDeltaPhiEven = cms.double(1)
elif GE11:
    process = cust_2019WithGem(process)
    process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiOdd = cms.double(1)
    process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiEven = cms.double(1)
else:
    process = cust_2019(process)

#process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiEven = cms.double(2.0)
#process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiOdd = cms.double(2.0)
#process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.debugMatching = cms.bool(True)
#process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.debugLUTs = cms.bool(True)

#process.simCscTriggerPrimitiveDigis.tmbSLHC.me11ILT.debugGemMatching = cms.untracked.bool(True)
#process.simCscTriggerPrimitiveDigis.tmbSLHC.me11ILT.debugLUTs = cms.untracked.bool(True)


#process.simCsctfTrackDigis.SectorProcessor.isCoreVerbose = cms.bool(True) 
#print "PTLUT", process.simCsctfTrackDigis.SectorProcessor.PTLUT
#print "SRLUT", process.simCsctfTrackDigis.SectorProcessor.SRLUT
#print "firmwareSP", process.simCsctfTrackDigis.SectorProcessor.firmwareSP
#print "initializeFromPSet", process.simCsctfTrackDigis.SectorProcessor.initializeFromPSet
#print "clctNplanesHitPattern", process.simCscTriggerPrimitiveDigis.clctSLHC.clctNplanesHitPattern
#print "SectorProcessor", process.simCsctfTrackDigis.SectorProcessor

#print "commonParam", process.simCscTriggerPrimitiveDigis.commonParam
#print "clctSLHC", process.simCscTriggerPrimitiveDigis.clctSLHC
#print "clctParam07", process.simCscTriggerPrimitiveDigis.clctParam07
#print "alctSLHC", process.simCscTriggerPrimitiveDigis.alctSLHC
#print "alctParam07", process.simCscTriggerPrimitiveDigis.alctParam07
#print "tmbParam", process.simCscTriggerPrimitiveDigis.tmbParam
#print "tmbSLHC", process.simCscTriggerPrimitiveDigis.tmbSLHC
#print "me11tmbSLHCGEM", process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM
