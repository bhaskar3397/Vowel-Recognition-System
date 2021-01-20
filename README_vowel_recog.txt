==========================================================
Name : Bhaskar Sharma
Roll no. : 204101018
Assignment-3 :Vowel Recognition
==========================================================
"I have faced problem during execution of my program in visual studio 2010,it does not give any output/terminate unexpectedly but it works in other compiler(dev-c++)".
This problem is happening due to stack overflow,since i have implemented too much calculation on the referenced and test data i think it is overflowing.

So it is a humble request to please do one of the following-
1.Please increase the stack size (in visual studio go to project->properties->configuration properties->Linker->system->in right side(stack reserve size)->Set this to 16000000000). 
2.(or)It is a humble request to please excute my code in Dev-C++ to check the output.
========================================================================================================================================================================
Procedure:
1.Firstly i have done 100 recordings (20 for each vowel) taken out its data in .txt file and then i have taken 10 files for reference and 10 files for testing for each vowel.

2.Took 10 reference file for each vowel and then created a average reference file which consists of 5 rows and 12 columns of Cis.
	a)Here first,i took raw data then i applied DC-shift,normalization and hamming window to it.

    	b)After that ,i took the 5 stable frames(which have high energy) from the data we have.

        c)Then calculated Ris,Ais and Cis(after applying raised sine window) for each frame of each reference file.

        d)And after going through 10 reference files for each vowel i calculated the average Cis and stored it into an array.

        e)So now,i will have 5 reference file through which i will compare the test data.	

3.Then i have provided two options to implement vowel recognition i.e. With or Without Recording Module.

4.If User choose With Recording module option then it will create "inputfile.txt" and put all the data into it and we will test which vowel has been spoken by user.

5.But if user want to go without recording module then it will take 50 test files (which i have put in debug folder) and test those files one by one.
	a)In testing,firstly i have applied the DC-shift,normalization and hamming window to it.

	b)Then calculated the Cis(after applying the raised sine window) for the test file and stored it into an array.

	c)After that i have compared my test file data with the referenced file data to find the tokhura distance.

	d)Took my test file,for each frame calculated the Tokhura distance using the referenced file,summed up the distance for 60 Cis and stored it into an array.

	e)Which ever tokhura distance is smallest it will predict that vowel.
========================================================================================================================================
How to run the code?
1.Run the .cpp file.
2.Wait for 5-6 seconds after the program start executing (as we are implementing the reference file and test file in same code,it takes time to show the output).
3.After that you will be given two options to test the data(with or without recording module).
4.if you choose recording module,it will prompt the output for that and get executed.
5.And if you choose without recording module then it will display the output on the 50 test files.
   