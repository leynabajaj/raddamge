void drawTransRef(){
  //TFile *fin=TFile::Open("o_TransRefl.root","READ");
  TFile *fin=TFile::Open("Al_TransRefl_PDGid2112_Ecut0.root","READ");
  const int nThickness = 7;
  const int nEnergy = 3;
  string materials[1] = {"G4_Al"};
		/*	 "G4_W",
			 "G4_Pb",
			 "G4_Fe",
			 "G4_POLYETHYLENE",
			 "G4_CONCRETE"
  };*/
  //double thick[nThickness]={1,2,5,10,15,20};
  //double thick[nThickness]={5,10,20,30,40,50,60};
  double thick[nThickness]={5,10,20,30,40,50,60};
  double energy[nEnergy] = {10,50,100};
  /*double thick[4]={5,7.5,10,12.5};
  double energy[7] = {500, 1000, 1500, 2000, 3000, 5000, 8000};*/
  int colorMat[1]={1};
  string typeHit[2]={"hits","hitsE"};

  TGraph *gr;
  TCanvas *can[nThickness+nEnergy];
  for(int i=0;i<nThickness+nEnergy;i++){//thickness + energy == 4 + 21
    can[i]=new TCanvas(Form("c%d",i),Form("c%d",i),1400,800);
    can[i]->Divide(2);

    string gNm="",title="";
    for(int j=0;j<1;j++)
      for(int k=0;k<2;k++){
	if(i<4){
	  gNm = Form("TvsE_%s_%s_%d" ,materials[j].c_str(),typeHit[k].c_str(),int(thick[i]));
	  title = Form("%s for 1e5 ev| %d thick;energy[MeV]",typeHit[k].c_str(),int(thick[i]));
	}else{
	  gNm = Form("TvsZ_%s_%s_%d" ,materials[j].c_str(),typeHit[k].c_str(),int(energy[i-4]));
	  title = Form("%s for 1e5 ev| E=%d MeV; Thickness [cm]; fraction",
		       typeHit[k].c_str(),int(energy[i-4]));
	}
	cout<<"reading "<<gNm<<"\t"<<title<<endl;
	gr = (TGraph*)fin->Get(gNm.c_str());
	if(gr==NULL) {
	  cout<<"\t<<<<<<<<<<< skipping this one\n\n";
	  continue;
	}
	can[i]->cd(k+1);

	if(j==0){
	  gr->SetTitle(title.c_str());
	  if(i>=4)
	    //gr->SetMinimum(0.001);
	  gr->Draw("APL");
	gr->GetYaxis()->SetTitleOffset(1.5);
	//gr->GetYaxis()->SetNDdivisions(404);
	}else
	  gr->Draw("P");
      }
    can[i]->cd(1);
    //gPad->SetLogy(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    can[i]->cd(2);
    //gPad->SetLogy(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
  }


  fin->Close();
  delete fin;
}
