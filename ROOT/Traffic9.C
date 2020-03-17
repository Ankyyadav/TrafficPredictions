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
// *                --  Density plots for traffic signal on single lane   *
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


const int TotSites = 20;
int Traffic9()
{
	
	srand(time(NULL));
	cout<<"This should work!!!\n\n\n";
	int TotCars = 9;

	int Tmax = 100;
	int Vmax = 5;
	float p = 1.0/64.0;
	float pv = 0;
	float p0 = 0.75;
	p0=p=0.75;
	int TimePerTSignal = 10;
	int TSignalLength = 5;
	bool TSignal = false;
	int TSignalPos = 40;
	float CarDensity[TotSites] = {0} ;
	float temp[TotSites];
	int N = 1000;
	
	int x=0,v=0,d=0,temp1=0,id1=0,id2=0,id=0;
	Vehical car[TotCars];
//Initialisation Part
	
	
	cout << "\nCar Density ->  " << float(TotCars)/float(TotSites);
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
	
//	car[0].SetCarData(0,0,3,5);
//	car[1].SetCarData(0,0,8,4);	
//	car[2].SetCarData(0,0,14,0);
//	car[3].SetCarData(0,0,15,0);
//	car[4].SetCarData(0,0,18,3);
			
for (int k=0; k<N; k++)
{
    x=v=d=id=temp1=id1=id2=0;
	for (int i = 1; i <= Tmax; i++)
	{
		if (i % TimePerTSignal >=0 && i % TimePerTSignal < TSignalLength )
        {
            TSignal = true;
            temp1 = Distance(car,i-1,TSignalPos,TotCars,0,TotSites,true,id1);
        }
	
	
		for (int j = 0; j < TotCars; j++ )
		{
			x = car[j].CarData[i-1].Position;
			v = car[j].CarData[i-1].Velocity;
			
			if (TSignal == true && id1 == j && temp1 >0 && temp1 <= v)
                v = temp1;
            else if (TSignal == true && id1 == j && temp1 ==0 && temp1 <= v)
                v = 0;
            else
            {
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
				if (Probability(p) == true && v > 0)
					v = v - 1;
//STEP 4
			}
				if (x+v <= TotSites)
					x = x+v;
				else
					x = v - (TotSites - x);
			
				
			car[j].SetCarData(i,i,x,v);						

			
		}//LOOP FOR CARS
		TSignal = false;
	}//LOOP FOR TIME
	
int flag =0;	



/*	
//Display	
	for (int i=0; i<= Tmax; i++)
	{
		if (i % TimePerTSignal >=0 && i % TimePerTSignal < TSignalLength  && i>0)
		    TSignal = true;
		for (int j=1; j<=TotSites; j++)
		{	
		
			if (j == TSignalPos + 1 && TSignal == true)
		        cout << " R ";
		    else if(j == TSignalPos + 1 && TSignal == false)
		        cout << " G ";
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
		TSignal = false;
		cout << "\n";
	}//For all time values
*/	
	
	
	for (int j=1; j<=TotSites; j++)
	{	
		temp[j-1]=0;
		for (int i=0; i<= Tmax; i++)
		{
			for (int k=0; k<TotCars; k++)
			{
				if (car[k].CarData[i].Position == j)
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
	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,800,800);
	//    c1->SetGrid();
    TGraph *step;
	step = new TGraph();
	for (int j=0; j<TotSites; j++)  
        step->SetPoint(j,j,CarDensity[j]); 

	auto* legend = new TLegend(0.1,0.7,0.48,0.9);
	legend->AddEntry(step, " p (brake) = 0.3", "s");
	legend->AddEntry(step, " Traffic Signal between 10^{th} and 11^{th} cell", "-");
	legend->AddEntry(step, " turns on every 15 sec for 5 sec", "-");
	legend->AddEntry(step, " Total number of cars fixed (4)", "-");	
    step->SetMarkerStyle(3);
//    step->SetLineColor(4);
    step->SetTitle(";Position->;Density->");
    step->GetYaxis()->SetRangeUser(0,1);
    step->SetLineWidth(5);
    step->Draw("same AC");
	legend->Draw("same");
		
/*	
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
        step[i]->SetMarkerColor(1);        
	}
	mg->Draw("A pm p");
	ShiftXAxis(mg,TotSites);
	ReverseYAxis(mg);
	
*/
	return 0;
}



