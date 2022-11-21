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
	ofstream out("result.txt");
	cout.rdbuf(out.rdbuf());
	//FILE*fp_1520 = fopen ("DataR_catana0017_analyzed.txt","r");
	Int_t n = 140;
	Float_t x[4] = {1440, 1460, 1480, 1500};
	Int_t Channel[4];
	Int_t Board[4];
	Float_t y[4];
	Double_t par[4];
	Double_t V;
	TF1 *fitFcn = new TF1("fitFcn",fitFunction,1400,1550,2);
	fitFcn -> SetParameters(9.54591e-21,6.85252);
	//fitFcn -> SetParLimits(0,0,0.01);
	//fitFcn -> SetParLimits(1,0,10);
	std::cout<<"Board"<<"\t"<<"Channel"<<"\t"<<"A:"<<"\t"<<"n:"<<"\t"<<"chi-square"<<"\t"<<"Voltage"<<std::endl;
	
	for(int i=0;i<n;i++)
	{	
		fscanf(fp_1440,"%i %i %f",&Board[0],&Channel[0],&y[0]);
		fscanf(fp_1460,"%i %i %f",&Board[1],&Channel[1],&y[1]);
		fscanf(fp_1480,"%i %i %f",&Board[2],&Channel[2],&y[2]);
		fscanf(fp_1500,"%i %i %f",&Board[3],&Channel[3],&y[3]);
		//fscanf(fp_1520,"%i %i %f",&Board[4],&Channel[4],&y[4]);
		//cout << Board[0] << "\t" << Channel[0] <<"\t"<<y[0]<<endl;
		TGraph *gr1 = new TGraph (4, x, y);
		ostringstream ss0,ss1;
		ss0 << Board[0];
		ss1 << Channel[0];
		TString str0 = ss0.str();
		TString str1 = ss1.str();
		gr1->SetTitle("Board: "+str0+" Channel: "+str1);
		gr1->Fit("fitFcn","Q");
		gr1->Draw("AC*");
		fitFcn->Draw("same");
		ostringstream ss2;
		ss2 << i;
		TString str2 = ss2.str();
		c1->Print("Figure/h"+str2+"_fit.jpg","jpg");
		fitFcn->GetParameters(&par[0]);
		V = TMath::Power(((4000*1.173/300)/par[0]),1/par[1]);
		std::cout<< Board[0] << "\t" << Channel[0] <<"\t"<<par[0]<<"\t"<<par[1]<<"\t"<<
		fitFcn->GetChisquare()<<"\t"<<V<<std::endl;
	}
	
}
