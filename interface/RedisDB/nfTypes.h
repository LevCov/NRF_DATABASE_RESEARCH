#pragma once

#include <string>
#include <vector>

inline const std::vector<std::string_view> nfTypes{
    "NRF",  "UDM",   "AMF", "SMF",  "AUSF", "NEF",    "PCF",
    "SMSF", "NSSF",  "UDR", "LMF",  "GMLC", "5G_EIR", "SEPP",
    "UPF",  "N3IWF", "AF",  "UDSF", "BSF",  "CHF",    "NWDAF"};  // 21 nftypes.
