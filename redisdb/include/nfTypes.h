//===----------------------------------------------------------------------===//
#pragma once

#include <string>
enum NfTypes {
    NRF,   // 0.
    UDM, 
    AMF, 
    SMF, 
    AUSF,                               
    NEF,   // 5.
    PCF, 
    SMSF, 
    NSSF, 
    UDR, 
    LMF,   // 10.
    GMLC, 
    _5G_EIR, 
    SEPP, 
    UPF,
    N3IWF, // 15.
    AF, 
    UDSF, 
    BSF, 
    CHF, 
    NWDAF  // 20.
};

const std::vector<std::string> nfTypes { "NRF", "UDM", "AMF", "SMF", "AUSF", 
                                         "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                         "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                         "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                         "NWDAF" }; // 21 nftypes.