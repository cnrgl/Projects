#include <stdio.h>
struct treeNode{
	char character;
	unsigned freq;
	struct treeNode *leftLeaf,*rightLeaf;
};
struct heapNode{
	struct treeNode *leaves;
	struct heapNode *next;
};
void insertion_swap(struct heapNode **head) // inserted value started from head
{
	struct heapNode *prev=*head, *node = (*head)->next, *second;
	// prev -> node -> second
	if(node != NULL) 
		if (prev->leaves->freq > node->leaves->freq)		
		{
			prev->next = node->next;
			node->next = prev;
			*head = node;//only way to change head referance
			prev=node;
			node = prev->next; 
			while(node->next != NULL)
			{
				second=node->next;
				if(node->leaves->freq > (second)->leaves->freq)
				{
					prev->next = second;
					node->next = second->next;
					second->next=node;
					prev = second;
				}
				else
					break;
			}
		}
}
struct treeNode *newLeaf(char character, unsigned freq, struct treeNode **leftLeaf, struct treeNode **rightLeaf)
{
	struct treeNode *leaf = (struct treeNode*)malloc(sizeof(struct treeNode));
	leaf->leftLeaf=*leftLeaf;
	leaf->rightLeaf=*rightLeaf;
	leaf->freq=freq;
	leaf->character=character;
	return leaf;
}
void pushHeap(struct heapNode **headref, struct treeNode **leaf)
{
	struct heapNode *node = (struct heapNode*)malloc(sizeof(struct heapNode));
	node->next = *headref;
	node->leaves = *leaf;
	*headref = node;
	insertion_swap(headref);
}
struct treeNode *combine(struct heapNode **head)
{
	struct heapNode *second=(*head)->next;
	struct treeNode *node;
	while(second != NULL)
	{
		node=newLeaf(0,((*head)->leaves->freq + second->leaves->freq), &(second->leaves), &((*head)->leaves));
		*head=second->next;
		pushHeap(&(*head), &node); 
		second=(*head)->next;
	}
	return (*head)->leaves;
}
int encode(struct treeNode *top, char character, int cypher)// eq binary tree search 
{
	int value;
	//printf("L = %d\n",top->freq); // node debug
	if(top->character > 0)//left leaf
	{
		cypher<<1;
		if(top->character == character)
			return cypher;
	}
	if(top->character > 0)// right leaf
	{
		cypher<<=1;
		cypher++;
		if(top->character == character)
			return cypher;
		else
			return -1; // empty root
	}
	else // else right node
	{
		cypher<<=1;
		cypher++; 
		value=encode(top->rightLeaf, character, cypher);
		if(value>=0)
			return value;
	}
	// last option is left node (!!! not leaf)
	cypher<<1;
	return encode(top->leftLeaf, character, cypher); // if there are no such character function will return -1
}
int main(void)
{
	char str[50];
	unsigned freq[255]={0},i=-1;
	struct heapNode *heap=NULL;
	struct treeNode *root, *null=NULL;
	
	printf("input a text=");
	gets(str);
	
	while(str[++i]!='\0') // set frequencies
		freq[str[i]]+=1;
	
	for(i=0;i<255;i++)
	{
		if(freq[i]>0)
		{
			root=(struct treeNode*)(newLeaf((char)i,freq[i], &null, &null)); 
			//printf("f=%d\n",freq[i]); // debug for see frequencies
			pushHeap(&heap,&root); // still every leaf is also root
		}
	}
	root=combine(&heap); // tree constructed
	i=-1;
	printf("\ncypher= ");
	while(str[++i]!='\0')
		printf("%d ",encode(root,str[i],0)); // if you get -1 this mean there is no such a character
		
	return 0;
}
