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


//////////////////////////////////  KEYMAP  /////////////////////////////////////////////
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

//////////////////////////////////  DECIPHER  /////////////////////////////////////////////
string decipher(string ctext, map<char, char> mapkey)
{//This function uses the key mapping to translate the ciphertext into a temporary plaintext.
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

//////////////////////////////////  CALC_FREQ  /////////////////////////////////////////////
map<string, double> calc_freq(string file)
{//This function calculates the frequencies of the bigrams in war and peace and then returns calculated probabilities of each bigram as a map
//the parameter "file" is either war and peace text or the cipher text
	char a, b; //a and b are the letters of the pairings in the bigram, M
	int flag = 0, total_bigram = 0;
	string str;
	map<string, double> bigram; //bigram of the war and peace text with probabilities
	map<string, double>::iterator it;

	for (unsigned int i=0; i < file.length(); i++) //going through the entire text char by char
	{
		if (flag == 0) //getting 1st char in the first bigram pair
		{
			a = file[i];
			flag = 1;
			continue;
		}
		
		if (flag == 2) //lets the second char from the previous bigram pair to be the 1st char in the current pair
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
				bigram[str] += 1; //counts 
				total_bigram += 1;
			}
			flag = 2;
		}
	}
	for (it = bigram.begin(); it != bigram.end(); ++it) //turns frequencies into probabilities
	{
		bigram[it->first] = it->second/total_bigram;
		//cout << it->first << " => " << it->second << endl;
	}
	return bigram;
}

//////////////////////////////////  SCORING  /////////////////////////////////////////////
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
 	int index = 0;
 
 	for (it = decoded_bigram.begin(); it != decoded_bigram.end(); ++it)
 	{
 		if (bigram[it->first] == 0) //if a bigram from the decoded text does not exist in M
 			score = score - 14.73702005; //ln(1/total_bigram) = -14.73702005
 		else
 			score = score + log(bigram[it->first]); //sum of all the ln(plausibilities)
 		++index;
 	}
 	//cout << index << endl;
 	decoded_bigram.clear();
 	
 	return score;
}

//////////////////////////////////  SWAP  /////////////////////////////////////////////
vector<char> swap(vector<char> key)
{
	char tmp;
	int i = rand() % 26;
	int j = rand() % 26;
	while (j == i)
	{
		j = rand() % 26;
	}
	
	tmp = key[j];
	key[j] = key[i];
	key[i] = tmp;
	return key;
}

//////////////////////////////////  COINFLIP  /////////////////////////////////////////
bool coinflip(double prob)
{
	double r = (double)rand() / (double)RAND_MAX;
	return (r <= prob);
	//ln(pl(f*))/ln(pl(f)) gets smaller as pl(f*) increases
}

//////////////////////////////////  MAIN  /////////////////////////////////////////////
int main()
{
	srand(time(0));
	char file[] = "warpeace.txt"; //****must name the war and peace text file as "warpeace.txt"****
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
	vector<char> key(letters, letters+26), newkey(key), alpha(key), bestkey(key);
	map<string, double> bigram; //bigram with probabilities
	double score, besttrialscore = 0, bestscore = 0;
	double p = 0; //p is the probability being passed into the biased coin flip
	map<string, double>::iterator it;
	
	ifstream inf(file); //open war and peace text
	string content((istreambuf_iterator<char>(inf)), (istreambuf_iterator<char>())); //transfers war and peace text into the string, content
	bigram = calc_freq(content);

	for (int i=0; i < 5000; ++i) //this for-loop resets the key with a new random permutation
	{
		random_shuffle(key.begin(), key.end());
		besttrialscore = 0;
		for (int j=0; j < 700; j++) //swapping of two letters in this for-loop
		{
			newkey = swap(key);
			/*for (int z = 0; z < 26; z++)
				cout << newkey[z];
			cout << endl;*/
			score = scoring(ciphertext, newkey, bigram);
			cout << "score: " << score << endl;
			if (j == 0)
			{
				besttrialscore = score;
				continue;
			}
			if (score > besttrialscore)
			{
				key = newkey;
				besttrialscore = score;
				cout << "besttrialscore: " << besttrialscore << endl;
			}
			else if (score <= besttrialscore)
			{
				p = besttrialscore/score;
				if (coinflip(p) == 0) //if heads, accept newkey
				{
					cout << "coinflip result: " << coinflip(p) << endl;
					key = newkey;
					besttrialscore = score;
					cout << "coin flip: " << besttrialscore << endl;
				}
			}
		}
		if (i == 0)
		{
			bestscore = besttrialscore;
			cout << "******************************************************best score: " << bestscore << endl;
			continue;
		}
		if (besttrialscore > bestscore)
		{
			bestkey = key;
			bestscore = besttrialscore;
			cout << "*****************************************************best score: " << bestscore << endl;
		}
		//cout << bestscore << endl;
	}
	
	plaintext = decipher(ciphertext, keymap(key, alpha)); 
	cout << "best score: " << bestscore << endl;
	cout << plaintext<<endl; 
}
		
