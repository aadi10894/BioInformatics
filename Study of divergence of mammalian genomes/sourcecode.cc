#include<iostream>
#include<fstream>
#include <bits/stdc++.h> 
#include<string>
#include <list>
  
using namespace std; 

enum PositionCategory {
INTERGENIC, INTRON, EXON, CODING, UTR5, UTR3, PROMOTOR
};

string positionCategoryNames[] = {
  "INTERGENIC", "INTRON", "EXON", "CODING", "UTR5", "UTR3", "PROMOTOR"
};

const int PROMOTER_LENGTH = 1000;

class Gene {
   public:
      string name;   // Length of a box
      string chrom;
      string strand;
      int txStart;
      int txEnd;
      int cdsStart;
      int cdsEnd;
      int exonCount;
      vector<int> exonStarts;
      vector<int> exonEnds;
};
vector<Gene> genes;

	int cumulativeMatchCount[7];
 	int cumulativeMismatchCount[7];;
 	int cumulativeTransitionsCount[7];;
  	int cumulativeTransversionsCount[7];;
  	int cumulativeGapCount[7];;
  	int gapCountByLen[7][1000000];
  	int maxGap[7];
  	int pairCounts[7][4][4];
    
void resetCounts() {
  for (int i =0; i<7;i++) {
    cumulativeMatchCount[i] = 0;
    cumulativeMismatchCount[i] = 0;
    cumulativeTransitionsCount[i] = 0;
    cumulativeTransversionsCount[i] = 0;
    cumulativeGapCount[i] = 0;

  }
}
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

void updatePositions(int startingPosition, string chrom, PositionCategory positions[],int len, int promoterLen)  {
  //initialize as intergenic
  for (int i=0; i< len; i++) {
    positions[i] = INTERGENIC;
  }

  for (auto gene = genes.begin(); gene != genes.end(); ++gene) {
    if (chrom.compare(gene->chrom) == 0) {
      for (int i=0; i < len; i++) {
        int position = startingPosition + i;
        if (position >= gene->txStart && position <= gene->txEnd) {
          if (position < gene->cdsStart) {
            positions[i] = UTR5;
          } else if (position > gene->cdsEnd) {
            positions[i] = UTR3;
          } else {
            // loop through the exons and check if exon
            bool isExon = false;
            for (int j=0; j < gene->exonCount; j++) {
              if (position >= gene->exonStarts[j] && position <=  gene->exonEnds[j]) {
                isExon = true;
                break;
              }
            }
            if (isExon) {
              positions[i] = EXON;
            } else {
              //if not exon, then intron
              positions[i] = INTRON;
            }
          }
        } else if (position >= gene->txStart - promoterLen && position < gene->txStart) {
          positions[i]=PROMOTOR;
        }
      }
    }
  }

}

void getCounts(string x, string y, PositionCategory positions[]) 
{
  	//Assuming both strings are same length. Else Take Min
  	int len = x.length();  
  	int n = y.length(); 
  	
	if (n < len) 
	{
    		len = n;
  	}

//maintain count by position type
   	int matchCount[7];
  	int mismatchCount[7];
  	int transitionsCount[7];
  	int transversionsCount[7];
  	int gapCount[7];
  	bool insideGap = false;
  	int currentGapLen = 0;
    int currentPositionCategory;

  for (int i =0; i<7;i++) {
    matchCount[i] = 0;
    mismatchCount[i] = 0;
    transitionsCount[i] = 0;
    transversionsCount[i] = 0;
    gapCount[i] = 0;

  }    


  	for (int i =0; i < len; i++) 
	{
        PositionCategory positionCategory = positions[i];
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
              currentPositionCategory = positionCategory;
        			currentGapLen = 1;
        			gapCount[positionCategory] ++;
      			}
    		} 
		else 
		{
      			if (insideGap) 
			{
        			// update count of gaps by length
        			gapCountByLen[currentPositionCategory][currentGapLen]++;
        			if (currentGapLen > maxGap[currentPositionCategory]) 
				{
          				maxGap[currentPositionCategory] = currentGapLen;
        			}
        
      			}
      			insideGap = false;

      			if (c1 == c2) 
			{
        			matchCount[positionCategory]++;
              pairCounts[positionCategory][index1][index2]++;
      			} 
			else 
			{
        			mismatchCount[positionCategory]++;
              pairCounts[positionCategory][index1][index2]++;
        			if ((c1 == 'A' && c2 == 'G') || (c1 == 'G' && c2 == 'A') || (c1 == 'C' && c2 == 'T') 					|| (c1 == 'T' && c2 == 'C') ) 
				{
          				transitionsCount[positionCategory]++;
        			} 
				else 
				{
          				transversionsCount[positionCategory]++;
        			}
      			}
    		}	
  	}
  	if (insideGap) 
	{
      		// update count of gaps by length
      		gapCountByLen[currentPositionCategory][currentGapLen]++;
      		if (currentGapLen > maxGap[currentPositionCategory]) 
		{
        		maxGap[currentPositionCategory] = currentGapLen;
      		}
  	}


