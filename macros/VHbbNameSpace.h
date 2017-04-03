#include "TMath.h"


double weight_QCD(int nGenHiggsBoson, int nGenTop, int nGenVbosons, double lheHT, int GenVbosons_pdgId) {
  // QCD reweighting applied to W+Jets samples.
  if (nGenHiggsBoson==0 && nGenTop==0 && nGenVbosons==1 && lheHT>100 && abs(GenVbosons_pdgId)==24) {
    return (lheHT>100 && lheHT<200)*(1.459/1.21) + (lheHT>200 && lheHT<400)*(1.434/1.21) + (lheHT>400 && lheHT<600)*(1.532/1.21) + (lheHT>600)*(1.004/1.21);
  }
  return 1.;
}

double weight_EWK(int nGenHiggsBoson, int nGenTop, int nGenVbosons, double GenVbosons_pt, int VtypeSim, int GenVbosons_pdgId) {
  // EWK reweighting applied to W+Jets and Z+Jets samples.
  if (nGenHiggsBoson==0 && nGenTop==0 && nGenVbosons==1 && GenVbosons_pt>100 && GenVbosons_pt<3000) {
    if ((VtypeSim==0 || VtypeSim==1 || VtypeSim==4 || VtypeSim==5) && GenVbosons_pdgId==23) {
      return -0.1808051 + 6.04146*TMath::Power(GenVbosons_pt + 759.098, -0.242556);
	} else if ((VtypeSim==2 || VtypeSim==3) && GenVbosons_pdgId==24) {
      return -0.830041 + 7.93714*TMath::Power(GenVbosons_pt + 877.978, -0.213831);
    }
  }
  return 1.;
}

double weight_LOtoNLO(int nGenHiggsBoson, int nGenTop, int nGenVbosons, int GenVbosons_pdgId, double deta_jj, int nGenBJets) {
  // LO to NLO reweighting applied to Drell-Yan samples.
  if (nGenHiggsBoson==0 && nGenTop==0 && nGenVbosons==1 && GenVbosons_pdgId==23 && deta_jj<5) {
    if (nGenBJets < 1) {
      return 0.935422 + 0.0403162*deta_jj - 0.0089026*deta_jj*deta_jj + 0.0064324*deta_jj*deta_jj*deta_jj - 0.000212443*deta_jj*deta_jj*deta_jj*deta_jj;
    } else if (nGenBJets == 1) {
      return 0.962415 + 0.0329463*deta_jj - 0.0414479*deta_jj*deta_jj + 0.0240993*deta_jj*deta_jj*deta_jj - 0.00278271*deta_jj*deta_jj*deta_jj*deta_jj;
    } else if (nGenBJets >= 2) {
      return (0.721265 - 0.105643*deta_jj - 0.0206835*deta_jj*deta_jj + 0.00558626*deta_jj*deta_jj*deta_jj)*TMath::Exp(0.450244*deta_jj);
    }
  }
  return 1.;
}

double weight_TTbar(int nGenTop, double GenTop_pt1, double GenTop_pt2, int nJetsCentral) {
  if (nGenTop == 2) {
    double sf_top1 = exp(0.0615 - 0.0005*GenTop_pt1);
    double sf_top2 = exp(0.0615 - 0.0005*GenTop_pt2);
    double sf_njets = 1.2217 * (0.924773 - 0.0212496*nJetsCentral);
    return sqrt(sf_top1 * sf_top2) * sf_njets;
  }
  return 1.;
}

