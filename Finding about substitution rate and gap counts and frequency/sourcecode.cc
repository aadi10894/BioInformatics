#include<iostream>
#include<fstream>
#include <bits/stdc++.h> 
  
using namespace std; 

	int cumulativeMatchCount = 0;
 	int cumulativeMismatchCount = 0;
 	int cumulativeTransitionsCount = 0;
  	int cumulativeTransversionsCount =0;
  	int cumulativeGapCount = 0;
  	int gapCountByLen[1000000];
  	int maxGap = 0;
  	int pairCounts[4][4];

int convertBaseToIndex(char c) 
{
  	int index = -1;
  	
	switch(c) 
	{
    		case 'A': index = 0; break;
    		case 'C':index = 1; break;
    		case 'G':index = 2; break;
    		case 'T':index = 3; break;
  	}
  	return index;
}

void getCounts(string x, string y) 
{
  	//Assuming both strings are same length. Else Take Min
  	int len = x.length();  
  	int n = y.length(); 
  	
	if (n < len) 
	{
    		len = n;
  	}

   	int matchCount = 0;
  	int mismatchCount = 0;
  	int transitionsCount = 0;
  	int transversionsCount =0;
  	int gapCount = 0;
  	bool insideGap = false;
  	int currentGapLen = 0;


  	for (int i =0; i < len; i++) 
	{
    		char c1 = toupper(x[i]);
    		char c2 = toupper(y[i]);
    		int index1 = convertBaseToIndex(c1);
    		int index2 = convertBaseToIndex(c2);

    		if (c1 == '-' || c2 == '-') 
		{
      			if (insideGap) 
			{
          			currentGapLen++;
      			} 
			else 
			{
        			insideGap = true;
        			currentGapLen = 1;
        			gapCount ++;
      			}
    		} 
		else 
		{
      			if (insideGap) 
			{
        			// update count of gaps by length
        			gapCountByLen[currentGapLen]++;
        			if (currentGapLen > maxGap) 
				{
          				maxGap = currentGapLen;
        			}
        
      			}
      			insideGap = false;
		        pairCounts[index1][index2]++;

      			if (c1 == c2) 
			{
        			matchCount++;
      			} 
			else 
			{
        			mismatchCount++;
        			if ((c1 == 'A' && c2 == 'G') || (c1 == 'G' && c2 == 'A') || (c1 == 'C' && c2 == 'T') 					|| (c1 == 'T' && c2 == 'C') ) 
				{
          				transitionsCount++;
        			} 
				else 
				{
          				transversionsCount++;
        			}
      			}
    		}	
  	}
  	if (insideGap) 
	{
      		// update count of gaps by length
      		gapCountByLen[currentGapLen]++;
      		if (currentGapLen > maxGap) 
		{
        		maxGap = currentGapLen;
      		}
  	}

  	cout << "The counts are = \n"; 
  	cout << "match count:" << matchCount << "\n"; 
  	cout << "mismatch count:" << mismatchCount << "\n"; 
  	cout << "transitions count:" << transitionsCount << "\n"; 
  	cout << "transversions count:" << transversionsCount << "\n"; 
  	cout << "gap count:" << gapCount << "\n"; 

  	cumulativeMatchCount += matchCount;
  	cumulativeMismatchCount += mismatchCount;
  	cumulativeTransitionsCount += transitionsCount;
  	cumulativeTransversionsCount += transversionsCount;
  	cumulativeGapCount += gapCount;  

}
  
