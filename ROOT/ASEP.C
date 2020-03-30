//
// ************************************************************************
// * Disclaimer!!!                                                        *
// * Copyright (C) 12 March 2020 Ankur Yadav <ankuryadavt20cool@gmail.com>*
// *                                                                      *
// * This file is part of TrafficPredictions project.                     *
// * https://github.com/Ankyyadav/TrafficPredictions/tree/master          *
// *                                                                      *
// * TrafficPrediction can not be copied and/or distributed without the   *
// * express permission of the author                                     *
// * This is a ROOT macro which tries to simulate traffic scenarios       * 
// * using thesimplistic NaSch's four step model with some                *
// * extensions.                                                          *
// *                                                                      *
// *                        AUTHOR :- Ankur Yadav                         *
// *                                  University of Bonn                  *
// *                                  ankuryadavt20cool@gmail.com         *
// *                                                                      *
// * Extension Type --  NaSch Model ASEP                                  *
// ************************************************************************
//

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<vector>

bool Probability(float f)
{
	if (rand()%100 < f*100)
		return true;
	else
		return false;
}
const int TotSites=30;
int ASEP()
{
	srand(time(NULL));
	int N = 1000;
	int Tmax = 10000;
	//int TotSites = 30;
	float p = 0.2;
	p=1.0-p;
	float alpha = 0.15;
	float beta = 0.10;
	float CarDensity[TotSites] = {0} ;
	int car[Tmax][TotSites];
	float temp[Tmax];
	
	
	
	for (int k=0; k<N; k++)
	{
		//cout << "\nAlpha = " <<alpha << "\nbeta = "<<beta<<"\np(to move) = "<<p<<endl; 
			
		for (int i=0; i<Tmax; i++)
			for (int j=0; j<TotSites; j++)
				car[i][j] = 0;
				
		for (int i=1; i<Tmax; i++)
		{
			if (car[i-1][0] == 0 && Probability(alpha)==true)
				car[i][0] = 1;
			
			for (int j=0; j<TotSites-1; j++)
			{
				if (car[i-1][j] > 0)
				{
					if (car[i-1][j+1] == 0 && Probability(p)==true)
					{
						car[i][j+1] = 1;
						car[i][j] = 0;
					}
					else
						car[i][j] = 1;
				}
			}	
			if (car[i-1][TotSites-1] > 0)
			{
				if (Probability(beta)==true)
					car[i][TotSites-1] = 0;
				else
					car[i][TotSites-1] = 1;
			}
		}
//Printing
/*
		for (int i=0; i<Tmax; i++)
		{
			for (int j=0; j<TotSites; j++)
			{
				if (car[i][j] > 0)
					cout << car[i][j]<<" ";
				else
					cout << ". ";
			}
			cout << "\n";
		}
*/	
		for (int j=0; j<TotSites; j++)
		{		
			temp[j]=0;
			for (int i=1; i<Tmax; i++)
				if (car[i][j] > 0)
					temp[j]++;
			temp[j]/=float(Tmax);
			CarDensity[j]+=temp[j];			

		}
	}
	for (int i=0; i<TotSites; i++)
	{
		CarDensity[i]/=float(N);
		cout<< "\t\tDensity = "<<CarDensity[i]<<"\n";
	}
	
	//Plotting
	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,800,800);
	//    c1->SetGrid();
    TGraph *step;
	step = new TGraph();
	for (int j=0; j<TotSites; j++)  
        step->SetPoint(j,j,CarDensity[j]); 

	auto* legend = new TLegend(0.1,0.7,0.48,0.9);
	legend->AddEntry(step, " p (brake) = 0.2", "s");
	legend->AddEntry(step, " #alpha = 0.15", "s");
	legend->AddEntry(step, " #beta = 0.10", "s");
    step->SetMarkerStyle(3);
    step->SetLineColor(11);
    step->SetTitle(";Position->;Density->");
    step->GetYaxis()->SetRangeUser(0,1);
    step->SetLineWidth(5);
    step->Draw("same AC");
	legend->Draw("same");
	
	return 0;
}
