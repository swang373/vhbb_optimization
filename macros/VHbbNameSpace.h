#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TMath.h"
/*#if !defined(__CINT__) && !defined(__MAKECINT__)
  #include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
  #include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
  #endif*/

namespace VHbb {
  

  double deltaPhi(double phi1,double phi2)
  {
    double result = phi1 - phi2;
    while (result > TMath::Pi()) result -= 2*TMath::Pi();
    while (result <= -TMath::Pi()) result += 2*TMath::Pi();
    return result;
  }

  inline double deltaR(double eta1,double phi1,double eta2,double phi2)
    {
      double deta = eta1 - eta2;
      double dphi = deltaPhi(phi1, phi2);
      return TMath::Sqrt(deta*deta + dphi*dphi);
    }


  double Hmass( double V_eta,double V_phi,double V_pt, 
		double hJet1_eta,double hJet1_phi,double hJet1_pt, 
		double hJet2_eta,double hJet2_phi,double hJet2_pt ){
    
    TVector3 V(1,1,1);
    V.SetPtEtaPhi(V_pt,V_eta,V_phi);
    
    TVector3 H1(1,1,1);
    H1.SetPtEtaPhi(hJet1_pt,hJet1_eta,hJet1_phi);
    H1.SetMag(1/sin(H1.Theta()));
    
    TVector3 H2(1,1,1);
    H2.SetPtEtaPhi(hJet2_pt,hJet2_eta,hJet2_phi);
    H2.SetMag(1/sin(H2.Theta()));
    
    TVector3 n1(H1);
    TVector3 n2(H2);
    
    double det= n1.Px() * n2.Py() - n2.Px() * n1.Py();
    
    H1.SetMag( (  - n2.Py() * V.Px() + n2.Px() * V.Py() )  / (sin(n1.Theta()) *det ) );
    H2.SetMag( ( + n1.Py() * V.Px() - n1.Px() * V.Py() )  / (sin(n2.Theta())  *det ) );
    
    double mass=TMath::Sqrt( TMath::Power( (H1.Mag()+H2.Mag()),2 ) - TMath::Power(( ( H1+H2 ).Mag()),2) );
    
    return mass;
    
  }
  
  double Hmass_comb(double hJet1_eta,double hJet1_phi,double hJet1_pt, double hJet1_mass,
		    double hJet2_eta,double hJet2_phi,double hJet2_pt, double hJet2_mass){

    TLorentzVector H1, H2;
    H1.SetPtEtaPhiM(hJet1_pt,hJet1_eta,hJet1_phi, hJet1_mass);;
    H2.SetPtEtaPhiM(hJet2_pt,hJet2_eta,hJet2_phi, hJet2_mass);

    return (H1 + H2).M();

  }

  double Hmass_3j(double h_eta,double h_phi,double h_pt, double h_mass,
		  double aJet_eta,double aJet_phi,double aJet_pt, double aJet_mass){

    TLorentzVector H, H3;
    H.SetPtEtaPhiM( h_pt,h_eta,h_phi, h_mass);;
    H3.SetPtEtaPhiM(aJet_pt,aJet_eta,aJet_phi, aJet_mass);

    return (H + H3).M();

  }


  double GetEnergy(double pt, double eta, double mass){
    TLorentzVector m1;
    m1.SetPtEtaPhiM(pt, eta, 0, mass);
    return(m1.Energy());
  }

  double HVMass(double pt, double eta, double phi, double mass, double pt2, double eta2, double phi2, double mass2){
    TLorentzVector m1, m2, msum;
    m1.SetPtEtaPhiM(pt, eta, phi, mass);
    m2.SetPtEtaPhiM(pt2, eta2, phi2, mass2);
    msum = m1 + m2;
    return(msum.M());
  }

  double HV_mass(double pt, double eta, double phi, double mass, double pt2, double eta2, double phi2, double mass2){
    TLorentzVector m1, m2, msum;
    m1.SetPtEtaPhiM(pt, eta, phi, mass);
    m2.SetPtEtaPhiM(pt2, eta2, phi2, mass2);
    msum = m1 + m2;
    return(msum.M());
  }
  
