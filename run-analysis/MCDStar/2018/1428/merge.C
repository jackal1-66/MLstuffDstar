#include <string>

void merge() {

  TGrid::Connect("alien://");
  TFileMerger *f = new TFileMerger();
  f->OutputFile("AnalysisResults_merged.root");
  TString rootstr;
  for(int i=2;i<17;i++){
    cout << "\nDoing cycle " <<i << '\n';
    rootstr = "alien:///alice/cern.ch/user/a/alitrain/PWGHF/D2H_pp_MC/1428_20190711-0931/merge_runlist_"+std::to_string(i)+"/AnalysisResults.root";
    f->AddFile(rootstr.Data());  
  }
  f->Merge();

 }