double weight_EWK_VH(int is_ZH, double GenVbosons_pt) {
  // Apply the EWK signal correction to ZH signal sample.
  if (is_ZH) {
    if (GenVbosons_pt>0 && GenVbosons_pt<20) {
      return 0.963285466565;
    } else if (GenVbosons_pt>20 && GenVbosons_pt<40) {
      return 0.960945354673;
    } else if (GenVbosons_pt>40 && GenVbosons_pt<60) {
      return 0.958125845181;
    } else if (GenVbosons_pt>60 && GenVbosons_pt<80) {
      return 0.956040178567;
    } else if (GenVbosons_pt>80 && GenVbosons_pt<100) {
      return 0.954829397636;
    } else if (GenVbosons_pt>100 && GenVbosons_pt<120) {
      return 0.955710296883;
    } else if (GenVbosons_pt>120 && GenVbosons_pt<140) {
      return 0.958933532209;
    } else if (GenVbosons_pt>140 && GenVbosons_pt<160) {
      return 0.960145045414;
    } else if (GenVbosons_pt>160 && GenVbosons_pt<180) {
      return 0.958798002291;
    } else if (GenVbosons_pt>180 && GenVbosons_pt<200) {
      return 0.954417774192;
    } else if (GenVbosons_pt>200 && GenVbosons_pt<220) {
      return 0.950510442261;
    } else if (GenVbosons_pt>220 && GenVbosons_pt<240) {
      return 0.943227377306;
    } else if (GenVbosons_pt>240 && GenVbosons_pt<260) {
      return 0.93876408527;
    } else if (GenVbosons_pt>260 && GenVbosons_pt<280) {
      return 0.929695202634;
    } else if (GenVbosons_pt>280 && GenVbosons_pt<300) {
      return 0.924760963363;
    } else if (GenVbosons_pt>300 && GenVbosons_pt<320) {
      return 0.915490487966;
    } else if (GenVbosons_pt>320 && GenVbosons_pt<340) {
      return 0.906099906197;
    } else if (GenVbosons_pt>340 && GenVbosons_pt<360) {
      return 0.901215165553;
    } else if (GenVbosons_pt>360 && GenVbosons_pt<380) {
      return 0.891758657639;
    } else if (GenVbosons_pt>380 && GenVbosons_pt<400) {
      return 0.882966044972;
    } else if (GenVbosons_pt>400 && GenVbosons_pt<420) {
      return 0.87385231926;
    } else if (GenVbosons_pt>420 && GenVbosons_pt<440) {
      return 0.868965659921;
    } else if (GenVbosons_pt>440 && GenVbosons_pt<460) {
      return 0.866066608534;
    } else if (GenVbosons_pt>460 && GenVbosons_pt<480) {
      return 0.854245464035;
    } else if (GenVbosons_pt>480 && GenVbosons_pt<500) {
      return 0.846544787867;
    } else if (GenVbosons_pt>500) {
      return 0.846544787867;
    }
  }
  return 1.;
}


