import FWCore.ParameterSet.Config as cms



DTTwoStationCorridor = cms.PSet(
    RefinementType = cms.string('DTTwoStationCorridorPtRefinement'),
    corridorFile = cms.FileInPath('L1TriggerDPGUpgrade/L1TMuon/'\
                                  'data/corridor_corrections/'\
                                  'DT_dPhi_Cuts_85per.root'),
    clip_fraction = cms.int32(85), # value of 0-100 in percent
    #half-GeV bins up to 200 GeV
    #this can be configured to any binning, but be reasonable
    pt_bins = cms.vdouble([float(x)/2.0 for x in range(401)]),
    bx_match_window = cms.uint32(6)
    )
