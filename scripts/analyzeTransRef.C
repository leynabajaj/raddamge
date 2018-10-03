TCanvas *c1=new TCanvas("c1","c1",1400,600);
string onm="y_TransRef.pdf";

void analyzeTransRef(){

  string materials[5] = {
			 "G4_W",
			 "G4_Pb",
			 "G4_Fe",
			 "G4_CONCRETE",
			 "G4_POLYETHYLENE"
  };
  int colorMat[5]={1,2,3,4,6};
  double thicknesses[5][4]={
			    {1,5,10,15},
			    {1.69,8.47,16.95,25.42},
			    {2.44,12.20,24.39,36.59},
			    {8.33,41.66,83.33,125.00},
			    {20.41,102.04,204.08,306.12}
  };

  c1->Print(Form("%s[",onm.c_str()),"pdf");

  int pdgID = 2212;
  TGraph *gr[5][2][2][18];
  for(int i=0;i<5;i++)
    analyzeOneMaterial(materials[i].c_str(), pdgID, gr[i], thicknesses[i]);

  for(int j=0;j<2;j++)
    for(int i=0;i<18;i++){
      for(int k=0;k<5;k++){
	c1->cd(1);
	gr[k][j][0][i] -> SetMarkerStyle(20);
	gr[k][j][0][i] -> SetMarkerColor(colorMat[k]);
	gr[k][j][0][i] -> SetLineColor(colorMat[k]);
	if(k==0)
	  gr[k][j][0][i] -> Draw("APL");
	else
	  gr[k][j][0][i] -> Draw("PL");

	c1->cd(2);
	gr[k][j][1][i] -> SetMarkerStyle(20);
	gr[k][j][1][i] -> SetMarkerColor(colorMat[k]);
	gr[k][j][1][i] -> SetLineColor(colorMat[k]);
	if(k==0)
	  gr[k][j][1][i] -> Draw("APL");
	else
	  gr[k][j][1][i] -> Draw("PL");
      }
      c1->Print(onm.c_str(),"pdf");
    }

  c1->Print(Form("%s]",onm.c_str()),"pdf");
}

//g [0-3] T/R vs E for 4 thicknesses, g[4-18] T/R vs thickness for 14 E
void analyzeOneMaterial(string material, int pdgID, TGraph *g[2][2][18], double thickness[4]){

  //[hits, EweightHits][thickenss][energy]
  double trans[2][4][14], refl[2][4][14];
  int energy[14] = {500, 600, 700, 800, 900, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000};

  for(int i = 0 ; i < 4 ; i++ )
    for(int j = 0 ; j < 14 ; j++){
      double tran[2],ref[2];
      extractOneFile(energy[j], thickness[i], material, pdgID, tran[2], refl[2]);
      trans[0][i][j] = tran[0];
      trans[0][i][j] = tran[1];
      refl[0][i][j] = ref[0];
      refl[0][i][j] = ref[1];
    }

  string eW[2] = {"", "E weight"};
  for(int i=0;i<2;i++){
    for(int j=0;j<4;j++){
      g[i][0][j] = new TGraph(14,energy,trans[i][j]);
      g[i][0][j] -> SetTitle(Form("%s Trans for ; energy [MeV]; hits [#]",eW[i].c_str(), pdgID));
      g[i][1][j] = new TGraph(14,energy,refl[i][j]);
      g[i][1][j] -> SetTitle(Form("%s Reflc for ; energy [MeV]; hits [#]",eW[i].c_str(),pdgID));
    }

    for(int j=4;j<18;j++){
      double tVal[4],rVal[4];
      for(int k=0;k<4;k++){
	tVal [k] = trans[i][k][j-4];
	rVal [k] = refl[i][k][j-4];
      }
      g[i][0][j] = new TGraph(4,thickness,tVal);
      g[i][0][j] -> SetTitle(Form("%s Trans for ; target thickness [mm]; hits [#]",eW[i].c_str(), pdgID));
      g[i][1][j] = new TGraph(4,thickness,rVal);
      g[i][1][j] -> SetTitle(Form("%s Reflc for ; target thickness [mm]; hits [#]",eW[i].c_str(),pdgID));
    }
}

void extractOneFile(int e, double thick, string mat, int pdgID,
		    double trans[2], double ref[2]){
  string fnm = Form("%dMeV-%6.2fcm%s", e, thick, mat.c_str());
  TFile *fin = TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("t");

  TH2D *hT = new TH2D("hT","",200,-2000,2000,200,-2000,2000);
  TH2D *hR = new TH2D("hR","",200,-2000,2000,200,-2000,2000);

  c1->Clear();
  c1->Divide(2);

  gStyle->SetOptStat("eMRou");
  c1->cd(1);
  t->Project("hT","prePosY:prePosX",Form("pType==%d && detID == 1"),"colz");
  hT->SetTitle(Form("Trans for %d | E=%d | thick = %f;x[mm];y[mm]",pdgID,e,thick));
  trans[0] = hT->Integral();
  hT->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->cd(2);
  t->Project("hR","prePosY:prePosX",Form("pType==%d && detID == 0"),"colz");
  ref[0] = hR->Integral();
  hR->SetTitle(Form("Refl for %d | E=%d | thick = %f;x[mm];y[mm]",pdgID,e,thick));
  hR->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->Print(onm.c_str(),"pdf");

  c1->cd(1);
  t->Project("hT","prePosY:prePosX",Form("preKE*(pType==%d && detID == 1)"),"colz");
  hT->SetTitle(Form("Eweight Trans for %d | E=%d | thick = %f;x[mm];y[mm]",pdgID,e,thick));
  trans[1] = hT->Integral();
  hT->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->cd(2);
  t->Project("hR","prePosY:prePosX",Form("preKE*(pType==%d && detID == 0)"),"colz");
  ref[1] = hR->Integral();
  hR->SetTitle(Form("Eweight Refl for %d | E=%d | thick = %f;x[mm];y[mm]",pdgID,e,thick));
  hR->DrawCopy("colz");
  gPad->SetLogz(1);
  c1->Print(onm.c_str(),"pdf");

  fin->Close();
  delete fin;
}
