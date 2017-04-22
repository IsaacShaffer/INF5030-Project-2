#include <fstream>
#include <iostream>
#include <random>

using namespace std;

struct Node
{
	Node *A;
	Node *C;
	Node *G;
	Node *T;
	int *loc;
};

class randomNum { 

    std::mt19937 rng;

public:
    randomNum() : rng(std::random_device()()) {}
    int operator()(int low, int high) { 
        int uni = low + rng() % (high-low);
        return uni;
        }
};

class PrefixTrie
{
	private:
	Node *root;
	unsigned int nodeCount, maxReps;
	
	public:
	PrefixTrie()
	{
		this->nodeCount = 0;
		this->maxReps = 0;
		this->root = newNode();
	}
	
	PrefixTrie(char *seq, int seqSize, int wordSize)
	{
		int i,j;
		char loadSeq[wordSize];
		this->nodeCount = 0;
		this->maxReps = 0;
		this->root = newNode();
		for (i = 0; i < seqSize-wordSize+1; i++)
		{
			for (j = 0; j < wordSize; j++)
			{
				loadSeq[j] = seq[i+j]; 
			}// end j for
			add(loadSeq, wordSize, i); 
		}// end i for
	}
	
	~PrefixTrie()
	{
		prune(this->root);
		this->root = NULL;
	}

	Node *newNode()
	{
		Node *current = new Node;
		current->A = NULL;
		current->C = NULL;
		current->G = NULL;
		current->T = NULL;
		current->loc = new int;
		current->loc[0] = 1;
		this->nodeCount++;
		return current;
	}
	
	void deleteNode(Node *current)
	{
		delete current->loc;
		delete current;
		this->nodeCount--;
	}
	
	void prune(Node *current)
	{
		if (current->A)
		{
			prune(current->A);
		}
		if (current->C)
		{
			prune(current->C);
		}
		if (current->G)
		{
			prune(current->G);
		}
		if (current->T)
		{
			prune(current->T);
		}
		deleteNode(current);
	}
	
	void add(char *seq, int seqSize, int locInG)
	{
		Node *current = this->root;
		for (int i = 0; i < seqSize; i++)
		{
			switch (seq[i])
			{
			case 'A':
				if (current->A)
				{
					current = current->A;
				} else {
					current->A = newNode();
					current = current->A;
				}
				break;
			
			case 'C':
				if (current->C)
				{
					current = current->C;
				} else {
					current->C = newNode();
					current = current->C;
				}
				break;
			
			case 'G':
				if (current->G)
				{
					current = current->G;
				} else {
					current->G = newNode();
					current = current->G;
				}
				break;
			
			case 'T':
				if (current->T)
				{
					current = current->T;
				} else {
					current->T = newNode();
					current = current->T;
				}
				break;			
			}// end switch
		}// end for
		int *temp = current->loc;
		current->loc = new int[temp[0]+1];
		for (int k=0; k < temp[0]; k++)
		{
			current->loc[k] = temp[k];
		}
		current->loc[current->loc[0]] = locInG;
		current->loc[0]++;
		if (temp[0] > maxReps) {maxReps = temp[0]; cout << "Max Reps: "<<maxReps<<endl;}
		delete temp;
	}
	
	unsigned int size()
	{
		return this->nodeCount;
	}
	
	int *found(char *seq, int seqSize)
	{
		Node *current = this->root;
		
		int i = 0;
		
		while ((current != NULL) && (i < seqSize))
		{
			switch (seq[i])
			{
				case 'A':	
					current = current->A;
					break;

				case 'C':
					current = current->C;
					break;

				case 'G':
					current = current->G;
					break;
					
				case 'T':
					current = current->T;
					break;
			}//end switch
			i++;
		}// end while
		if (current != NULL)
		{
			return (current->loc);
		} else {
			return NULL;
		}
	}	
};

int main(int argc, char *argv[])
{
	unsigned int i, j, k, pos;
	unsigned int N = 100, wordSize = 23, G = 0;
	randomNum randy;
	char temp;
	
	if (argc > 1)
	{
		N = std::atoi(argv[1]);
	}
	
	// Load DENV2
	fstream f;
	f.open("./test_genome.fasta", std::fstream::in);
	while (!f.eof()) 
	{
		f >> temp;
		G++;
	}
	
	cout << "Genome Size: "<<G<<endl;
	
	f.clear();
	f.seekg(0, std::ios::beg);
	char genome[G];
	for (i=0; i<G; i++)
	{
		f >> genome[i];
	}
	
	PrefixTrie PT(genome, G, wordSize);
	
	cout << "Trie Size: " << PT.size() << endl;
	
/*	for (i=0; i < N; i++)
	{
		pos = randy(0, G-merSize);	
		for (j = 0; j < merSize; j++)
		{
			randyMer[j] = DENV2[j+pos];
		}
		PT.add(randyMer, merSize);
//		randyMer[1] = 'A';
//		if (i%(N/100)==0)
//		{
//			if (PT.found(randyMer, merSize))
//			{
//				cout << i << " Hooray!!" << endl;
//			}
//		}
	}
	
	cout<< endl << "For "<<N<<" random fragments, "<<PT.size()<<" nodes were required."<< endl;
*/	
	return 0;
}

	