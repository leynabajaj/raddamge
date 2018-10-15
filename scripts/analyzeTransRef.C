TCanvas *c1=new TCanvas("c1","c1",1400,600);
string onm="y_TransRef_PDGid2112_Ecut0.pdf";
string ofnm = "o_TransRefl_PDGid2112_Ecut0.root";
int nEnergy = 21;
const float nEv=1e5;
int pdgID = 2112;
string eCut="preKE>0";

void analyzeOneMaterial(string material, int pdgID, TGraph *g[2][2][25], double thickness[4], double effThick[4]);
void extractOneFile(int e, double thick, string mat, int pdgID,double trans[2], double ref[2]);

void analyzeTransRef(){

  string materials[5] = {
			 "G4_W",
			 "G4_Pb",
			 "G4_Fe",
			 "G4_POLYETHYLENE",
			 "G4_CONCRETE"
  };
  int colorMat[5]={1,2,3,4,6};
  double thicknesses[5][4]={
			    {1,5,10,15},
			    {1.69,8.47,16.95,25.42},
			    {2.44,12.20,24.39,36.59},
			    {20.41,102.04,204.08,306.12},
			    {8.33,41.66,83.33,125.00}
  };

  c1->Print(Form("%s[",onm.c_str()),"pdf");

  TFile *fout=new TFile(ofnm.c_str(),"RECREATE");

  TGraph *gr[5][2][2][25];
  int nMat = 5;

  for(int i=0;i<nMat;i++)
    analyzeOneMaterial(materials[i], pdgID, gr[i], thicknesses[i],thicknesses[0]);

  // c1->Print(Form("%s]",onm.c_str()),"pdf");
  // return;

  fout->cd();
  for(int j=0;j<2;j++)
    for(int i=0;i<4+nEnergy;i++){
      for(int k=0;k<nMat;k++){
      	gr[k][j][0][i] -> SetMarkerStyle(20);
      	gr[k][j][0][i] -> SetMarkerColor(colorMat[k]);
      	gr[k][j][0][i] -> SetLineColor(colorMat[k]);

      	gr[k][j][1][i] -> SetMarkerStyle(20);
      	gr[k][j][1][i] -> SetMarkerColor(colorMat[k]);
      	gr[k][j][1][i] -> SetLineColor(colorMat[k]);

	gr[k][j][0][i]->Write();
	gr[k][j][1][i]->Write();
      }
    }
  fout->Close();
  delete fout;
  c1->Print(Form("%s]",onm.c_str()),"pdf");
}

//g [0-3] T/R vs E for 4 thicknesses, g[4-25] T/R vs thickness for 21 E
void analyzeOneMaterial(string material, int pdgID, TGraph *g[2][2][25], double thickness[4], double effThick[4]){
  cout<<"Started with "<<material<<endl;
  //[hits, EweightHits][thickenss][energy]
  double trans[2][4][21], refl[2][4][21];
  double energy[21] = {25, 50, 100, 150, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000};

  for(int i = 0 ; i < 4 ; i++ )
    for(int j = 0 ; j < nEnergy; j++){
      double tran[2],ref[2];
      extractOneFile(int(energy[j]), thickness[i], material, pdgID, tran, ref);
      //cout<<"anaOneMat\t"<<tran[0]<<"\t"<<tran[1]<<"\t"<<ref[0]<<"\t"<<ref[1]<<endl;
      trans[0][i][j] = tran[0];
      trans[1][i][j] = tran[1];
      refl[0][i][j] = ref[0];
      refl[1][i][j] = ref[1];
    }

  string eW[2] = {"hits", "hitsE"};
  for(int i=0;i<2;i++){
    for(int j=0;j<4;j++){
      g[i][0][j] = new TGraph(nEnergy,energy,trans[i][j]);
      g[i][0][j] -> SetTitle(Form("%s %s Trans for %d effThick %d; energy [MeV]; hits [#]",
				  material.c_str(),eW[i].c_str(), pdgID, int(effThick[j])));
      g[i][0][j] -> SetName(Form("TvsE_%s_%s_%d",
				 material.c_str(), eW[i].c_str(), int(effThick[j])));

      g[i][1][j] = new TGraph(14,energy,refl[i][j]);
      g[i][1][j] -> SetTitle(Form("%s %s Reflc for %d effThick %d; energy [MeV]; hits [#]",
				  material.c_str(),eW[i].c_str(),pdgID, int(effThick[j])));
      g[i][1][j] -> SetName(Form("RvsE_%s_%s_%d",
				 material.c_str(),eW[i].c_str(), int(effThick[j])));
    }

    for(int j=4;j<4+nEnergy;j++){
      double tVal[4],rVal[4];
      for(int k=0;k<4;k++){
	tVal [k] = trans[i][k][j-4];
	rVal [k] = refl[i][k][j-4];
      }
      g[i][0][j] = new TGraph(4,effThick,tVal);
      g[i][0][j] -> SetTitle(Form("%s %s Trans for %d Ebeam=%d; effective target thickness [mm]; hits [#]",
				  material.c_str(),eW[i].c_str(), pdgID,int(energy[j-4])));
      g[i][0][j] -> SetName(Form("TvsZ_%s_%s_%d",
				 material.c_str(),eW[i].c_str(),int(energy[j-4])));

      g[i][1][j] = new TGraph(4,effThick,rVal);
      g[i][1][j] -> SetTitle(Form("%s %s Reflc for %d Ebeam=%d; effective target thickness [mm]; hits [#]",
				  material.c_str(),eW[i].c_str(),pdgID,int(energy[j-4])));
      g[i][1][j] -> SetName(Form("RvsZ_%s_%s_%d",
				 material.c_str(),eW[i].c_str(),int(energy[j-4])));
    }
  }
}

