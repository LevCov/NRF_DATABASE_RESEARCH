#ifndef NRF_DATABASE_RESEARCH_NF_PROFILE_REPO_H
#define NRF_DATABASE_RESEARCH_NF_PROFILE_REPO_H

#include <vector>
#include "nf_profile.h"

template<typename T>
class NfProfileRepo {
    NfProfileRepo(T driver);
    ~NfProfileRepo();
    save(NfProfile profile);
    vector<NfProfile> search_by_param(int param);
};


#endif //NRF_DATABASE_RESEARCH_NF_PROFILE_REPO_H