  double ANGLELZ(double pt, double eta, double phi, double mass, double pt2, double eta2, double phi2, double mass2){
    TLorentzVector m1, m2, msum;
    m1.SetPtEtaPhiM(pt, eta, phi, mass);
    m2.SetPtEtaPhiM(pt2, eta2, phi2, mass2);
    msum = m1 + m2;

    TVector3 bZ =  msum.BoostVector();

    m1.Boost(-bZ);  
    m2.Boost(-bZ);  

    TVector3 b1;


    if((int) (pt) % 2 == 0)
      b1 =  m1.BoostVector();
    else
      b1 =  m2.BoostVector();

    double cosTheta = b1.Dot(msum.BoostVector()) / (b1.Mag()*msum.BoostVector().Mag());
    return(cosTheta);
  }


  double ANGLEHB(double pt, double eta, double phi, double e, double pt2, double eta2, double phi2, double e2){
    TLorentzVector m1, m2, msum;
    m1.SetPtEtaPhiE(pt, eta, phi, e);
    m2.SetPtEtaPhiE(pt2, eta2, phi2, e2);
    msum = m1 + m2;

    TVector3 bZ =  msum.BoostVector();

    m1.Boost(-bZ);
    m2.Boost(-bZ);  

    TVector3 b1;

    if((int) (pt) % 2 == 0)
      b1 =  m1.BoostVector();
    else
      b1 =  m2.BoostVector();

    double cosTheta = b1.Dot(msum.BoostVector()) / (b1.Mag()*msum.BoostVector().Mag());
    return(cosTheta);
  }

  double ANGLEHBWithM(double pt, double eta, double phi, double mass, double pt2, double eta2, double phi2, double mass2){
    TLorentzVector m1, m2, msum;
    m1.SetPtEtaPhiM(pt, eta, phi, mass);
    m2.SetPtEtaPhiM(pt2, eta2, phi2, mass2);
    msum = m1 + m2;

    TVector3 bZ =  msum.BoostVector();

    m1.Boost(-bZ);
    m2.Boost(-bZ);  

    TVector3 b1;

    if((int) (pt) % 2 == 0)
      b1 =  m1.BoostVector();
    else
      b1 =  m2.BoostVector();

    double cosTheta = b1.Dot(msum.BoostVector()) / (b1.Mag()*msum.BoostVector().Mag());
    return(cosTheta);
  }

  double metCorSysShift(double met, double metphi, int Nvtx, int EVENT_run)
  {
    double metx = met * cos(metphi);
    double mety = met * sin(metphi);
    double px = 0.0, py = 0.0;
    if (EVENT_run!=1) {
      //pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_data
      px = +1.68804e-01 + 3.37139e-01*Nvtx;
      py = -1.72555e-01 - 1.79594e-01*Nvtx;
    } else {
      //pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_mc
      px = +2.22335e-02 - 6.59183e-02*Nvtx;
      py = +1.52720e-01 - 1.28052e-01*Nvtx;
    }
    metx -= px;
    mety -= py;
    return std::sqrt(metx*metx + mety*mety);
  }

  double metphiCorSysShift(double met, double metphi, int Nvtx, int EVENT_run)
  {
    double metx = met * cos(metphi);
    double mety = met * sin(metphi);
    double px = 0.0, py = 0.0;
    if (EVENT_run!=1) {

      //pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_data
      px = +1.68804e-01 + 3.37139e-01*Nvtx;
      py = -1.72555e-01 - 1.79594e-01*Nvtx;
    } else {
      //pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_mc
      px = +2.22335e-02 - 6.59183e-02*Nvtx;
      py = +1.52720e-01 - 1.28052e-01*Nvtx;
    }
    metx -= px;
    mety -= py;
    if (metx == 0.0 && mety == 0.0)
      return 0.0;

    double phi1 = std::atan2(mety,metx);
    double phi2 = std::atan2(mety,metx)-2.0*M_PI;
    if (std::abs(phi1-metphi) < std::abs(phi2-metphi)+0.5*M_PI)
      return phi1;
    else
      return phi2;
  }

