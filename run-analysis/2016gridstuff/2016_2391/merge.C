void merge() {

  TGrid::Connect("alien://");
  TFileMerger *f = new TFileMerger();
  f->OutputFile("AnalysisResults_merged.root");
  std::ifstream file("totallist.txt");
  std::string str;
  TString rootstr;
  while (std::getline(file,str))
	{
          rootstr = str;
          rootstr.Prepend("alien://");
	  f->AddFile(rootstr.Data());
}
  f->Merge();

}
