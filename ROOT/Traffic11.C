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
// * Extension Type --  NaSch Model in a circle                           *
// *                --  TimeStamp made a structure                        *
// *                --  Plotting added                                    *
// *                --  Two lanes created for two sided traffic           *
// *                --  Different number of cars                          *
// *                --  Overtaking of cars using other lane allowed       *
// *                --  Edited to allow density calculations              *
// ************************************************************************
//



#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<bits/stdc++.h>
#include<algorithm> 

using namespace std;

//Function for plotting
void ReverseYAxis (TMultiGraph *h);
void ReverseYAxis (TMultiGraph *h)
{
   // Remove the current axis
   h->GetYaxis()->SetLabelOffset(999);
   h->GetYaxis()->SetTickLength(0);

   // Redraw the new axis
   gPad->Update();
   TGaxis *newaxis = new TGaxis(gPad->GetUxmin(),
                                gPad->GetUymax(),
                                gPad->GetUxmin()-0.001,
                                gPad->GetUymin(),
                                h->GetYaxis()->GetXmin()-1,
                                h->GetYaxis()->GetXmax()+2,
                                510,"+");
   newaxis->SetLabelOffset(-0.03);
   newaxis->SetTitle(" Time ---->>>");
   newaxis->SetLabelSize(0.035);
   newaxis->Draw();
}


void ShiftXAxis (TMultiGraph *h, int Xmax);
void ShiftXAxis (TMultiGraph *h, int Xmax)
{
   // Remove the current axis
   h->GetXaxis()->SetLabelOffset(999);
   h->GetXaxis()->SetTickLength(0);

   // Redraw the new axis
   gPad->Update();
   TGaxis *newaxis = new TGaxis(gPad->GetUxmin(),
                                gPad->GetUymax(),
                                gPad->GetUxmax(),
                                gPad->GetUymax(),
                                -1,
                                Xmax+2,
                                510,"-");
   newaxis->SetLabelOffset(0.);
   newaxis->SetLabelSize(0.035);
   newaxis->SetTitle(" Position ---->>>");
   newaxis->Draw();
}


//Class for keeping info o One car
struct TimeStamp
{
	int Time;
	int Position;
	int Velocity;
};

//Class to keep track of all cars
class Vehical: public TimeStamp
{
    private:
    int VehicalID;
  
    
    public:
    TimeStamp CarData[100];
//Constructors
    Vehical()
    {VehicalID = 0;} 
    Vehical(int id)
    {VehicalID = id;}
//Setter Functions    
    void SetCarData(int i, int t,int x, int v)
    {
    	CarData[i].Time = t;
    	CarData[i].Position = x;
    	CarData[i].Velocity = v;
    }   
    void SetVehicalID(int id)
    {VehicalID = id;}   
    
//Getter functions    
    int GetVehicalID()
    {return VehicalID;}   
    
    int GetCarTime(int i)
    {return CarData[i].Time;}   
    
    int GetCarPosition(int i)
    {return CarData[i].Position;}
    
    int GetCarVelocity(int i)
    {return CarData[i].Velocity;}
    
// Print Function
	void PrintCarInfo(int i)
	{
		cout << " \nTime\tPosition\tvelocity\n " << endl;
		cout << CarData[i].Time <<"\t"<< CarData[i].Position <<"\t"<< CarData[i].Velocity << endl;
	}        
};

//Probability Function
bool Prob(float f)
{
	if (rand()%100 < f*100)
		return true;
	else
		return false;
}


int Distance(Vehical NewCar[], int t, int x, int Tot, int pos, int Dmax, bool reverse, int &id)
{
    int dInit[Tot][2],temp = 0;
    if(reverse)
        for (int i = 0; i < Tot; i++)
        {
            dInit[i][1]=i;
            if (x-NewCar[i].CarData[t].Position >= 0)
                dInit[i][0] = x-NewCar[i].CarData[t].Position;
            else
                dInit[i][0] = Dmax + x -NewCar[i].CarData[t].Position ;
        }
    else
        for (int i = 0; i < Tot; i++)
        {
            if (NewCar[i].CarData[t].Position-x >= 0)
                dInit[i][0] = NewCar[i].CarData[t].Position-x;
            else
                dInit[i][0] = NewCar[i].CarData[t].Position + Dmax -x;
        }      
    //sort(dInit, dInit + Tot);   
    
    for (int i = 0; i < Tot; i++)
        for (int j = i+1; j < Tot; j++)
            if (dInit[i][0] > dInit[j][0])
            {
                temp = dInit[i][0];  
                dInit[i][0] = dInit[j][0];
                dInit[j][0] = temp;
                temp = dInit[i][1];  
                dInit[i][1] = dInit[j][1];
                dInit[j][1] = temp;
            }
    id = dInit[pos][1];
    return dInit[pos][0];
}