matchCount[CODING] = matchCount[EXON] + matchCount[INTRON];
matchCount[EXON] += matchCount[UTR3] + matchCount[UTR5];
mismatchCount[CODING] = mismatchCount[EXON] + mismatchCount[INTRON];
mismatchCount[EXON] += mismatchCount[UTR3] + mismatchCount[UTR5];
transitionsCount[CODING] = transitionsCount[EXON] + transitionsCount[INTRON];
transitionsCount[EXON] += transitionsCount[UTR3] + transitionsCount[UTR5];
transversionsCount[CODING] = transversionsCount[EXON] + transversionsCount[INTRON];
transversionsCount[EXON] += transversionsCount[UTR3] + transversionsCount[UTR5];
gapCount[CODING] = gapCount[EXON] + gapCount[INTRON];
gapCount[EXON] += gapCount[UTR3] + gapCount[UTR5];


    for (int i=0; i < 7; i++) {
/*
    cout << "The counts are = \n"; 
  	cout << "match count:" << matchCount[i] << "\n"; 
  	cout << "mismatch count:" << mismatchCount[i] << "\n"; 
  	cout << "transitions count:" << transitionsCount[i] << "\n"; 
  	cout << "transversions count:" << transversionsCount[i] << "\n"; 
  	cout << "gap count:" << gapCount[i] << "\n"; 
*/    
  	cumulativeMatchCount[i] += matchCount[i];
  	cumulativeMismatchCount[i] += mismatchCount[i];
  	cumulativeTransitionsCount[i] += transitionsCount[i];
  	cumulativeTransversionsCount[i] += transversionsCount[i];
  	cumulativeGapCount[i] += gapCount[i];  
    }


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

