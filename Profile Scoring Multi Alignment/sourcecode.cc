// Standard Headers required to run the program

#include <iostream>     //Library that defines the standard input/output stream objects.
#include<string>		//Library to make use of string data type in c++
#include <stdlib.h>     //Library to use random.  
#include <time.h>       //Library defines four variable types, two macro and various functions for manipulating date and time.
#include <limits.h>		//Library determines various properties of the various variable types. 
#include <chrono>		//Library to make use of standard ctime
#include <ctime>		//Library to make use of standard ctime
#include <stdio.h>		//Library to have the standard Input/Output

using namespace std;	//standard c++ library routine used for checking namespace.

bool DEBUG = false;

string BASES = "ACGT";


// The following function is used to copy the blocks we generate.


void copyBlocks(string target_blocks[], string source_blocks[], int block_index, int pos, int N) 
{
	for (int i=0; i < N; i++) 
	{
		char c = source_blocks[block_index + i][pos];
		target_blocks[i].insert(0,1,c);
	}
}


// The following function is used to copy the gaps we generate.


void copyGap(string target_blocks[], int N) 
{
  for (int i=0; i < N; i++) 
  {
	target_blocks[i].insert(0,1,'-');
  }
}


// The following function is used to display the aligned blocks we generate.


void display_aligned_blocks(int L1, int L2, int N, char **dir, string blocks[], int left, int right) 
{
	string aligned_left_blocks[N];
	string aligned_right_blocks[N];
	int i = L1-1;
	int j  = L2-1;
	while(i > 0 && j > 0) 
	{
		switch (dir[i][j]) 
		{
			case 'b': 
						copyBlocks(aligned_left_blocks, blocks, left,i, N);
						copyBlocks(aligned_right_blocks, blocks,right,j, N);
						i--;
						j--;
						break;
			
			case 'l': 
						copyBlocks(aligned_left_blocks, blocks, left,i, N);
						copyGap(aligned_right_blocks, N);
						i--;
						break;
    
			case 'r': 
						copyGap(aligned_left_blocks, N);
						copyBlocks(aligned_right_blocks, blocks,right,j, N);   
						j--;
						break;
		} 
	}
	while (i >= 0) 
	{
		copyBlocks(aligned_left_blocks, blocks, left,i, N);
		i--;
	}
	while (j >= 0) 
	{
		copyBlocks(aligned_right_blocks, blocks,right,j, N);
		j--;
	}
    cout << "Aligned Blocks=" << endl;
    for (int i=0; i<N; i++) 
	{
		cout << aligned_left_blocks[i] << endl;
	}
	cout << endl;
	for (int i=0; i<N; i++) 
	{
		cout << aligned_right_blocks[i] << endl;
	}
}


//1. Randomly produce a string of length L composed of letters A, C, G, and T, each of 25%.


string generateString(int L)
 {
	string block;
	for (int i=0; i< L; i++) 
	{
		/* generate random number between 0 and 3: */
		int r = rand() % 4;
		block += BASES[r];
	}
	return block;
}


//1.2) Replicate N-1 copies of above string. Now you have N strings in total. Line them up as in an alignment block.


string * replicateString(string block, int N) 
{
	string * copies = new string[N];
	for (int i=0; i<N; i++)
	{
		copies[i] = string(block);
	}
	return copies;
}


//1.3) Simulate substitutions: for each string, randomly generate 1%~30% substitutions. Substitutions are uniformly distributed in the string.


string simulateSubstitution(string block) 
{
	/* generate random number between 1 and 30: */
	int r = rand() % 30 + 1;
	int L = block.length();
	int num_substitutions = L * r / 100;
	for (int i=0; i<num_substitutions; i++) 
	{
		/* generate random number between 0 and L */
		int position = rand() % L;
		/* generate random number between 1 and 3 */
		int s = rand() % 3 + 1;
		size_t base_num = BASES.find(block[position]);
		base_num =(base_num + s) % 4;
		block[position]=BASES[base_num];
	}
	return block;
}


