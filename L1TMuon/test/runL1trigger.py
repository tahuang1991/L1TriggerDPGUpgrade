LPC = False
runevents = 1000
#runevents = -1;
#LPC = True

import os
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
    fileOutputName = "SingleMuPt2-50_1M_SLHC11_2023Muon_DIGI_PU0"
if data == 1:
    fileOutputName = "SingleMuPt2-50_1M_SLHC11_2023Muon_DIGI_PU140"
if data == 2:
    fileOutputName = "SingleNu_SLHC12_2023Muon_DIGI_PU140"
    doRate = True

#
#fileOutputName = "SingleMuPt2-50_1M_SLHC11_2023Muon_DIGI_PU200"
#fileOutputName = "SingleMu_SLHC12_PU0"
## input
#directory = '/u/user/jlee/scratch/CMSSW_6_2_0_SLHC11_PU0/'
directory = '/u/user/jlee/scratch/'
saveDir = ''
if LPC:
    directory = '/uscms/home/jlee/cmsrun/'
    saveDir = '/eos/uscms/store/user/lpcgem/jlee/l1csctf/'
directory = directory + fileOutputName + '/'

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
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = 500

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
#theInputFiles = ['file:/u/user/jlee/scratch/SingleMuPt2-50Fwdv2_50k_DIGI_PU0_SLHC10_2023Muon/out_digi_10_1_UPn.root']

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

if not doRate:
    process.L1TAnalyser = cms.EDAnalyzer('L1TAnalyser',
        minPt = cms.untracked.double(2),
        minEta = cms.untracked.double(1.6),
        maxEta = cms.untracked.double(2.4),
    )
    process.pL1TAnalyser = cms.Path(process.L1TAnalyser)
if doRate:
    process.L1TTriggerRate = cms.EDAnalyzer('L1TTriggerRate',
        minPt = cms.untracked.double(2),
        minEta = cms.untracked.double(1.6),
        maxEta = cms.untracked.double(2.4),
    )
    process.pL1TAnalyser = cms.Path(process.L1TTriggerRate)
    
process.schedule = cms.Schedule(process.L1simulation_step,process.endjob_step,process.FEVTDEBUGHLToutput_step,process.pL1TAnalyser)

from SLHCUpgradeSimulations.Configuration.combinedCustoms import *
from GEMCode.SimMuL1.GEMCSCdPhiDict import *
pt = 'Pt10' # 'Pt10', 'Pt15', 'Pt20'
process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiOdd = dphi_lct_pad_ME11[pt]['odd']
process.simCscTriggerPrimitiveDigis.me11tmbSLHCGEM.gemMatchDeltaPhiEven = dphi_lct_pad_ME11[pt]['even']

process.simCscTriggerPrimitiveDigis.me21tmbSLHCGEM.gemMatchDeltaPhiOdd = dphi_lct_pad_ME21[pt]['odd']
process.simCscTriggerPrimitiveDigis.me21tmbSLHCGEM.gemMatchDeltaPhiEven = dphi_lct_pad_ME21[pt]['even']

if GE21:
    process = cust_2023Muon(process)
elif GE11:
    process = cust_2019WithGem(process)
else:
    process = cust_2019(process)

#process.simCsctfTrackDigis.SectorProcessor.isCoreVerbose = cms.bool(True) 
print "PTLUT", process.simCsctfTrackDigis.SectorProcessor.PTLUT
print "firmwareSP", process.simCsctfTrackDigis.SectorProcessor.firmwareSP
print "clctNplanesHitPattern", process.simCscTriggerPrimitiveDigis.clctSLHC.clctNplanesHitPattern