const int TotSites = 30;
int Traffic11()
{
	
	srand(time(NULL));
	cout<<"This should work!!!\n\n";
	int TotCarsUp = 6;
	int TotCarsDown = 3;
	int Tmax = 100;
	int Vmax = 5;
	float pBrake = 0.6;
	float pPass = 0;
	float CarDensity[TotSites] = {0} ;
	float temp[Tmax];
	int N = 1000;
	
	int x2=0,x=0,v=0,d=0,ch1=0,ch2=0,ch3=0,ch4=0,id=0,temp1=0,temp2=0,id1=0,id2=0;
	Vehical carLane1[TotCarsUp];
	Vehical carLane2[TotCarsDown];
//Initialisation Part
	
	carLane1[0].SetCarData(0,0,1,4);
	carLane1[1].SetCarData(0,0,10,5);	
	carLane1[2].SetCarData(0,0,12,0);
	carLane1[3].SetCarData(0,0,15,5);
	carLane1[4].SetCarData(0,0,16,1);	
	carLane1[5].SetCarData(0,0,24,0);
//	carLane1[6].SetCarData(0,0,52,1);
//	carLane1[7].SetCarData(0,0,60,4);
//	carLane1[8].SetCarData(0,0,70,5);
	
	carLane2[0].SetCarData(0,0,3,2);
	carLane2[1].SetCarData(0,0,20,1);	
	carLane2[2].SetCarData(0,0,26,4);
//	carLane2[3].SetCarData(0,0,28,0);
//	carLane2[4].SetCarData(0,0,49,4);	
//	carLane2[5].SetCarData(0,0,53,3);
//	carLane2[6].SetCarData(0,0,58,0);
//	carLane2[7].SetCarData(0,0,59,0);
//	carLane2[8].SetCarData(0,0,62,1);
	

	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,800,800);	
	TMultiGraph *mg = new TMultiGraph();
	TGraph *step[2];
for (int m=0; m<=1; m++)
{
for (int i =0; i<TotSites; i++)
	CarDensity[i] = 0 ;
pPass = float(m);	
for (int k=0; k<N; k++)
{
    x2=x=v=d=ch1=ch2=ch3=ch4=id=temp1=temp2=id1=id2=0;

	for (int i = 1; i <= Tmax; i++)
    {
        for (int j = 0; j < TotCarsUp; j++ )
        {
		    x = carLane1[j].CarData[i-1].Position;
	        v = carLane1[j].CarData[i-1].Velocity;
			
			d = Distance(carLane1,i-1,x,TotCarsUp,1,TotSites,false,id);
//STEP 1	
    	    if (v < Vmax && d > v + 1 )
   		    	v = v + 1; 
//STEP 2  
// + OVERTAKING	
    	    if (d <= v && v > 0 )   	 
    	    {
    	        ch1 = Distance(carLane1,i-1,x+v,TotCarsUp,0,TotSites,true,id);
    	        ch3 = carLane1[id].CarData[i-1].Velocity;
				ch4 = carLane1[id].CarData[i-1].Position;
    	        if ( x + v <= ch4 + ch3 +1)
    	            ch3 = 1;
    	        else
    	        	ch3 = 0;
    	        
    	        ch2 = Distance(carLane2,i-1,TotSites+1-x,TotCarsDown,0,TotSites,true,id);
			if (ch3==1 || ch2 <= 2*Vmax || ch1==0 || Prob(pPass)==false)     
	        	v = d - 1;
	        else 
	        	{cout << "\nOvertaking at  " << i << endl;
	        	ch2 = -1;}
	        }
//STEP 3
	        if (Prob(pBrake) == true && v > 0 && ch2 != -1)
	        	v = v - 1;
//STEP 4
	        if (x+v <= TotSites)
	        	x = x+v;
	        else
	        	x = v - (TotSites - x);
				
	        carLane1[j].SetCarData(i,i,x,v);						
        }
        
        
        
        for (int j = 0; j < TotCarsDown; j++ )
        {
		x = carLane2[j].CarData[i-1].Position;
	        v = carLane2[j].CarData[i-1].Velocity;
			
		d = Distance(carLane2,i-1,x,TotCarsDown,1,TotSites,false,id);
//STEP 1	
    	    if (v < Vmax && d > v + 1 )
   		    	v = v + 1; 
//STEP 2		
// + OVERTAKING	
    	    if (d <= v && v > 0 )   	 
    	    {
    	        ch1 = Distance(carLane2,i-1,x+v,TotCarsDown,0,TotSites,true,id);
    	        ch3 = carLane2[id].CarData[i-1].Velocity;
				ch4 = carLane2[id].CarData[i-1].Position;
    	        if ( x + v <= ch4 + ch3 +1)
    	            ch3 = 1;
    	        
    	        ch2 = Distance(carLane1,i-1,TotSites+1-x,TotCarsDown,0,TotSites,true,id);
    	        if (ch3==1 || ch2 <= 2*Vmax || ch1==0 || Prob(pPass)==false)     
	        	    v = d - 1;
	        	else 
	        	    {cout << "\nOvertaking at  " << i << endl;
	        	     ch2 = -1;}
	        }
//STEP 3
	        if (Prob(pBrake) == true && v > 0)
	        	v = v - 1;
//STEP 4
	        if (x+v <= TotSites)
	        	x = x+v;
	        else
	        	x = v - (TotSites - x);
				
	        carLane2[j].SetCarData(i,i,x,v);						
        }//LOOP FOR CARS
	}//LOOP FOR TIME

/*
int flag =0;	
//Display	
	for (int i=0; i<= Tmax; i++)
	{
		
//For Lane 1
        cout << " --->|";		
		for (int j=1; j<=TotSites; j++)
		{	
			flag=0;
			for (int k=0; k<TotCarsUp; k++)
			{
				if (carLane1[k].CarData[i].Position == j)
				{
					flag=1;
					cout << carLane1[k].CarData[i].Velocity << "";
					break;
				}				
			}//for comparing every car's position
			
			if (flag == 0)
				cout << "." ;
			
		}// for every site
		cout << "| --->\n     ";
		for (int m = 0; m<TotSites+2; m++)
		    cout << "_";
		cout << "\n\n";
		
//For Lane 2	
		cout << " <---|";		
		for (int j=TotSites; j>=1; j--)
		{	
			flag=0;
			for (int k=0; k<TotCarsDown; k++)
			{
				if (carLane2[k].CarData[i].Position == j)
				{
					flag=1;
					cout << "" << carLane2[k].CarData[i].Velocity ;
					break;
				}				
			}//for comparing every car's position
			
			if (flag == 0)
				cout << "." ;
			
		}
		cout << "| <--- At Time t = " << i << "\n\n\n\n";
	}//For all time values
*/

	for (int j=1; j<=TotSites; j++)
	{	
		temp[j-1]=0;
		for (int i=0; i<= Tmax; i++)
		{
			for (int k=0; k<TotCarsUp; k++)
			{
				if (carLane1[k].CarData[i].Position == j)
				{
					temp[j-1]++;
					break;
				}				
			}//for comparing every car's position
		}
		temp[j-1]/=float(Tmax);
		CarDensity[j-1]+=temp[j-1];
	}

}


for (int i=0; i<TotSites; i++)
	{
		CarDensity[i]/=float(N);
		cout<< "\t\tDensity at site "<<i+1<<" = "<<CarDensity[i]<<"\n";
	}


	//Plotting
	//    c1->SetGrid();
	step[m] = new TGraph();
	for (int j=0; j<TotSites; j++)  
        step[m]->SetPoint(j,j,CarDensity[j]); 

	mg->Add(step[m]);
    step[m]->SetMarkerStyle(3);
    step[m]->SetLineColor(m+1);
    step[m]->GetYaxis()->SetRangeUser(0,1);
    step[m]->SetLineWidth(5);
//    step[m]->Draw("same AC");
//	legend[m]->Draw("same");
}
auto* legend = new TLegend(0.1,0.7,0.48,0.9);
legend->AddEntry(step[0], " Without overtake", "l");
legend->AddEntry(step[1], " With overtake allowed with probability 1", "l");
mg->Draw("same AC");
legend->Draw("same");
mg->SetTitle(";Position->;Density->");