/* 1.4) Simulate deletions: for each string, randomly generate 0.2%~3.6% deletions by replacing contiguous segment of letters by ‘- ‘s.
        the gap length is uniformly distributed between 1 and 10 */


string simulateDeletion(string block) 
{
	/* generate random number between 2 and 36: */
	int r = rand() % 35 + 2;
	int L = block.length();
	int num_deletions = L * r / 1000;
	while (num_deletions > 0)
	{
		int max_gap = 10;
		int gap_length = rand() % max_gap + 1;
	    /* generate random number between 0 and L */
		int position = rand() % (L - gap_length);
		/* delete upto gap_length */
		for (int i=0; i < gap_length and num_deletions > 0; i++) 
		{
			block[position + i] = '-';
			num_deletions--;
		}
	}
	return block;
}


//1.5) Divide above alignment block into two, each of N/2 rows. The number of columns of each sub-alignment block at this time is still L.


void simulateInsertion(string block[], int start, int end)
 {
	//In each sub-alignment block, randomly generate 1% insertions:
	int L = block[start].length();
	//cout << "L=" << L << endl;
	int num_insertions = L  / 100;
	//cout << "num_insertions=" << num_insertions << endl;
	while (num_insertions > 0) 
	{
		//1.5.1) Randomly pick a row and a random position.
		/* generate random number between stat and end */
		int row = rand() % (end - start + 1) +  start;
		L = block[start].length();
		/* generate random number between 0 and L*/
		int pos = rand() % L;
	
		
		//1.5.2) Insert a sequence of length x, x is a random number between 1 and 5. The sequence is randomly generated from {A, C, G, T}.
		int x = rand() % 5 + 1;
		string sequence = generateString(x);


//1.5.3) Insert a segment of '-'s of length x in all other rows at the same position to keep all rows of the alignment block with the same number of columns.

		string* gaps = new string();
		for (int i=0; i< x; i++)
		{
			*gaps += '-';
		}
		for (int i=start; i <= end; i++)
		{
			string toInsert = *gaps;
			if (i == row) 
			{
				toInsert = sequence;
			}
			// cout << "toInsert " << toInsert << " at " << pos << endl;
			block[i].insert(pos, toInsert);
		}
		num_insertions = num_insertions - x;
	}

}

int T[5][5] = {{91,-114,-31,-123,-31}, {-114,100,-125,-31,-31}, {-31,-125,100,-114,-31},
{-123, -31,-114,91,-31}, {-31,-31,-31,-31,0}};
string ENTRY = "ACGT-";

long delta(int i, int j, string blocks[], int left, int right, int N) 
{
	long sum = 0;
	for (int m=0; m < N; m++)
	{
		string first = blocks[left + m];
		for (int n=0; n < N; n++)
		{
			string second = blocks[right + n];
			size_t base1 = ENTRY.find(first[i]);
			size_t base2 = ENTRY.find(second[j]);
			sum += T[base1][base2];
			//cout << "bases=" <<first[i] << "," << second[j] << "score=" << T[base1][base2] << endl;
		}
	}
	return sum;
}

long delta1(int i, string blocks[], int start, int N)
{
	long sum = 0;
	for (int m=0; m < N; m++)
	{
		string first = blocks[start + m];
		size_t base1 = ENTRY.find(first[i]);
		sum += T[base1][4];
	}
	return sum;
}


// The following function is used to compute using the technique "Sum of Pair Alignment".


