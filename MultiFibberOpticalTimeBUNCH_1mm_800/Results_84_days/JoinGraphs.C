
Int_t GetNumberOfFiles(TList *files){

	Int_t counter=0;
	TSystemFile *file;
		TString fname;
		TIter next(files);
		while ((file=(TSystemFile*)next())) {
			fname = file->GetName();
			if (!file->IsDirectory() && fname.BeginsWith("AllDataSingle") && fname.EndsWith(".root")) {
				counter++;
			}
		}
		
	cout<<counter<<" files"<<endl;	
	return counter;
}


Int_t *SortActivity(TList *files, const Int_t nFiles){
	
	
	Int_t *arrActive=new Int_t[nFiles];
	Int_t *sortedActive=new Int_t[nFiles];
	Int_t idx[nFiles];
	Int_t counter=0;
	TSystemFile *file;
	TString fname;
	TIter next(files);
	while ((file=(TSystemFile*)next())) {
			fname = file->GetName();
			if (!file->IsDirectory() && fname.BeginsWith("AllDataSingle") && fname.EndsWith(".root")) {
				TString activString=fname;
				activString.ReplaceAll("AllDataSingle","");
				activString.ReplaceAll("BqL.root","");
							
				arrActive[counter]=(Int_t)activString.Atoi();
				counter++;
				
			}
		}
		
		
	TMath::Sort(nFiles,arrActive, idx,false);	
	
	
	for(Int_t j=0;j<nFiles;j++) {
		
		sortedActive[j]=arrActive[idx[j]];
		
		}
	
	return sortedActive;
	
	}



