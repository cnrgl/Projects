#include <iostream>
using namespace std;

class node
{
	public:
		int data;
		node* next;
	node(int d)
	{
			data=d;
			next=NULL;
	}
};
void insert(node *head,int data)
{
	node* n=new node(data);
	node *temp=head;
	// Traverse till we get to end of 
    // the linked list 
    
    while(temp->next!=NULL)
    	temp=temp->next;
    	
    temp->next=n;
}
void push(node **head,int data)
{
	node * n=new node(data);
	n->next=head;
	head=n;
}
int detect_loop(node *list)
{
	node *slow=list,*fast=list;
	while(slow&&fast&&fast->next)
	{
		slow=slow->next;
		fast=(fast->next)->next;
		if(slow==fast)
			return 1;
	}
	return 0;
}
int main(void)
{
	
	node *head=NULL;
	push(head,5);
	push(head,4);
	push(head,3);
	push(head,2);
	push(head,1);
	
	head->next->next->next->next=head;
	printf("result is %d",detect_loop(head));
	
	
	
	
	return 0;
}
