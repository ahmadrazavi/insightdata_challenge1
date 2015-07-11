#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

//input and output files
ofstream output1;
ofstream output2;
ifstream inputfile;

//binary search tree for holding the whole words.
typedef struct word1{
	string word;
	int word_count;
} word1;

//binary search tree for holding the whole words.
typedef struct node_t{
	string word;
	int count;
	struct node_t *left, *right;
} node_t;

//traverse the tree to print the words in order.
void travers_BST(struct node_t *head){
	if (head){
		travers_BST(head->left);
		output1 << head->word << "  " << head->count << endl;
		travers_BST(head->right);
	}
}

// add a word to BST. it is not recuirsive to support a huge amount of data.
void add_BST(struct node_t *head, string s, int count)
{
	node_t *p;
	int left_node = 0;
	
	if (head->count == 0){

	head->count = count;
	head->left = NULL;
	head->right = NULL;
	head->word = s;
	return;
	}
	
	p = head;

	while (1){
		if ((head!=NULL) && s == head->word)
		{
			head->count = head->count + count;
//			cout << endl << "tree:" << head->word << ":" << head->count << endl;
			break;
		}
		else if (head == NULL){
			node_t* newnode = new node_t;
			newnode->word = s;
			newnode->left = NULL;
			newnode->right = NULL;
			newnode->count = count;
			head = newnode;
			if (left_node == 1)
				p->left = head;
			else
				p->right = head;
			//cout << endl << "new node created" << newnode->word << endl;
			break;
		}
		else if (s < head->word){
			p = head;
			head = head->left;
			if(head==NULL)left_node = 1;
		}
		else
		{
			p = head;
			head = head->right;
			if (head == NULL)left_node = 0;
		}
	}
}

int count_unique(string s, struct node_t *head){
	istringstream iss(s);
	
	vector<word1> list;
	int unique_iworld = 0;
	
	//return 0 for empty tweet
	if (s == "")return 0;
	do
	{
		unique_iworld = 1;
		string sub;
		iss >> sub;

		if (list.size() == 0) { list.push_back(word1()); list[0].word = sub; list[0].word_count = 1; }
		else
		{
			for (unsigned i = 0; i < list.size(); ++i) {
				if (list[i].word == sub){ list[i].word_count++; unique_iworld = 0; }
			}
			if (unique_iworld==1 && sub!="")
			{
				int last = list.size();
				list.push_back(word1()); 
				list[last].word = sub;
				list[last].word_count = 1;
			}
		}			
	} while (iss);

	//add words to the BST
	for (unsigned i = 0; i < list.size(); i++) {
		add_BST(head, list[i].word, list[i].word_count);
	}

	//return the number of unique words in the current tweet
	return list.size();
}

void main()
{	

	inputfile.open("tweet_input/tweets.txt");
	output1.open("tweet_output/ft1.txt");
	output2.open("tweet_output/ft2.txt");

	string itweet;
	
	//the number of unique words in whole tweets
	vector<int> unique_word_whole;
	
	//the root of BST
	node_t* root = new node_t;
	root->count = 0;
	
	while (std::getline(inputfile, itweet))
	{
		//count the number of unique words in each tweet and add it to the BST
		int iunique = count_unique(itweet, root);
		
		//the number of unique words in whole tweets
		unique_word_whole.push_back(iunique);
	
		//calculate the median and print in the output file
		int middle = unique_word_whole.size() / 2;
		if (unique_word_whole.size() % 2 == 0)	{
			output2 << (unique_word_whole[middle - 1] + unique_word_whole[middle]) / (float)2<<endl;
		}
		else
			output2 << unique_word_whole[middle]<<endl;
	}

	//traverse the BST
	travers_BST(root);
	
	inputfile.close();
	output1.close();
	output2.close();
	
	return;
}
