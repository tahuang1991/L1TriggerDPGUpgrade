import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.load('L1TriggerConfig/L1ScalesProducers/L1MuTriggerScalesConfig_cff')
process.load('L1TriggerConfig/L1ScalesProducers/L1MuTriggerPtScaleConfig_cff')

process.TFileService = cms.Service(
    "TFileService",
    fileName=cms.string('L1ITMu_effs.root')
    )

process.L1TMuonEffs_bdt = cms.EDAnalyzer(
    'L1TMuonTrackEfficiencies',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    trackSrc = cms.InputTag('DTTwoStationAssignedTrackProducer',
                            'AssignmentOnly'),
    binning = cms.vdouble([0.5*x for x in range(401)]),
    thresholds = cms.vdouble(0,1,2,3,4,5,7,10,16,20,25,30,35,40,50),
    sim_pt_bins = cms.vdouble(0,5,10,15,20,25,30,35,50),
    allowed_modes = cms.vuint32(3,5,9,6,10,12)
)

process.L1TMuonEffs_dttf = cms.EDAnalyzer(
    'L1TMuonTrackEfficiencies',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    trackSrc = cms.InputTag('L1DTTFTrackConverter'),
    binning = cms.vdouble([0.5*x for x in range(401)]),
    thresholds = cms.vdouble(0,1,2,3,4,5,7,10,16,20,25,30,35,40,50),
    sim_pt_bins = cms.vdouble(0,5,10,15,20,25,30,35,50),
    allowed_modes = cms.vuint32(3,5,9,6,10,12)
)

process.L1TMuonEffs_csctf = cms.EDAnalyzer(
    'L1TMuonTrackEfficiencies',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    trackSrc = cms.InputTag('L1CSCTFTrackConverter'),
    binning = cms.vdouble([0.5*x for x in range(401)]),
    thresholds = cms.vdouble(0,1,2,3,4,5,7,10,16,20,25,30,35,40,50),
    sim_pt_bins = cms.vdouble(0,5,10,15,20,25,30,35,50),
    allowed_modes = cms.vuint32(3,5,9,6,10,12)
)

infile = 'file:L1TMuon.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1TMuonSequence = cms.Sequence(process.L1TMuonEffs_bdt  +
                                       process.L1TMuonEffs_dttf +
                                       process.L1TMuonEffs_csctf 
                                       
    )

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

process.schedule = cms.Schedule(process.L1TMuonPath)
