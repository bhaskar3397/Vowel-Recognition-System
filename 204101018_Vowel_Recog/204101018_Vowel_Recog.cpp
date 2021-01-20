
#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;


//C_avg finds the average of all the Cis for each referenced file
//It takes 10 referece file per vowel and then generate the one average referenced file 
long double avg[60];
long double *C_avg(long double a1[],long double a2[],long double a3[],long double a4[],long double a5[],long double a6[],long double a7[],long double a8[],long double a9[],long double a10[])
{

	
	for(int i = 0 ; i < 60 ; i++)
	{
		avg[i] = (a1[i]+a2[i]+a3[i]+a4[i]+a5[i]+a6[i]+a7[i]+a8[i]+a9[i]+a10[i])/10;
	}

return avg;
}


//Lev_durbin calculates the Ais and Cis values and return the Cis at the end
long double Cis[13];
long double * Lev_durbin(long double k[])
{

	long double Ris[13];                          //holds the Ri values
	long double ais[13];                         //holds the ai values
	long double a[13][13];                      //holds the intermediate ais
	long double E[13];
	long double value = 0;
	int p = 12;

	for(int h = 0 ; h < 13;h++)
	{
		Ris[h] = k[h];
	}
	
	E[0] = Ris[0];
	
	for( int i = 1 ; i <= p ; i++)
	{   
	    value = 0;	
		for(int j = 1 ; j <= i-1 ; j++)
		{
				value = value + (a[j][i-1]*Ris[i-j]);
		}
		
		a[i][i]= (Ris[i] - value)/E[i-1];
		
		for( int j = 1 ;j < i; j++)
		{
			a[j][i] = a[j][i-1] - (a[i][i] * a[i-j][i-1]);	
		}
			
		E[i] = E[i-1]* (1 - pow(a[i][i],2));
		
	}
	
	//Here it will hold 12 Ais
	for( int i = 1 ; i <= p ; i++)
	{	
	ais[i] = a[i][p]; 			
	}

	Cis[0] = log(Ris[0]*Ris[0]);

	long double sum = 0 ;
	long double q;
	for( int m = 1 ; m <= p;m++)
	{
		sum = 0;
		for (int k = 1; k <= m -1 ; k++)
		{
			q = (long double)k/(long double)m;
			sum = sum + q*Cis[k]*ais[m-k];	
		}
		
		//Here we will get the 12 Cis using Ais
		Cis[m] = ais[m] + sum;
	}

	//Applying the Raised Sine Window to multiply the cis
	long double raisedsin[13];
	int counter = 0;
	
	ifstream f3;

	f3.open("raisedsinewindow.txt");
	
	while(!f3.eof())
	{
		f3 >> raisedsin[counter];
		counter++;
	}
	
	for(int i = 0 ; i<13;i++)
	{   
	    //multiplying Ci values with the raised sine window
		Cis[i] = Cis[i]*raisedsin[i];
	}

	return Cis;
}

//This function calculates the Ris which we will use to calculate the Ais and Cis
long double Ris[13];
long double *calculate_Ris(long double a[])
{
	
	long double Amp[320] ;

	for( int i = 0 ; i < 320 ; i++)
	{
		Amp[i] = a[i];
	}
	
	for( int i = 0 ; i < 13 ; i++)
	{
		Ris[i] = 0;
	}
	
	//calculating Ris for each frame using the amplitude values for 320 samples	
	for(int i = 0 ; i < 13 ; i++)
	{
		for(int j = 0 ; j <= 320-1-i ; j++)
		{
			Ris[i] = Ris[i] + Amp[j]*Amp[j+i];
		}
	}

    return Ris;
}

