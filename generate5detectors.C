void generate5detectors(TString filename){
	
	Int_t detector_1;
	Int_t detector_2;
	Int_t detector_3;
	Int_t detector_4;
	Int_t detector_5;
	Int_t sum_5detectors;
	Int_t time;
	Int_t runNumber;
	
	
	
	TString newFileName=filename;
	newFileName.ReplaceAll("AllDataSmall","AllDataSmall_5detectors_");
	TFile *oldFile=new TFile(filename,"READ");
	TFile *newFile=new TFile(newFileName,"RECREATE");
	TTree *newTree=new TTree("countsData_5detectors_small","countsData_5detectors_small");
	newTree->Branch("detector_1",&detector_1,"detector_1/I");
	newTree->Branch("detector_2",&detector_2,"detector_2/I");
	newTree->Branch("detector_3",&detector_3,"detector_3/I");
	newTree->Branch("detector_4",&detector_4,"detector_4/I");
	newTree->Branch("detector_5",&detector_4,"detector_5/I");
	newTree->Branch("sum_5detectors",&sum_5detectors,"sum_5detectors/I");
	newTree->Branch("time",&time,"time/I");
	newTree->Branch("runNumber",&runNumber,"runNumber/I");
	
	
	TTree *oldTree=(TTree*)oldFile->Get("countsData_small");
	Int_t max=oldTree->GetMaximum("countsCoincidence");
	Int_t min=oldTree->GetMinimum("countsCoincidence");
	TH1I *histo=new TH1I("histoCounts","histoCounts", max-min,min,max);
	oldTree->Draw("countsCoincidence>>histoCounts","","goff");
	
	for (Int_t i=0;i<10100;i++){
		
		runNumber=i;
		time=i*60;
		detector_1=histo->GetRandom();
		detector_2=histo->GetRandom();
		detector_3=histo->GetRandom();
		detector_4=histo->GetRandom();
		detector_5=histo->GetRandom();
		sum_5detectors=detector_1+detector_2+detector_3+detector_4+detector_5;
		//cout<<i<<" "<<detector_1<<" "<<detector_2<<" "<<detector_3<<" "<<detector_4<<" "<<sum_5detectors<<endl;
		newTree->Fill();
		
		
		}
	
	newTree->Write();
	oldFile->Close();
	newFile->Close();
	
	
	}
