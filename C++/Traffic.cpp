#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>

using namespace std;

class TimeStamp
{
	private:
	int AtTime;
	int Position;
	int Velocity;
	
	public:
	TimeStamp()
	{
		AtTime = 0;
		Position = 0;
		Velocity = 0;
	}
	
	TimeStamp(int t, int x, int v)
	{
		AtTime = t;
		Position = x;
		Velocity = v;
	}
	
	void SetTime(int t)
	{
		AtTime = t;
	}
	
	void SetPosition(int x)
	{
		Position = x;
	}
	
	void SetVelocity(int v)
	{
		Velocity = v;
	}
	
	int GetTime()
	{
		return AtTime;
	}
	
	int GetPosition()
	{
		return Position;
	}
	
	int GetVelocity()
	{
		return Velocity;
	}
	
	void Print()
	{
		cout << " \nTime\tPosition\tvelocity\n " << endl;
		cout << AtTime <<"\t\t\t"<< Position <<"\t\t\t"<< Velocity << endl;
	}
	
};


class Vehical: public TimeStamp
{
    private:
    int VehicalID;
    TimeStamp CarData[100];
    
    public:
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
    	CarData[i].SetTime(t);
    	CarData[i].SetPosition(x);
    	CarData[i].SetVelocity(v);
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
    	return CarData[i].GetTime();
    }   
    int GetCarPosition(int i)
    {
    	return CarData[i].GetPosition();
    }
    int GetCarVelocity(int i)
    {
    	return CarData[i].GetVelocity();
    }
    
// Print Function
	void PrintCarInfo(int i)
	{
		CarData[i].Print();
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


int main()
{
	
	srand(time(NULL));
	cout<<"This should work!!!\n\n\n";
	int TotCars = 5;
	int TotSites = 20;
	int Tmax = 30;
	int Vmax = 5;
	float p = 0.5;
	int x2=0,x=0,v=0,d=0;
	Vehical car[TotCars];
//Initialisation Part
//	int xfirst
/*
	cars[0].SetCarData(0,0,rand()%Vmax,rand()%Vmax);
	for (int i = 1; i < TotCars; i++)
	{
		v = rand()%Vmax;
		x = car[i-1].GetCarPosition(0) + rand()%car[]
		car[i].SetVehicalID(i+1);
//		cars[i].SetCarData(0,0)
	}	
*/	
	
	car[0].SetCarData(0,0,3,5);
	car[1].SetCarData(0,0,8,4);	
	car[2].SetCarData(0,0,14,0);
	car[3].SetCarData(0,0,15,0);
	car[4].SetCarData(0,0,18,3);	
			

	for (int i = 1; i <= Tmax; i++)
	{
		for (int j = 0; j < TotCars; j++ )
		{
			x = car[j].GetCarPosition(i-1);
			v = car[j].GetCarVelocity(i-1);
			
			if (j+1 == 5)
				x2 = car[0].GetCarPosition(i-1);
			else
				x2 = car[j+1].GetCarPosition(i-1);
			if (x2 - x < 0)
				d = TotSites - x + x2;
			else
				d = x2 - x;

//STEP 1	
			if (v < Vmax && d > v + 1 )
					v = v + 1; 
//STEP 2		
			if (d <= v && v > 0)
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
				if (car[k].GetCarPosition(i) == j)
				{
					flag=1;
					cout << car[k].GetCarVelocity(i) << "\t";
					break;
				}
			}//for comparing every car's position
			
			if (flag == 0)
				cout << ".\t" ;
			
		}// for every site
		
		cout << "\n";
	}//For all time values
	
/*	
	for (int i=0; i<TotCars; i++)
	{
		cout << "Car ID ->>" << car[i].GetVehicalID() << "\n\n" << endl;
		for (int j=0; j<=Tmax; j++)
			car[i].PrintCarInfo(j);
	}
*/	
	
	return 0;
}






























