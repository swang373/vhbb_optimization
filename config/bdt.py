#-------------------------------------------------
# BDT Configuration
#-------------------------------------------------

## The expression used to weight the training and testing events.
EVENT_WEIGHT = 'sign(genWeight) * puWeight * bTagWeightMoriondCMVA * VHbb::weight_QCD(nGenHiggsBoson, nGenTop, nGenVbosons, lheHT, Alt$(GenVbosons_pdgId[0],0)) * VHbb::weight_EWK(nGenHiggsBoson, nGenTop, nGenVbosons, Alt$(GenVbosons_pt[0],0), VtypeSim, Alt$(GenVbosons_pdgId[0],0)) * VHbb::weight_LOtoNLO(nGenHiggsBoson, nGenTop, nGenVbosons, Alt$(GenVbosons_pdgId[0],0), abs(Jet_eta[hJCMVAV2idx[0]]-Jet_eta[hJCMVAV2idx[1]]), Sum$(GenJet_pt>20&&abs(GenJet_eta)<2.4&&GenJet_numBHadrons)) * VHbb::weight_TTbar_genPt(nGenTop, Alt$(GenTop_pt[0],0), Alt$(GenTop_pt[1],0)) * VHbb::weight_EWK_VH(is_ZH, Alt$(GenVbosons_pt[0],0))'

## The training features.
FEATURES = [
    'HCMVAV2_reg_mass',
    'HCMVAV2_reg_pt',
    'abs(TVector2::Phi_mpi_pi(HCMVAV2_reg_phi-V_new_phi))',
    'V_new_pt',
    '(Jet_eta[hJCMVAV2idx[0]]-Jet_eta[hJCMVAV2idx[1]])',
    'Jet_btagCMVAV2[hJCMVAV2idx[0]]',
    'Jet_btagCMVAV2[hJCMVAV2idx[1]]',
    'softActivityVH_njets5',
    'TVector2::Phi_mpi_pi(Jet_phi[hJCMVAV2idx[0]]-Jet_phi[hJCMVAV2idx[1]])',
    'max(hJetCMVAV2_pt_reg_0,hJetCMVAV2_pt_reg_1)',
    'min(hJetCMVAV2_pt_reg_0,hJetCMVAV2_pt_reg_1)',
    'Max$(Jet_btagCMVAV2[aJCMVAV2idx])',
    'Max$(Jet_pt[aJCMVAV2idx])',
    'MinIf$(abs(TVector2::Phi_mpi_pi(Jet_phi-V_new_phi))-3.1415,Jet_pt>30&&Jet_puId>=4)',
]

