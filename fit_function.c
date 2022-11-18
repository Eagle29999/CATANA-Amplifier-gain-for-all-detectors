#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <TH2.h>
#include <TStyle.h>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TObject.h"
#include "TNamed.h"
#include "TGraphErrors.h"
#include "TSpectrum.h"
#include "TVirtualPad.h"

Double_t fitFunction(Double_t *x, Double_t *par) {
	
  return par[0]*TMath::Power(x[0], par[1]);
  
}

void fit_function() {
	TCanvas *c1 = new TCanvas("c1", "c1",18,41,919,777);
	FILE*fp_1440 = fopen ("DataR_catana0016_analyzed.txt","r");
	FILE*fp_1460 = fopen ("DataR_catana0015_analyzed.txt","r");
	FILE*fp_1480 = fopen ("DataR_catana0014_analyzed.txt","r");
	FILE*fp_1500 = fopen ("DataR_catana0013_analyzed.txt","r");
	FILE*fp_1520 = fopen ("DataR_catana0017_analyzed.txt","r");
	ofstream out("result.txt");
	cout.rdbuf(out.rdbuf());
	static const Int_t n = 140;
	Float_t x[5] = {1440, 1460, 1480, 1500, 1520};
	static const Int_t Channel[5];
	static const Int_t Board[5];
	Float_t y[5];
	Double_t par[5];
	Double_t V;
	TF1 *fitFcn = new TF1("fitFcn",fitFunction,1400,1550,2);
	fitFcn -> SetParameters(1,1);
	std::cout<<"Board"<<"\t"<<"Channel"<<"\t"<<"A:"<<"\t"<<"n:"<<"\t"<<"chi-square"<<"\t"<<"Voltage"<<std::endl;
	for(int i=0;i<n;i++)
	{	
		fscanf(fp_1440,"%i %i %f",&Board[0],&Channel[0],&y[0]);
		fscanf(fp_1460,"%i %i %f",&Board[1],&Channel[1],&y[1]);
		fscanf(fp_1480,"%i %i %f",&Board[2],&Channel[2],&y[2]);
		fscanf(fp_1500,"%i %i %f",&Board[3],&Channel[3],&y[3]);
		fscanf(fp_1520,"%i %i %f",&Board[4],&Channel[4],&y[4]);
		//cout << Board[0] << "\t" << Channel[0] <<"\t"<<y[0]<<endl;
		TGraph *gr1 = new TGraph (5, x, y);
		gr1->Fit("fitFcn");
		ostringstream ss2;
		ss2 << i;
		TString str2 = ss2.str();
		c1->Print("Figure/h"+str2+".jpg","jpg");
		fitFcn->GetParameters(&par[0]);
		V = TMath::Power(((4000*1.173/300)/par[0]),1/par[1]);
		std::cout<< Board[0] << "\t" << Channel[0] <<"\t"<<par[0]<<"\t"<<par[1]<<"\t"<<
		fitFcn->GetChisquare()<<"\t"<<V<<std::endl;
	}
	
}