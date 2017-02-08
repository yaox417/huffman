#include "huffman.h"

/* TURN ONE FREQUENCY COUNT INTO A LEAF */
struct tree *one_leaf(int i, int freq) {
	struct tree *leaf = malloc(sizeof(struct tree));
	leaf->c = i;
	leaf->freq = freq;
	leaf->left = leaf->right = NULL;
	leaf->is_leaf = 1;
	for(int i = 0; i < 256; i++) {
		leaf->children[i] = 0;
	}
	leaf->children[i] = 1;

	return leaf;
}

/* TURN AN ARRAY OF FREQUENCY COUNTS INTO A LINKED LIST OF TREES */
struct node *leaves (int count[]) {
	
	struct node *prev = malloc(sizeof(struct node));
	struct node *head = prev;
	struct node *curr = prev;

	for(int i = 0; i < 256; i++) {
		if(count[i]) { // if this character appeared at least once
			curr->leaf = one_leaf(i, count[i]);
			curr->next = malloc(sizeof(struct node));
			prev = curr;
			curr = curr->next;
		}
	}
	
	prev->next = NULL;
	free(curr);

	return head;
}