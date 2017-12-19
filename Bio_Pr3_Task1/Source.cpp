#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <time.h>
//motif and median string search
using namespace std;

const string filename3 = "Input.txt";

vector<char> Alphabet = { 'A', 'C', 'G', 'T' };

int Score(int l, int i, vector<vector<int> > ProfileMatrix)
{
	int* max = new int[l];
	int score = 0;

	for (int k = 0; k < l; k++)
	{
		max[k] = ProfileMatrix[0][k];//init max
		for (int j = 0; j < i; j++)
		{
			if (ProfileMatrix[j][k] > max[k])
			{
				max[k] = ProfileMatrix[j][k];
			}
		}
		score += max[k];
	}
	score += (ProfileMatrix.size() - i)*Alphabet.size();
	delete[] max;
	return score;
}

int Score(int l, vector<vector<int> > ProfileMatrix)
{
	int* max = new int[l];
	int score = 0;

	for (int k = 0; k < l; k++)
	{
		max[k] = ProfileMatrix[0][k];//init max
		for (int j = 0; j < ProfileMatrix.size(); j++)
		{
			if (ProfileMatrix[j][k] > max[k])
			{
				max[k] = ProfileMatrix[j][k];
			}
		}
		score += max[k];
	}
	delete[] max;
	return score;
}


//Returns char index in alphabeth array where char == c;
int CharIndex(vector<char> alphabet, char c)
{
	for (int i = 0; i < alphabet.size(); i++)
		if (alphabet[i] == c)
			return i;
	return -1;
}

//DNA  - array of strings, t - num of str, l - pattern length, n - num of letters in row
vector<string> motifSearch(string *DNA, int t, int l, int n)
{
	int global_score = 0;
	int score = 0;
	int* max = new int[l];
	std::string consensus;
	consensus.resize(l);//allocate memory
	vector<string> AlignmentMatrix(t);
	vector<vector<int> > ProfileMatrix(Alphabet.size());
	vector<string> Output(t);
	int *ind = new int[t];//iterator
	for (int i = 0; i < t; i++)//first combination
		ind[i] = 0;

	int c = pow(n - l + 1, t); // number of different positions
	for (int i = 0; i < c; i++)
	{
		//Fill ProfileMatrix with 0
		for (int i = 0; i < ProfileMatrix.size(); i++)
		{
			ProfileMatrix[i].reserve(l);
			ProfileMatrix[i].resize(l);
			for (int j = 0; j < l; j++)
			{
				ProfileMatrix[i][j] = 0;
			}
		}
		//Fill AlignmentMatrix with current combination
		for (int j = 0; j < t; j++)
		{
			AlignmentMatrix[j] = DNA[j].substr(ind[j], l);
		}
		//Fill Profile Matrix
		for (int k = 0; k < l; k++)
		{
			for (int j = 0; j < t; j++)
			{
				ProfileMatrix[CharIndex(Alphabet, toupper(AlignmentMatrix[j][k]))][k]++;
			}
		}
		//[ Score calculating ]
		if (score == 0)
		{
			score = Score(l, ProfileMatrix);
		}
		else
		{
			if (Score(l, 3, ProfileMatrix) < global_score) //Calculate first 3 rows. For 1 =~ 250 ms. For 3 ~= 200ms.
			{
				goto ByPass;
			}
			else
			{
				score = Score(l, ProfileMatrix);
			}
		}

		if (score == global_score)
		{
			Output = AlignmentMatrix;
		}
		else
		{
			if (score > global_score)
			{
				global_score = score;
				Output.clear();
				Output = AlignmentMatrix;
				cout << "New max score:" << global_score << endl;
			}
		}
		ByPass:
		//Next combination for AlignmentMatrix
		for (int j = 0; j < t; j++)
		{
			ind[j]++;
			if (ind[j] < (n - l + 1))
				break;
			ind[j] = 0;
		}
	}
	delete[] ind;
	AlignmentMatrix.clear();
	ProfileMatrix.clear();
	return Output;
}

int main()
{
	clock_t t1, t2;
	string* DNA;
	vector<string> motifs;
	ifstream fin(filename3, ios_base::in);
	//Fill num[]
	int *num = new int[2];//num[0] = 3, num[1] = 5 from file;
	for (int i = 0; i < 2; i++)
		fin >> num[i];
	//Fill DNA with rows
	DNA = new string[num[1]];
	int i = 0;
	fin.seekg(to_string(num[0] + num[1]).size() + 1, ios::cur);//skip first row: 3 5
	while (getline(fin, DNA[i]))
		i++;
	for (int i = 0; i < num[1]; i++)
		cout << DNA[i] << endl;
	//Alghorithm
	t1 = clock();
	motifs = motifSearch(DNA, num[1], num[0], DNA[0].size());
	t2 = clock();
	//Output
	for (int i = 0; i < motifs.size(); i++)
		cout << motifs[i] << endl;
	cout << t2 - t1 << "ms" << endl;
	delete[] num;
	delete[] DNA;
	return 0;
}