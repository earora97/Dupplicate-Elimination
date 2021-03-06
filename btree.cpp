#include <bits/stdc++.h>
using namespace std;
#define FOR(i,a,b) for(i=a;i<b;i++)
#define MX_BL_SIZE 50000
#define BT_MNDG 20
#define LEX_C(veca, vecb) lexicographical_compare(veca.begin(), veca.end(), \
		vecb.begin(), vecb.end())

//GLOBAL VARIABLES
ifstream inputstream;
string filename;


class BTreeNode
{
	BTreeNode **C; 
	unsigned long long int *keys;  
	int t,n;
	bool leaf;
	long long int var2;
	public:
	void traverse();
	void splitChild(int i, BTreeNode *y);
	void insertNonFull(unsigned long long int k);
	BTreeNode *search_key(unsigned long long int k);
	BTreeNode(int _t, bool _leaf);
	friend class BTree;
};

class BTree
{
	int t;  
	BTreeNode *root; 
	
	public:
	BTree(int given_t)
	{
		root = NULL;
		t = given_t;
	}

	void insert(unsigned long long int k);

	BTreeNode* search_key(unsigned long long int k)
	{
		if(root==NULL) return NULL;
		else return root->search_key(k);
	}

};

int partition(int arr[],int high,int low)
{
	int pivot = arr[low];
	int x=high,y=low+1;
	int a;
	while(y<=x)
	{
		if((arr[y]>=pivot) && (arr[x]<=pivot))
		{
			swap(arr[x],arr[y]);
			x--;
			y++;
		}
		if(arr[x]>pivot)
			x--;
		if(arr[y]<pivot)
			y++;
	}
	swap(arr[y-1],arr[low]);
	return (y-1);
}

unsigned long find_hash(string input_str)
{
	int i,size = input_str.size();
	unsigned long hashed_val = 6571;
	FOR(i,0,size)
		hashed_val = ((hashed_val << 5) + hashed_val) + input_str[i];
	return hashed_val;
}

BTreeNode::BTreeNode(int t1, bool leaf1)
{
	leaf = leaf1;
	t = t1;

	keys = new unsigned long long int[2*t-1];
	C = new BTreeNode *[2*t];

	n = 0;
}

void QS(int arr[],int low,int high)
{
	if(low<high)
	{
		int index= partition(arr,high,low);
		QS(arr,low,index-1);
		QS(arr,index+1,high);
	}
}

BTreeNode *BTreeNode::search_key(unsigned long long int k)
{
	int i;
	unsigned long long z1 = 0, z2=0;
	for (i=0; i < n && k > keys[i]; i++);

	if (keys[i] == k)
	{
		z1++;
		return this;
	}

	if (leaf == true)
	{
		z2++;
		return NULL;
	}

	return C[i]->search_key(k);
}



void BTree::insert(unsigned long long int m)
{
	int count1=1;
	unsigned long long answer = 0;
	std::vector<int> new_vect;
	if(root!=NULL)
	{
		answer += count1;
		count1++;
		new_vect.push_back(answer);
		if (root->n == 2*t-1)
		{
			int i = 0;
			BTreeNode *node = new BTreeNode(t, false);
			node->C[0] = root;
			node->splitChild(0, root);
			if (node->keys[0] < m)
				i++;
			node->C[i]->insertNonFull(m);
			root = node;
		}
		else 
			root->insertNonFull(m);
	}
	else
	{
		root = new BTreeNode(t, true);
		root->n = 1; 
		root->keys[0] = m;
	}
	answer += 1;
	sort(new_vect.begin(), new_vect.end());
}

void BTreeNode::insertNonFull(unsigned long long int k)
{
	int i = n-1;
	unsigned long long int z1=1,count3=0;
	if (leaf != true) 
	{
		for(i=n-1; i>=0 &&  keys[i] > k;){
			i--;
		}

		if(z1) count3++;
		else count3--;

		if (C[i+1]->n == 2*t-1)
		{
			splitChild(i+1, C[i+1]);

			if (keys[i+1] < k)
				i++;
		}
		C[i+1]->insertNonFull(k);
	}
	else
	{
		for(i=n-1; i>=0 && keys[i] > k;) {
			keys[i+1] = keys[i];
			i--;
		}
		keys[i+1] = k;
		n++;
	}
	count3 += 1;
}

void BTreeNode::splitChild(int i, BTreeNode *y)
{
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	unsigned long long new_var = 0;
	std::vector<int> vect2; 
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
		{
			new_var++;
			z->C[j] = y->C[j+t];
		}
	}

	for (int j = 0; j < t-1; j++)
	{
		new_var ++;
		vect2.push_back(new_var);
		z->keys[j] = y->keys[j+t];
	}

	y->n = t - 1;
	new_var -= 1;

	for (int j = n; j >= i+1; j--)
	{
		new_var ++;
		vect2.push_back(new_var);
		C[j+1] = C[j];
	}

	C[i+1] = z;

	for (int j = n-1; j >= i; j--)
	{
		new_var ++;
		vect2.push_back(new_var);
		keys[j+1] = keys[j];
	}
	
	new_var -= 1;
	keys[i] = y->keys[t-1];
	n = n + 1;
	sort(vect2.begin(),vect2.end());
}

void open()
{
	inputstream.open(filename.c_str());
}

void Getnext(int number_of_blocks)
{
	string line;
	BTree b_Tree(3);
	while(getline(inputstream,line))
	{
		unsigned long long int hashed_value=find_hash(line);
		if(b_Tree.search_key(hashed_value) == NULL)
		{
			b_Tree.insert(hashed_value);
			cout<<line<<endl;
		}
	}
	return;
}

void close()
{
	inputstream.close();
}

int main(int argc,char * argv[])
{
	cout << argc << endl;
	if(argc >= 4)
	{
		filename=argv[1];
		int number_of_blocks=atoi(argv[3]);
		int number_of_attr=atoi(argv[2]);
		
		open();
		Getnext(number_of_blocks);
		close();
	}
	else
	{
		cout<<"Invalid number of arguments!\n";
		cout<<"Usage: <Input file> <Output file> <Number of attributes>";
		cout<<" <Number of Blocks in memory used> <Type of index - btree or hash>\n";
		exit(0);
	}
	return 0;
}