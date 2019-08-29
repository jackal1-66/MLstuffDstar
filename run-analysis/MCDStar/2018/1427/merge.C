void merge() {

  TGrid::Connect("alien://");
  TFileMerger *f = new TFileMerger();
  f->OutputFile("AnalysisResults_merged.root");
  f->AddFile("alien:///alice/cern.ch/user/a/alitrain/PWGHF/D2H_pp_MC/1427_20190710-1131/merge_runlist_1/AnalysisResults.root");
  f->AddFile("alien:///alice/cern.ch/user/a/alitrain/PWGHF/D2H_pp_MC/1427_20190710-1131/merge_runlist_2/AnalysisResults.root");
  f->Merge();

 }
