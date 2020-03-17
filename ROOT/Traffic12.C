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
// *                --  Flow vs Density Plots                             *
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
    TimeStamp CarData[500];
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

int Traffic12()
{
	
	srand(time(NULL));
	cout<<"This should work!!!\n\n\n";
	int TotCars = 10;

	int Tmax = 500;
	int Vmax = 5;
	float p = 1.0/64.0;
	float pv = 0;
	float p0 = 0.75;
//	p0=p=0.75;
//	p0=p=1.0/64.0;
	int N = 1000;
	float density[10] = {0};
	float flow[10] = {0};
	float temp[10] = {0};
	int TotSites = 0;
	
	int x=0,v=0,d=0,temp1=0,id1=0,id2=0,id=0,space=0,flag=0;
	Vehical car[TotCars];
//Initialisation Part
	
	

	cout << "\nMaximum Velocity ->  "<< Vmax << "\nTotal time steps ->  " << Tmax << endl;
	cout << "Probability of random braking ->  " << p <<endl;
	car[0].SetCarData(0,0,1,0);
	car[1].SetCarData(0,0,2,0);	
	car[2].SetCarData(0,0,3,0);
	car[3].SetCarData(0,0,4,0);
	car[4].SetCarData(0,0,5,0);	
	car[5].SetCarData(0,0,6,0);
	car[6].SetCarData(0,0,7,0);
	car[7].SetCarData(0,0,8,0);
	car[8].SetCarData(0,0,9,0);
	car[9].SetCarData(0,0,10,0);
	
	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,800,800);
    TGraph *step;
    step = new TGraph();		
		
		for (int m=0; m<N; m++)	
		{
	
			for (int k=0; k<10; k++)
			{
    
    			x=v=d=id=temp1=id1=id2=0;
    			TotSites = TotCars*(k+1);
    			density[k] = float(TotCars)/float(TotSites);
    			space=TotSites/TotCars;
    			
    			for (int init=0; init<TotCars; init++)
    				car[init].SetCarData(0,0,init*space+1,Vmax);
    			
    			
				for (int i = 1; i <= Tmax; i++)
				{
						for (int j = 0; j < TotCars; j++ )
						{
							x = car[j].CarData[i-1].Position;
							v = car[j].CarData[i-1].Velocity;
            				d = Distance(car,i-1,x,TotCars,1,TotSites,false,id);

//STEP 0
							if (v == 0)
								pv = p0;
							else
								pv = p;
//STEP 1			
							if (v < Vmax && d > v + 1 )
								v = v + 1; 
//STEP 2		
							if (d <= v && v > 0 )
								v = d - 1;
//STEP 3
							if (Probability(pv) == true && v > 0)
								v = v - 1;
//FOR FLOW
							if (x<=10 && x+v>10)
								temp[k]++;
//STEP 4								
							if (x+v <= TotSites)
								x = x+v;
							else
								x = v - (TotSites - x);
			
				
							car[j].SetCarData(i,i,x,v);						

			
						}//LOOP FOR CARS
				}//LOOP FOR TIME
				
				/*
				for (int i=0; i<= Tmax; i++)
				{
					for (int j=TotSites; j>=1; j--)
					{	
						flag=0;
						for (int n=0; n<TotCars; n++)
						{
							if (car[n].CarData[i].Position == j)
							{
								flag=1;
								cout << "" << car[n].CarData[i].Velocity ;
								break;
							}				
						}//for comparing every car's position
			
						if (flag == 0)
						cout << "." ;
					}
					cout <<"\n";
				}//For all time values
				
				*/
				
				temp[k]=temp[k]/float(Tmax);
				flow[k]=flow[k]+temp[k];
//				cout << "\nCar Density ->  " << density[k];
//				cout << "\nFlow Rate ->  " << temp[k] << endl;
				temp[k]=0;
				}	
				

		}
		
		for (int i=0; i<10; i++)
			step->SetPoint(i,density[i],flow[i]/float(N));
		step->SetPoint(10,0,0);
		TFile *file =new TFile("NaSch3.root","RECREATE","p_075");
		
		step->Draw("same AL");
		step->SetMarkerStyle(3);
		step->SetLineColor(3);
		step->SetTitle(";Density(#rho)->;Flow Rate(j)->");
		step->GetYaxis()->SetRangeUser(0,1);
		step->GetXaxis()->SetLimits(0,1);
		step->SetLineWidth(5);
		auto* legend = new TLegend(0.1,0.7,0.48,0.9);
		//legend->AddEntry(step, "NaSch: p = 1/64", "s");
		legend->AddEntry(step, "NaSch: p = 0.75", "s");
		legend->Draw("same");
		step->Write("Free_VDR");
		file->Close();


return 0;
}



