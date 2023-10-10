#ifndef NRF_DATABASE_RESEARCH_NF_PROFILE_REPO_H
#define NRF_DATABASE_RESEARCH_NF_PROFILE_REPO_H

#include "nf_profile.h"

template<typename T>
class nf_profile_repo {
    nf_profile_repo(T driver);
    ~nf_profile_repo();
    save(nf_profile profile);
    search_by_param(int param);
};


#endif //NRF_DATABASE_RESEARCH_NF_PROFILE_REPO_H
