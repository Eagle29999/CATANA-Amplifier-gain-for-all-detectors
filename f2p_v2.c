#include<TCanvas.h>
#include<TTree.h>
#include<TROOT.h>
#include<TH1.h>
#include<TH2D.h>
#include<TF1.h>
#include<TFile.h>
#include<TStyle.h>
#include<TMath.h>
#include<math.h>
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

void f2p_v2(TString filename="DataR_catana0016.root")
{
	//Open the root file and set the branch
	TFile* f = TFile::Open(filename);
	TTree *tree = (TTree*)f->Get("Data_R");
	UShort_t gEnergy;
	tree->SetBranchAddress("Energy",&gEnergy);
	UShort_t gBoard;
	tree->SetBranchAddress("Board",&gBoard);
	UShort_t gchannel;
	tree->SetBranchAddress("Channel",&gchannel);
	
	//Define the function
	TH1D* h1 = new TH1D("h1", "h1",245,5,250);
	TF1 *Gaus_fitFcn = new TF1("Gaus_fitFcn","gaus(0)+gaus(3)",1,200);
	
	int nevent=tree->GetEntries();
	
	//Check the single detector
	cout<<"Do you want to check the single detector? yes or no"<<endl;
	string flag;
	cin >> flag;
	if (flag == "yes" || flag == "y" ){
		Int_t B, C;
		Double_t par[6];
		TSpectrum *sp;
		sp = new TSpectrum(4);
		Float_t *xpeaks;
		cout<<"Please insert the Board and Channel number"<<std::endl;
		cin >> B >> C ;
		for (Int_t neve=0; neve<nevent; neve++){
			if(neve%10000000==0){
			printf("%i / %i\n",neve, nevent);
			}	
			tree->GetEvent(neve);
				if (gBoard == B&&gchannel == C) h1->Fill(gEnergy);
			}
			Int_t nfound = sp->Search(h1,1,"",0.05);
			printf("Found %i candidate peaks to fit\n",nfound);
			xpeaks = sp->GetPositionX();
			for(int i2=0;i2<nfound-1;i2++){
				for(int j2=0;j2<nfound-1-i2;j2++){
					if(xpeaks[j2]>xpeaks[j2+1]){
						Float_t temp = xpeaks[j2];
						xpeaks[j2] = xpeaks[j2+1];
						xpeaks[j2+1] = temp;
					}
				}
			}
			printf("The position is %f %f",xpeaks[nfound-2],xpeaks[nfound-1]);
			Gaus_fitFcn -> SetParameters(1000,xpeaks[nfound-2],1,1000,xpeaks[nfound-1],1);
			Gaus_fitFcn -> SetParLimits(0,0,2000);
			Gaus_fitFcn -> SetParLimits(1,xpeaks[nfound-2]-3,xpeaks[nfound-2]+3);
			Gaus_fitFcn -> SetParLimits(2,0,20);
			Gaus_fitFcn -> SetParLimits(3,0,2000);
			Gaus_fitFcn -> SetParLimits(4,xpeaks[nfound-1]-3,xpeaks[nfound-1]+3);
			Gaus_fitFcn -> SetParLimits(5,0,20);
			
			h1 -> Fit("Gaus_fitFcn", "0","",xpeaks[nfound-2]-5,xpeaks[nfound-1]+5);
			Gaus_fitFcn->GetParameters(&par[0]);
			h1 -> Draw();
			Gaus_fitFcn -> Draw("same");
			cout << "Board number: " <<B<< " Channel number: "<<C<<std::endl;
			cout<<"Mean1: "<<par[1]<<" Sigma1: "<<par[2]<<" Mean2: "<<par[4]
			<<" Sigma2: "<<par[5]<<" chi-square: "<<Gaus_fitFcn->GetChisquare()<<std::endl;
	}	
	
	//Get all the histograms and fit them
	else{
		TCanvas *c1 = new TCanvas("c1", "c1",18,41,919,777);
		TString fileoutname = filename;
		fileoutname.ReplaceAll(".root","_analyzed.txt");//the result file
		ofstream out(fileoutname);
		cout.rdbuf(out.rdbuf());
		TSpectrum *sp;
		sp = new TSpectrum(4);
		Float_t *xpeaks;
		TH1D* h[140];
		for (Int_t i=0; i<140; i++){
			ostringstream ss,ss2,ss3;
			int Board_NUMBER2 = i/14;
			int Channel_NUMBER2 = i%14;
			ss << i;
			ss2 << Board_NUMBER2;
			ss3 << Channel_NUMBER2;
			TString str = ss.str();
			TString str3 = ss2.str();
			TString str4 = ss3.str();
			h[i] = new TH1D("h"+str, "Board "+str3+" Channel: "+str4,245,5,250);
		}
		Double_t par[6];
		cout<<"Board"<<"\t"<<"Channel"<<"\t"<<"Mean1"<<"\t"<<"Sigma1"<<"\t"<<"Mean2"<<"\t"<<"Sigma2"<<"\t"<<"chi-square"<<endl;
		for (Long64_t neve=0; neve<nevent; neve++){
				tree->GetEvent(neve);
				if(neve%10000000==0){
				printf("%i / %i\n",neve, nevent);
				}	
				if(gchannel<14){
				h[gBoard*14 + gchannel]->Fill(gEnergy);}
			}
		for (Int_t j=0; j<140; j++){
			Int_t nfound = sp->Search(h[j],1,"",0.05);
			printf("Found %i candidate peaks to fit\n",nfound);
			xpeaks = sp->GetPositionX();
			for(int i2=0;i2<nfound-1;i2++){
				for(int j2=0;j2<nfound-1-i2;j2++){
					if(xpeaks[j2]>xpeaks[j2+1]){
						Float_t temp = xpeaks[j2];
						xpeaks[j2] = xpeaks[j2+1];
						xpeaks[j2+1] = temp;
					}
				}
			}
			printf("The position is %f %f",xpeaks[nfound-2],xpeaks[nfound-1]);
			Gaus_fitFcn -> SetParameters(1000,xpeaks[nfound-2],1,1000,xpeaks[nfound-1],1);
			Gaus_fitFcn -> SetParLimits(0,0,2000);
			Gaus_fitFcn -> SetParLimits(1,xpeaks[nfound-2]-3,xpeaks[nfound-2]+3);
			Gaus_fitFcn -> SetParLimits(2,0,20);
			Gaus_fitFcn -> SetParLimits(3,0,2000);
			Gaus_fitFcn -> SetParLimits(4,xpeaks[nfound-1]-3,xpeaks[nfound-1]+3);
			Gaus_fitFcn -> SetParLimits(5,0,20);
			
			h[j] -> Fit("Gaus_fitFcn", "0","",xpeaks[nfound-2]-5,xpeaks[nfound-1]+5);
			Gaus_fitFcn->GetParameters(&par[0]);
			int Board_NUMBER = j/14;
			int Channel_NUMBER = j%14;
			h[j]->Draw();
			Gaus_fitFcn->Draw("same");
			ostringstream ss2;
			ss2 << j;
			TString str2 = ss2.str();
			c1->Print("Figure/h"+str2+".jpg","jpg");
			cout<<Board_NUMBER<<"\t"<<Channel_NUMBER<<"\t"<<par[1]<<"\t"<<par[2]<<"\t"<<par[4]<<"\t"<<
			par[5]<<"\t"<<Gaus_fitFcn->GetChisquare()<<std::endl;
		}	
	}
	
}
