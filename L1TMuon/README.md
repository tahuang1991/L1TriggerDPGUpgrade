L1IntegratedMuonTrigger
=======================

Code for implementing and studying a L1 trigger that uses trigger primitive information from all muon subsystems of CMS.

cmsrel CMSSW_6_2_0_SLHC5<br>
cd CMSSW_6_2_0_SLHC5/src<br>
cmsenv<br>
git cms-merge-topic gem-sw:CMSSW_6_2_X_SLHC<br>
git submodule add git://github.com/gem-sw/L1TriggerDPGUpgrade.git<br>
scram b -j10<br>

run using<br>
cmsRun $CMSSW_BASE/src/L1TriggerDPGUpgrade/L1TMuon/test/run_singleMu_gen_sim_L1TMuon.py<br>

get efficiency with<br>
cmsRun $CMSSW_BASE/src/L1TriggerDPGUpgrade/L1TMuon/test/calc_effs_cfg.py<br>
