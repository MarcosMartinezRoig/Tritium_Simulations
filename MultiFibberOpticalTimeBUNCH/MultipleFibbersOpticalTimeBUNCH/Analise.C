void Analise(TString fname){
	
	Int_t counts, countsNoCoincidence, totalCounts;
	Int_t time;
	Int_t runNumber;
	TString runString=fname;

	
	TObjArray *texto = runString.Tokenize("_");
    TObjString *ObjStr=(TObjString *)texto->At(3);
     TString run=ObjStr->GetString();
	run.ReplaceAll(".root","");
	runNumber=run.Atoi();
	TFile *file=new TFile(fname,"READ");
	TTree *tree=(TTree*)file->Get("TritiumTree");
	
	TTree *runConditions=(TTree*)file->Get("RunConditions");
	Int_t acquisitionTime_s = runConditions->GetMaximum("AcquisitionTime_s");
	Int_t nEvents=runConditions->GetMaximum("NEventsGenerated");
	TString newName=fname;
	newName.ReplaceAll(".root","_small.root");
	TFile *newFile=new TFile(newName,"RECREATE");
	TTree *newTree;
	TString treeName;

	treeName="countsData_small";
	newTree=new TTree(treeName,treeName);
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
	newTree->Fill();
	newTree->Write();
	file->Close();
	newFile->Close();
	exit(0);
	
}
