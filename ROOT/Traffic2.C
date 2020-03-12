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
// ************************************************************************
//

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>

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
    {
    	VehicalID = 0;
    } 
    Vehical(int id)
    {
    	VehicalID = id;
    }
//Setter Functions    
    void SetCarData(int i, int t,int x, int v)
    {
    	CarData[i].Time = t;
    	CarData[i].Position = x;
    	CarData[i].Velocity = v;
    }   
    void SetVehicalID(int id)
    {
    	VehicalID = id;
    }   
    
//Getter functions    
    int GetVehicalID()
    {
    	return VehicalID;
    }   
    int GetCarTime(int i)
    {
    	return CarData[i].Time;
    }   
    int GetCarPosition(int i)
    {
    	return CarData[i].Position;
    }
    int GetCarVelocity(int i)
    {
    	return CarData[i].Velocity;
    }
    
// Print Function
	void PrintCarInfo(int i)
	{
		cout << " \nTime\tPosition\tvelocity\n " << endl;
		cout << CarData[i].Time <<"\t"<< CarData[i].Position <<"\t"<< CarData[i].Velocity << endl;
	}        
};

//Probability Function
bool Probability(float f)
{
	if (rand()%100 < f*100)
		return true;
	else
		return false;
}


int Traffic2()
{
	
	srand(time(NULL));
	cout<<"This should work!!!\n\n\n";
	int TotCars = 9;
	int TotSites = 70;
	int Tmax = 30;
	int Vmax = 5;
	float p = 0.5;
	int x2=0,x=0,v=0,d=0;
	Vehical car[TotCars];
//Initialisation Part
	
	car[0].SetCarData(0,0,9,4);
	car[1].SetCarData(0,0,26,5);	
	car[2].SetCarData(0,0,33,5);
	car[3].SetCarData(0,0,40,5);
	car[4].SetCarData(0,0,49,0);	
	car[5].SetCarData(0,0,50,0);
	car[6].SetCarData(0,0,52,1);
	car[7].SetCarData(0,0,60,4);
	car[8].SetCarData(0,0,70,5);
	
//	car[0].SetCarData(0,0,3,5);
//	car[1].SetCarData(0,0,8,4);	
//	car[2].SetCarData(0,0,14,0);
//	car[3].SetCarData(0,0,15,0);
//	car[4].SetCarData(0,0,18,3);
			

	for (int i = 1; i <= Tmax; i++)
	{
		for (int j = 0; j < TotCars; j++ )
		{
			x = car[j].CarData[i-1].Position;
			v = car[j].CarData[i-1].Velocity;
			
			if (j+1 == TotCars)
				x2 = car[0].CarData[i-1].Position;
			else
				x2 = car[j+1].CarData[i-1].Position;
			if (x2 - x < 0)
				d = TotSites - x + x2;
			else
				d = x2 - x;

//STEP 1	
			if (v < Vmax && d > v + 1 )
					v = v + 1; 
//STEP 2		
			if (d <= v && v > 0 )
				v = d - 1;
//STEP 3
			if (Probability(p) == true && v > 0)
				v = v - 1;
//STEP 4
			if (x+v <= TotSites)
				x = x+v;
			else
				x = v - (TotSites - x);
				
			car[j].SetCarData(i,i,x,v);						

			
		}//LOOP FOR CARS
	}//LOOP FOR TIME
	
int flag =0;	



	
//Display	
	for (int i=0; i<= Tmax; i++)
	{
		for (int j=1; j<=TotSites; j++)
		{	
			flag=0;
			for (int k=0; k<TotCars; k++)
			{
				if (car[k].CarData[i].Position == j)
				{
					flag=1;
					cout << car[k].CarData[i].Velocity << "";
					break;
				}
			}//for comparing every car's position
			
			if (flag == 0)
				cout << "." ;
			
		}// for every site
		
		cout << "\n";
	}//For all time values
	
	
		
	
//Plotting
	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,800,800);
//    c1->SetGrid();
    TGraph *step[TotCars];
    TMultiGraph *mg = new TMultiGraph();
	for (int i=0; i<TotCars; i++)
	{
	    step[i] = new TGraph();
	    for (int j=0; j<=Tmax; j++)
        {   
            step[i]->SetPoint(j,car[i].CarData[j].Position,Tmax-car[i].CarData[j].Time);
            //step[i]->Draw("same A*");
            step[i]->SetMarkerStyle(i*2+1);
            c1->Update();

        } 
        mg->Add(step[i]); 
        step[i]->SetMarkerStyle(20+i);
        step[i]->SetMarkerColor(i+1);        
	}
	mg->Draw("A pm p");
	ShiftXAxis(mg,TotSites);
	ReverseYAxis(mg);
	return 0;
}


