void drawTransRef(){
  //TFile *fin=TFile::Open("o_TransRefl.root","READ");
  TFile *fin=TFile::Open("Mu2LeadMuTransRefl_PDGid11_Ecut0.root","READ");
  const int nThickness = 9;
  const int nEnergy = 3;
  string materials[1] = {"G4_W"};
		/*	 "G4_W",
			 "G4_Pb",
			 "G4_Fe",
			 "G4_POLYETHYLENE",
			 "G4_CONCRETE"
  };*/
  //double thick[nThickness]={1,2,5,10,15,20};
  //double thick[nThickness]={5,10,20,30,40,50,60};
  double thick[nThickness]={2,5,8,10,11,12,13,14};
  double energy[nEnergy] = {200,300,400};
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
	if(i<nThickness){
	  gNm = Form("TvsE_%s_%s_%d" ,materials[j].c_str(),typeHit[k].c_str(),int(thick[i]));
	  title = Form("%s for 1e5 evt| %d thick;energy[MeV]",typeHit[k].c_str(),int(thick[i]));
	}else{
	  gNm = Form("TvsZ_%s_%s_%d" ,materials[j].c_str(),typeHit[k].c_str(),int(energy[i-nThickness]));
	  title = Form("%s for 1e5 evt| E=%d MeV; Thickness [cm]; fraction",
		       typeHit[k].c_str(),int(energy[i-nThickness]));
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
	  if(i>=nThickness)
	  gr->Draw("APL");
	if(k==0){
	gr->GetYaxis()->SetNdivisions(404);
	gr->SetMinimum(0.9);
	gr->SetMaximum(1.05);}
	else {
	gr->GetYaxis()->SetNdivisions(404);
	gr->SetMinimum(1.0e-1);
	gr->SetMaximum(1.0);}
	gr->GetYaxis()->SetTitleOffset(1.5);
	}else
	  gr->Draw("P");
      }
    can[i]->cd(1);
    //gPad->SetLogy(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    can[i]->cd(2);
    gPad->SetLogy(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
  }


  fin->Close();
  delete fin;
}
