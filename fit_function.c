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
	FILE*fp_1480 = fopen ("1480.txt","r");
	FILE*fp_1500 = fopen ("1500.txt","r");
	ofstream out("result.txt");
	cout.rdbuf(out.rdbuf());
	static const Int_t n = 9;
	Float_t x[2] = {1480, 1500};
	static const Int_t Channel[2];
	static const Int_t Board[2];
	Float_t y[2];
	Double_t par[2];
	Double_t V;
	TF1 *fitFcn = new TF1("fitFcn",fitFunction,1400,1550,2);
	fitFcn -> SetParameters(1,1);
	std::cout<<"Detector ID"<<"\t"<<"Voltage"<<std::endl;
	for(int i=0;i<n;i++)
	{	
		fscanf(fp_1480,"%i %i %f",&Board[0],&Channel[0],&y[0]);
		fscanf(fp_1500,"%i %i %f",&Board[1],&Channel[1],&y[1]);
		cout << Board[0] << "\t" << Channel[0] <<"\t"<<y[0]<<endl;
		TGraph *gr1 = new TGraph (2, x, y);
		gr1->Fit("fitFcn");
		fitFcn->GetParameters(&par[0]);
		V = TMath::Power(((4000*1.173/300)/par[0]),1/par[1]);
		std::cout<<i<<"\t"<<V<<std::endl;
	}
	
}