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
fileOutputName = "Neutrino_SLHC25_PU140_0511"
#input: L1 sample
inputDir = ['/eos/uscms/store/user/tahuang/SLHC25_patch1_2023Neutrino_100k_L1_PU140/tahuang/SLHC25_patch1_2023Neutrino_gen_sim_100k/SLHC25_patch1_2023Neutrino_100k_L1_PU140/1bf93df4dfbb43dc918bd6e47dedbf79/']
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

histofileName= "Rate_"+fileOutputName+".root"

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

process.L1TTriggerRate = cms.EDAnalyzer('L1TTriggerRate',
        minPt = cms.double(2.0),
        maxPt = cms.double(100.0),
        minEta = cms.double(1.6),
        maxEta = cms.double(2.4),
    )
process.pL1TAnalyser = cms.Path(process.L1TTriggerRate)
    

process.schedule = cms.Schedule(process.pL1TAnalyser)
