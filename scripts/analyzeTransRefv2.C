TCanvas *c1=new TCanvas("c1","c1",1400,600);
string onm="y_TransRef_PDGid2112_15_20cm.pdf";
string ofnm = "E_TranRef_PDGid2112_15_20cm.root";
const int nEnergy = 1;
const float nEv=1e5;
const int nMat = 1;
const int nThick = 2;
const int pdgID = 2112;

void niceLogBins(TH1*);
void extractOneFile(int e, double thick, double effThick,
		    string mat,TH1D *h[2]);

void analyzeTransRefv2(){

  string materials[nMat] ={"G4_Pb"}; 
  
  /*{
			 "G4_W",
			 "G4_Pb",
			 "G4_Fe",
			 "G4_POLYETHYLENE",
			 "G4_CONCRETE"  };*/

  double thickness[nMat][nThick]={{15,20}};
				 /* {1,5,10,15},
				  {1.69,8.47,16.95,25.42},
				  {2.44,12.20,24.39,36.59},
				  {20.41,102.04,204.08,306.12},
				  {8.33,41.66,83.33,125.00}  };*/

 /* double energy[21] = {25, 50, 100, 150, 200, 300, 400, 500,
		       600, 700, 800, 900, 1000, 1250, 1500,
		       1750, 2000, 2250, 2500, 2750, 3000};*/

  double energy[nEnergy] = {3000};
  c1->Print(Form("%s[",onm.c_str()),"pdf");

  TFile *fout=new TFile(ofnm.c_str(),"RECREATE");
  TH1D *hE[nMat][nThick][nEnergy][2];

  for(int i=0;i<nMat;i++)
    for(int k=0;k<nThick;k++)
      for(int j=0;j<nEnergy;j++)
	extractOneFile(energy[j],thickness[i][k],thickness[0][k],
		       materials[i],hE[i][k][j]);


  fout->cd();
  for(int i=0;i<nMat;i++)
    for(int k=0;k<nThick;k++)
      for(int j=0;j<nEnergy;j++)
	for(int iTR=0;iTR<2;iTR++)
	  hE[i][k][j][iTR]->Write();
  fout->Close();
  delete fout;
  c1->Print(Form("%s]",onm.c_str()),"pdf");
}

void extractOneFile(int e, double thick, double effThick,
		    string mat,TH1D *h[2]){
  string tr[2]={"T","R"};
  string trCut[2]={"detID==1","detID==0"};
  std::cout<<"Inside ExtractOneFile  - e "<<e<<"  thick  "<<thick<<"  mat  "<<mat<<std::endl;
  string fnm = Form("./%04dMeV-%03.1fcm%s/E_TranRef%04dMeV-%03.1fcm%s-1e6.root",
		    e, thick, mat.c_str(), e, thick, mat.c_str());
  cout<<"trying "<<fnm<<endl;
  TFile *fin = TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("t");

  TH2D *hits = new TH2D("hits","",200,-2000,2000,200,-2000,2000);

  c1->Clear();
  c1->Divide(2);
  gStyle->SetOptStat("eMRou");

  for(int i=0;i<2;i++){
    string hTl=Form("%s | Ebeam=%d | thick = %6.2f | TR = %s;kinetic energy [MeV]",
		    mat.c_str(),e,thick,tr[i].c_str());
    string hnm=Form("%s_%s_%d_%d",tr[i].c_str(),mat.c_str(),int(e),int(effThick));
    h[i] = new TH1D(hnm.c_str(),hTl.c_str(),120,-2,4);
    niceLogBins(h[i]);

    c1->cd(1);
    t->Project("hits","prePosY:prePosX",Form("pType==%d && %s",pdgID,trCut[i].c_str()));
    hits->DrawCopy("colz");
    gPad->SetLogz(1);

    c1->cd(2);
    t->Project(hnm.c_str(),"preKE",Form("pType==%d && %s",pdgID,trCut[i].c_str()));
    h[i]->DrawCopy();
    gPad->SetLogx(1);

    c1->Print(onm.c_str(),"pdf");

    h[i]->SetDirectory(0);
  }

  fin->Close();
  delete fin;

}

void niceLogBins(TH1*h)
{
  TAxis *axis = h->GetXaxis();
  int bins = axis->GetNbins();

  double from = axis->GetXmin();
  double to = axis->GetXmax();
  double width = (to - from) / bins;
  double *new_bins = new double[bins + 1];

  for (int i = 0; i <= bins; i++) {
    new_bins[i] = pow(10, from + i * width);
  }
  axis->Set(bins, new_bins);
  delete[] new_bins;
}