  TVector2 metType1Reg(double met, double metphi, double corr1, double corr2, double pt1, double eta1, double phi1, double e1, double pt2, double eta2, double phi2, double e2)
  {
    double metx = met * cos(metphi);
    double mety = met * sin(metphi);
    TLorentzVector j1;
    TLorentzVector j2;
    j1.SetPtEtaPhiE(pt1,eta1,phi1, e1 );
    j2.SetPtEtaPhiE(pt2,eta2,phi2, e2 );
    metx += j1.Px()*(1-corr1);
    metx += j2.Px()*(1-corr2);
    mety += j1.Py()*(1-corr1);
    mety += j2.Py()*(1-corr2);
    TVector2 corrMET(metx, mety);
    return corrMET;
  }

  double metType1Phi(double met, double metphi, double corr1, double corr2, double pt1, double eta1, double phi1, double e1, double pt2, double eta2, double phi2, double e2){
    return metType1Reg(met, metphi, corr1, corr2, pt1, eta1, phi1, e1, pt2, eta2, phi2, e2).Phi();

  }
  double metType1Et(double met, double metphi, double corr1, double corr2, double pt1, double eta1, double phi1, double e1, double pt2, double eta2, double phi2, double e2){
    return metType1Reg(met, metphi, corr1, corr2, pt1, eta1, phi1, e1, pt2, eta2, phi2, e2).Mod();

  }


  double met_MPF(double met, double metphi, double pt, double phi)
  {
    return 1.+met*pt*std::cos( deltaPhi(metphi,phi) ) / (pt*pt);

  }

  double resolutionBias(double eta)
  {
    // return 0;//Nominal!
    if(eta< 0.5) return 0.052;
    if(eta< 1.1) return 0.057;
    if(eta< 1.7) return 0.096;
    if(eta< 2.3) return 0.134;
    if(eta< 5) return 0.28;
    return 0;
  }

  double evalJERBias( double ptreco, double ptgen, double eta1){
    double eta = fabs(eta1);
    double cor =1;   
    if ((fabs(ptreco - ptgen)/ ptreco)<0.5) { //Limit the effect to the core 
      cor = (ptreco +resolutionBias(eta) *(ptreco-ptgen))/ptreco;   
    }
    if (ptgen > 0.) return ptreco*cor;
    else return ptreco;
  }

  double evalEt( double pt, double eta, double phi, double e){
    TLorentzVector j;
    j.SetPtEtaPhiE(pt,eta,phi, e );
    return j.Et(); 
  }

  double evalEtFromPtEtaPhiM( double pt, double eta, double phi, double m){
    TLorentzVector j;
    j.SetPtEtaPhiM(pt,eta,phi, m );
    return j.Et(); 
  }

  double evalMt( double pt, double eta, double phi, double e){
    TLorentzVector j;
    j.SetPtEtaPhiE(pt,eta,phi, e );
    return j.Mt(); 
}

  double evalMtFromPtEtaPhiM( double pt, double eta, double phi, double m){
    TLorentzVector j;
    j.SetPtEtaPhiM(pt,eta,phi, m );
    return j.Mt(); 
  }
  /*double evalJECUnc( double pt, double eta){
  // Total uncertainty for reference
  JetCorrectionUncertainty *total = new JetCorrectionUncertainty("/shome/nmohr/CMSSW_5_2_6_patch1/src/UserCode/VHbb/data/START53_V15MC_Uncertainty_AK5PFchs.txt");

  total->setJetPt(pt);
  total->setJetEta(eta);
  double uncert =  total->getUncertainty(true);
  delete total;
  return uncert;
  }*/

  float triggerMET(float minMHTMET) {
    /* "exponential", "[2]*(1e0-exp(-[0]*(x-[1])))",120,500);
       1  p0           5.27300e-02   1.26236e-02   1.24859e-05  -1.53523e-03
       2  p1           1.04024e+02   1.02448e+01   1.05355e-02  -4.18185e-05
       3  p2           9.76189e-01   1.48333e-03   2.67820e-06  -1.33843e-01
    */
    if (minMHTMET < 120) {
      return 0.;
    } else if (minMHTMET >= 120 && minMHTMET < 500) {
      return 9.76189e-01 * (1e0 - exp(-5.27300e-02 * (minMHTMET-1.04024e+02)));
    } else {
      return 9.76189e-01;
    }
  }