//calculate_Cis calculates the 13 Ris then pass it into the Lev_durbin function
//to calculate the Ais and Cis for each stable frame of the vowel 
long double final_Cis[60];
long double *calculate_Cis(long double A[][320])
{
	for(int i=0;i<60;i++)
	{
		final_Cis[i]=0;
		
	}

	//These variables hold the Ri values for each frame(1,2,3,4,5) respectively
	long double *Ris,ris_1[13],ris_2[13],ris_3[13],ris_4[13],ris_5[13];
	//long double *Ref[5];
    
	//calculating Ris for first frame
	Ris = calculate_Ris(A[0]);
	for(int i = 0 ; i < 13;i++)
	{
	ris_1[i] = Ris[i];
	//cout<<ris_1[i]<<" "; 
	}
	//cout<<endl;
	
	//calculating Ris for second frame
	Ris = calculate_Ris(A[1]);
	for(int i = 0 ; i < 13;i++)
	{
	ris_2[i] = Ris[i];
	}
    
    //calculating Ris for third frame
	Ris = calculate_Ris(A[2]);
	for(int i = 0 ; i < 13;i++)
	{
	ris_3[i] = Ris[i];
	}
    
    //calculating Ris for fourth frame
	Ris = calculate_Ris(A[3]);
	for(int i = 0 ; i < 13;i++)
	{
	ris_4[i] = Ris[i];
	}
    
    //calculating Ris for fifth frame
	Ris = calculate_Ris(A[4]);
	for(int i = 0 ; i < 13;i++)
	{
	ris_5[i] = Ris[i];
	}


    
	int c1=0;
    long double *cis;
	long double ci1[13],ci2[13],ci3[13],ci4[13],ci5[13];
	
	//calculating Cis for first frame
	cis = Lev_durbin(ris_1);
	for(int i = 1 ; i < 13;i++)
	{
		ci1[i] = cis[i];
		final_Cis[c1]=ci1[i];
		c1++;
	}
	
	//calculating Cis for second frame
	cis = Lev_durbin(ris_2);
	for(int i = 1 ; i < 13;i++)
	{
		ci2[i] = cis[i];
		final_Cis[c1]=ci2[i];
		c1++;
	}

	//calculating Cis for third frame
	cis = Lev_durbin(ris_3);
	for(int i = 1 ; i < 13;i++)
	{
		ci3[i] = cis[i];
		final_Cis[c1]=ci3[i];
		c1++;
	}
    
    //calculating Cis for fourth frame
	cis = Lev_durbin(ris_4);
	for(int i = 1 ; i < 13;i++)
	{
		ci4[i] = cis[i];
		final_Cis[c1]=ci4[i];
		c1++;
	}

	//calculating Cis for fifth frame
	cis = Lev_durbin(ris_5);
	for(int i = 1 ; i < 13;i++)
	{
		ci5[i] = cis[i];
	    final_Cis[c1]=ci5[i];
		c1++;
	}
	
	return final_Cis;
}


