
// 
// Class: L1ITMu::TriggerPrimitive
//
// Info: This class implements a unifying layer between DT, CSC and RPC
//       trigger primitives (TPs) such that TPs from different subsystems
//       can be queried for their positional and orientation information
//       in a consistent way amongst all subsystems.
//
// Note: Not all input data types are persistable, so we make local
//       copies of all data from various digi types.
//
// Author: L. Gray (FNAL)
//

// DT digi types
class DTChamberId;
class L1MuDTChambPhDigi;
class L1MuDTChambThDigi;

// CSC digi types
class CSCCorrelatedLCTDigi;
class CSCDetId;

// RPC digi types
class RPCDigi;
class RPCDetId;

namespace L1ITMu {

  class TriggerPrimitive {
  public:
    // define the subsystems that we have available
    enum subsystem_type{kDT,kCSC,kRPC,kNSubsystems};
    
    // define the data we save locally from each subsystem type
    // variables in these structs keep their colloquial meaning
    // within a subsystem
    struct RPCData {
      unsigned sector;      
    };

    struct CSCData {
      unsigned sector;
      unsigned subsector;
    };

    struct DTData {
      unsigned sector;
    };

    //Persistency
    TriggerPrimitive() {}

    //constuctors from DT data
    TriggerPrimitive(const DTChamberId&,
		     const L1MuDTChambPhDigi&);
    TriggerPrimitive(const DTChamberId&,
		     const L1MuDTChambThDigi&);
    
    //constructor from CSC data
    TriggerPrimitive(const CSCDetId&,
		     const CSCCorrelatedLCTDigi&);

    //constructor from RPC data
    TriggerPrimitive(const RPCDetId&,
		     const RPCDigi&);
    
    // return the subsystem we belong to
    subsystem_type subsystem() const { return _subsystem; }

    // floating point global eta and phi in CMS coordinates
    double eta() const { return _eta; }
    double phi() const { return _phi; }

    //setters for global eta and phi
    void setEta(double eta) { _eta = eta; }
    void setPhi(double phi) { _phi = phi; }

  private:
    
    DTData  _dt;
    CSCData _csc;
    RPCData _rpc;
    
    subsystem_type _subsystem;

    unsigned _globalsector; // [1,6] in 60 degree sectors
    unsigned _subsector; // [1,2] in 30 degree partitions of a sector 
    double _eta,_phi;
  };

}