const vector<string> explode(const string& s, const char& c)
{
	string buff = "";
	vector<string> v;
  int len = s.length();
	
  for (int i =0; i < len; i++)
	{
    char n = s[i];
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

const vector<int> parseList(const string& s, const char& c)
{
	string buff = "";
	vector<int> v;
  int len = s.length();
	
  for (int i =0; i < len; i++)
	{
    char n = s[i];
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(atoi(buff.c_str())); 
    buff = ""; }
	}
	if(buff != "") v.push_back(atoi(buff.c_str()));
	
	return v;
}



int process()
{ 
  resetCounts();
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
      string chrom;
      int startingPosition;
    	while (getline(in, line)) 
	{
      		lineCount++;
          cout << lineCount << endl;
      		if (lineCount > 1000000) {
        		break;
      		}
      
		if (line.length() == 0) 
		{
        		continue;
      		}
      
		char c = line[0];
      		if (c == 's') 
		{
       vector<string> v  = explode(line, ' ');
       //for (auto it = v.begin(); it != v.end(); ++it) {
         //''cout << *it << endl;
       //}
       if (geneCount == 0) {
         chrom = v[1];
         std::size_t found = line.find_last_of(".");
         chrom = chrom.substr(found-1);
         startingPosition = atoi(v[2].data());
         //cout << chrom << endl;
         //cout << startingPosition << endl;
       }
        		std::size_t found = line.find_last_of(" ");
        		genes[geneCount++] = line.substr(found+1);
        		
			if (geneCount == 2) 
			{
                int len = genes[0].length();
                PositionCategory positions[len];
                updatePositions(startingPosition, chrom, positions, len, PROMOTER_LENGTH);

          			getCounts(genes[0], genes[1],positions);
          			geneCount = 0;
        		}
      		}
    	}

	for (int i=0; i < 4; i++) {
    	for (int j=0; j < 4; j++) {
       pairCounts[CODING][i][j] = pairCounts[EXON][i][j] + pairCounts[INTRON][i][j];
       pairCounts[EXON][i][j] += pairCounts[UTR3][i][j] + pairCounts[UTR5][i][j];
      }
  }

  maxGap[CODING] = max(maxGap[EXON],maxGap[INTRON]);
  for (int i=1; i <= maxGap[CODING]; i++) {
    gapCountByLen[CODING][i] = gapCountByLen[EXON][i] + gapCountByLen[INTRON][i];
  }
  maxGap[EXON] = max(maxGap[EXON],max(maxGap[UTR3],maxGap[UTR5]));
  for (int i=1; i <= maxGap[CODING]; i++) {
    gapCountByLen[EXON][i] += gapCountByLen[UTR3][i] + gapCountByLen[UTR5][i];
  }  

for ( int position = INTERGENIC; position != PROMOTOR+1; position++ )
{
  cout << endl;
  cout << "For category " << positionCategoryNames[position] << endl;
   
 	cout << "The cumulative counts are = \n"; 
  	cout << "Cumulative match count:" << cumulativeMatchCount[position] << "\n"; 
  	cout << "Cumulative mismatch count:" << cumulativeMismatchCount[position] << "\n"; 
  	cout << "Cumulative transitions count:" << cumulativeTransitionsCount[position] << "\n"; 
  	cout << "Cumulative transversions count:" << cumulativeTransversionsCount[position] << "\n"; 
  	cout << "Cumulative gap count:" << cumulativeGapCount[position] << "\n"; 

  	float substitutionRate = ((float)cumulativeMismatchCount[position]) / (cumulativeMatchCount[position] + cumulativeMismatchCount[position]);

  	float titvratio = float(cumulativeTransitionsCount[position]) / cumulativeTransversionsCount[position];

  	std::cout.precision(4);
  	cout << "Substitution rate:" << std::setw(10) << substitutionRate << endl;
  	cout << "ti/tv ratio:" << titvratio << endl;


	string bases = "ACGT";
	cout << "Pair Counts\tA \tC \tG \tT" <<  endl;
  	for (int i=0; i < 4; i++) {
    	cout << bases[i] << "\t";
    	for (int j=0; j < 4; j++) {
      	cout << "\t" << pairCounts[position][i][j];
    	}
    	cout << endl;
  }

	float gapRate = float(cumulativeGapCount[position]) / (cumulativeMatchCount[position] + cumulativeMismatchCount[position] + cumulativeGapCount[position]);
	cout << "Gap rate:" << gapRate << endl;


        int totalGapCount=0;

	for (int i = 1; i <= maxGap[position]; i++) 
	{
  		totalGapCount += gapCountByLen[position][i];
	}

	cout << "Gap Length (Bases) \t 	Gap Count \tGap Frequency" << endl;
	
	for (int i = 1; i <= maxGap[position]; i++) 
	{
  		cout << "\t\t" << i <<  "\t\t" << gapCountByLen[position][i] << "\t\t" << float(gapCountByLen[position][i]) / cumulativeGapCount[position] << endl;;
	}	

	cout << "\t\tTotal"  <<  "\t\t" << totalGapCount << "\t\t" << float(totalGapCount) / cumulativeGapCount[position] << endl;;
}

      in.close();
 
      return 0; 
}




void print_genes() {
  cout << "genes" << endl;
  for (auto gene = genes.begin(); gene != genes.end(); ++gene) {
   cout << gene->name << endl;
    cout << gene->chrom << endl;
    cout << gene->strand << endl;
    cout << gene->txStart << endl;
    cout << gene->txEnd << endl;
    cout << gene->cdsStart << endl;
    cout << gene->cdsEnd << endl;
    cout << gene->exonCount << endl;
    cout << "exonStarts" << endl;
    for (auto it2 = gene->exonStarts.begin(); it2 != gene->exonStarts.end(); ++it2) {
       cout << *it2 << endl;
     }
      
          cout << "exonEnds" << endl;
    for (auto it2 = gene->exonEnds.begin(); it2 != gene->exonEnds.end(); ++it2) {
       cout << *it2 << endl;
     }
}
}


int main() 
{
  //read knowngene
  string filename = "knowngene.txt";
  ifstream in(filename, ios::out);

  	if (!in.is_open()) 
	{
        	cerr << "Error: Unable to open file \"" << filename << "\" for reading!" << endl;
        	return -1;
    	}


cout << "opened file successfully \n";
int lineCount = 0;
string line;

//load the file in a structure
	while (getline(in, line)) 
	{
    lineCount++;
    //cout << line << endl;
      
		if (line.length() == 0) {
    	continue;
    }
//populate structure from line
    char c = line[0];
    if (c == '#') {
      continue;
    }
    vector<string> v  = explode(line, '\t');
    //cout << "fields" << endl;
    Gene* gene = new Gene();
    int column =0;
    for (auto it = v.begin(); it != v.end(); ++it) {
      column++;
      switch(column) {
        case 1:gene->name=*it;break;
        case 2:gene->chrom=*it;break;
        case 3:gene->strand=*it;break;
        case 4:gene->txStart=atoi(it->data());break;
        case 5:gene->txEnd=atoi(it->data());break;
        case 6:gene->cdsStart=atoi(it->data());break;
        case 7:gene->cdsEnd=atoi(it->data());break;
        case 8:gene->exonCount=atoi(it->data());break;
        case 9:gene->exonStarts =parseList(it->data(), ',');break;
        case 10:gene->exonEnds =parseList(it->data(), ',');break;
      }

    }
genes.push_back(*gene);


}
//print_genes();
 process();
}
