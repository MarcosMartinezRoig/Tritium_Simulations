void ViewGeometry()
{
	gSystem->Load("libGeom");
	gSystem->Load("libGdml");
	TGeoManager *geom = new TGeoManager();
	//TGeoChecker *checker = new TGeoChecker(geom);
	geom->SetNsegments(4); // Doesn't matter keep low
	
	geom->Import("TritiumGeo.gdml");

	geom->SetCheckingOverlaps(true);

	//int colors[9] =
	//{ kBlack, kYellow, kGray, kBlue, kOrange, kGreen, kRed, kCyan, kMagenta};
	//double transparency[9] =
	//{ 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,0.5 };


	//geom->CheckGeometryFull();





	TObjArray *volumes = geom->GetListOfVolumes();
	TIter nextV(volumes);
	TGeoVolume *vol;
	TString volName;
	int z = 0;
	while ((vol = (TGeoVolume*) nextV()))
	{
			volName=vol->GetName();
		if (volName=="teflonTubeLogical"){
				vol->SetTransparency(90);	
			}
			
		//sourceLogical is hard to Draw in GMDL, use X3D intead to view	
		if (volName=="sourceLogical"){
			vol->SetVisibility(false);
				//vol->SetTransparency(80); 	
			}	
			
			
			
		if (volName=="endCapLogical"){
					vol->SetTransparency(50);
					//vol->SetVisibility(false);
			}
			
			
		//if (volName=="fibbersLogical"){
				//vol->SetVisibility(false); 	
			//}	
			
		if (volName=="siliconeLogical"){
				vol->SetVisibility(false); 	
			}

		if (volName=="pmtLogical"){
				vol->SetLineColor(kBlack);
				vol->SetTransparency(50);
				//vol->SetVisibility(false);
			}

		//if (volName=="photocathodeLogical"){
				//vol->SetVisibility(false); 	
			//}


			cout<<volName<<endl;
	}
	geom->PrintOverlaps();
	

	geom->GetTopVolume()->Draw("ogl");
	//geom->GetTopVolume()->Draw("x3d");
	
	//geom->GetVolume("sourceLogical")->Draw("");
	
	//geom->GetTopVolume()->Draw("");


	geom->Export("TritiumGeo.root");
	//geom->Export("Tritium_geo.C");
	//geom->Export("Tritium_geo.gdml");

}
