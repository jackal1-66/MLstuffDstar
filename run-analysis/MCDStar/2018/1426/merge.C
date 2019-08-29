void merge() {

  TGrid::Connect("alien://");
  TFileMerger *f = new TFileMerger();
  f->OutputFile("AnalysisResults_merged.root");
  f->AddFile("");
  f->AddFile("");
  f->Merge();

 }
