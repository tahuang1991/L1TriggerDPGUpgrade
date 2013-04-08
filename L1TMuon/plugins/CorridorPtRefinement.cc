#include "L1TriggerDPGUpgrade/L1TMuon/interface/CorridorPtRefinement.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TFile.h>
#include <TGraph.h>
#include <TString.h>

using namespace L1TMuon;

CorridorPtRefinement::CorridorPtRefinement( const edm::ParameterSet& ps ):
  PtRefinementUnit(ps) {
  _fcorridors = ps.getParameter<edm::FileInPath>("corridorFile");
  _N_PT_BINS = ps.getParameter<unsigned>("N_PT_BINS");
  _COR_PT_MAX = ps.getParameter<double>("COR_PT_MAX");
  get_corridors_from_file();
}

// this modifies the track in place!!!
void CorridorPtRefinement::refinePt(const edm::EventSetup& es, 
				    InternalTrack&) const {
  
}

void CorridorPtRefinement::get_corridors_from_file() {
  std::unique_ptr<TFile> fcorridor(TFile::Open(_fcorridors.fullPath().c_str(),
					       "READ"));
  
  for (int sta=1; sta<=3; sta++) {
    for (int stb=sta+1; stb<=4; stb++) {
      TString gName = Form("gCut_dPhi%i%i",sta,stb);
      pTGraph temp((TGraph *)fcorridor->Get(gName)->Clone());
      _dphi_corridors[sta-1][stb-1] = pTGraph();
      _dphi_corridors[sta-1][stb-1].swap(temp);
      std::cout << _dphi_corridors[sta-1][stb-1]->GetName() 
		<< " loaded " <<  std::endl; 
    }    
  }
  for (int sta=1; sta<=4; sta++) {
    if (sta!=3) {
      TString gName = Form("gCut_Phib%i",sta);
      pTGraph temp((TGraph *)fcorridor->Get(gName)->Clone());
      _phib_corridors[sta-1] = pTGraph();
      _phib_corridors[sta-1].swap(temp);	
      std::cout << _phib_corridors[sta-1]->GetName() 
		<< " loaded " <<  std::endl; 
    }
  }
  
  fcorridor->Close();
}

Float_t CorridorPtRefinement::solveCorridor(double ptHyp, 
					    double val, 
					    const pTGraph& g) const {
  // solveCor() takes the PT hypothesis (generally from BDT) 
  // and checks for the maximum PT consistent with the input 
  // value (such as dPhiAB) and input corridor given by the 
  // TGraph pointer. For the given input value, the maximum acceptable 
  // PT is returned such that the corridor condition is satisfied.   
  
  // for the pt hypothesis get the bin
  unsigned theBIN = std::min((unsigned)(ptHyp*_N_PT_BINS/_COR_PT_MAX) + 1,
			     _N_PT_BINS + 1);
  Float_t thePT = theBIN;
  
  float maxPt = 1e6;		  
  float cut = g->Eval( theBIN ) ; 
  
  // is a cut defined at this PT? Sometimes the training sample 
  // has too few events for a given PT bin. 
  if (cut>-1) {
    // check if the value is greater than the corridor cut
    if (fabs(val) > cut ) {
      maxPt  = 1; 
      // The corridor TGraphs count PT bins by "0"...sorry. 
      for (float p=thePT-1; p>0; p=p-1) {
	// Get the maximum acceptable input value at this test-PT 
	float eval = g->Eval((float)p);
	// make sure there is corridor value defined here, or else 
	// we ignore this test-PT and move on
	if (eval>=0) {
	  // Finally, is the input value below the corridor at the 
	  //test-PT? If so, we can stop. 
	  if (fabs(val) < eval) {
	    maxPt = p;
	    break;
	  }
	}
      }
    }
  }
  
  float output = ptHyp;
  // If the final test-PT is lower than the original hypothesis, 
  // we need to use the final test-PT. 
  if (maxPt <  output) output = maxPt;

  return output;
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnitFactory.h"
DEFINE_EDM_PLUGIN( PtRefinementUnitFactory, 
		   CorridorPtRefinement, 
		   "CorridorPtRefinement");
