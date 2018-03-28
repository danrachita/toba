//Modified at B_CLIENT_VERSION 1.1.0.0
#include "net.h"
#include "masternodeconfig.h"
#include "util.h"
#include <base58.h>

CMasternodeConfig masternodeConfig;


void CMasternodeConfig::add(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex, std::string rewardAddress, std::string rewardPercent) {
    CMasternodeEntry cme(alias, ip, privKey, txHash, outputIndex, rewardAddress, rewardPercent);
    entries.push_back(cme);
}

bool CMasternodeConfig::read(boost::filesystem::path path) {
    boost::filesystem::ifstream streamConfig(GetMasternodeConfigFile());
    if (!streamConfig.good()) {
        return true; // No masternode.conf file is OK
    }

    for(std::string line; std::getline(streamConfig, line); )
    {
        if(line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string alias, ip, privKey, txHash, outputIndex, reward, rewardAddress, rewardPercent;
        if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex >> reward)) {
            rewardAddress = "";
            rewardPercent = "";
            iss.str(line);
            iss.clear();
            if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
                LogPrintf("Could not parse masternode.conf line: %s\n", line.c_str());
                streamConfig.close();
                return false;
            }
        } else {
            size_t pos = reward.find_first_of(":");
            if(pos == string::npos) { // no ":" found
                rewardPercent = "100";
                rewardAddress = reward;
            } else {
                rewardPercent = reward.substr(pos + 1);
                rewardAddress = reward.substr(0, pos);
            }
            CBitcoinAddress address(rewardAddress);
            if (!address.IsValid()) {
                LogPrintf("Invalid TOBA address in masternode.conf line: %s\n", line.c_str());
                streamConfig.close();
                return false;
            }
        }

        add(alias, ip, privKey, txHash, outputIndex, rewardAddress, rewardPercent);
    }

    streamConfig.close();
    sort();
    return true;
}


bool CMasternodeConfig::writeAll() {
    sort();
	
	boost::filesystem::ofstream stream(GetMasternodeConfigFile(), ios::out | ios::trunc); //overwrite all(trunc)
	
	if (stream.is_open()) {
		BOOST_FOREACH(CMasternodeEntry mne, getEntries()) {
			std::string sAlias = mne.getAlias();
			std::string sAddress = mne.getIp();
			std::string sMasternodePrivKey = mne.getPrivKey();
			std::string sTxHash = mne.getTxHash();
			std::string sOutputIndex = mne.getOutputIndex();
			std::string sRewardAddress = mne.getRewardAddress();
			std::string sRewardPercentage = mne.getRewardPercentage();
			
			stream << sAlias << " " << sAddress << " " << sMasternodePrivKey << " " << sTxHash << " " << sOutputIndex;
			if (sRewardAddress != "" && sRewardPercentage != ""){
			    stream << " " << sRewardAddress << ":" << sRewardPercentage << std::endl;
			} else {
			    stream << std::endl;
			}
		}
		
		stream.close();
	}
	
	return true;
}


bool CMasternodeConfig::remove(std::string sAlias) {
	
	if (sAlias == "")	return false;
	
	for(int i=0; i<(int)entries.size(); i++) {
		if (entries[i].getAlias() == sAlias) {
			entries.erase(entries.begin() + i);
			return true;
		}
	}
	
	return false;
}


bool cmp_alias(const CMasternodeConfig::CMasternodeEntry& a, const CMasternodeConfig::CMasternodeEntry& b) {
	return a.getAlias() > b.getAlias();
}

void CMasternodeConfig::sort() {
	
	 std::sort(entries.begin(), entries.end(), cmp_alias);
	
	return;
}