void getgenomealignment(string x, string y, int pxy, int pgap) 
{ 
	int i, j;  
    	int m = x.length();  
    	int n = y.length(); 
    	int dp[n+m+1][n+m+1];
    	//int dp[n+m+1][n+m+1] = {0}; 
    
	for (i = 0; i <= (n+m); i++) 
    	{ 
        	dp[i][0] = i * pgap; 
        	dp[0][i] = i * pgap; 
    	}     
  
     
    	for (i = 1; i <= m; i++) 
    	{ 
        	for (j = 1; j <= n; j++) 
        	{ 
            		if (x[i - 1] == y[j - 1]) 
            		{ 
                		dp[i][j] = dp[i - 1][j - 1]; 
            		} 
            		else
            		{ 
                		dp[i][j] = min({dp[i - 1][j - 1] + pxy , dp[i - 1][j] + pgap , dp[i][j - 1] + pgap}); 
           		 } 
        	} 
    	} 
  
     
    	int l = n + m; 
      
    	i = m; 
	
	j = n; 
      
    	int xposition = l; 
    
	int yposition = l; 
  
 	int xanswer[l+1], yanswer[l+1]; 
      
    	while ( !(i == 0 || j == 0)) 
    	{ 
        
		if (x[i - 1] == y[j - 1]) 
        	{ 
            		xanswer[xposition--] = (int)x[i - 1]; 
            		yanswer[yposition--] = (int)y[j - 1]; 
            		i--; 
			j--; 
        	} 
        
		else if (dp[i - 1][j - 1] + pxy == dp[i][j]) 
        	{ 
            		xanswer[xposition--] = (int)x[i - 1]; 
            		yanswer[yposition--] = (int)y[j - 1]; 
            		i--; 
			j--; 
        	} 
        
		else if (dp[i - 1][j] + pgap == dp[i][j]) 
        	{ 
            		xanswer[xposition--] = (int)x[i - 1]; 
            		yanswer[yposition--] = (int)'_'; 
            		i--; 
        	} 
        
		else if (dp[i][j - 1] + pgap == dp[i][j]) 
        	{ 
            		xanswer[xposition--] = (int)'_'; 
            		yanswer[yposition--] = (int)y[j - 1]; 
            		j--; 
        	} 
    	} 
    	
	while (xposition > 0) 
    	{ 
        	if (i > 0) xanswer[xposition--] = (int)x[--i]; 
        	
		else xanswer[xposition--] = (int)'_'; 
    	} 
    
	while (yposition > 0) 
    	{ 
        	if (j > 0) yanswer[yposition--] = (int)y[--j]; 
        
		else yanswer[yposition--] = (int)'_'; 
        } 
  
    	int id = 1; 
    
	for (i = l; i >= 1; i--) 
    	{ 
        	if ((char)yanswer[i] == '_' && (char)xanswer[i] == '_') 
        	{ 
        		id = i + 1; 
            		break; 
        	} 
    	} 
  
   	cout << "The mismatch is = "; 
    	cout << dp[m][n] << "\n"; 
    	cout << "The aligned genes are :\n"; 
    
	for (i = id; i <= l; i++) 
    	{ 
        	cout<<(char)xanswer[i]; 
    	} 
    
	cout << "\n"; 
    	
	for (i = id; i <= l; i++) 
    	{ 
        	cout << (char)yanswer[i]; 
    	} 

    	return; 
} 
  

int main()

{ 
	string filename = "input.txt";
    	ifstream in(filename, ios::out);
    
	if (!in.is_open()) 
	{
        	cerr << "Error: Unable to open file \"" << filename << "\" for reading!" << endl;
        	return -1;
    	}
    	
	cout << "opened file successfully \n";
 
    	int lineCount = 0;
    	string genes[2];
    	int geneCount = 0;
    	string line;
    	while (getline(in, line)) 
	{
      		lineCount++;
      		if (lineCount > 10000000) 
		{
        		break;
      		}
      
		if (line.length() == 0) 
		{
        		continue;
      		}
      
		char c = line[0];
      		if (c == 's') 
		{
        		std::size_t found = line.find_last_of(" ");
        		genes[geneCount++] = line.substr(found+1);
        		
			if (geneCount == 2) 
			{
          			getCounts(genes[0], genes[1]);
          			geneCount = 0;
        		}
      		}
    	}


 	cout << "The cumulative counts are = \n"; 
  	cout << "Cumulative match count:" << cumulativeMatchCount << "\n"; 
  	cout << "Cumulative mismatch count:" << cumulativeMismatchCount << "\n"; 
  	cout << "Cumulative transitions count:" << cumulativeTransitionsCount << "\n"; 
  	cout << "Cumulative transversions count:" << cumulativeTransversionsCount << "\n"; 
  	cout << "Cumulative gap count:" << cumulativeGapCount << "\n"; 

  	float substitutionRate = ((float)cumulativeMismatchCount) / (cumulativeMatchCount + cumulativeMismatchCount);

  	float titvratio = float(cumulativeTransitionsCount) / cumulativeTransversionsCount;

  	std::cout.precision(4);
  	cout << "Substitution rate:" << std::setw(10) << substitutionRate << endl;
  	cout << "ti/tv ratio:" << titvratio << endl;

	string bases = "ACGT";
	cout << "Pair Counts\tA \tC \tG \tT" <<  endl;
  	for (int i=0; i < 4; i++) {
    	cout << bases[i] << "\t";
    	for (int j=0; j < 4; j++) {
      	cout << "\t" << pairCounts[i][j];
    	}
    	cout << endl;
  }

	float gapRate = float(cumulativeGapCount) / (cumulativeMatchCount + cumulativeMismatchCount + cumulativeGapCount);
	cout << "Gap rate:" << gapRate << endl;

        int totalGapCount;

	for (int i = 1; i <= maxGap; i++) 
	{
  		totalGapCount += gapCountByLen[i];
	}

	cout << "Gap Length (Bases) \t 	Gap Count \tGap Frequency" << endl;
	
	for (int i = 1; i <= maxGap; i++) 
	{
  		cout << "\t\t" << i <<  "\t\t" << gapCountByLen[i] << "\t\t" << float(gapCountByLen[i]) / cumulativeGapCount << endl;;
	}	

	cout << "\t\tTotal"  <<  "\t\t" << totalGapCount << "\t\t" << float(totalGapCount) / cumulativeGapCount << endl;;

      in.close();
 
      return 0; 
}
