TTree* SortTree(TTree *tree, TFile *file){

	Int_t nentries = (Int_t)tree->GetEntries();
	tree->Draw("time","","goff");
	Int_t *index = new Int_t[nentries];
	TMath::Sort(nentries,tree->GetV1(),index,false);
	file->cd();
	//TFile f2("hsimple_sorted.root","recreate");
	TTree *tsorted = (TTree*)tree->CloneTree(0);
	for (Int_t i=0;i<nentries;i++) {
		tree->GetEntry(index[i]);
		tsorted->Fill();
	}
	tsorted->Write();
	delete [] index;
return tsorted;

}

void MakeSingleGraphs(TString fnameData){
	
	Int_t nEntries;
	TString fname= fnameData;
	TFile *graphsFile=new TFile(fnameData.ReplaceAll("Small","Single"),"RECREATE");
        Int_t nPoints=6;
	Int_t time,counts, activity;
	Int_t countsIntegrated;
	TString graphName, activityString;

	TString graphString;
	TLegend *leg = new TLegend (0.75, 0.75, 0.85, 0.88, "Activity", "brNDC");
	cout << fname.Data() << endl;
	TFile *ficheiro =new TFile(fname.Data(),"READ");
	TTree *dataUnsorted;
	TGraph *graphs;
	TTree *integrationData;
	TGraph *graphsIntegration;
	
	TMultiGraph *multi;
	

	
	multi=new TMultiGraph();
	multi->SetName("MultiGraph");
	multi->SetTitle("MultiGraph");
	dataUnsorted=(TTree*)ficheiro->Get("countsData_small");
	TTree* data=SortTree(dataUnsorted,graphsFile);
	nEntries=data->GetEntries();

        cout << "nEntries: " << nEntries << endl;

	data->SetBranchAddress("countsCoincidence",&counts);
	data->SetBranchAddress("time",&time);
	auto activityBranch = data->Branch("activity", &activity, "activity/I");
	

	data->Draw("countsCoincidence:time","","goff");
	graphs=new TGraph(nEntries, data->GetV2 (),data->GetV1 ());
	graphString=fname;
	graphString.ReplaceAll("AllDataSmall","");
	graphString.ReplaceAll(".root","");

	cout<<graphString<<endl;
	graphs->SetName(graphString);
	graphs->SetTitle(graphString);
	integrationData=new TTree ("integrationTree","integrationTree");
	integrationData->Branch("countsIntegrated",&countsIntegrated,"countsIntegrated/I");
	integrationData->Branch("time",&time,"time/I");
	integrationData->Branch("activity", &activity, "activity/I");
	
	leg->AddEntry(graphs,graphString);
	multi->Add(graphs,"L");
	graphsFile->cd();
	graphs->Write();
	
	
	activityString=fname;
	activityString.ReplaceAll("AllDataSmall","");
	activityString.ReplaceAll("BqL.root","");
	
	activity=activityString.Atoi();
	
	//MAKE integrationS
	graphName=graphs->GetTitle();
		
	countsIntegrated=0;
	
	for (Long64_t i=0;i<nEntries;i++){
	
		data->GetEntry(i);
		activityBranch->Fill();//Add data to new branch
				////60min integration
		if ((i%nPoints)==0 && i!=0){ // % means the rest od division
			integrationData->Fill();
			countsIntegrated=0;
		}
		
		else {countsIntegrated=countsIntegrated+counts;}
		
		
/////END INTEGRATION
	

	}
	integrationData->Draw("countsIntegrated:time","","goff");
	graphsIntegration=new TGraph(integrationData->GetEntries(), integrationData->GetV2 (),integrationData->GetV1 ());
	graphsIntegration->SetName(graphName+"_integration");
	graphsIntegration->SetTitle(graphName+"_integration");
	

	leg->AddEntry(graphsIntegration,graphName+"_integration");
	graphsIntegration->SetLineColor(kRed);
	multi->Add(graphsIntegration,"L");
	graphsFile->cd();
	graphsIntegration->Write();
	//TCanvas *canvas = new TCanvas();
	multi->Draw("ALP");
	multi->GetXaxis()->SetTimeDisplay(1);
	multi->GetXaxis()->SetTimeFormat("%d");
	multi->GetXaxis()->SetTitle("Time (Day)");
	leg->Draw();
	multi->Write();	
	integrationData->Write();
	data->Write("", TObject::kOverwrite); // save only the new version of the tree
	delete data;
	graphsFile->Close();
	delete ficheiro;
	delete graphsFile;

}