  double ptWeightDY( double lheV_pt, double sign = 1.)
  {
    double SF = 1.;
    if (50. < lheV_pt && lheV_pt < 100.){
      SF = 0.873885+0.00175853*sign*lheV_pt;
    }
    else if (lheV_pt > 100){
      SF = 1.10651-0.000705265*sign*lheV_pt;
    }
    return SF;
  }

  // weights correction for EWK NLO correction
  double ptWeightZllH( double genHPt){
    double SF = 1.;
    if ( genHPt > 1.) SF = 0.999757-0.000174527*genHPt;
    return SF>0?SF:0;
  }


  double minCSVold(int EVENT_run, double hJet_csv0, double hJet_csv1, double hJet_csvOld0, double hJet_csvOld1){

    if( EVENT_run < 2 )
      return std::min(hJet_csvOld0,hJet_csvOld1);
    else
      return std::min(hJet_csv0,hJet_csv1);
  }

  double maxCSVold(int EVENT_run, double hJet_csv0, double hJet_csv1, double hJet_csvOld0, double hJet_csvOld1){

    if( EVENT_run < 2 )
      return std::max(hJet_csvOld0,hJet_csvOld1);
    else
      return std::max(hJet_csv0,hJet_csv1);
  }


  // weights correction for EWK NLO correction from Atlas
  double ewkAtlas8TeVZllH( double genHPt, double genZPt){
    if (genHPt < 1.) return 1.;
    double hll8_contents[95] = {0.000664024, -0.00357095, -0.00767076, -0.00967366, -0.0134844, -0.0157148, -0.0181885, -0.0209647, -0.0232788, -0.0252373, -0.0265634, -0.0275069, -0.0285776, -0.0281683, -0.0294206, -0.0299975, -0.0308047, -0.0311716, -0.030913, -0.0324821, -0.0323192, -0.0324639, -0.0319356, -0.0322621, -0.0331146, -0.0338905, -0.0345189, -0.0358591, -0.0358407, -0.040018, -0.0396389, -0.0407177, -0.0445103, -0.0441406, -0.0471215, -0.0463301, -0.0513777, -0.0536773, -0.0546446, -0.0568508, -0.0590333, -0.0612157, -0.0633981, -0.0655805, -0.067763, -0.0699454, -0.0721278, -0.0743103, -0.0764927, -0.0786751, -0.0808575, -0.08304, -0.0852224, -0.0874048, -0.0895872, -0.0917697, -0.0939521, -0.0961345, -0.098317, -0.100499, -0.102682, -0.104864, -0.107047, -0.109229, -0.111412, -0.113594, -0.115776, -0.117959, -0.120141, -0.122324, -0.124506, -0.126689, -0.128871, -0.131053, -0.133236, -0.135418, -0.137601, -0.139783, -0.141965, -0.144148, -0.14633, -0.148513, -0.150695, -0.152878, -0.15506, -0.157242, -0.159425, -0.161607, -0.16379, -0.165972, -0.168155, -0.170337, -0.172519, -0.174702, -0.176884};
    int corrBin = (int) ( (genZPt - 25.) / 5. );
    if (corrBin < 0) corrBin = 0;
    if (corrBin > 94) corrBin = 94;
    double SF = 1.+hll8_contents[corrBin];
    return SF;
  }
  
double eleEff(double pt, double eta){
if (pt >= 20.0 && pt < 25.0 && eta < -1.57000005245 && eta > -2.5) return 0.955429255962 ;
if (pt >= 20.0 && pt < 25.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.927380621433 ;
if (pt >= 20.0 && pt < 25.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.752811849117 ;
if (pt >= 20.0 && pt < 25.0 && eta < 0.0 && eta > -0.800000011921) return 0.758898377419 ;
if (pt >= 20.0 && pt < 25.0 && eta < 0.800000011921 && eta > 0.0) return 0.755395233631 ;
if (pt >= 20.0 && pt < 25.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.765102207661 ;
if (pt >= 20.0 && pt < 25.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.889689266682 ;
if (pt >= 20.0 && pt < 25.0 && eta < 2.5 && eta > 1.57000005245) return 0.960859179497 ;
if (pt >= 25.0 && pt < 30.0 && eta < -1.57000005245 && eta > -2.5) return 0.973749041557 ;
if (pt >= 25.0 && pt < 30.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.95562505722 ;
if (pt >= 25.0 && pt < 30.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.854979991913 ;
if (pt >= 25.0 && pt < 30.0 && eta < 0.0 && eta > -0.800000011921) return 0.859907507896 ;
if (pt >= 25.0 && pt < 30.0 && eta < 0.800000011921 && eta > 0.0) return 0.856850981712 ;
if (pt >= 25.0 && pt < 30.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.854210674763 ;
if (pt >= 25.0 && pt < 30.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.961790204048 ;
if (pt >= 25.0 && pt < 30.0 && eta < 2.5 && eta > 1.57000005245) return 0.984032571316 ;
if (pt >= 30.0 && pt < 35.0 && eta < -1.57000005245 && eta > -2.5) return 0.975343048573 ;
if (pt >= 30.0 && pt < 35.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.971944391727 ;
if (pt >= 30.0 && pt < 35.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.916224777699 ;
if (pt >= 30.0 && pt < 35.0 && eta < 0.0 && eta > -0.800000011921) return 0.918354153633 ;
if (pt >= 30.0 && pt < 35.0 && eta < 0.800000011921 && eta > 0.0) return 0.914323210716 ;
if (pt >= 30.0 && pt < 35.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.918937265873 ;
if (pt >= 30.0 && pt < 35.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.949005782604 ;
if (pt >= 30.0 && pt < 35.0 && eta < 2.5 && eta > 1.57000005245) return 0.978435099125 ;
if (pt >= 35.0 && pt < 40.0 && eta < -1.57000005245 && eta > -2.5) return 0.977416038513 ;
if (pt >= 35.0 && pt < 40.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.96676659584 ;
if (pt >= 35.0 && pt < 40.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.949674129486 ;
if (pt >= 35.0 && pt < 40.0 && eta < 0.0 && eta > -0.800000011921) return 0.955099225044 ;
if (pt >= 35.0 && pt < 40.0 && eta < 0.800000011921 && eta > 0.0) return 0.95435655117 ;
if (pt >= 35.0 && pt < 40.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.949129760265 ;
if (pt >= 35.0 && pt < 40.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.966977357864 ;
if (pt >= 35.0 && pt < 40.0 && eta < 2.5 && eta > 1.57000005245) return 0.974912643433 ;
if (pt >= 40.0 && pt < 45.0 && eta < -1.57000005245 && eta > -2.5) return 0.976379692554 ;
if (pt >= 40.0 && pt < 45.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.96400809288 ;
if (pt >= 40.0 && pt < 45.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.967367231846 ;
if (pt >= 40.0 && pt < 45.0 && eta < 0.0 && eta > -0.800000011921) return 0.976113498211 ;
if (pt >= 40.0 && pt < 45.0 && eta < 0.800000011921 && eta > 0.0) return 0.970276534557 ;
if (pt >= 40.0 && pt < 45.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.965076506138 ;
if (pt >= 40.0 && pt < 45.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.962332487106 ;
if (pt >= 40.0 && pt < 45.0 && eta < 2.5 && eta > 1.57000005245) return 0.975438177586 ;
if (pt >= 45.0 && pt < 50.0 && eta < -1.57000005245 && eta > -2.5) return 0.974642693996 ;
if (pt >= 45.0 && pt < 50.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.976564764977 ;
if (pt >= 45.0 && pt < 50.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.975869596004 ;
if (pt >= 45.0 && pt < 50.0 && eta < 0.0 && eta > -0.800000011921) return 0.984652400017 ;
if (pt >= 45.0 && pt < 50.0 && eta < 0.800000011921 && eta > 0.0) return 0.976347208023 ;
if (pt >= 45.0 && pt < 50.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.974825322628 ;
if (pt >= 45.0 && pt < 50.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.968582391739 ;
if (pt >= 45.0 && pt < 50.0 && eta < 2.5 && eta > 1.57000005245) return 0.973198652267 ;
if (pt >= 50.0 && pt < 200.0 && eta < -1.57000005245 && eta > -2.5) return 0.976521909237 ;
if (pt >= 50.0 && pt < 200.0 && eta < -1.44000005722 && eta > -1.57000005245) return 0.968289792538 ;
if (pt >= 50.0 && pt < 200.0 && eta < -0.800000011921 && eta > -1.44000005722) return 0.980251729488 ;
if (pt >= 50.0 && pt < 200.0 && eta < 0.0 && eta > -0.800000011921) return 0.988307952881 ;
if (pt >= 50.0 && pt < 200.0 && eta < 0.800000011921 && eta > 0.0) return 0.983105957508 ;
if (pt >= 50.0 && pt < 200.0 && eta < 1.44000005722 && eta > 0.800000011921) return 0.976873219013 ;
if (pt >= 50.0 && pt < 200.0 && eta < 1.57000005245 && eta > 1.44000005722) return 0.970499277115 ;
if (pt >= 50.0 && pt < 200.0 && eta < 2.5 && eta > 1.57000005245) return 0.972752988338 ;
return 1.;
}
  
  double mueEff(int Vtype, double eta0, double eta1, double pt0, double pt1){
      if (Vtype == 0) return 1.;
      if (Vtype == 1) {
          return eleEff(pt0,eta0)*eleEff(pt1,eta1);
      }
      return 1.;
  }

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

  double ptWeightEWK(int nGenVbosons, double GenVbosons_pt, int VtypeSim, int GenVbosons_pdgId) {
    double SF = 1.;
    if (nGenVbosons == 1) {
      if (VtypeSim == 0 || VtypeSim == 1 || VtypeSim == 4 || VtypeSim == 5) {
        if (GenVbosons_pdgId == 23) { // Z
          if (GenVbosons_pt > 100 && GenVbosons_pt < 3000) SF = -0.1808051 + 6.04146*TMath::Power(GenVbosons_pt+759.098, -0.242556);
        }
      } else if (VtypeSim == 2 || VtypeSim == 3) { // W
        if (abs(GenVbosons_pdgId) == 24) {
          if (GenVbosons_pt > 100 && GenVbosons_pt < 3000) SF = -0.830041 + 7.93714*TMath::Power(GenVbosons_pt+877.978, -0.213831);
        }
      }
    }
    return SF > 0 ? SF : 0;
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

  double weight_TTbar_genPt(int nGenTop, double GenTop_pt1, double GenTop_pt2) {
    if (nGenTop == 2) {
      double sf_top1 = exp(0.0615 - 0.0005*GenTop_pt1);
      double sf_top2 = exp(0.0615 - 0.0005*GenTop_pt2);
      return sqrt(sf_top1 * sf_top2);
    }
    return 1.;
  }

  double weight_TTbar_nJetCentral(int nGenTop, int nJetsCentral) {
    if (nGenTop == 2) {
      double sf_njets = 1.2217 * (0.924773 - 0.0212496*nJetsCentral);
      return sf_njets;
    }
    return 1.;
  }

   double weight_TTbar_nJetCentral_Up(int nGenTop, int nJetsCentral) {
    if (nGenTop == 2) {
      double sf_njets = 1.2222 * (0.9058043 - 0.01751612*nJetsCentral);
      return sf_njets;
    }
    return 1.;
  }
 
  double weight_TTbar_nJetCentral_Down(int nGenTop, int nJetsCentral) {
    if (nGenTop == 2) {
      double sf_njets = 1.2213 * (0.9437417 - 0.02498308*nJetsCentral);
      return sf_njets;
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

  float puWeight_ichep(int i) {
    double puw[38] = {
      0.00026954692859,
	  0.00834201570744,
      0.0146551644559,
      0.0267593718187,
      0.045546866213,
      0.0351739785649,
      0.0389242914756,
      0.131305445658,
      0.337172065156,
      0.645651266938,
      0.908493559723,
      1.09739459449,
      1.26533979742,
      1.41586705341,
      1.53524109717,
      1.49032150282,
      1.39123249386,
      1.45737145535,
      1.38588035333,
      1.4464676134,
      1.23282560995,
      1.08387439504,
      1.02663484504,
      0.97464906611,
      0.829692105572,
      0.758597838706,
      0.554540190093,
      0.442016886186,
      0.291492210196,
      0.203297812168,
      0.131804681706,
      0.0834846669777,
      0.0680713812995,
      0.0497105409204,
      0.0496692836227,
      0.0581182475108,
      0.089209326104,
      0.0941178579122,
	};
	if (i < 0) {
      return 1.;
    } else if (i > 37) {
      return puw[37];
    } else {
      return puw[i];
    }
  }

}


