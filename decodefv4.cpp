#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h>
using namespace std;

//////////////////////////////////  KEYMAP  /////////////////////////////////////////
map<char, char> keymap(vector<char> from, vector<char> to)
{//This function creates a map of the key to the original alphabet.
	map<char, char> mapkey;
	map<char, char>::iterator it;
	
	for (int i=0; i < 26; i++)
	{
		
		mapkey[from[i]] = to[i];
	}
	return mapkey;
}

//////////////////////////////////  DECIPHER  ///////////////////////////////////////
string decipher(string ctext, map<char, char> mapkey)
{//This function uses the key mapping to translate the ciphertext into a potential plaintext.
	map<char, char>::iterator it;
	for (unsigned int i=0; i < ctext.length(); ++i)
	{
		for (it = mapkey.begin(); it != mapkey.end(); ++it)
		{
			if (!isalpha(ctext[i])) //ignores any whitespaces and commas
				continue;
			if (ctext[i] == it->first)
			{
				ctext[i] = it->second;
				break;
			}
		}
	}
	return ctext;		
}

////////////////////////////////  CALC_FREQ  ////////////////////////////////////////
map<string, double> calc_freq(string file)
{//This function calculates the frequencies of the bigrams in war and peace and then returns calculated probabilities of each bigram as a map
//the parameter "file" is either war and peace text or the cipher text
	char a, b; //a and b are the letters of the pairings in the bigram
	int flag = 0, total_bigram = 0;
	string str;
	map<string, double> bigram; 
	map<string, double>::iterator it;

	for (unsigned int i=0; i < file.length(); i++) 
	{//going through the entire text char by char
		if (flag == 0) //getting the 1st char in the first bigram pair
		{
			a = file[i];
			flag = 1;
			continue;
		}
		
		if (flag == 2) //lets the 2nd char from the previous bigram pair to be the 1st char in the current pair
		{
			a = b;
			flag = 1;
		}
		
		if (flag == 1) //getting 2nd char in the bigram pair
		{
			b = file[i];
			if (!isalpha(a) && !isalpha(b)) //move on to get the next char if both chars are whitespaces
				continue;
			else
			{
				str = a;
				str += b;
				bigram[str] += 1; //counts the frequency of the bigram pair
				total_bigram += 1; //counts the total bigram
			}
			flag = 2;
		}
	}
	
	for (it = bigram.begin(); it != bigram.end(); ++it) 
	{//turns frequencies into probabilities
		bigram[it->first] = it->second/total_bigram;
		//cout << it->first << " => " << it->second << endl;
	}
	return bigram;
}

//////////////////////////////////  SCORING  ////////////////////////////////////////
double scoring(string ciphertext, vector<char> key, map<string, double> bigram)
{
	string decoded;
	char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	vector<char> alpha(letters, letters+26);
  map<string, double> decoded_bigram;
  map<string, double>::iterator it;
	decoded = decipher(ciphertext, keymap(key, alpha)); //look up how to transform
 	decoded_bigram = calc_freq(decoded);
 	double score = 0;
 
 	for (it = decoded_bigram.begin(); it != decoded_bigram.end(); ++it)
 	{
 		if (bigram[it->first] == 0) //if a bigram from the decoded text does not exist in M
 			score = score - 14.73702005; //ln(1/total_bigram) = -14.73702005
 		else
 			score = score + log(bigram[it->first]); //sum of all the ln(plausibilities)
 	}
 	decoded_bigram.clear(); //clear out decoded_bigram contents to save memory
 	
 	return score;
}

////////////////////////////////  RANDINT  //////////////////////////////////////////
int randint(int min, int max)
{
	unsigned long bins = max-min + 1,
								rands = (unsigned long)RAND_MAX + 1,
								bin_size = rands / bins,
								overflow = rands % bins;
	int r;
	do {
		r = rand();
	} while (rands - overflow <= (unsigned long)r);
	
	return r/bin_size + min;
}

///////////////////////////////////  SWAP  //////////////////////////////////////////
vector<char> swap(vector<char> key)
{
	char tmp;
	int i = randint(0,25);
	int j = randint(0,25);
	
	while (j == i)
	{
		j = randint(0,25);
	}

	tmp = key[j];
	key[j] = key[i];
	key[i] = tmp;

	return key;
}

////////////////////////////////  COINFLIP  /////////////////////////////////////////
bool coinflip(double prob)
{
	double r = (double)rand() / (double)RAND_MAX;
	return (r <= prob);
	//ln(pl(f*))/ln(pl(f)) gets smaller as pl(f*) increases
}

////////////////////////////////  MAIN  ////////////////////////////////////////////
int main()
{
	//****must name the war and peace text file as "warpeace.txt"****
	char file[] = "warpeace.txt"; 
	string ciphertext = "qkne l knixw tkn onixenw iytxrerjnx,\
qkne tkn uxrray, tkn almbxny, qnxn xiemnw le crobjey hnarxn jn,\
qkne l qiy ykrqe tkn ckixty iew wlimxijy, tr iww, wlvlwn, iew jniybxn tknj,\
qkne l ylttlem knixw tkn iytxrerjnx qknxn kn onctbxnw qltk jbck iuuoibyn le tkn\
onctbxn xrrj,\
krq yrre beiccrbetihon l hncijn tlxnw iew ylcd,\
tloo xlylem iew molwlem rbt l qiewnxnw raa hz jzynoa,\
le tkn jzytlcio jrlyt elmkt ilx, iew axrj tljn tr tljn,\
orrdnw bu le unxanct ylonecn it tkn ytixy.";
	string plaintext;
	char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	vector<char> key(letters, letters+26), swapkey(key), alpha(key), bestkey(key);
	map<string, double> bigram; //bigram with probabilities
	double score, permu_score = 0, bestscore = 0;
	double p = 0; //p is the probability being passed into the biased coin flip
	map<string, double>::iterator it;
	
	ifstream inf(file); //open war and peace text
	//transfers war and peace text into the string, content
	string content((istreambuf_iterator<char>(inf)), (istreambuf_iterator<char>())); 
	//make the M(x,y) bigram matrix map: key = bigram pair, value = probability
	bigram = calc_freq(content);

	for (int i=0; i < 400; ++i) 
	{//this outer for-loop resets the key with a new random permutation
		random_shuffle(key.begin(), key.end()); //permutation of key
		permu_score = 0; //score of the permutation key resets
		for (int j=0; j < 5000; j++) 
		{//swapping of two letters in this inner for-loop
			swapkey = swap(key);
			score = scoring(ciphertext, swapkey, bigram);
			if (j == 0) 
			{//assigns score as the permutation score for the first swap
				permu_score = score;
				cout << permu_score << endl;
				continue;
			}
			if (score > permu_score) 
			//compares the previous best score with the new score
			{
				key = swapkey;
				permu_score = score;
				cout << permu_score << endl;
			}
			else if (score <= permu_score)
			{
				p = permu_score/score; //probability of getting heads or 1
				if (coinflip(p) == 0) //if tails or 0, accept swapkey
				{
					key = swapkey;
					permu_score = score;
				}
			}
		}
		if (i == 0)
		{//assigns permutation score as the best score for the first permutation
			bestscore = permu_score;
			continue;
		}
		if (permu_score > bestscore)
		{//keep permutation score as best score if permu_score is higher than bestscore
			bestkey = key;
			bestscore = permu_score;
		}
	}
	
	plaintext = decipher(ciphertext, keymap(bestkey, alpha)); 
	cout << "best score: " << bestscore << endl;
	cout << plaintext <<endl; 
}
		
