#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
using namespace std;

int main()
{
	char file[30];
	char a, b;
	int flag = 0, total_bigram = 0;
	string str;
	map<string, long double> bigram, freq; //bigram - bigram with probabilities, freq - bigram with frequencies
	map<string, long double>::iterator it;
	cout << "Please enter the file name to create bigram: ";
	cin >> file;//gets war-and-peace.txt
	ifstream inf(file);
	while (!inf.eof())
	{
		if (flag == 0) //getting 1st char in the bigram pair
		{
			a = inf.get();
			flag = 1;
			continue;
		}
		
		else if (flag == 2) //lets second char in the previous pair to be the 1st char in this pair
		{
			a = b;
			flag = 1;
			continue;
		}
		
		else if (flag == 1) //getting 2nd char in the bigram pair
		{
			b = inf.get();
			if (!isalpha(a) && !isalpha(b)){} //if the char in the pair are whitespaces, ignore it
			else
			{
				if (!isalpha(a)) a = ' ';
				else if (!isalpha(b)) b = ' ';
				str = a;
				str += b;
				if (freq.count(str) == 0) //checks if the bigram entry exists, then increment it
				{
					freq[str] = 1;
				}
				else freq[str] += 1;
				total_bigram += 1;
			}
			flag = 2;
		}
	}

	for (it = freq.begin(); it != freq.end(); ++it)
	{
		bigram[it->first] = (it->second)/total_bigram; //calculates probability of this pair
  }
  
  freq.clear();
  
  for (it = bigram.begin(); it != bigram.end(); ++it)
  {
  	cout << it->first << " => " << it->second << endl;
 	}
 	cout << total_bigram <<endl;	
}
		
