#pragma once

#include <string>
#include <vector>

enum NfTypes {
  NRF,
  UDM,
  AMF,
  SMF,
  AUSF,
  NEF,
  PCF,
  SMSF,
  NSSF,
  UDR,
  LMF,
  GMLC,
  _5G_EIR,
  SEPP,
  UPF,
  N3IWF,
  AF,
  UDSF,
  BSF,
  CHF,
  NWDAF
};

const std::vector<std::string> nfTypes{
    "NRF",  "UDM",   "AMF", "SMF",  "AUSF", "NEF",    "PCF",
    "SMSF", "NSSF",  "UDR", "LMF",  "GMLC", "5G_EIR", "SEPP",
    "UPF",  "N3IWF", "AF",  "UDSF", "BSF",  "CHF",    "NWDAF"};  // 21 nftypes.