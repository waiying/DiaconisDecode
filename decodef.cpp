#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <time.h>
using namespace std;


//////////////////////////////////  MAKETRANS  /////////////////////////////////////////////
map<char, char> maketrans(vector<char> from, vector<char> to)
{
	map<char, char> map;
	for (int i=0; i < 26; i++)
	{
		map[from[i]] = to[i];
	}
	return map;
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
				ctext[i] = it->second;
		}
	}
	return ctext;		
}

//////////////////////////////////  CALC_FREQ  /////////////////////////////////////////////
map<string, int> calc_freq(string file)
{
	char a, b;
	int flag = 0; //total_bigram = 0;
	string str;
	map<string, int> bigram; //bigram - bigram with probabilities, freq - bigram with frequencies

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
		
		else if (flag == 2) //lets second char in the previous pair to be the 1st char in this pair
		{
			a = b;
			flag = 1;
			continue;
		}
		
		else if (flag == 1) //getting 2nd char in the bigram pair
		{
			b = file[i];
			if (!isalpha(b))
				continue; //if the char in the pair are whitespaces, ignore it
			else
			{
				str = a;
				str += b;
				if (bigram.count(str) == 0) //checks if the bigram entry exists, then increment it
				{
					bigram[str] = 1;
				}
				else bigram[str] += 1;
				//total_bigram += 1;
			}
			flag = 2;
		}
	}
	return bigram;
}

//////////////////////////////////  SCORING  /////////////////////////////////////////////
int scoring(string ciphertext, vector<char> key, map<string, int> bigram)
{
	string decoded;
	char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	vector<char> alpha(letters, letters+26);
  map<string, int> decoded_bigram;
  map<string, int>::iterator it;
	decoded = translate(ciphertext, maketrans(key, alpha)); //look up how to transform
 	decoded_bigram = calc_freq(decoded);
 	int score = 0;
 	for (it = bigram.begin(); it != bigram.end(); ++it)
 	{
 		if (decoded_bigram.count(it->first) == 0)
 		{}
 		else
 		score = score + decoded_bigram[it->first] * bigram[it->first];
 	}
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
	map<string, int> bigram; //bigram - bigram with probabilities, freq - bigram with frequencies
	int score, besttrialscore = 0;
	map<string, int>::iterator it;
	
	ifstream inf(file);
	string content((istreambuf_iterator<char>(inf)), (istreambuf_iterator<char>())); //transfers war and peace text into the string, content
	bigram = calc_freq(content);
	/*for (it = bigram.begin(); it != bigram.end(); ++it)
  {
  	cout << it->first << " => " << it->second << endl;
 	}*/
	
	/*random_shuffle(key, key+26);
	for(int i=0; i < 26; i++)
	{
		cout << key[i];
	}
	cout << endl;
	swap(key);
	for(int i=0; i<26; i++)
	{
		cout << key[i];
	}
	cout << endl;
	cout << translate(s, maketrans(key, alpha)) << endl;*/
	
	int bestscore = 0;
	for (int i=0; i < 30; ++i) //trial
	{
		random_shuffle(key.begin(), key.end());
		for (int z =0; z<26; z++)
			cout << key[z];
		cout << endl;
		besttrialscore = 0;
		for (int j=0; j < 60; j++) //swap
		{
			newkey = swap(key);
			for (int z =0; z<26; z++)
				cout << newkey[z];
			cout << endl;
			score = scoring(ciphertext, newkey, bigram);
			cout << score << endl;
			if (score > besttrialscore)
			{
				key = newkey;
				besttrialscore = score;
			}
			else if (score == besttrialscore)
			{
				if (rand() % 2 == 1)
				{
					key = newkey;
					//besttrialscore = score;
				}
			}
		}
		if (besttrialscore > bestscore)
		{
			bestkey = key;
			bestscore = besttrialscore;
		}
	}
	
	plaintext = translate(ciphertext, maketrans(key, alpha)); 
	cout << "best score: " << bestscore << endl;
	cout << plaintext<<endl; 
}
		
