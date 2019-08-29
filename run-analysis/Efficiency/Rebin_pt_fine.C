#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <Riostream.h>
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "AliCFContainer.h"
#include "AliCFEffGrid.h"
#include "TLegend.h"
#include "AliCFTaskVertexingHF.h"

// rebinvar bins (multiplicity)
const Int_t nRebinVarBins=11;
Double_t rebinVarLimits[nRebinVarBins+1] = {1.,2.,3.,4.,5.,6.,8.,12.,16.,24.,36.,50.};

// slice var bins (pt)
const Int_t nVarSliceBins=1;
Double_t varSliceLimits[nVarSliceBins+1] = {0.,999.};
Int_t color[nVarSliceBins]={kRed};//, kMagenta, kBlue, kGreen+1, kBlack};


//FOLLOWING FUNCTION CAUSES KERNEL PANIC. DO NOT USE.
/* void RebinCFContainer_vsMult() {
   doRebinCFContainerMult("", 0);
   doRebinCFContainerMult("_loose",0);
   doRebinCFContainerMult("_tight",0);
   doRebinCFContainerMult("", 1);
   doRebinCFContainerMult("_loose", 1);
   doRebinCFContainerMult("_tight", 1);


}*/

void Rebin_pt_fine(TString cutString="DStarcorrfw_mgiacalo",  Int_t selection=0,Int_t rebinVar=0, Int_t sliceVar=3){
//  gInterpreter->ExecuteMacro("LoadLibraries.C");
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetOptTitle(0);
	
  gSystem->SetIncludePath("-I. -I$ROOTSYS/include");
  gSystem->SetIncludePath("-I. -I$ALICE_ROOT/include  -I$ROOTSYS/include");
  gSystem->Load("libANALYSIS.so");
  gSystem->Load("libANALYSISalice.so");
  gSystem->Load("libCORRFW.so") ;
  gSystem->Load("libPWGHFbase.so");
  gSystem->Load("libPWGHFvertexingHF.so");
	
  TH1::AddDirectory(0);

  TFile* fd3 = new TFile("AnalysisfullemergeMC.root","READ");
  //TFile* fd3plus = new TFile("../DStarvsMult15-07/fullmerge/Analysisroot_fullmerge.root","READ");
 TString selectionStr;

  if (selection == 0){	
    selectionStr = "cOnly";
   } else {selectionStr = "bOnly";} 


  TDirectoryFile* dd3;
  AliCFContainer *datad3;
  if (selection == 0) {
     dd3 = (TDirectoryFile*)fd3->Get(Form("PWG3_D2H_CFtaskDstartoKpipi%s",cutString.Data())); //_weight_2040  _flatptw_2040
    datad3 = (AliCFContainer*) (dd3->Get(Form("CFHFccontainer0%s",cutString.Data())));
      if (!datad3) {cout << "PROMPT CONTAINER IN D3 NOT FOUND!! \n\n"; return;}
  } else {
     dd3 = (TDirectoryFile*)fd3->Get(Form("PWG3_D2H_CFtaskLctoK0SpCascadeKeepDfromBOnly%s",cutString.Data())); //_weight_2040  _flatptw_2040
    datad3 = (AliCFContainer*) (dd3->Get(Form("CFHFccontainer0DfromB%s",cutString.Data())));
      if (!datad3) {cout << "FEED CONTAINER IN D3 NOT FOUND!! \n\n"; return;}
}
/*  TDirectoryFile* dd3plus;
  AliCFContainer *datad3plus;
  
  if (selection == 0) {

     dd3plus = (TDirectoryFile*)fd3plus->Get(Form("PWG3_D2H_CFtaskLctoK0SpCascade%s",cutString.Data())); //_weight_2040  _flatptw_2040
    datad3plus = (AliCFContainer*) (dd3plus->Get(Form("CFHFccontainer0%s",cutString.Data())));

      if (!datad3plus) {cout << "PROMPT CONTAINER IN D3PLUS NOT FOUND!! \n\n"; return;}
  } else {
     dd3plus = (TDirectoryFile*)fd3plus->Get(Form("PWG3_D2H_CFtaskLctoK0SpCascadeKeepDfromBOnly%s",cutString.Data())); //_weight_2040  _flatptw_2040
      if (!dd3plus) {cout << "FEED DIRECTORY IN D3PLUS NOT FOUND!! \n\n"; return;}
    datad3plus = (AliCFContainer*) (dd3plus->Get(Form("CFHFccontainer0DfromB%s",cutString.Data())))->Clone("containerfast");
      if (!datad3plus) {cout << "FEED CONTAINER IN D3PLUS NOT FOUND!! \n\n"; return;}
}*/
//
cout << "INEREGE " << datad3->GetIntegral(2) << endl; //"     " << datad3plus->GetIntegral(2) << endl;

   AliCFContainer *data = (AliCFContainer*)datad3->Clone("datatest");
   //data->Add(datad3plus);


  data->Print("");
  data->PrintNBins();
  data->PrintBinLimits();

  Int_t stepNum=AliCFTaskVertexingHF::kStepRecoPID;
 
  //Int_t numer=AliCFTaskVertexingHF::kStepRecoPPR;
  Int_t stepDen=AliCFTaskVertexingHF::kStepAcceptance;

  const UInt_t ipTFast = 0;
  const UInt_t iyFast = 1;
  const UInt_t icTFast = 2;
  const UInt_t imultFast = 3;
       

  // define vars on which to make the slice
  
  //Int_t nvarSlice = 8;
  //Int_t* ivarSlice = new Int_t[nvarSlice];
  
  const Int_t nvarSlice = 8;
  Int_t ivarSlice[nvarSlice]={0};
  ivarSlice[0] = ipTFast;
  ivarSlice[1] = iyFast;
  ivarSlice[2] = icTFast;
  ivarSlice[3] = imultFast;

  Double_t mins[nvarSlice]={0.};
  Double_t maxs[nvarSlice]={0.};
  // pt
  mins[0] = 0.;
  maxs[0] = -1.;
  // y
  mins[1] = 0.;
  maxs[1] = -1.; 
  // ct
  mins[2] = 0.;
  maxs[2] = -1.; 
  // phi
  mins[3] = 0.;
  maxs[3] = -1.;
  // z
  mins[4] = 0.;
  maxs[4] = -1.; 
  // centrality
  mins[5] = 0.;
  maxs[5] = -1.;
  //fakes
  mins[6] = 0.;
  maxs[6] = -1.; 
  //mult
  mins[7] = 0.;
  maxs[7] = -1.;

  cout << "sliceVar:::: " << sliceVar << "  ; stepNum::: " << stepNum << endl;
  TAxis* axis = (TAxis*)data->GetAxis(sliceVar, stepNum);
   if (!axis) { cout << "no axis???\n"; return;}
  TCanvas *cEffReb = new TCanvas("EffReb","EffReb",1200,900);
  cEffReb->Divide(3,1);
  TCanvas *cEff = new TCanvas("Eff","Eff",1200,900);
  cEff->Divide(3,1);

  Float_t sumNumTot=0.;
  Float_t sumNumRebTot=0.;
  Float_t sumDenTot=0.;
  Float_t sumDenRebTot=0.;

  TH1D **h1num=new TH1D*[nVarSliceBins]; //numerator
  TH1D **h2num=new TH1D*[nVarSliceBins]; //numerator rebinned
  TH1D **h1=new TH1D*[nVarSliceBins]; //denominator
  TH1D **h2=new TH1D*[nVarSliceBins]; //denominator rebinned
  TH1D **heff=new TH1D*[nVarSliceBins]; //efficiency
  TH1D **heffreb=new TH1D*[nVarSliceBins]; //efficiency rebinned
  Int_t min, max; 
  for(Int_t iSlice=0; iSlice<nVarSliceBins; iSlice++){ // start loop on sliceVar bins

    min=axis->FindBin(varSliceLimits[iSlice]);
    Printf("MinBin = %d",min);
    max=axis->FindBin(varSliceLimits[iSlice+1])-1;
    Printf("MaxBin = %d",max);
    mins[sliceVar]=(Double_t)min;
    maxs[sliceVar]=(Double_t)max;
    Printf("\t Slicing on %s: minBin = %.0f edges = %.1f-%.1f\t maxBin = %.1f edges = %.1f-%.1f",data->GetVarTitle(sliceVar),mins[sliceVar],axis->GetBinLowEdge(min),axis->GetBinUpEdge(min),maxs[sliceVar],axis->GetBinLowEdge(max),axis->GetBinUpEdge(max)); 	

    TCanvas *cSum = new TCanvas(Form("Sum_%s_%.1f_%.1f",data->GetVarTitle(sliceVar),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)),Form("Sum_%s_%.1f_%.1f",data->GetVarTitle(sliceVar),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)),1200,900);
    cSum->Divide(2,1);
     

    AliCFContainer* slicedCont = data->MakeSlice(nvarSlice, ivarSlice, mins, maxs, true);

    // *********** NUMERATOR
    Printf("\nNUMERATOR");

    AliCFGridSparse* gridSparseNum = (AliCFGridSparse*)slicedCont->GetGrid(stepNum);
    THnSparse* numData = (THnSparse*)gridSparseNum->GetGrid();
    THnSparse* newnumData = (THnSparse*)numData->Clone("numNew");
    newnumData->Reset();
    TAxis* axisnum = (TAxis*)newnumData->GetAxis(rebinVar);
    axisnum->Set(nRebinVarBins,rebinVarLimits);
    newnumData->SetBinEdges(rebinVar,rebinVarLimits);
    newnumData->RebinnedAdd(numData, 1);

    // checking the bin contents
    
    h1num[iSlice] = (TH1D*)numData->Projection(rebinVar);
    h1num[iSlice]->SetNameTitle(Form("hnum_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)),Form("hnum_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)));
    h1num[iSlice]->SetMarkerStyle(27);
    h1num[iSlice]->SetMarkerColor(color[iSlice]);
    h1num[iSlice]->SetLineColor(color[iSlice]);
    cEff->cd(1);
    gPad->SetTicks();
    h1num[iSlice]->Draw("same hist");
    TH1D* hSumNum=(TH1D*)h1num[iSlice]->Clone("hSumNum");
    hSumNum->SetMarkerStyle(27);
    hSumNum->SetMarkerColor(color[iSlice]);
    hSumNum->SetLineColor(color[iSlice]);
    Float_t sumNum = 0.;
    Float_t sumNumnew = 0.;
    Printf(" Original THnSparse");
    for (Int_t ibin = 0; ibin<=h1num[iSlice]->GetNbinsX()+1; ibin++){
      Printf("ibin = %d, low edge = %f, content = %f",ibin,h1num[iSlice]->GetBinLowEdge(ibin),h1num[iSlice]->GetBinContent(ibin));
      sumNum+=h1num[iSlice]->GetBinContent(ibin);
      hSumNum->SetBinContent(ibin, sumNum);
      sumNumTot+=h1num[iSlice]->GetBinContent(ibin);
    }


    Printf(" THnSparse changing only one axis");
    h2num[iSlice] = (TH1D*)newnumData->Projection(rebinVar);
    h2num[iSlice]->SetNameTitle(Form("hnumReb_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)),Form("hnumReb_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)));
    h2num[iSlice]->SetMarkerStyle(20);
    h2num[iSlice]->SetMarkerColor(color[iSlice]);
    h2num[iSlice]->SetLineColor(color[iSlice]);
    cEffReb->cd(1);
    gPad->SetTicks();
    h2num[iSlice]->Draw("same hist");
    TH1D* hSumNumReb=(TH1D*)h2num[iSlice]->Clone("hSumNumReb");
    hSumNumReb->SetMarkerStyle(24);
    hSumNumReb->SetMarkerColor(color[iSlice]);
    hSumNumReb->SetLineColor(color[iSlice]);
    for (Int_t ibin = 0; ibin<=h2num[iSlice]->GetNbinsX()+1; ibin++){
      Printf("ibin = %d, low edge = %f, content = %f",ibin,h2num[iSlice]->GetBinLowEdge(ibin),h2num[iSlice]->GetBinContent(ibin));
      sumNumnew+=h2num[iSlice]->GetBinContent(ibin);
      hSumNumReb->SetBinContent(ibin,sumNumnew);
      sumNumRebTot+=h2num[iSlice]->GetBinContent(ibin);
    }

    Printf("sum = %f, sumnew = %f",sumNum, sumNumnew);

    cSum->cd(1);
    gPad->SetTicks();
    gPad->SetGridx();
    hSumNum->Draw();
    hSumNumReb->Draw("same");

    // *********** DENOMINATOR RECPID

    Printf("\nDENOMINATOR");

    AliCFGridSparse* gridSparseDen = (AliCFGridSparse*)slicedCont->GetGrid(stepDen);
    THnSparse* denData = (THnSparse*)gridSparseDen->GetGrid();
    
    THnSparse* newdenData = (THnSparse*)denData->Clone("denNew");
    newdenData->Reset();
    TAxis* axisden = (TAxis*)newdenData->GetAxis(rebinVar);
    axisden->Set(nRebinVarBins,rebinVarLimits);
    newdenData->SetBinEdges(rebinVar,rebinVarLimits);
    newdenData->RebinnedAdd(denData, 1);

    // checking the bin contents
    h1[iSlice] = (TH1D*)denData->Projection(rebinVar);
    h1[iSlice]->SetNameTitle(Form("hden_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)),Form("hden_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)));
    cEff->cd(2);
    gPad->SetTicks();
    h1[iSlice]->SetMarkerStyle(27);
    h1[iSlice]->SetMarkerColor(color[iSlice]);
    h1[iSlice]->SetLineColor(color[iSlice]);
    h1[iSlice]->Draw("same hist");
    TH1D* hSumDen=(TH1D*)h1[iSlice]->Clone("hSumDen");
    hSumDen->SetMarkerStyle(27);
    hSumDen->SetMarkerColor(color[iSlice]);
    hSumDen->SetLineColor(color[iSlice]);
    Float_t sumDen = 0.;
    Float_t sumDennew = 0.;
    Printf(" Original THnSparse");
    for (Int_t ibin = 0; ibin<=h1[iSlice]->GetNbinsX()+1; ibin++){
      Printf("ibin = %d, low edge = %f, content = %f",ibin,h1[iSlice]->GetBinLowEdge(ibin),h1[iSlice]->GetBinContent(ibin));
      sumDen+=h1[iSlice]->GetBinContent(ibin);
      hSumDen->SetBinContent(ibin,sumDen);
      sumDenTot+=h1[iSlice]->GetBinContent(ibin);
    }


    Printf(" THnSparse changing only one axis");
    h2[iSlice] =(TH1D*)newdenData->Projection(rebinVar);
    h2[iSlice]->SetNameTitle(Form("hdenReb_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)),Form("hdenReb_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)));
    h2[iSlice]->SetMarkerStyle(20);
    h2[iSlice]->SetMarkerColor(color[iSlice]);
    h2[iSlice]->SetLineColor(color[iSlice]);
    cEffReb->cd(2);
    gPad->SetTicks();
    h2[iSlice]->Draw("same hist");
    TH1D* hSumDenReb=(TH1D*)h2[iSlice]->Clone("hSumDenReb");
    hSumDenReb->SetMarkerStyle(24);
    hSumDenReb->SetMarkerColor(color[iSlice]);
    hSumDenReb->SetLineColor(color[iSlice]);
    for (Int_t ibin = 0; ibin<=h2[iSlice]->GetNbinsX()+1; ibin++){
      Printf("ibin = %d, low edge = %f, content = %f",ibin,h2[iSlice]->GetBinLowEdge(ibin),h2[iSlice]->GetBinContent(ibin));
      sumDennew+=h2[iSlice]->GetBinContent(ibin);
      hSumDenReb->SetBinContent(ibin,sumDennew);
      sumDenRebTot+=h2[iSlice]->GetBinContent(ibin);
    }

    Printf("sum = %f, sumnew = %f",sumDen, sumDennew);

    cSum->cd(2);
    gPad->SetTicks();
    gPad->SetGridx();
    hSumDen->Draw();
    hSumDenReb->Draw("same");

    heff[iSlice] = (TH1D*)h1num[iSlice]->Clone(Form("heff_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)));
    heff[iSlice]->SetMarkerStyle(20);
    heff[iSlice]->SetMarkerColor(color[iSlice]);
    heff[iSlice]->SetLineColor(color[iSlice]);
    heff[iSlice]->Sumw2();
    heff[iSlice]->Divide(h1num[iSlice],h1[iSlice],1,1,"B");
    heff[iSlice]->GetXaxis()->SetRange(mins[rebinVar],maxs[rebinVar]);
    heff[iSlice]->GetYaxis()->SetRangeUser(0.0,0.8);
    cEff->cd(3);
    gPad->SetTicks();
    heff[iSlice]->Draw("same");

    heffreb[iSlice] = (TH1D*)h2num[iSlice]->Clone(Form("heffReb_%s_%.0f-%.0f",data->GetStepTitle(stepNum),axis->GetBinLowEdge(min),axis->GetBinUpEdge(max)));
    heffreb[iSlice]->SetMarkerStyle(20);
    heffreb[iSlice]->SetMarkerColor(color[iSlice]);
    heffreb[iSlice]->SetLineColor(color[iSlice]);
    heffreb[iSlice]->Sumw2();
    heffreb[iSlice]->Divide(h2num[iSlice],h2[iSlice],1,1,"B");
    heffreb[iSlice]->GetXaxis()->SetRange(mins[rebinVar],maxs[rebinVar]);
    heffreb[iSlice]->GetYaxis()->SetRangeUser(0.0,0.8);
    cEffReb->cd(3);
    gPad->SetTicks();
    heffreb[iSlice]->Draw("same");


    TFile* fout = new TFile(Form("FDefficiencyfine_%s_over_%s_MultInt_%s_%s_%.0f_%.0f.root",data->GetStepTitle(stepNum),data->GetStepTitle(stepDen),selectionStr.Data(),cutString.Data(),varSliceLimits[iSlice],varSliceLimits[iSlice+1]),"RECREATE");
    heffreb[iSlice]->Write(Form("hpteffCF%s%s",data->GetStepTitle(stepNum),data->GetStepTitle(stepDen)));
    fout->Close();

  } // end loop on sliceVar bins

  TH1D *hSumNumRebTot = (TH1D*)h2num[0]->Clone("hSumNumRebTot");
  hSumNumRebTot->SetMarkerStyle(27);
  hSumNumRebTot->SetMarkerColor(kBlue);
  hSumNumRebTot->SetLineColor(kBlue);
  TH1D *hSumDenRebTot = (TH1D*)h2[0]->Clone("hSumDenRebTot");
  hSumDenRebTot->SetMarkerStyle(27);
  hSumDenRebTot->SetMarkerColor(kBlue);
  hSumDenRebTot->SetLineColor(kBlue);
  for(Int_t i=1; i<nVarSliceBins; i++){
    hSumNumRebTot->Add(h2num[i]);
    hSumDenRebTot->Add(h2[i]);
  }
  Float_t sumnumAddReb=0.;
  Float_t sumdenAddReb=0.;
  Printf(" Bin Content Numerator from Add Rebin");
  for (Int_t ibin = 0; ibin<=hSumNumRebTot->GetNbinsX()+1; ibin++){
    Printf("ibin = %d, low edge = %f, content = %f",ibin,hSumNumRebTot->GetBinLowEdge(ibin),hSumNumRebTot->GetBinContent(ibin));
    sumnumAddReb+=hSumNumRebTot->GetBinContent(ibin);
  }
  Printf(" Bin Content Denominator from Add Rebin");
  for (Int_t ibin = 0; ibin<=hSumDenRebTot->GetNbinsX()+1; ibin++){
    Printf("ibin = %d, low edge = %f, content = %f",ibin,hSumDenRebTot->GetBinLowEdge(ibin),hSumDenRebTot->GetBinContent(ibin));
    sumdenAddReb+=hSumDenRebTot->GetBinContent(ibin);
  }

  TH1D *hSumNumTot = (TH1D*)h1num[0]->Clone("hSumNumTot");
  hSumNumTot->SetMarkerStyle(24);
  hSumNumTot->SetMarkerColor(kRed);
  hSumNumTot->SetLineColor(kRed);
  TH1D *hSumDenTot = (TH1D*)h1[0]->Clone("hSumDenTot");
  hSumDenTot->SetMarkerStyle(24);
  hSumDenTot->SetMarkerColor(kRed);
  hSumDenTot->SetLineColor(kRed);
  for(Int_t i=1; i<nVarSliceBins; i++){
    hSumNumTot->Add(h1num[i]);
    hSumDenTot->Add(h1[i]);
  }
  Float_t sumnumAdd=0.;
  Float_t sumdenAdd=0.;
  Printf(" Bin Content Numerator from Add");
  for (Int_t ibin = 0; ibin<=hSumNumTot->GetNbinsX()+1; ibin++){
    Printf("ibin = %d, low edge = %f, content = %f",ibin,hSumNumTot->GetBinLowEdge(ibin),hSumNumTot->GetBinContent(ibin));
    sumnumAdd+=hSumNumTot->GetBinContent(ibin);
  }
  Printf(" Bin Content Denominator from Add");
  for (Int_t ibin = 0; ibin<=hSumDenTot->GetNbinsX()+1; ibin++){
    Printf("ibin = %d, low edge = %f, content = %f",ibin,hSumDenTot->GetBinLowEdge(ibin),hSumDenTot->GetBinContent(ibin));
    sumdenAdd+=hSumDenTot->GetBinContent(ibin);
  }

  Float_t sumnum=0.;
  TH1D *hNum=(TH1D*)data->Project(stepNum, rebinVar);
  hNum->SetNameTitle(Form("hNum_NoSlice_%s",data->GetVarTitle(rebinVar)),Form("hNum_NoSlice_%s",data->GetVarTitle(rebinVar)));
  hNum->SetMarkerColor(kGreen+1);
  hNum->SetMarkerStyle(25);
  hNum->SetLineColor(kGreen+1);
  Printf(" Bin Content Numerator from Projection");
  for (Int_t ibin = 0; ibin<=hNum->GetNbinsX()+1; ibin++){
    Printf("ibin = %d, low edge = %f, content = %f",ibin,hNum->GetBinLowEdge(ibin),hNum->GetBinContent(ibin));
    sumnum+=hNum->GetBinContent(ibin);
  }
  Float_t sumden=0.;
  TH1D *hDen=(TH1D*)data->Project(stepDen, rebinVar);
  hDen->SetNameTitle(Form("hDen_NoSlice_%s",data->GetVarTitle(rebinVar)),Form("hDen_NoSlice_%s",data->GetVarTitle(rebinVar)));
  hDen->SetMarkerColor(kGreen+1);
  hDen->SetMarkerStyle(25);
  hDen->SetLineColor(kGreen+1);
  Printf(" Bin Content Denominator from Projection");
  for (Int_t ibin = 0; ibin<=hDen->GetNbinsX()+1; ibin++){
    Printf("ibin = %d, low edge = %f, content = %f",ibin,hDen->GetBinLowEdge(ibin),hDen->GetBinContent(ibin));
    sumden+=hDen->GetBinContent(ibin);
  }
  
  Printf("");
  Printf("sum num from Add = %f\t sum num from Projection = %f\t sum num from Add rebin = %f",sumnumAdd, sumnum, sumnumAddReb);
  Printf("sum den from Add = %f\t sum den from Projection = %f\t sum den from Add rebin = %f",sumdenAdd, sumden, sumdenAddReb);

  TH1D *hEffTot=(TH1D*)hSumNumTot->Clone("hEffFromSumTot");
  hEffTot->Divide(hSumDenTot);
  hEffTot->SetMarkerColor(kRed);
  hEffTot->SetLineColor(kRed);
  hEffTot->SetLineStyle(2);
  hEffTot->SetMarkerStyle(24);
  TH1D *hEffRebTot=(TH1D*)hSumNumRebTot->Clone("hEffFromSumTotReb");
  hEffRebTot->Divide(hSumDenRebTot);
  hEffRebTot->SetMarkerColor(kBlue);
  hEffRebTot->SetLineColor(kBlue);
  hEffRebTot->SetMarkerStyle(27);

  AliCFEffGrid *eff = new AliCFEffGrid("eff"," The efficiency",*data);
  eff->CalculateEfficiency(stepNum,stepDen);
  TH1D *hEff=(TH1D*)eff->Project(rebinVar);
  hEff->SetNameTitle(Form("hEff_NoSliceProject_%s",data->GetVarTitle(rebinVar)),Form("hEff_NoSliceProject_%s",data->GetVarTitle(rebinVar)));
  hEff->SetLineColor(kGreen+1);


  TCanvas *cSumTot = new TCanvas("SumTot","SumTot",1200,900);
  cSumTot->Divide(3,1);
  cSumTot->cd(1);
  gPad->SetTicks();
  gPad->SetGridx();
  hSumNumRebTot->Draw();
  hSumNumTot->Draw("same");
  hNum->Draw("same hist");
  cSumTot->cd(2);
  gPad->SetTicks();
  gPad->SetGridx();
  hSumDenRebTot->Draw();
  hSumDenTot->Draw("same");
  hDen->Draw("same hist");
  cSumTot->cd(3);
  gPad->SetTicks();
  gPad->SetGridx();
  hEff->Draw("hist");
  hEffTot->Draw("same hist");
  hEffRebTot->Draw("same");
  
//  TH1D *hEff_multInt = (TH1D*)eff->Project(sliceVar);
//  hEff_multInt->SetNameTitle("hEff_multInt","hEff_multInt");
//  TFile *fmultInt = new TFile(Form("efficiency_multInt_RecoPID_over_MCAcc_%s_%s.root",cutString.Data(),selectionStr.Data()),"recreate");
//  fmultInt->cd();
//  hEff_multInt->Write();


  /**********************************
   *         Clean up memory        *
   *     To prevent kernel panic... *
   *        We can only hope.       *
   **********************************/

   gDirectory->GetList()->Delete();
   fd3->Close();
//   fd3plus->Close();

}
