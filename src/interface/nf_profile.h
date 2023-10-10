#ifndef NRF_DATABASE_RESEARCH_NF_PROFILE_H
#define NRF_DATABASE_RESEARCH_NF_PROFILE_H


#include <string>
#include <vector>


class nf_profile {
    long nf_instance_id;
    string nf_type;
    string nf_status;
    int heart_beat_timer;
    vector<string> plmn_list;
    vector<string> snssais;
    // etc
};


#endif //NRF_DATABASE_RESEARCH_NF_PROFILE_H