void extractOneFile(int e, double thick, string mat, int pdgID,
		    double trans[2], double ref[2]){
  string fnm = Form("./%03dMeV-%06.2fcm%s/o-tranRef-%03dMeV-%06.2fcm%s-1e5.root",
		    e, thick, mat.c_str(), e, thick, mat.c_str());
  TFile *fin = TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("t");

  TH2D *hT = new TH2D("hT","",200,-2000,2000,200,-2000,2000);
  TH2D *hR = new TH2D("hR","",200,-2000,2000,200,-2000,2000);

  c1->Clear();
  c1->Divide(2);

  gStyle->SetOptStat("eMRou");
  c1->cd(1);
  t->Project("hT","prePosY:prePosX",Form("pType==%d && detID == 1 && %s",
					 pdgID,eCut.c_str()));
  hT->SetTitle(Form("%s Trans for %d %s| Ebeam=%d | thick = %f;x[mm];y[mm]",
		    mat.c_str(),pdgID,eCut.c_str(),e,thick));
  trans[0] = hT->Integral()/nEv;
  hT->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->cd(2);
  t->Project("hR","prePosY:prePosX",Form("pType==%d && detID == 0 && %s",
					 pdgID,eCut.c_str()));
  ref[0] = hR->Integral()/nEv;
  hR->SetTitle(Form("%s Refl for %d %s| Ebeam=%d | thick = %f;x[mm];y[mm]",
		    mat.c_str(),pdgID,eCut.c_str(),e,thick));
  hR->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->Print(onm.c_str(),"pdf");

  c1->cd(1);
  t->Project("hT","prePosY:prePosX",Form("preKE*(pType==%d && detID == 1 && %s)",
					 pdgID,eCut.c_str()));
  hT->SetTitle(Form("%s Eweight Trans for %d %s| E=%d | thick = %f;x[mm];y[mm]",
		    mat.c_str(),pdgID,eCut.c_str(),e,thick));
  trans[1] = hT->Integral()/(nEv*e);
  hT->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->cd(2);
  t->Project("hR","prePosY:prePosX",Form("preKE*(pType==%d && detID == 0 && %s)",
					 pdgID,eCut.c_str()));
  ref[1] = hR->Integral()/(nEv*e);
  hR->SetTitle(Form("%s Eweight Refl for %d %s| E=%d | thick = %f;x[mm];y[mm]",
		    mat.c_str(),pdgID,eCut.c_str(),e,thick));
  hR->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->Print(onm.c_str(),"pdf");

  c1->cd(1);
  t->Draw("preKE",Form("pType==%d && detID == 1 && %s",pdgID,eCut.c_str()));
  gPad->SetLogy(1);
  c1->cd(2);
  t->Draw("preKE",Form("pType==%d && detID == 0 && %s",pdgID,eCut.c_str()));
  gPad->SetLogy(1);
  c1->Print(onm.c_str(),"pdf");

  fin->Close();
  delete fin;
}
