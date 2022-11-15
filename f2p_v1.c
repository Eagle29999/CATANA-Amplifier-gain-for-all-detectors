#include<TCanvas.h>
#include<TTree.h>
#include<TROOT.h>
#include<TH1.h>
#include<TH2D.h>
#include<TF1.h>
#include<TFile.h>
#include<TStyle.h>
#include<TMath.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TVector3.h"
#include<TLorentzVector.h>
#include "TCutG.h"
#include "TChain.h"
#include "TStopwatch.h"
#include "TCut.h"

#include<algorithm>
#include <stdlib.h>
#include <cmath>
#include "TBackCompFitter.h"
#include "Math/Integrator.h"
#include "TApplication.h"
#include "Math/IFunction.h"
#include "TMinuit.h"
#include "TIterator.h"
#include "TVirtualFitter.h"
#include <TGraph.h>
#include <TColor.h>
#include <cstdio>
#include <iomanip>
#include <assert.h>

#include "TSystem.h"
using namespace std;

void f2p_v1(TString filename="Data_R.root")
{
	//Open the root file and set the branch
	TFile* f = TFile::Open(filename);
	TTree *tree = (TTree*)f->Get("Data_R");
	Float_t gEnergy;
	tree->SetBranchAddress("Energy",&gEnergy);
	Int_t gBoard;
	tree->SetBranchAddress("Board",&gBoard);
	Int_t gchannel;
	tree->SetBranchAddress("channel",&gchannel);
	
	//Define the function
	TH1D* h1 = new TH1D("h1", "h1",150,5,155);
	TF1 *Gaus_fitFcn = new TF1("Gaus_fitFcn","gaus(0)+gaus(3)",20,100,6);
	Gaus_fitFcn -> SetParameters(1,1,1,1,1,1);
	Gaus_fitFcn -> SetParLimits(1,25,50);
	Gaus_fitFcn -> SetParLimits(2,0,10);
	Gaus_fitFcn -> SetParLimits(4,25,50);
	Gaus_fitFcn -> SetParLimits(5,0,10);
	int nevent=tree->GetEntries();
	
	//Check the single detector
	cout<<"Do you want to check the single detector? yes or no"<<endl;
	string flag;
	cin >> flag;
	if (flag == "yes" || flag == "y" ){
		Int_t B, C;
		Double_t par[6];
		cout<<"Please insert the Board and Channel number"<<endl;
		cin >> B >> C;
		for (Long64_t neve=0; neve<nevent; neve++){
				tree->GetEvent(neve);
				if (gBoard == B&&gchannel == C) h1->Fill(gEnergy);
			}
			h1 -> Fit("Gaus_fitFcn", "0");
			Gaus_fitFcn->GetParameters(&par[0]);
			h1 -> Draw();
			Gaus_fitFcn -> Draw("same");
			cout << "Board number: " <<B<< " Channel number: "<<C<<endl;
			cout<<"Mean1: "<<par[1]<<" Sigma1: "<<par[2]<<" Mean2: "<<par[4]
			<<" Sigma2: "<<par[5]<<" chi-square: "<<fitFcn->GetChisquare()<<endl;
	}	
	
	//Get all the histograms and fit them
	else{
		TString fileoutname = filename;
		fileoutname.ReplaceAll(".root","_analysed.txt");//the result file
		ofstream out(fileoutname);
		cout.rdbuf(out.rdbuf());
		Int_t Board_max = 9;
		Int_t Channel_max = 16;
		Double_t par[6];
		cout<<"Board"<<"\t"<<"Channel"<"\t"<<<"Mean1"<<"\t"<<"Sigma1"<<"\t"<<"Mean2"<<"\t"<<"Sigma2"<<"\t"<<"chi-square"<<endl;
		for (Int_t i=0; i<Board_max; i++){
			for (Int_t j=0; j<Channel_max; j++){
				for (Long64_t neve=0; neve<nevent; neve++){
						tree->GetEvent(neve);
						if (gBoard == i&&gchannel == j) h1->Fill(gEnergy);
				}
					h1 -> Fit("Gaus_fitFcn", "0");
					Gaus_fitFcn->GetParameters(&par[0]);
					cout<<i<<"\t"<<j<<"\t"<<par[1]<<"\t"<<par[2]<<"\t"<<par[4]<<"\t"<<
					par[5]<<"\t"<<fitFcn->GetChisquare()<<endl;
			}
		}
	}
	
}