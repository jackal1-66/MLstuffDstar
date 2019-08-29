void macro(){
    TFile * a = new TFile("RawYieldBoth.root");
    TFile * b = new TFile("RawYieldBothMC.root");
    TH1D * th = (TH1D*)a->Get("hSigma");
    TH1D * thMC = (TH1D*)b->Get("hSigma");
    TCanvas * c1 = new TCanvas("c1", "hSigmadiff",1);
    c1->SetLeftMargin(0.15);
    c1->SetGrid();
    th->SetMarkerStyle(20);
    th->SetMarkerColor(2);
    thMC->SetMarkerStyle(21);
    thMC->SetMarkerColor(4);
    th->SetStats(0);
    thMC->SetStats(0);
    th->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    th->GetYaxis()->SetTitle("Sigma [GeV/c^{2}]");
    thMC->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    thMC->GetYaxis()->SetTitle("Sigma [GeV/c^{2}]");
    th->Draw();
    thMC->Draw("SAME");
    TLegend * leg = new TLegend(0.25,0.7,0.63,0.9);
    leg->AddEntry(th,"Sigma from Data", "p");
    leg->AddEntry(thMC,"Sigma from MC", "p");
    leg->Draw();
    c1->Print("comparison.png");
}
