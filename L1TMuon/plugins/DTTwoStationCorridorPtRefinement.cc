#include "L1TriggerDPGUpgrade/L1TMuon/interface/DTTwoStationCorridorPtRefinement.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TFile.h>
#include <TGraph.h>
#include <TString.h>

using namespace L1TMuon;

namespace {
  typedef std::vector<double> vdouble;
}

DTTwoStationCorridorPtRefinement::
DTTwoStationCorridorPtRefinement(const edm::ParameterSet& ps):
  PtRefinementUnit(ps) {
  _fcorridors = ps.getParameter<edm::FileInPath>("corridorFile"); 
  vdouble tmp = ps.getParameter<vdouble>("pt_bins");  
  ptBins.reset(new TH1F("hPT__","",tmp.size(),tmp.data()));  
  get_corridors_from_file();
  clip_frac = ps.getParameter<int>("clip_fraction");
}

void DTTwoStationCorridorPtRefinement::
updateEventSetup(const edm::EventSetup& es) {
  PtRefinementUnit::updateEventSetup(es);
}

// this modifies the track in place!!!
void DTTwoStationCorridorPtRefinement::refinePt(InternalTrack& trk) const {
  // sanitize the input pt value
  const double lowest_edge = pt_scale->getPtScale()->getLowEdge(0);
  double input_pt = trk.ptValue();
  input_pt = std::max(input_pt,lowest_edge);
  /*
    phi_MB1, phi_MB2, sector_MB1, and sector_MB2 come from L1MuDTTrackSegPhi.
    int sectordiff = ((int)sector_MB2 - (int)sector_MB1)%12;
    if ( sectordiff >= 6 ) sectordiff -= 12;
    if ( sectordiff < -6 ) sectordiff += 12;
    int offset = (2144 >> 0) * sectordiff;
    int the_dPhi = ((int)phi_MB2 - (int)phi_MB1 + offset) << 0;    
  */
  // replace with call to get max
  double sane_pt = 0.0;
  sane_pt = std::max(sane_pt, lowest_edge); 
  unsigned sane_pt_packed = pt_scale->getPtScale()->getPacked(sane_pt);
  trk.setPtValue(sane_pt);
  trk.setPtPacked(sane_pt_packed);
}

void DTTwoStationCorridorPtRefinement::get_corridors_from_file() {
  std::unique_ptr<TFile> fcorridor(TFile::Open(_fcorridors.fullPath().c_str(),
					       "READ"));

  for (int sta=1; sta<=3; sta++) {
    for (int stb=sta+1; stb<=4; stb++) {
      TString gName = Form("gCut_dPhi%i%i",sta,stb);      
      pTGraph temp((TGraph *)fcorridor->Get(gName)->Clone(gName+"_New"));
      _dphi_corridors[sta-1][stb-1] = pTGraph();
      _dphi_corridors[sta-1][stb-1].swap(temp);
      std::cout << _dphi_corridors[sta-1][stb-1]->GetName() 
		<< " loaded " <<  std::endl; 
    }    
  }
  for (int sta=1; sta<=4; sta++) {
    if (sta!=3) {
      TString gName = Form("gCut_Phib%i",sta);
      pTGraph temp((TGraph *)fcorridor->Get(gName)->Clone(gName+"_New"));
      _phib_corridors[sta-1] = pTGraph();
      _phib_corridors[sta-1].swap(temp);	
      std::cout << _phib_corridors[sta-1]->GetName() 
		<< " loaded " <<  std::endl; 
    }
  }  
  fcorridor->Close();
}

double DTTwoStationCorridorPtRefinement::
solveCorridor(double ptHyp, 
	      double val, 
	      const pTGraph& g) const {
  // solveCor() takes the PT hypothesis (generally from BDT) 
  // and checks for the maximum PT consistent with the input 
  // value (such as dPhiAB) and input corridor given by the 
  // TGraph pointer. For the given input value, the maximum acceptable 
  // PT is returned such that the corridor condition is satisfied.   
  
  // for the pt hypothesis get the bin
  int theBIN = ptBins->FindBin(ptHyp); 
  double thePT = theBIN;
  
  double maxPt = 1e6;		  
  double cut = g->Eval( theBIN ) ; 
  
  // is a cut defined at this PT? Sometimes the training sample 
  // has too few events for a given PT bin. 
  if (cut>-1) {
    // check if the value is greater than the corridor cut
    if (fabs(val) > cut ) {
      maxPt  = 1; 
      // The corridor TGraphs count PT bins by "0"...sorry. 
      for (double p=thePT-1; p>0; p=p-1) {
	// Get the maximum acceptable input value at this test-PT 
	double eval = g->Eval((float)p);
	// make sure there is corridor value defined here, or else 
	// we ignore this test-PT and move on
	if (eval>=0) {
	  // Finally, is the input value below the corridor at the 
	  //test-PT? If so, we can stop. 
	  if (std::abs(val) < eval) {
	    maxPt = p;
	    break;
	  }
	}
      }
    }
  }
  
  double output = ptHyp;
  // If the final test-PT is lower than the original hypothesis, 
  // we need to use the final test-PT. 
  if (maxPt <  output) output = maxPt;

  return output;
}

double DTTwoStationCorridorPtRefinement::
calculateMaxAllowedPt(double ptHyp, 
		      int stA, int stB, double dPhi, 
		      double PhibA, double PhibB, int perCUT) {  
  // getCorMaxPT() solves for the maximum acceptable PT value given 
  // an ensemble of corridors defined for the dPhi and PhiBend input 
  // values. For a given station pair, this will call the solve function 
  // above for dPhiAB and PhiBendA, and will choose the minimum of the 
  // best PT solutions. The minimum of the PTs will satisfy all corridors 
  // simultaniously, I promise. 

  if ((stA == stB) || (stB<stA) || (stA<1) || (stA>3) || (stB<2) || (stB>4))
    return -999;

  stA = stA - 1; // StationA, counting by 0 here.
  stB = stB - 1; // StationB, counting by 0 here.  

  // Get the best PT using dPhiAB corridors. 
  double maxPt_dPhi = solveCorridor(ptHyp, dPhi, _dphi_corridors[stA][stB]); 
  double maxPt_PhibA = -999;
  double maxPt_PhibB = -999;
  // Get the best PT using PhiBendA corridor (except from MB3 of course).
  if ((stA+1) != 3)     
    maxPt_PhibA = solveCorridor(ptHyp, PhibA, _phib_corridors[stA]);
  // Get the best PT using PhiBendB corridor (except from MB3 or course). 
  if ((stB+1) != 3)
    maxPt_PhibB = solveCorridor(ptHyp, PhibB, _phib_corridors[stB]); 
    
  // Now take the minimum of the four possible PT values. 
  double ptOut = ptHyp;

  if (maxPt_dPhi < ptOut)
    ptOut = maxPt_dPhi;

  if ((stA+1)!=3)
    if (maxPt_PhibA < ptOut)
      ptOut = maxPt_PhibA;

  if ((stB+1)!=3)
    if (maxPt_PhibB < ptOut)
      ptOut = maxPt_PhibB;

  return ptOut;
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnitFactory.h"
DEFINE_EDM_PLUGIN( PtRefinementUnitFactory, 
		   DTTwoStationCorridorPtRefinement, 
		   "DTTwoStationCorridorPtRefinement");
