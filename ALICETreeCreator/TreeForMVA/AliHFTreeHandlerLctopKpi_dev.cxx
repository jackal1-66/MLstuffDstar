/* Copyright(c) 1998-2008, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//*************************************************************************
// \class AliHFTreeHandlerLctopKpi_dev
// \brief helper class to handle a tree for Lc->pKpi cut optimisation and MVA analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// A. Festanti, andrea.festanti@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
// G. Innocenti, gian.michele.innocenti@cern.ch
// F. Prino, prino@to.infn.it
// L. Vermunt, luuk.vermunt@cern.ch
// L. van Doremalen, lennart.van.doremalen@cern.ch
// J. Norman, jaime.norman@cern.ch
/////////////////////////////////////////////////////////////

#include <TString.h>
#include <TRandom3.h>
#include "AliHFTreeHandlerLctopKpi_dev.h"
#include "AliAODRecoDecayHF3Prong.h"

/// \cond CLASSIMP
ClassImp(AliHFTreeHandlerLctopKpi_dev);
/// \endcond

//________________________________________________________________
AliHFTreeHandlerLctopKpi_dev::AliHFTreeHandlerLctopKpi_dev():
  AliHFTreeHandler_dev(),
  fImpParProng(),
  fSigmaVertex(),
  fNormd0MeasMinusExp(),
  fRandom()
{
  //
  // Default constructor
  //

  fNProngs=3; // --> cannot be changed
  fRandom = new TRandom3();
}

//________________________________________________________________
AliHFTreeHandlerLctopKpi_dev::AliHFTreeHandlerLctopKpi_dev(int PIDopt):
  AliHFTreeHandler_dev(PIDopt),
  fImpParProng(),
  fSigmaVertex(),
  fNormd0MeasMinusExp(),
  fRandom()
{
  //
  // Standard constructor
  //

  fNProngs=3; // --> cannot be changed
  fRandom = new TRandom3();
}

//________________________________________________________________
AliHFTreeHandlerLctopKpi_dev::~AliHFTreeHandlerLctopKpi_dev()
{
  //
  // Default Destructor
  //
}

//________________________________________________________________
TTree* AliHFTreeHandlerLctopKpi_dev::BuildTree(TString name, TString title) 
{
  fIsMCGenTree=false;

  if(fTreeVar) {
    delete fTreeVar;
    fTreeVar=0x0;
  }
  fTreeVar = new TTree(name.Data(),title.Data());

  //set common variables
  AddCommonDmesonVarBranches();

  //set Lc variables
  fTreeVar->Branch("sig_vert",&fSigmaVertex);
  fTreeVar->Branch("max_norm_d0d0exp",&fNormd0MeasMinusExp);
  for(unsigned int iProng=0; iProng<fNProngs; iProng++){
    fTreeVar->Branch(Form("imp_par_prong%d",iProng),&fImpParProng[iProng]);
  }

  //set single-track variables
  AddSingleTrackBranches();

  //set PID variables
  if(fPidOpt!=kNoPID) AddPidBranches(true,true,true,true,true);

  return fTreeVar;
}

//________________________________________________________________
bool AliHFTreeHandlerLctopKpi_dev::SetVariables(AliAODRecoDecayHF* cand, float bfield, int masshypo, AliPIDResponse* pidrespo)
{
  if(!cand) return false;
  if(fFillOnlySignal) { //if fill only signal and not signal candidate, do not store
    if(!(fCandTypeMap&kSignal)) return true;
  }
  fNCandidates++;

  //topological variables
  //common
  fCandType.push_back(fCandTypeMap);
  fPt.push_back(cand->Pt());
  fY.push_back(cand->Y(411));
  fEta.push_back(cand->Eta());
  fPhi.push_back(cand->Phi());
  fDecayLength.push_back(cand->DecayLength());
  fDecayLengthXY.push_back(cand->DecayLengthXY());
  fNormDecayLengthXY.push_back(cand->NormalizedDecayLengthXY());
  fCosP.push_back(cand->CosPointingAngle());
  fCosPXY.push_back(cand->CosPointingAngleXY());
  fImpParXY.push_back(cand->ImpParXY());
  fNormd0MeasMinusExp.push_back(ComputeMaxd0MeasMinusExp(cand,bfield));

  //Lc -> pKpi variables
  if(masshypo==1){ //pKpi
    fInvMass.push_back(((AliAODRecoDecayHF3Prong*)cand)->InvMassLcpKpi());
  }
  else if(masshypo==2){ //piKp
    fInvMass.push_back(((AliAODRecoDecayHF3Prong*)cand)->InvMassLcpiKp());
  }
  else return false;
  fSigmaVertex.push_back(((AliAODRecoDecayHF3Prong*)cand)->GetSigmaVert());
  for(unsigned int iProng=0; iProng<fNProngs; iProng++) {
    fImpParProng[iProng].push_back(cand->Getd0Prong(iProng));
  }
    
  //single track variables
  AliAODTrack* prongtracks[3];
  for(unsigned int iProng=0; iProng<fNProngs; iProng++) prongtracks[iProng] = (AliAODTrack*)cand->GetDaughter(iProng);
  bool setsingletrack = SetSingleTrackVars(prongtracks);  
  if(!setsingletrack) return false;

  //pid variables
  if(fPidOpt==kNoPID) return true;

  bool setpid = SetPidVars(prongtracks,pidrespo,true,true,true,true,true);
  if(!setpid) return false;

  return true;
}

//________________________________________________________________
void AliHFTreeHandlerLctopKpi_dev::FillTree() {
  fTreeVar->Fill();
  
  //VERY IMPORTANT: CLEAR ALL VECTORS
  if(!fIsMCGenTree) {
    ResetDmesonCommonVarVectors();
    fSigmaVertex.clear();
    fNormd0MeasMinusExp.clear();
    for(unsigned int iProng=0; iProng<fNProngs; iProng++) fImpParProng[iProng].clear();
    ResetSingleTrackVarVectors();
    if(fPidOpt!=kNoPID) ResetPidVarVectors();
  }
  else {
    ResetMCGenVectors();
  }
  fCandTypeMap=0;
  fNCandidates=0;
}
