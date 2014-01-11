L1IntegratedMuonTrigger
=======================

Code for implementing and studying a L1 trigger that uses trigger primitive information from all muon subsystems of CMS.

cmsrel CMSSW_6_2_0_SLHC5
cd CMSSW_6_2_0_SLHC5/src
cmsenv
git cms-merge-topic gem-sw:CMSSW_6_2_X_SLHC
git submodule add git://github.com/gem-sw/L1TriggerDPGUpgrade.git
scram b -j10

run using
cmsRun $CMSSW_BASE/src/L1TriggerDPGUpgrade/L1TMuon/test/run_singleMu_gen_sim_L1TMuon.py

get efficiency with
cmsRun $CMSSW_BASE/src/L1TriggerDPGUpgrade/L1TMuon/test/calc_effs_cfg.py
