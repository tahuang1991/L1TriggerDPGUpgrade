# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: SingleMuPt100_cfi --conditions auto:upgrade2019 -n 100 --eventcontent FEVTDEBUGHLT -s L1 --datatier L1 --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2019WithGem --geometry Extended2019 --magField 38T_PostLS1 --filein file:out_digi.root --fileout file:out_L1_cust_2019WithGem.root

withGEM = True
#withGEM = False
runevents = 1000
runevents = -1;

import FWCore.ParameterSet.Config as cms
process = cms.Process('L1analysis')

# import of standard configurations
process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

#theInputFiles = ['file:/u/user/jlee/scratch/L1data0417/out_L1_cust_2019NewTF.root']

fileOutputName = "out_L1_cust_2019NewTF"
#theInputFiles = ['file:/u/user/jlee/scratch/L1data0423/out_L1_cust_2019NewTF.root']
directory = '/eos/uscms/store/user/tahuang/SingleMuPt2-50Fwdv2_SLHC10_100k_L1_PU0_Pt0_All_fixedQual/'
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
#fileOutputName = "out_L1_cust_2019PtMethod32NewTF"
#theInputFiles = ['file:L1data0423/out_L1_cust_2019PtMethod32NewTF.root']
#fileOutputName = "out_L1_cust_2019UnFlipHsInLutWithGemNewTF"
#theInputFiles = ['file:L1data0423/out_L1_cust_2019UnFlipHsInLutWithGemNewTF.root']
#fileOutputName = "out_L1_cust_2019WithGemNewTF"
#theInputFiles = ['file:L1data0423/out_L1_cust_2019WithGemNewTF.root']

#if withGEM:
#    fileOutputName = fileOutputName+"WithGem"
#    theInputFiles = ['file:/u/user/jlee/scratch/L1data0417/out_L1_cust_2019WithGemNewTF.root']
histofileName= "Ana_histo_"+fileOutputName+".root"
fileOutputName = "file:"+"Ana_"+fileOutputName+".root"

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

process.options = cms.untracked.PSet(

)

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

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

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

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
if withGEM:
    from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019WithGem 
    process = cust_2019WithGem(process)
else :
    from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019
    process = cust_2019(process)
