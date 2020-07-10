void Analise(TString fname){
	
	Int_t counts, countsNoCoincidence, totalCounts;
	Int_t time;
	Int_t runNumber;
	TString runString=fname;

        //First I get the run number
        TObjArray *texto = runString.Tokenize("_"); //Split each time that "_" appear
        TObjString *ObjStr=(TObjString *)texto->At(3);//I get the last part (0, 1, 2, 3) -> 1.root
        TString run=ObjStr->GetString();
        run.ReplaceAll(".root","");//I remove ".root" so I get just the run number, that's a integer.
	runNumber=run.Atoi();

        //I read the file
	TFile *file=new TFile(fname,"READ");

        //I get the TritiumTree and runCondition Ntuples. I also get some parameters.
        TTree *tree=(TTree*)file->Get("TritiumTree");
	TTree *runConditions=(TTree*)file->Get("RunConditions");
	Int_t acquisitionTime_s = runConditions->GetMaximum("AcquisitionTime_s");
	Int_t nEvents=runConditions->GetMaximum("NEventsGenerated");

        //I create the name of the small file
	TString newName=fname;
	newName.ReplaceAll(".root","_small.root");

        //I open the newFile with a new TTree
	TFile *newFile=new TFile(newName,"RECREATE");
	TTree *newTree;
        TString treeName;
        treeName="countsData_small";
        newTree=new TTree(treeName,treeName);

        //Create the interesting branch
	newTree->Branch("countsCoincidence",&counts,"countsCoincidence/I");
	newTree->Branch("countsNoCoincidence",&countsNoCoincidence,"countsNoCoincidence/I");
	newTree->Branch("totalInteractions",&totalCounts,"totalInteractions/I");	
	newTree->Branch("time",&time,"time/I");
	newTree->Branch("runNumber",&runNumber,"runNumber/I");
	newTree->Branch("nEventsGenerated",&nEvents,"nEventsGenerated/I");
	totalCounts=tree->GetEntries();
	counts=tree->GetEntries("Coincidence==1");
	countsNoCoincidence=tree->GetEntries("Coincidence==0");
        time=runNumber*acquisitionTime_s;

        //Fill, write and close files.
	newTree->Fill();
	newTree->Write();
	file->Close();
	newFile->Close();
	exit(0);
	
}