double weights_TTbar_QCD_EWK_LOtoNLO(
  int nGenHiggsBoson,
  int isZH,
  int nGenTop,
  double GenTop_pt1,
  double GenTop_pt2,
  int nGenVbosons,
  int GenVbosons_pdgId,
  double GenVbosons_pt,
  int VtypeSim,
  double lheHT,
  double hj1_eta,
  double hj2_eta,
  int nGenBJets,
  int nJetsCentral) {
  if (nGenHiggsBoson > 0) {
    if (isZH) {
  	// Apply the EWK signal correction to ZH signal sample.
      if (GenVbosons_pt > 0 && GenVbosons_pt < 20) {
  	  return 0.963285466565;
  	} else if (GenVbosons_pt > 20 && GenVbosons_pt < 40) {
  	  return 0.960945354673;
      } else if (GenVbosons_pt > 40 && GenVbosons_pt < 60) {
  	  return 0.958125845181;
      } else if (GenVbosons_pt > 60 && GenVbosons_pt < 80) {
  	  return 0.956040178567;
      } else if (GenVbosons_pt > 80 && GenVbosons_pt < 100) {
  	  return 0.954829397636;
  	} else if (GenVbosons_pt > 100 && GenVbosons_pt < 120) {
  	  return 0.955710296883;
      } else if (GenVbosons_pt > 120 && GenVbosons_pt < 140) {
  	  return 0.958933532209;
  	} else if (GenVbosons_pt > 140 && GenVbosons_pt < 160) {
  	  return 0.960145045414;
      } else if (GenVbosons_pt > 160 && GenVbosons_pt < 180) {
  	  return 0.958798002291;
  	} else if (GenVbosons_pt > 180 && GenVbosons_pt < 200) {
  	  return 0.954417774192;
  	} else if (GenVbosons_pt > 200 && GenVbosons_pt < 220) {
  	  return 0.950510442261;
      } else if (GenVbosons_pt > 220 && GenVbosons_pt < 240) {
  	  return 0.943227377306;
  	} else if (GenVbosons_pt > 240 && GenVbosons_pt < 260) {
  	  return 0.93876408527;
      } else if (GenVbosons_pt > 260 && GenVbosons_pt < 280) {
  	  return 0.929695202634;
  	} else if (GenVbosons_pt > 280 && GenVbosons_pt < 300) {
  	  return 0.924760963363;
  	} else if (GenVbosons_pt > 300 && GenVbosons_pt < 320) {
  	  return 0.915490487966;
      } else if (GenVbosons_pt > 320 && GenVbosons_pt < 340) {
  	  return 0.906099906197;
  	} else if (GenVbosons_pt > 340 && GenVbosons_pt < 360) {
  	  return 0.901215165553;
      } else if (GenVbosons_pt > 360 && GenVbosons_pt < 380) {
  	  return 0.891758657639;
  	} else if (GenVbosons_pt > 380 && GenVbosons_pt < 400) {
  	  return 0.882966044972;
  	} else if (GenVbosons_pt > 400 && GenVbosons_pt < 420) {
  	  return 0.87385231926;
      } else if (GenVbosons_pt > 420 && GenVbosons_pt < 440) {
  	  return 0.868965659921;
  	} else if (GenVbosons_pt > 440 && GenVbosons_pt < 460) {
  	  return 0.866066608534;
      } else if (GenVbosons_pt > 460 && GenVbosons_pt < 480) {
  	  return 0.854245464035;
  	} else if (GenVbosons_pt > 480 && GenVbosons_pt < 500) {
  	  return 0.846544787867;
  	} else if (GenVbosons_pt > 500) {
  	  return 0.846544787867;
  	} else {
  	  return 1.;
  	}
    } else {
      // No corrections applied to other signal samples.
      return 1.;
    }
  } else if (nGenTop == 2) {
    // Top pT reweighting applied to TTbar sample.
    double sf_top1 = exp(0.0615 - 0.0005*GenTop_pt1);
    double sf_top2 = exp(0.0615 - 0.0005*GenTop_pt2);
    double sf_njets = 1.2217 * (0.924773 - 0.0212496*nJetsCentral);
    return sqrt(sf_top1 * sf_top2) * sf_njets;
  } else if (nGenTop > 0) {
    return 1.;
  } else {
    // QCD reweighting applied to W+Jets samples.
    double sf_qcd = 1.;
    if (nGenVbosons == 1 && lheHT > 100 && abs(GenVbosons_pdgId) == 24) {
      sf_qcd = (lheHT > 100 && lheHT < 200)*(1.459/1.21) + (lheHT > 200 && lheHT < 400)*(1.434/1.21) + (lheHT > 400 && lheHT < 600)*(1.532/1.21) + (lheHT > 600)*(1.004/1.21);
    }
    // EWK reweighting applied to W+Jets and Z+Jets samples.
    double sf_ewk = 1.;
    if (nGenVbosons == 1 && GenVbosons_pt > 100 && GenVbosons_pt < 3000) {
      if ((VtypeSim == 0 || VtypeSim == 1 || VtypeSim == 4 || VtypeSim == 5) && GenVbosons_pdgId == 23) {
  	  sf_ewk = -0.1808051 + 6.04146*TMath::Power(GenVbosons_pt + 759.098, -0.242556);
  	} else if ((VtypeSim == 2 || VtypeSim == 3) && GenVbosons_pdgId == 24) {
  	  sf_ewk = -0.830041 + 7.93714*TMath::Power(GenVbosons_pt + 877.978, -0.213831);
  	}
    }
    // LO to NLO weights applied to Z+Jets samples.
    double sf_nlo = 1.;
    if (nGenVbosons == 1 && GenVbosons_pdgId == 23) {
      double deta_jj = abs(hj1_eta - hj2_eta);
  	if (deta_jj < 5) {
  	  if (nGenBJets < 1) {
  	    sf_nlo = 0.935422 + 0.0403162*deta_jj - 0.0089026*deta_jj*deta_jj + 0.0064324*deta_jj*deta_jj*deta_jj - 0.000212443*deta_jj*deta_jj*deta_jj*deta_jj;
  	  } else if (nGenBJets == 1) {
  	    sf_nlo = 0.962415 + 0.0329463*deta_jj - 0.0414479*deta_jj*deta_jj + 0.0240993*deta_jj*deta_jj*deta_jj - 0.00278271*deta_jj*deta_jj*deta_jj*deta_jj;
  	  } else if (nGenBJets >= 2) {
  	    sf_nlo = (0.721265 - 0.105643*deta_jj - 0.0206835*deta_jj*deta_jj + 0.00558626*deta_jj*deta_jj*deta_jj)*TMath::Exp(0.450244*deta_jj);
  	  }
  	}
    }
    return sf_qcd * sf_ewk * sf_nlo;
  }
}

double reweight_ttbar_njets_central(int nGenTop, int nJetsCentral) {
  if (nGenTop == 2) {
    return 1.2217 * (0.924769 - 0.021249 * nJetsCentral);
  } else {
    return 1.;
  }
}