void sumOfPairsAlignment(string blocks[], int left, int right, int N)
{
	int L1 = blocks[left].length();
	int L2 = blocks[right].length();
	long s[L1][L2];
	char **dir;
	dir = new char *[L1];
	for(int i = 0; i <L1; i++)
	{
		dir[i] = new char[L2];
	}
	for (int i=0;i<L1;i++)
	{
		for (int j=0; j<L2; j++)
		{
			s[i][j] = LONG_MIN;
		}
	}
	s[0][0] = 0;
	for (int i=0;i<L1;i++)
	{
		for (int j=0; j<L2; j++)
		{
			if (i>0 && j>0)
			{
				long d = s[i-1][j-1] + delta(i,j,blocks,left,right,N);
				if (d > s[i][j])
				{
					s[i][j] = d;
					dir[i][j] = 'b';
				}
			}
			if (i>0)
			{
				long d = s[i-1][j] + delta1(i,blocks,left,N);
				if (d > s[i][j])
				{
					s[i][j] = d;
					dir[i][j] = 'l';
				}
			}
			if (j > 0)
			{
				long d = s[i][j-1] + delta1(j,blocks,right,N);
				if (d > s[i][j])
				{
					s[i][j] = d;
					dir[i][j] = 'r';
				}
			}
		}
	}
	if (DEBUG)
	{
		cout << "scores" << endl;
		for (int i=0; i<L1; i++)
		{
			for (int j=0; j<L2; j++)
			{
				printf ("%8ld \t", s[i][j]);
			}
			cout << endl;
		}
		display_aligned_blocks(L1, L2, N, dir, blocks, left, right);
	}
}


long profileScore(int i, int j, string blocks[], int left, int right, int N)
{
	long sum = 0;
	long c1[5];
	long c2[5];
	for (int m=0; m < 5; m++)
	{
		c1[m] = 0;
		c2[m] = 0;
	}
	for (int m=0; m < N; m++)
	{
		string first = blocks[left + m];
		string second = blocks[right + m];
		size_t base1 = ENTRY.find(first[i]);
		size_t base2 = ENTRY.find(second[j]);
		c1[base1] += 1;
		c2[base2] += 1;
	}
	for (int m=0; m < 5; m++) 
	{
		for (int n=0; n < 5; n++)
		{
			sum += c1[m] * c2[n] * T[m][n];
		}
	}
	//cout << sum << endl;
	return sum;
}


long profileScore1(int i, string blocks[], int start, int N) 
{
	//(int i, int j, string blocks[], int left, int right, int N) {
	long sum = 0;
	long c1[5];
	for (int m=0; m < 5; m++)
	{
		c1[m] = 0;
	}
	for (int m=0; m < N; m++)
	{
		string first = blocks[start + m];
		size_t base1 = ENTRY.find(first[i]);
		c1[base1] += 1;
	}
	for (int m=0; m < 5; m++)
	{
		sum += c1[m] * T[m][4];
	}
	return sum;
}


// The following function is used to compute using the technique "Profile Scoring".


void profileScoresAlignment(string blocks[], int left, int right, int N)
{
	int L1 = blocks[left].length();
	int L2 = blocks[right].length();
	long s[L1][L2];
	char **dir;
	dir = new char *[L1];
	for(int i = 0; i <L1; i++)
	{
		dir[i] = new char[L2];
	}
	for (int i=0;i<L1;i++)
	{
		for (int j=0; j<L2; j++)
		{
			s[i][j] = LONG_MIN;
		}
	}
	s[0][0] = 0;
	for (int i=0;i<L1;i++)
	{
		for (int j=0; j<L2; j++)
		{
			if (i>0 && j>0)
			{
				long d = s[i-1][j-1] + profileScore(i,j,blocks,left,right,N);
				if (d > s[i][j])
				{
					s[i][j] = d;
					dir[i][j] = 'b';
				}
			}
			if (i>0)
			{
				long d = s[i-1][j] + profileScore1(i,blocks,left,N);
				if (d > s[i][j])
				{
					s[i][j] = d;
					dir[i][j] = 'l';
				}
			}
			if (j > 0)
			{
				long d = s[i][j-1] + profileScore1(j,blocks,right,N);
				if (d > s[i][j])
				{
					s[i][j] = d;
					dir[i][j] = 'r';
				}
			}
		}
	}
	if (DEBUG)
	{
		cout << "scores" << endl;
		for (int i=0; i<L1; i++)
		{
			for (int j=0; j<L2; j++)
			{
				printf ("%8ld \t", s[i][j]);
			}
			cout << endl;
		}
		display_aligned_blocks(L1, L2, N, dir, blocks, left, right);
	}
}

