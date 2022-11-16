{
	TSpectrum *sp2[12];
	TH1D* h[3];
	h[0] = new TH1D("h1", "h1",100,-5,5);
	h[1] = new TH1D("h2", "h2",100,-5,5);
	//h[3] = new TH1D("h3", "h3",100,-5,5);
	h[0] -> FillRandom("gaus",1000);
	h[1] -> FillRandom("gaus",10000);
	h[1]->Draw();
	h[0]->Draw("same");
}