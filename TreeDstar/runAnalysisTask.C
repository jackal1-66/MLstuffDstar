#if !defined (__CINT__) || defined (__CLING__)
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"
#include "AliAODInputHandler.h"
#include "AliPhysicsSelectionTask.h"
#include "AliMultSelectionTask.h"
#include "AliTaskCDBconnect.h"
#include "AliAnalysisTaskSEImpParResSparse.h"
#include "AliAnalysisTaskPIDResponse.h"
#include "AliAnalysisTaskSEImproveITS.h"
#include "AliAnalysisTaskSEDStarSpectra.h"
#include "AliAnalysisTaskSEHFTreeCreator.h"
#include "TInterpreter.h"
#endif

/*
#ifdef __CLING__
// Tell  ROOT where to find AliRoot headers
//R__ADD_INCLUDE_PATH($ALICE_ROOT)
//#include <ANALYSIS/macros/train/AddESDHandler.C>

// Tell ROOT where to find AliPhysics headers
R__ADD_INCLUDE_PATH($ALICE_PHYSICS)
#include <OADB/macros/AddTaskPhysicsSelection.C>
#include <PWGPP/PilotTrain/AddTaskCDBconnect.C>
#include <PWGPP/macros/AddTaskImpParResSparse.C>
#endif
*/
void runAnalysis()
{
    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = kFALSE;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = kTRUE;
    
    TGrid::Connect("alien://");
    
    // since we will compile a class, tell root where to look for headers
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT->ProcessLine(".include $ROOTSYS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif
    
    // create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTaskExample");
    AliAODInputHandler *aodH = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);
    
    //AliMCEventHandler  *mcH = new AliMCEventHandler();
    //mgr->SetMCtruthEventHandler(mcH);
    
    
    
    // compile the class and load the add task macro
    // here we have to differentiate between using the just-in-time compiler
    // from root6, or the interpreter of root5
    
    //AliPhysicsSelectionTask *physseltask = reinterpret_cast<AliPhysicsSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C"),kFALSE)));
    
    //AliMultSelectionTask *multSelTask = reinterpret_cast<AliMultSelectionTask *>(gInterpreter->ProcessLine(Form(".x %s", gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"))));
    
   // AliAnalysisTaskPIDResponse *pidresp = reinterpret_cast<AliAnalysisTaskPIDResponse *>(gInterpreter->ProcessLine(Form(".x %s (%d)", gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"),kFALSE)));
   
   // AliAnalysisTaskSEDStarSpectra *taskDstar = reinterpret_cast<AliAnalysisTaskSEDStarSpectra *>(gInterpreter->ProcessLine(Form(".x %s (%d, %d, %d, \"%s\", \"%s\")", gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskDStarSpectra.C"),0,0,100,"DStarCuts.root","mgiacaloDstar")));
 
    AliAnalysisTaskSEHFTreeCreator * tasktree = reinterpret_cast<AliAnalysisTaskSEHFTreeCreator *>(gInterpreter->ProcessLine(Form(".x %s (%s, \"%s\", %d, %d)", gSystem->ExpandPathName("$ALICE_PHYSICS/PWGHF/treeHF/AliAnalysisTaskSEDStarSpectra.cxx"),"DstarTreeCreator_mgiacalo","D0DsDplusDstarLcBplusBsLbCuts_pp.root",100,"DStarCuts.root","mgiacaloDstar",2,2)));

    //AliTaskCDBconnect *CDBconnect= reinterpret_cast<AliTaskCDBconnect*>(gInterpreter->ProcessLine(Form(".x %s (\"%s\")",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C"),"raw://")));
    
    
//     AliAnalysisTaskSEImpParResSparse *task0 = reinterpret_cast<AliAnalysisTaskSEImpParResSparse*>(gInterpreter->ProcessLine(Form(".x %s (%d,%d,%d,%d,%d,%d,%d,\"%s\",%d)",gSystem->ExpandPathName("$ALICE_PHYSICS/PWGPP/macros/AddTaskImpParResSparse.C"),kFALSE,-1,kTRUE,kFALSE,0,1000000,0,"0",kTRUE)));
//    task0->SelectCollisionCandidates(AliVEvent::kINT7);
    

    
    
    
    if(!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(2);
    mgr->PrintStatus();
    mgr->SetUseProgressBar(1, 25);
    
    if(local) {
        // if you want to run locally, we need to define some input
        TChain* chain = new TChain("aodTree");
        // add a few files to the chain (change this so that your local files are added)
        chain->Add("AliAOD.root");
        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chain);
    } else {
        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();
        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
        // make sure your source files get copied to grid
        //alienHandler->SetAdditionalLibs("AliAnalysisTaskSEBasic.cxx AliAnalysisTaskSEBasic.h");
        //alienHandler->SetAnalysisSource("AliAnalysisTaskSEBasic.cxx");
        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        
       alienHandler->SetUser("mgiacalo");
       alienHandler->SetAliPhysicsVersion("vAN-20190828_ROOT6-1");
        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");
        // select the input data
        alienHandler->SetGridDataDir("/alice/data/2018/LHC18l/");
        alienHandler->SetDataPattern("/AOD208/*AliAOD.root");
        alienHandler->SetFriendChainName("./AliAOD.treeHF.root");
        // MC has no prefix, data has prefix 000
        alienHandler->SetRunPrefix("000");
        // runnumber
        const Int_t nruns=10;
        Int_t runlist[nruns]={289971, 289966, 289965, 289943, 289941, 289940, 289935, 289931, 289928, 289884 };//, 282099, 282118, 282119, 282120, 282122, 282123, 282125, 282126, 282127, 282146, 282147, 282189, 282206, 282224, 282227, 282229, 282230, 282247, 282302, 282303, 282304, 282305, 282306, 282307, 282309, 282312, 282313, 282314, 282340, 282341, 282342, 282343, 282365, 282366, 282367};
        for(Int_t k=0;k<nruns;k++){
            alienHandler->AddRunNumber(runlist[k]);
        }
        alienHandler->SetNrunsPerMaster(nruns);
        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(4);
        alienHandler->SetExecutable("myTask_treetestDstar_290819.sh");
        // specify how many seconds your job may take
        alienHandler->SetTTL(70000);
        alienHandler->SetJDLName("myTask_treetestDstar_290819.jdl");
        
        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);
        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate")
        // (see below) mode, set SetMergeViaJDL(kFALSE)
        // to collect final results
        alienHandler->SetMaxMergeStages(1);
        alienHandler->SetMergeViaJDL(kTRUE);
        
        // define the output folders
        alienHandler->SetGridWorkingDir("treetestDstar_290819");
        alienHandler->SetGridOutputDir("myOutputDir");
        
        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);
        if(gridTest) {
            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        } else {
            // else launch the full grid analysis
            alienHandler->SetRunMode("run");
            mgr->StartAnalysis("grid");
        }
    }
}