/*	
//Plotting
	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,800,800);
//    c1->SetGrid();
    c1->Divide(1,2);
    TGraph *step1[TotCarsUp];
    TGraph *step2[TotCarsDown];
    TMultiGraph *mg1 = new TMultiGraph();
    TMultiGraph *mg2 = new TMultiGraph();
	for (int i=0; i<TotCarsUp; i++)
	{
	    step1[i] = new TGraph();
	    for (int j=0; j<=Tmax; j++)
        {   
            step1[i]->SetPoint(j,carLane1[i].CarData[j].Position,Tmax-carLane1[i].CarData[j].Time);
            step1[i]->SetMarkerStyle(i*2+1);
            c1->Update();
        } 
        mg1->Add(step1[i]);
        step1[i]->SetMarkerStyle(20+i);
        step1[i]->SetMarkerColor(1);  
        step1[i]->SetMarkerSize(1);      
	}
	
	
	for (int i=0; i<TotCarsDown; i++)
	{
	    step2[i] = new TGraph();   
	    for (int j=0; j<=Tmax; j++)
        {   
            step2[i]->SetPoint(j,TotSites-carLane2[i].CarData[j].Position+1,Tmax-carLane2[i].CarData[j].Time);
            step2[i]->SetMarkerStyle(i*2+1);            
            c1->Update();

        }        
        mg2->Add(step2[i]);    
        step2[i]->SetMarkerStyle(20+i);
        step2[i]->SetMarkerColor(1);
        step2[i]->SetMarkerSize(1);                      
	}
	
	c1->cd(1);
	mg1->SetTitle("For cars going left to right --->");
	mg1->Draw("A pm p");
	ShiftXAxis(mg1,TotSites);
	ReverseYAxis(mg1);
	
	c1->cd(2);
	mg2->SetTitle("For cars going to left to right <---");
	mg2->Draw("A pm p");
	ShiftXAxis(mg2,TotSites);
	ReverseYAxis(mg2);	
*/
 return 0;	
}



