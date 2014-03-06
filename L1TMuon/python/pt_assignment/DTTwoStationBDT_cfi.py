import FWCore.ParameterSet.Config as cms

prefix = 'L1TriggerDPGUpgrade/L1TMuon/data/'\
         'bdt_weights/dt_two_station_regression/'

mva_weights = ['Regression_0x3_0x3001_BDTG_test.weights.xml.gz', # 1-2
               'Regression_0x5_0x1002_BDTG_test.weights.xml.gz', # 1-3
               'Regression_0x9_0x9004_BDTG_test.weights.xml.gz', # 1-4
               'Regression_0x6_0x2008_BDTG_test.weights.xml.gz', # 2-3
               'Regression_0xa_0xa010_BDTG_test.weights.xml.gz', # 2-4
               'Regression_0xc_0x8020_BDTG_test.weights.xml.gz'] # 3-4

DTTwoStationBDT= cms.PSet(
    AssignmentType = cms.string('DTTwoStationBDTPtAssignment'),
    bx_match_window = cms.uint32(6),
    # tracks from station one
    weights_MB1_MB2 = cms.FileInPath(prefix+mva_weights[0]),
    mvaname_MB1_MB2 = cms.string('BDTG_0x3_0x3001'),
    weights_MB1_MB3 = cms.FileInPath(prefix+mva_weights[1]),
    mvaname_MB1_MB3 = cms.string('BDTG_0x5_0x1002'),
    weights_MB1_MB4 = cms.FileInPath(prefix+mva_weights[2]),
    mvaname_MB1_MB4 = cms.string('BDTG_0x9_0x9004'),
    # tracks from station two
    weights_MB2_MB3 = cms.FileInPath(prefix+mva_weights[3]),
    mvaname_MB2_MB3 = cms.string('BDTG_0x6_0x2008'),
    weights_MB2_MB4 = cms.FileInPath(prefix+mva_weights[4]),
    mvaname_MB2_MB4 = cms.string('BDTG_0xa_0xa010'),
    # tracks from station three
    weights_MB3_MB4 = cms.FileInPath(prefix+mva_weights[5]),
    mvaname_MB3_MB4 = cms.string('BDTG_0xc_0x8020')
    )
