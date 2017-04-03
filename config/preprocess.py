#-------------------------------------------------
# Preprocessing Configuration
#-------------------------------------------------

## The selection cuts defining the signal region.
SELECTION = 'Vtype_new==4 && (HCMVAV2_reg_mass>60&&HCMVAV2_reg_mass<160) && Sum$(abs(TVector2::Phi_mpi_pi(Jet_phi-V_new_phi))<0.5 && Jet_pt>30 && Jet_puId>=4)==0 && Jet_btagCMVAV2[hJCMVAV2idx[0]]>0.9432 && nselLeptons==0 && Jet_pt[hJCMVAV2idx[0]]>60 && abs(TVector2::Phi_mpi_pi(V_new_phi-tkMet_phi))<0.5 && Sum$(Jet_pt>30 && abs(Jet_eta)<2.4 && Jet_puId>=4)<4'

## The target luminosity in pb^-1. This is used to generate a new branch
## containing the effective luminosity of the Monte-Carlo sample.
TARGET_LUMI = 35900
    
## The branches kept in the preprocessed ntuples. These are usually
## the training features and those necessary for applying event weights.
BRANCHES = [
    'HCMVAV2_reg_mass',
    'HCMVAV2_reg_pt',
    'HCMVAV2_reg_phi',
    'V_new_pt',
    'V_new_phi',
    'hJCMVAV2idx',
    'Jet_btagCMVAV2',
    'softActivityVH_njets5',
    'Jet_pt',
    'Jet_pt_reg',
    'Jet_eta',
    'Jet_phi',
    'aJCMVAV2idx',
    'Jet_puId',
    'genWeight',
    'puWeight',
    'bTagWeightCMVAv2_Moriond',
    'nGenHiggsBoson',
    'is_ZH',
    'nGenTop',
    'GenTop_pt',
    'nGenVbosons',
    'GenVbosons_pdgId',
    'GenVbosons_pt',
    'VtypeSim',
    'lheHT',
    'Jet_eta',
    'GenJet_pt',
    'GenJet_eta',
    'GenJet_numBHadrons',
]

