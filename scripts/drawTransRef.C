void drawTransRef(){
  //TFile *fin=TFile::Open("o_TransRefl.root","READ");
  TFile *fin=TFile::Open("o_TransRefl_PDGid2112_Ecut0.root","READ");
  string materials[5] = {
			 "G4_W",
			 "G4_Pb",
			 "G4_Fe",
			 "G4_POLYETHYLENE",
			 "G4_CONCRETE"
  };
  double thick[4]={1,5,10,15};
  double energy[21] = {25, 50, 100, 150, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000};
  int colorMat[5]={1,2,3,4,6};
  string typeHit[2]={"hits","hitsE"};

  TGraph *gr;
  TCanvas *can[25];
  for(int i=0;i<25;i++){//thickness + energy == 4 + 21
    can[i]=new TCanvas(Form("c%d",i),Form("c%d",i),1400,800);
    can[i]->Divide(2);

    string gNm="",title="";
    for(int j=0;j<5;j++)
      for(int k=0;k<2;k++){
	if(i<4){
	  gNm = Form("TvsE_%s_%s_%d" ,materials[j].c_str(),typeHit[k].c_str(),int(thick[i]));
	  title = Form("%s for 1e5 ev| %d thick;energy[MeV]",typeHit[k].c_str(),int(thick[i]));
	}else{
	  gNm = Form("TvsZ_%s_%s_%d" ,materials[j].c_str(),typeHit[k].c_str(),int(energy[i-4]));
	  title = Form("%s for 1e5 ev| %d Ebeam;target length * densityW/densityThis [cm]",
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
	  if(i>4)
	    gr->SetMinimum(0);
	  gr->Draw("APL");
	}else
	  gr->Draw("PL");
      }
    can[i]->cd(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    can[i]->cd(2);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
  }


  fin->Close();
  delete fin;
}