void JoinGraphs(){
	
	Int_t nEntries;
	
	Int_t counter=0;
	TMultiGraph *multi=new TMultiGraph();
	multi->SetName("MultiGraph_RawData");
	multi->SetTitle("RawData");
	
	TMultiGraph *multiIntegrated=new TMultiGraph();
	multiIntegrated->SetName("MultiGraph_Integration");
	multiIntegrated->SetTitle("60min integration");
	
	TSystemDirectory dir(gSystem->pwd(), gSystem->pwd());
	TLegend *leg = new TLegend (0.88,0.48,0.96,0.98);
	leg->SetHeader("Activity (Bq/L)", "C");
	TLegend *leg2 = new TLegend (0.88,0.48,0.96,0.98);
        leg2->SetHeader("Activity (Bq/L)", "C");

	
	
   TList *files = dir.GetListOfFiles();
   const Int_t nFiles=GetNumberOfFiles(files);
	const Int_t *activity=SortActivity(files,nFiles);
   
	TChain *treeChain = new TChain ("countsData_small","countsData_small");
	TChain *treeIntegratedChain = new TChain ("integrationTree","integrationTree");
	
	

	
	/////*COLOR PALETTE SETTINGS*///////////////
	gStyle->SetPalette( kRainBow);
	Int_t nColors=TColor::GetNumberOfColors ();
	Int_t colorStep=nColors/nFiles;
	
	
	//////////////////////////////////////////
	
	

	
	
   
   if (files) {
	    
		TFile *graphsFile=new TFile("graphs.root","RECREATE");
		
		
		TH1I *histos[nFiles];
		TH1I *histosIntegrated[nFiles];
		TGraph *graphs[nFiles];
		TGraph *graphsIntegrated[nFiles];
		TSystemFile *file;
                TString fname,graphString;
		

		
		for (counter=0;counter<nFiles;counter++){
			fname="AllDataSingle"+TString::Itoa(activity[counter],10)+"BqL.root";
			cout << fname.Data() << endl;
                        TFile *ficheiro =new TFile(fname.Data(),"READ");
                        graphs[counter]=(TGraph*)ficheiro->Get(TString::Itoa(activity[counter],10)+"BqL");
                        //graphs[counter]->SetLineColor(TColor::GetColorPalette(counter*colorStep));
			leg->AddEntry(graphs[counter],TString::Itoa(activity[counter],10),"lp");
                        multi->Add(graphs[counter]);
			graphsIntegrated[counter]=(TGraph*)ficheiro->Get(TString::Itoa(activity[counter],10)+"BqL_integration");
                        //graphsIntegrated[counter]->SetLineColor(TColor::GetColorPalette(counter*colorStep));
			multi->Add(graphs[counter]);
                        multiIntegrated->Add(graphsIntegrated[counter]);
			
			treeChain->Add(fname);
			treeIntegratedChain->Add(fname);

			delete ficheiro;	
                        }
			
				
		graphsFile->cd();
		
		Int_t maxCounts=treeChain->GetMaximum("countsCoincidence");
		Int_t maxCountsIntegrated=treeIntegratedChain->GetMaximum("countsIntegrated");
		TString histoname, histonameIntegrated, cut;
                THStack *rawCountsStack=new THStack("rawData","rawData");
		THStack *integratedCountsStack=new THStack("integratedCounts","integratedCounts");
		for (Int_t i=0;i<nFiles;i++){
		histoname="histo_"+TString::Itoa(activity[i],10)+"BqL";
		histonameIntegrated="histo_integrated"+TString::Itoa(activity[i],10)+"BqL";
		histos[i]=new TH1I(histoname,histoname,maxCounts+1,0,maxCounts);	
		histosIntegrated[i]=new TH1I(histonameIntegrated,histonameIntegrated,maxCountsIntegrated+1,0,maxCountsIntegrated);	
		cut=	"activity=="+TString::Itoa(activity[i],10);
		treeChain->Draw("countsCoincidence>>"+histoname,cut,"goff");	
		treeIntegratedChain->Draw("countsIntegrated>>"+histonameIntegrated,cut,"goff");	
		histos[i]->SetFillColor(TColor::GetColorPalette(i*colorStep));
		histos[i]->SetLineColor(TColor::GetColorPalette(i*colorStep));
		histosIntegrated[i]->SetFillColor(TColor::GetColorPalette(i*colorStep));
		histosIntegrated[i]->SetLineColor(TColor::GetColorPalette(i*colorStep));
		rawCountsStack->Add(histos[i]);
		integratedCountsStack->Add(histosIntegrated[i]);	
		leg2->AddEntry(histos[i],TString::Itoa(activity[i],10),"f");

		}
		
		
		

		
		
		TCanvas *canvas0=new TCanvas("Canvas_RawData","Canvas_RawData");
		multi->	Draw("AL");
		multi->GetXaxis()->SetTimeDisplay(1);
		multi->GetXaxis()->SetTitle("Time (days/month)");
		multi->GetYaxis()->SetTitle("counts (min^{-1})");
		leg->Draw();
		multi->Write();
		
		TCanvas *canvas1=new TCanvas("Canvas_IntegratedData","Canvas_IntegratedData");
		multiIntegrated->	Draw("AL");
		multiIntegrated->GetXaxis()->SetTimeDisplay(1);
		multiIntegrated->GetXaxis()->SetTitle("Time (days/month)");
		multiIntegrated->GetYaxis()->SetTitle("counts (60 min^{-1})");
		leg->Draw();
		multiIntegrated->Write();
		
		TCanvas *canvas2=new TCanvas("Canvas_histoCounts_rawData","Canvas_histoCounts_rawData");
		rawCountsStack->Draw("nostack");
		leg2->Draw();
		rawCountsStack->GetXaxis()->SetTitle("counts (min^{-1})");
		rawCountsStack->Write();
		
		TCanvas *canvas3=new TCanvas("Canvas_histoCounts_integratedData","Canvas_histoCounts_integratedData");
		integratedCountsStack->Draw("nostack");
		leg2->Draw();
		integratedCountsStack->GetXaxis()->SetTitle("counts (60 min^{-1})");

		integratedCountsStack->Write();
		
		canvas0->SaveAs("Canvas_MultiGraph_RawData.pdf");
		canvas1->SaveAs("Canvas_MultiGraph_IntegratedData.pdf");	
		canvas2->SaveAs("Canvas_Stack_histoCounts.pdf");	
		canvas3->SaveAs("Canvas_Stack_histoCountsIntegrated.pdf");	
			
		}
		
	}
	
	
	//TCanvas *canvas0=new TCanvas("Canvas_MultiGraph_60_Average","Canvas_MultiGraph_60_Average");
	//multi->Draw("AL");
	//multi->GetXaxis()->SetTimeDisplay(1);
	//multi->GetXaxis()->SetTitle("Time (days/month)");
	//multi->GetYaxis()->SetTitle("Averaged counts (min^{-1})");
	//leg->Draw();
	//multi->Write();
	//canvas0->SaveAs("Canvas_MultiGraph_60_Average.pdf");
	
	//TMultiGraph *multi2=new TMultiGraph();
	//multi2->SetTitle("AveragedData_Thresholds");
	//multi2->SetName("AveragedData_Thresholds");
	//TMultiGraph *multi3=new TMultiGraph();
	//multi3->SetTitle("StdData_Thresholds");
	//multi3->SetName("StdData_Thresholds");
	
	//for(Int_t i=0;i<nFiles;i++){
		//averageGraph[i]->SetMarkerColor(TColor::GetColorPalette(i*colorStep));
		//averageGraph[i]->SetMarkerStyle(i+1);
		//averageGraph[i]->SetLineColor(TColor::GetColorPalette(i*colorStep));
		//stdGraph[i]->SetMarkerColor(TColor::GetColorPalette(i*colorStep));
		//stdGraph[i]->SetMarkerStyle(i+1);
		//stdGraph[i]->SetLineColor(TColor::GetColorPalette(i*colorStep));
		//multi2->Add(averageGraph[i],"LP");
		//multi3->Add(stdGraph[i],"LP");	
		//leg2->AddEntry(averageGraph[i],TString::Itoa(activity[i],10),"lpe");
		
	//}

	//TMultiGraph *multi4=new TMultiGraph();
	//multi4->SetTitle("Thresholds_AVR");
	//multi4->SetName("Thresholds_AVR");
	//TMultiGraph *multi5=new TMultiGraph();
	//multi5->SetTitle("Thresholds_STD");
	//multi5->SetName("Thresholds_STD");
	

	//for(Int_t i=0;i<nThreshold;i++){
		//thres[i]->SetMarkerColor(TColor::GetColorPalette(i*colorStep2));
		//thres[i]->SetMarkerStyle(i+1);
		//thres[i]->SetLineColor(TColor::GetColorPalette(i*colorStep2));
		//multi4->Add(thres[i],"LP");
		//stdAct[i]->SetMarkerColor(TColor::GetColorPalette(i*colorStep2));
		//stdAct[i]->SetMarkerStyle(i+1);
		//stdAct[i]->SetLineColor(TColor::GetColorPalette(i*colorStep2));
		//multi5->Add(stdAct[i],"LP");
		//leg3->AddEntry(thres[i],TString::Itoa(i+1,10),"lpe");
		//thres[i]->Write();
		//stdAct[i]->Write();
	//}



	//TCanvas *canvas=new TCanvas("CanvasAveragedTresholds","CanvasAveragedTresholds");
	//multi2->Draw("APL");
	//multi2->GetXaxis()->SetTitle("Threshold (Detected photons)");
	//multi2->GetYaxis()->SetTitle("Averaged Counts (min^{-1})");
	//leg2->Draw();
	//multi2->Write();
	//canvas->Write();
	//canvas->SaveAs("AverageTresholds.pdf");
	//TCanvas *canvas2=new TCanvas("StdTresholds","Stdresholds");
	//multi3->Draw("APL");
	//multi3->GetXaxis()->SetTitle("Threshold (Detected photons)");
	//multi3->GetYaxis()->SetTitle("STD (min^{-1})");
	//leg2->Draw();
	//multi3->Write();
	//canvas2->Write();
	//canvas2->SaveAs("StdTresholds.pdf");
	
	//TCanvas *canvas4=new TCanvas("TresholdsAvr","ThresholdsAvr");
	//multi4->Draw("APL");
	//multi4->GetXaxis()->SetTitle("Activity (Bq/L)");
	//multi4->GetYaxis()->SetTitle("Averaged counts (min^{-1})");
	//leg3->Draw();
	//multi4->Write();
	//canvas4->Write();
	//canvas4->SaveAs("TresholdsAvr.pdf");
	
	//TCanvas *canvas5=new TCanvas("TresholdsStd","ThresholdsStd");
	//multi5->Draw("APL");
	//multi5->GetXaxis()->SetTitle("Activity (Bq/L)");
	//multi5->GetYaxis()->SetTitle("STD counts (min^{-1})");
	//leg3->Draw();
	//multi5->Write();
	//canvas5->Write();
	//canvas5->SaveAs("TresholdsStd.pdf");
	////graphsFile->Close();
	//delete file;
	//delete graphsFile;

//}