//L
//N
//1,2,3
//D

int main(int argc, char *argv[]) 
{
	/* initialize random seed: */
	srand (time(NULL));
	int L = 100;
	int N = 4;
	if (argc < 4) {
	cout << "Incorrect parameters passed" << endl;
	cout << "Usage " << argv[0] << " L N scoreType" << endl;
	return(-1);
}

L = atoi(argv[1]);
N = atoi(argv[2]);
int scoretype = atoi(argv[3]);
if (argc >= 5) 
{
	DEBUG = true;
}
string block = generateString(L);

//cout << "Random Block=" << block << endl;
string * blocks = replicateString(block, N);

//cout << "Replicated Blocks=" << endl;
//for (int i=0; i<N; i++) 
//{  
	//  cout << blocks[i] << endl;
	//
//}
//cout << "Performing Substitutions" << endl;

for (int i=0; i<N; i++) 
{
    //cout << endl << blocks[i] << endl;
    blocks[i] = simulateSubstitution(blocks[i]);
    //cout << blocks[i] << endl;
}

//cout << "Performing Deletions" << endl;

for (int i=0; i<N; i++)
{
	//cout << endl << blocks[i] << endl;
    blocks[i] = simulateDeletion(blocks[i]);
    //cout << blocks[i] << endl;
}

//cout << "Performing Insertions" << endl << endl;

int half = N/2;
simulateInsertion(blocks,0, half - 1);
simulateInsertion(blocks,half, N -1);
cout << "Generated Blocks" << endl;
for (int i=0; i<N; i++)
{
	if (i ==  half) 
	{
		cout <<endl;
	}
   cout << blocks[i] << endl;
}

//  cout << "Computing delta 0 0" << endl << endl;
//  long score = delta(0,0,blocks,0,half, half);
//  cout << score << endl;

if (scoretype == 1)
{
	cout << "running sum of pairs alignment" << endl;
	clock_t begin = clock();
	sumOfPairsAlignment(blocks,0,half,half);
	clock_t end = clock();
	double elapsedTime =  double(end - begin) / CLOCKS_PER_SEC;
	cout << "L=" << L << ", N = " << N  << ", scoreType= " << scoretype << ", elapsed Time = " << elapsedTime << endl;
} 
else if (scoretype == 2)
{  
	cout << "running profile score alignment" << endl;
	clock_t begin = clock();
	profileScoresAlignment(blocks,0,half,half);
	clock_t end = clock();
	double elapsedTime =  double(end - begin) / CLOCKS_PER_SEC;
	cout << "L=" << L << ", N = " << N  << ", scoreType= " << scoretype << ", elapsed Time = " << elapsedTime << endl;
} 
else
{
	cout << "running sum of pairs alignment" << endl;
	clock_t begin = clock();
	sumOfPairsAlignment(blocks,0,half,half);
	clock_t end = clock();
	double elapsedTime =  double(end - begin) / CLOCKS_PER_SEC;
	cout << "L=" << L << ", N = " << N  << ", scoreType= " << 1 << ", elapsed Time = " << elapsedTime << endl;
	cout << "running profile score alignment" << endl;
	begin = clock();
	profileScoresAlignment(blocks,0,half,half);
	end = clock();
	elapsedTime =  double(end - begin) / CLOCKS_PER_SEC;
	cout << "L=" << L << ", N = " << N  << ", scoreType= " << 2 << ", elapsed Time = " << elapsedTime << endl;  
}
return 0;
}
