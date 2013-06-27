import FWCore.ParameterSet.Config as cms

process = cms.Process('TEXTDUMP')

process.TFileService = cms.Service(
    "TFileService",
    fileName=cms.string('L1ITMu_caloInspector_New.root')
    )

process.L1TMuonCaloInsp = cms.EDAnalyzer(
    'L1TMuonCaloInspector',
    doGen = cms.untracked.bool(True),
    genSrc = cms.InputTag("genParticles"),
    rpcSrc = cms.InputTag("L1RPCbTFTrackConverter"),
    dttfSrc = cms.InputTag("L1DTTFTrackConverter"),
    hcalSrc = cms.InputTag("L1TMuonTriggerPrimitives","HCAL"),
    stdmuSrc = cms.InputTag("standAloneMuons"),
    glbmuSrc = cms.InputTag("globalMuons"),
    dRtruthToRpc  = cms.untracked.double(0.2),
    dRrpcToDttf   = cms.untracked.double(0.2),
    dRdttfToHcal  = cms.untracked.double(0.2),
    dRhcalToStdMu = cms.untracked.double(0.2),
    dRdttfToStdMu = cms.untracked.double(0.2)
)

infile = 'file:L1TMuon.root'

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    )

process.L1TMuonSequence = cms.Sequence(process.L1TMuonCaloInsp)

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

process.schedule = cms.Schedule(process.L1TMuonPath)
