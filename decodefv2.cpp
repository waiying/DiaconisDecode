#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
//#include <random>
#include <time.h>
using namespace std;


//////////////////////////////////  MAKETRANS  /////////////////////////////////////////////
map<char, char> maketrans(vector<char> from, vector<char> to)
{
	map<char, char> mapkey;
	map<char, char>::iterator it;
	
	for (int i=0; i < 26; i++)
	{
		
		mapkey[from[i]] = to[i];
	}
	/*cout << "from: " ;
	for (int i=0; i < 26; i++)
	{
		cout << from[i];
	}
	cout << endl;
	
	cout << "to:   ";
	for (int i=0; i < 26; i++)
	{
		cout << to[i];
	}		
	cout << endl;
	for (it = mapkey.begin(); it != mapkey.end(); ++it)
	{
		cout << it->first << " => " << it->second << endl;
	}*/
	return mapkey;
}

//////////////////////////////////  TRANSLATE  /////////////////////////////////////////////
string translate(string ctext, map<char, char> mapkey)
{
	map<char, char>::iterator it;
	for (unsigned int i=0; i < ctext.length(); ++i)
	{
		for (it = mapkey.begin(); it != mapkey.end(); ++it)
		{
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
{
	char a, b;
	int flag = 0, total_bigram = 0;
	string str;
	map<string, double> bigram; //bigram with probabilities
	map<string, double>::iterator it;

	for (unsigned int i=0; i < file.length(); i++)
	{
		if (flag == 0) //getting 1st char in the bigram pair
		{
			a = file[i];
			if (!isalpha(a))
				continue;
			else
				flag = 1;
			continue;
		}
		
		if (flag == 2) //lets second char in the previous pair to be the 1st char in this pair
		{
			a = b;
			flag = 1;
		}
		
		if (flag == 1) //getting 2nd char in the bigram pair
		{
			b = file[i];
			if (!isalpha(b))
				continue; 
			else
			{
				str = a;
				str += b;
				bigram[str] += 1;
				/*if (bigram[str] == 0) //checks if the bigram entry exists, then increment it
				{
					bigram[str] = 1;
				}
				else bigram[str] += 1;*/
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
	decoded = translate(ciphertext, maketrans(key, alpha)); //look up how to transform
	/*cout << "key:   " ;
	for (int i = 0; i < 26; i++)
		cout << key[i];
	cout << endl;
	cout << "alpha: " ;
	for (int i = 0; i < 26; i++)
		cout << alpha[i];
	cout << endl;
	cout << decoded << endl;*/
 	decoded_bigram = calc_freq(decoded);
 	/*for (it = decoded_bigram.begin(); it != decoded_bigram.end(); ++it) //turns frequencies into probabilities
	{
		decoded_bigram[it->first] = it->second;///total_bigram;
		cout << it->first << " => " << it->second << endl;
	}
	cout << "end decoded bigram" << endl;*/
 	double score = 0;
 	int index = 0;
 
 	for (it = decoded_bigram.begin(); it != decoded_bigram.end(); ++it)
 	{
 		if (bigram[it->first] == 0)
 			score = score - 14.73702005;
 		else
 			score = score + log(bigram[it->first]);
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
	/*random_device random_device;
	mt19937 random_engine{random_device()};
	bernoulli_distribution coin{prob};
	
	bool outcome = coin(random_engine);*/
	return (r <= prob);
	//ln(pl(f*))/ln(pl(f)) gets smaller as pl(f*) increases
}

//////////////////////////////////  MAIN  /////////////////////////////////////////////
int main()
{
	srand(time(0));
	char file[] = "warpeace.txt";
	string ciphertext = "qknelknixwtknonixenwiytxrerjnx\
qknetknuxrraytknalmbxnyqnxnxiemnwlecrobjeyhnarxnjn\
qknelqiyykrqetknckixtyiewwlimxijytriwwwlvlwniewjniybxntknj\
qknelylttlemknixwtkniytxrerjnxqknxnknonctbxnwqltkjbckiuuoibynletkn\
onctbxnxrrj\
krqyrrebeiccrbetihonlhncijntlxnwiewylcd\
tlooxlylemiewmolwlemrbtlqiewnxnwraahzjzynoa\
letknjzytlciojrlytelmktilxiewaxrjtljntrtljn\
orrdnwbuleunxanctylonecnittknytixy";
	//string s = "qknelknixwtknonixenwiytxrerjnx";
	string plaintext;
	char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	vector<char> key(letters, letters+26), newkey(key), alpha(key), bestkey(key);
	map<string, double> bigram; //bigram - bigram with probabilities, freq - bigram with frequencies
	double score, besttrialscore = 0;
	map<string, double>::iterator it;
	
	ifstream inf(file);
	string content((istreambuf_iterator<char>(inf)), (istreambuf_iterator<char>())); //transfers war and peace text into the string, content
	bigram = calc_freq(content);
	/*for (it = bigram.begin(); it != bigram.end(); ++it) //turns frequencies into probabilities
	{
		bigram[it->first] = it->second;///total_bigram;
		cout << it->first << " => " << it->second << endl;
	}
	cout << "end bigram" << endl;*/
		
	double bestscore = 0;
	double p = 0;
	for (int i=0; i < 50; ++i) //trial
	{
		random_shuffle(key.begin(), key.end());
		besttrialscore = 0;
		for (int j=0; j < 10000; j++) //swap
		{
			newkey = swap(key);
			for (int z = 0; z < 26; z++)
				cout << newkey[z];
			cout << endl;
			score = scoring(ciphertext, newkey, bigram);
			//cout << ciphertext << endl;
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
	
	plaintext = translate(ciphertext, maketrans(key, alpha)); 
	cout << "best score: " << bestscore << endl;
	cout << plaintext<<endl; 
}
		
