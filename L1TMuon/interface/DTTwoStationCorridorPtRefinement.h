#ifndef __L1TMUON_DTTWOSTATIONCORRIDORPTREFINEMENT_H__
#define __L1TMUON_DTTWOSTATIONCORRIDORPTREFINEMENT_H__
// 
// Class: L1TMuon::DTTwoStationCorridorPtRefinement
//
// Info: Implements the 'corridor' (confidence interval)
//       based tail-clipping from B. Scurlock.
//       This is the initial version that works only for 
//       two station tracks in the DTs.
//
// Author: L. Gray (FNAL), B. Scurlock (UF)
//
#include <memory>
#include <vector>
#include <unordered_map>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TH1F.h"

class TGraph;
class TH1F;

namespace L1TMuon {
  
  class DTTwoStationCorridorPtRefinement: public PtRefinementUnit {    
  public:
    typedef std::unique_ptr<TH1F>   ptBinning;
    typedef std::unique_ptr<TGraph> pTGraph;
    typedef std::unordered_map<unsigned, std::unordered_map<unsigned, pTGraph > > 
      corridor_2stn_map;
    
    DTTwoStationCorridorPtRefinement(const edm::ParameterSet&);
    ~DTTwoStationCorridorPtRefinement() {}

    virtual void updateEventSetup(const edm::EventSetup&);

    virtual void refinePt(InternalTrack&);
  private:
    void get_corridors_from_file();
    double solveCorridor(double rawPtHypothesis, 
			 double observable,
			 const pTGraph &corridor_belt);
    double calculateMaxAllowedPt(double ptHypothesis, 
				 int first_station, 
				 int second_station, 
				 double dphi, 
				 double phib_first, 
				 double phib_second, 
				 int clip_fraction = 85);
    edm::FileInPath _fcorridors;
    int clip_frac;
    ptBinning ptBins;
    corridor_2stn_map _dphi_corridors;
    std::unordered_map<unsigned, std::unique_ptr<TGraph> > _phib_corridors; 
    int bx_window;
  };
}

#endif