//The main function will implement the calculation on the data of refernce file and text file  
int _tmain(int argc, _TCHAR* argv[])
{   
	cout <<"--------------------------------------------------------------------------"<<endl;
	cout <<"NAME :- BHASKAR SHARMA"<<endl<<"ROLL NO :- 204101018"<<endl<<"M.tech CSE(1st YEAR)"<<endl<<"Assignment-3 :Vowel Recognition"<<endl;
	cout<<"(Please wait 5-7 seconds to get the options on screen)"<<endl;
	cout <<"--------------------------------------------------------------------------"<<endl;
	
	//count variable will keep track of how many files we have seen till now
    int count=0;	
    
    //Creating 50 objects of ifstream
    ifstream myfile[50];
    
    //Here a1,a2,a3......a10 holds the cis for 10 reference file for each vowel
    //A_cis,E_cis,I_cis,O_cis,U_cis holds the average reference file for A,E,I,O,U respectively
	long double *a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9,*a10,A_cis[60],E_cis[60],I_cis[60],O_cis[60],U_cis[60];
	
	//these pointers will hold the intermeditate ci values for A,E,I,O,U
	long double *AA,*EE,*II,*UU,*OO;
	
                  
	 //The array of Reference file holds the 50 reference text file ,10 files for one vowel                  
	 string Ref_file[50]={"204101018_a_1.txt","204101018_a_2.txt","204101018_a_3.txt","204101018_a_4.txt","204101018_a_5.txt","204101018_a_6.txt","204101018_a_7.txt","204101018_a_8.txt","204101018_a_9.txt","204101018_a_10.txt",
               	           "204101018_e_1.txt","204101018_e_2.txt","204101018_e_3.txt","204101018_e_4.txt","204101018_e_5.txt","204101018_e_6.txt","204101018_e_7.txt","204101018_e_8.txt","204101018_e_9.txt","204101018_e_10.txt",
               	           "204101018_i_1.txt","204101018_i_2.txt","204101018_i_3.txt","204101018_i_4.txt","204101018_i_5.txt","204101018_i_6.txt","204101018_i_7.txt","204101018_i_8.txt","204101018_i_9.txt","204101018_i_10.txt",
               	           "204101018_o_1.txt","204101018_o_2.txt","204101018_o_3.txt","204101018_o_4.txt","204101018_o_5.txt","204101018_o_6.txt","204101018_o_7.txt","204101018_o_8.txt","204101018_o_9.txt","204101018_o_10.txt",
               	           "204101018_u_1.txt","204101018_u_2.txt","204101018_u_3.txt","204101018_u_4.txt","204101018_u_5.txt","204101018_u_6.txt","204101018_u_7.txt","204101018_u_8.txt","204101018_u_9.txt","204101018_u_10.txt",
	
	 	                 };
	                          
						          
	//This for loop is fetching the reference file one by one then call the Intermediate() function to calculate the Cis
	//And after seeing the 10 files for each vowel it will create an average reference file for each vowel 
	for(int op=0; op<50; op++)
	{
         
	    int X;
	    //this array holds the amplitude of the signal
        long double data[100000];
        
		//this variable holds the maximum amplitude for a file
        long double maximum = 0;
    
     	for(int i = 0; i < 100000;i++)
		{
			data[i] = 0;
		}
 		
 		//Here we are opening each file one by one
        myfile[op].open(Ref_file[op]);
	
		
	    int i=0;
	    //Reading the data from reference file and storing the amplitudes into data[]	
	 	while(!myfile[op].eof())
	 	{
	 		myfile[op] >> data[i];
	 		i++;
	
	 	}
	 	
        //This will keep track of total number of samples we have
	    int Total_Length = i;
        long double sum=0;
        
        //Here i am applying the DC-Shift
		for(int i=0;i<1000;i++)
		{
			sum+=data[i];
		}
		
		sum=sum/1000;

		for( int j = 0 ; j < Total_Length ; j++)
		{
			data[i]-=sum;
		}
	    
	    //choosing the maximum amplitude for nomalization
	 	for(int i=0 ;i<Total_Length; i++)
		{
		maximum = maximum > fabs(data[i]) ? maximum : data[i];		
	    }
	
   		//Applying the normalization
	    for( int j = 0 ; j < Total_Length ; j++)
		{
			data[j] = data[j]*(10000/maximum);
		}
         
         
         //Applying the Hamming window
		long double W;
		for(int i=0;i<Total_Length;)
		{
			for(int m=0;m<320;m++)
			{   
			    //this is the formula for hamming window
				W = 0.54 - 0.46*(cos(2*3.14*m/319));
	   			data[i]=data[i]*W;
	   			i++;	
   			}
		}

	 
		//Dividing data into frames
		//frame[] will hold the address of 1st amplitude then 320th amplitude and so on
		long double *frame[180] ;
		for(int i = 0 ; i < 180 ; i++)
		{
			X=320*i;
			frame[i] = &data[X];
		}
         
        
		long double Ref[5][320];
		
		//Taking index of middle frame
		int z = Total_Length/640;
		
		//Here i am taking 5 stable frames
		for(int i = 0 ; i < 5;i++)
		{
			for(int j = 0 ; j < 320; j++)
			{ 
			
			Ref[i][j] = frame[i+z-3][j]; 		 
			}
		}

 		//This will hold the intermediate Cepstral coefficients each time when loop runs
  	    long double *Cepstral;
  	    
  	    //Passing 5 stable frame's data to calculate Cis for each frame
    	Cepstral = calculate_Cis(Ref);
    
        //Keeps counting the number of reference file we have seen till now
        count++;
   
	    //This step is to hold the Cepstral coefficients for 10 reference file for each vowel
	    if(op%10==1) 	  a1=Cepstral;
	 	else if(op%10==2) a2=Cepstral;
		else if(op%10==3) a3=Cepstral;
		else if(op%10==4) a4=Cepstral;
		else if(op%10==5) a5=Cepstral;
		else if(op%10==6) a6=Cepstral;
	    else if(op%10==7) a7=Cepstral;
		else if(op%10==8) a8=Cepstral;
		else if(op%10==9) a9=Cepstral;
		else if(op%10==0) a10=Cepstral;

    	if(count==10)
    	{   //holds the average ci values for vowel-A
		    AA = C_avg(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
	
	 		for(int i=0;i<60;i++)
   			{
   				A_cis[i]=AA[i] ;	   
		   	}
		}

   	
    	else if(count==20)
    	{	//holds the average ci values for vowel-E
			EE=C_avg(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
			
			for(int i=0;i<60;i++)
   			{
   				E_cis[i]=EE[i];	   
		    }
	
	   }
   
		else if(count==30)
 		{    //holds the average ci values for vowel-I
			 II=C_avg(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
		 	
			  for(int i=0;i<60;i++)
   			{	
   			 	I_cis[i]=II[i];
		   	}
		}
		else if(count==40)
		{   //holds the average ci values for vowel-O
			OO=C_avg(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
			
			for(int i=0;i<60;i++)
   			{
   	   			O_cis[i]=OO[i];	   
		   }
		}
		
		else if(count==50)
		{   //holds the average ci values for vowel-U
			UU=C_avg(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
			for(int i=0;i<60;i++)
   			{
   	   			U_cis[i]=UU[i];
   		 
		   }
		}

		   
    
}

//Creating 50 objects of ifstream for test files    
ifstream myfile2[50];

                  
	//The array of Reference file holds the 50 reference text file ,10 files for one vowel                    
	string Test_file[50]={"204101018_a_11.txt","204101018_a_12.txt","204101018_a_13.txt","204101018_a_14.txt","204101018_a_15.txt","204101018_a_16.txt","204101018_a_17.txt","204101018_a_18.txt","204101018_a_19.txt","204101018_a_20.txt",
               	           "204101018_e_11.txt","204101018_e_12.txt","204101018_e_13.txt","204101018_e_14.txt","204101018_e_15.txt","204101018_e_16.txt","204101018_e_17.txt","204101018_e_18.txt","204101018_e_19.txt","204101018_e_20.txt",
               	           "204101018_i_11.txt","204101018_i_12.txt","204101018_i_13.txt","204101018_i_14.txt","204101018_i_15.txt","204101018_i_16.txt","204101018_i_17.txt","204101018_i_18.txt","204101018_i_19.txt","204101018_i_20.txt",
               	           "204101018_o_11.txt","204101018_o_12.txt","204101018_o_13.txt","204101018_o_14.txt","204101018_o_15.txt","204101018_o_16.txt","204101018_o_17.txt","204101018_o_18.txt","204101018_o_19.txt","204101018_o_20.txt",
               	           "204101018_u_11.txt","204101018_u_12.txt","204101018_u_13.txt","204101018_u_14.txt","204101018_u_15.txt","204101018_u_16.txt","204101018_u_17.txt","204101018_u_18.txt","204101018_u_19.txt","204101018_u_20.txt",
	
	                  };
	                  
	                  
	                  
	int io;
	                  
	cout <<"Press 1 to implement vowel recognition on Test files"<<endl;
	cout <<"Press 2 to implement vowel recognition using recording modeule"<<endl;
	cin >> io; 
	cout<<"--------------------------------------------------------------------------------------"<<endl;
	
	int input;      
	ifstream f;
	   
	switch(io) 
	 {
	    case 1 :input=1;
	 	        break;
	
	 	case 2 :input=2;
	 	        system("Recording_Module.exe 2 inputfile.wav inputfile.txt");
			    f.open("inputfile.txt");
				break;
	  }                
	                  
	                  
	 				   
    //This for loop is fetching the test file one by one then call the calculate_Cis() function to calculate the Cis 
    //for each file then calculate the Tokhura distance and tells which vowel has been spoken
    for( int op1=0;op1<50;op1++)
    {	 
    
		int X;
	    //this array holds the amplitude of the test file
        long double data[100000];
        
		//this variable holds the maximum amplitude for a test file
        long double maximum = 0;
    
     	for(int i = 0; i < 100000;i++)
		{
			data[i] = 0;
		}
 		
 		//Here we are opening each test file one by one
        myfile2[op1].open(Test_file[op1]);
         
		 string x=Test_file[op1];
	
	    int i=0;
	 if(input==1)
	  {
	      //This is to show the content of test files on screen
		  cout << "Test file is : ";
		  if(op1>=0 && op1<=9)cout<<"A";
		  if(op1>=10 && op1<=19)cout<<"E";
		  if(op1>=20 && op1<=29)cout<<"I";
		  if(op1>=30 && op1<=39)cout<<"O";
		  if(op1>=40 && op1<=49)cout<<"U";
		  cout<<endl;
		
	    //Reading the data from test file and storing the amplitudes into data[]	
	 	while(!myfile2[op1].eof())
	 	{
	 		myfile2[op1] >> data[i];
	 		i++;
	 	}
		 
	  }
	 	
	 if(input==2)
	   {
	 		while(!f.eof())
	 		{
	 			f >> data[i];
	 			i++;
			 }
	 		cout<<"Test file is : inputfile.txt "<< endl;	
	   }
       
        //This will keep track of total number of samples we have in test file
	    int Total_Length = i;
        long double sum=0;
        
        //Here i am applying the DC-Shift on test file
		for(int i=0;i<1000;i++)
		{
			sum+=data[i];
		}
		
		sum=sum/1000;

		for( int j = 0 ; j < Total_Length ; j++)
		{
			data[i]-=sum;
		}
	    
		//choosing the maximum amplitude for nomalization
	 	for(int i=0 ;i<Total_Length; i++)
		{
		maximum = maximum > fabs(data[i]) ? maximum : data[i];		
	    }
	
   		//Applying the normalization on test file
	    for( int j = 0 ; j < Total_Length ; j++)
		{
			data[j] = data[j]*(10000/maximum);
		}
         
         
         //Applying the Hamming window
		long double W;
		for(int i=0;i<Total_Length;)
		{
			for(int m=0;m<320;m++)
			{   
			    //this is the formula for hamming window
				W = 0.54 - 0.46*(cos(2*3.14*m/319));
	   			data[i]=data[i]*W;
	   			i++;	
   			}
		}

	 
		//Dividing data into frames
		//frame[] will hold the address of 1st amplitude then 320th amplitude and so on
		long double *frame[180] ;
		for(int i = 0 ; i < 180 ; i++)
		{
			X=320*i;
			frame[i] = &data[X];
		}
         
        
		long double Ref[5][320];
		
		//Taking index of middle frame
		int z = Total_Length/640;
		
		//Here i am taking 5 stable frames of test file
		for(int i = 0 ; i < 5;i++)
		{
			for(int j = 0 ; j < 320; j++)
			{ 
			
			Ref[i][j] = frame[i+z-3][j]; 		 
			}
		}

 		//This will hold the intermediate Cepstral coefficients each time when loop runs
  	    long double *Cepstral_Test;
  	    
  	    //Passing 5 stable frame's data to calculate Cis for each frame
    	Cepstral_Test = calculate_Cis(Ref);
        
 
		//Storing tokhura weights in an array of size 12	     
    	long double tokhura_weight[12] = { 1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0,61.0};
    	
    	//This array stores the tokhura distance for each frame,means one test file
    	//will be comapred to all the 5 reference file and tokhura distance will be stored into it
		long double tok[25];
		
		//Initializing tok array to zero
		for( int i=0 ; i < 25;i++)
		{
			tok[i]=0;
		}
		
	  	int m,n,o,p,q;
		int j=0;
		long double tokhura_final[5];
	
	//Calculating Tokhura distance for first frame of test data using A's reference file
    	for(  m=0 ; m < 12;m++)
	  	{
			tok[0] = tok[0] + tokhura_weight[j]*pow((A_cis[m] - Cepstral_Test[m]),2);
			j++;
	  	}
		
		j=0;
		for(  ; m < 24; m++)
		{
			tok[1] = tok[1] + tokhura_weight[j]*pow((A_cis[m] - Cepstral_Test[m]),2);
			j++;
		}
    
		j=0;
		for(  ; m < 36;m++)
		{
			tok[2] = tok[2] + tokhura_weight[j]*pow((A_cis[m] - Cepstral_Test[m]),2);
			j++;
		}
    	
		j=0;
		for(  ; m < 48;m++)
		{
		tok[3] = tok[3] + tokhura_weight[j]*pow((A_cis[m] - Cepstral_Test[m]),2);
		j++;
		}

		//Calculating Tokhura distance for fifth frame of test data using A's reference file
		j=0;
		for(  ; m < 60;m++)
		{
			tok[4] = tok[4] + tokhura_weight[j]*pow((A_cis[m] - Cepstral_Test[m]),2);
			j++;
		}
        
        //this holds the final tokhura distance using the A's reference file
		tokhura_final[0]=tok[0]+tok[1]+tok[2]+tok[3]+tok[4];

		//Calculating Tokhura distance for first frame of test data using E's reference file
		j=0;
		for(  n = 0 ; n < 12;n++)
		{
			tok[5] = tok[5] + tokhura_weight[j]*pow((E_cis[n] - Cepstral_Test[n]),2);
			j++;
		}

		j=0;
		for( ; n < 24;n++)
		{
			tok[6] = tok[6] +tokhura_weight[j]*pow((E_cis[n] - Cepstral_Test[n]),2);
			j++;
		}

		j=0;
		for( ; n < 36;n++)
		{
			tok[7] = tok[7] + tokhura_weight[j]*pow((E_cis[n] - Cepstral_Test[n]),2);
			j++;
		}

		j=0;
		for(  ; n < 48;n++)
		{
			tok[8] = tok[8] + tokhura_weight[j]*pow((E_cis[n] - Cepstral_Test[n]),2);
 			j++;
		}
	
  		//Calculating Tokhura distance for fifth frame of test data using E's reference file
		j=0;
		for( ; n < 60;n++)
		{
			tok[9] = tok[9] + tokhura_weight[j]*pow((E_cis[n] - Cepstral_Test[n]),2);
			j++;
		}
		
		//this holds the final tokhura distance using the E's reference file
		tokhura_final[1]=tok[5]+tok[6]+tok[7]+tok[8]+tok[9];	
        
        //Calculating Tokhura distance for first frame of test data using I's reference file
		j=0;
		for( o=0; o< 12;o++)
		{
			tok[10] = tok[10] + tokhura_weight[j]*pow((I_cis[o] - Cepstral_Test[o]),2);
			j++;
		}

		j=0;
		for(  ; o < 24;o++)
		{
			tok[11] = tok[11] + tokhura_weight[j]*pow((I_cis[o] - Cepstral_Test[o]),2);
			j++;
		}

		j=0;
		for( ; o < 36;o++)
		{
			tok[12] = tok[12] + tokhura_weight[j]*pow((I_cis[o] - Cepstral_Test[o]),2);
			j++;
		}

		j=0;
		for(  ; o < 48;o++)
		{
			tok[13] = tok[13] + tokhura_weight[j]*pow((I_cis[o] - Cepstral_Test[o]),2);
			j++;
		}
		
		//Calculating Tokhura distance for fifth frame of test data using I's reference file
		j=0;
		for(  ; o < 60;o++)
		{
			tok[14] = tok[14] + tokhura_weight[j]*pow((I_cis[o] - Cepstral_Test[o]),2);
			j++;
		}
        
        //this holds the final tokhura distance using the I's reference file
		tokhura_final[2]=tok[10]+tok[11]+tok[12]+tok[13]+tok[14];
        
        //Calculating Tokhura distance for first frame of test data using O's reference file
		j=0;
		for( p=0; p < 12;p++)
		{
			tok[15] = tok[15] + tokhura_weight[j]*pow((O_cis[p] - Cepstral_Test[p]),2);
			j++;
		}

		j=0;
		for( ;p < 24;p++)
		{
			tok[16] = tok[16] + tokhura_weight[j]*pow((O_cis[p] - Cepstral_Test[p]),2);
			j++;
		}
	
		j=0;
		for(; p < 36;p++)
		{
			tok[17] = tok[17] + tokhura_weight[j]*pow((O_cis[p] - Cepstral_Test[p]),2);
			j++;
		}
		
		j=0;
		for(  ; p < 48;p++)
		{
			tok[18] = tok[18] + tokhura_weight[j]*pow((O_cis[p] - Cepstral_Test[p]),2);
			j++;
		}
        
        //Calculating Tokhura distance for fifth frame of test data using O's reference file
		j=0;
		for( ; p < 60;p++)
		{
			tok[19] = tok[19] + tokhura_weight[j]*pow((O_cis[p] - Cepstral_Test[p]),2);
			j++;
		}
		
		//this holds the final tokhura distance using the O's reference file
		tokhura_final[3]=tok[15]+tok[16]+tok[17]+tok[18]+tok[19];
        
        //Calculating Tokhura distance for first frame of test data using U's reference file
		j=0;
		for( q = 0 ; q < 12;q++)
		{
			tok[20] = tok[20] + tokhura_weight[j]*pow((U_cis[q] - Cepstral_Test[q]),2);
			j++;
		}

		j=0;
		for( ; q < 24;q++)
		{
			tok[21] = tok[21] + tokhura_weight[j]*pow((U_cis[q] - Cepstral_Test[q]),2);
			j++;
		}

		j=0;
		for(  ; q < 36;q++)
		{
			tok[22] = tok[22] + tokhura_weight[j]*pow((U_cis[q] - Cepstral_Test[q]),2);
			j++;
		}

		j=0;
		for(  ; q < 48;q++)
		{
			tok[23] = tok[23] + tokhura_weight[j]*pow((U_cis[q] - Cepstral_Test[q]),2);
			j++;
		}
        
        //Calculating Tokhura distance for fifth frame of test data using U's reference file
		j=0;
		for(  ; q < 60;q++)
		{
			tok[24] = tok[24] + tokhura_weight[j]*pow((U_cis[q] - Cepstral_Test[q]),2);
			j++;
		}
        
        //this holds the final tokhura distance using the U's reference file
		tokhura_final[4]=tok[20]+tok[21]+tok[22]+tok[23]+tok[24];
	
	
	
      	
	   //min variable holds the minimum tokhura distance	
		long double min;
		min = tokhura_final[0];
		int index = 0;

        //Here finding minimum tokhura distance assigning to the variable "min"
		for(int i = 0 ; i < 5;i++)
		{	//cout<< tokhura_final[i];
			if(min > tokhura_final[i])
			{
				index = i;
				min = tokhura_final[i];
			}
		}
        cout<<endl;
        cout<<"Predicted vowel is : ";
        
		//Which ever tokhura distance is minimum it will predict that vowel
		if(index == 0)
		{
			cout<<"A "<<endl;
		}

		else if(index==1)
		{
			cout<<"E "<<endl;
		}
		else if(index== 2)
		{
			cout<<"I "<<endl;
		}
		else if(index ==3)
		{
			cout<<"O"<<endl;
		}
		else 
		{
			cout<<"U"<<endl;
		}	
		
		cout <<"--------------------------------------------------------------------------"<<endl;
		cout << endl;
		
		if(input==2)
		{
		break;}
    
	}
    	
		


return 0;	
}
