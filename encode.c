#include "huffman.h"

/* BUILDING HUFFMAN TREE
 * in order to build a huffman tree, i need the following functions (all of which are readily available in scheme...):
 * 1. find the tree with the minimum frequency in a forest (do this twice)
 * 2. remove the tree with the minimum frequency from the forest 
 * 3. combine the two trees into one tree, by adding up the frequency, disregarding the character, and placing the two trees as children
 * 4. add this new tree back into the forest 
 * 5. do this recursively until the forest is one big tree - that's our huffman tree
 */

/* combine_two_leaves should take two trees and return a new tree that adds up the frequency and has the two trees as children */
struct tree* combine_two_trees (struct tree* min1, struct tree* min2) { // always return pointers to tree
	struct tree* new_tree = malloc(sizeof(struct tree));
	new_tree->freq = min1->freq + min2->freq;
	new_tree->left = min1;
	new_tree->right = min2;
	for(int i = 0; i < 256; i++) {
		new_tree->children[i] = min1->children[i] | min2->children[i];
	}
	new_tree->is_leaf = 0;
	
	return new_tree;
}

/* remove tree from forest */
struct node* remove_target(struct node* forest, struct tree* to_remove) {
	if(forest->leaf == to_remove) {
		struct node* to_delete = forest;
		forest = forest->next;
		free(to_delete);

		return forest;
	}

	struct node *prev = forest;

	for(struct node *track = forest->next; track != NULL; track = track->next) {
		if(track->leaf == to_remove) {
			prev->next = prev->next->next;
			free(track);
			break;
		}
		prev = prev->next;
	}

	return forest;
}

/* find the tree with the lowest frequency and return it as a single-standing tree */ 
struct tree* find_min(struct node* forest) {
	int comp = forest->leaf->freq; /* point of reference */
	struct tree* target = forest->leaf;

	for(struct node* temp = forest->next; temp != NULL; temp = temp->next) {
		if(temp->leaf->freq < comp) {
			comp = temp->leaf->freq;
			target = temp->leaf;
		}	
	}

	return target;
}

/* add_to_forest takes a newly built tree and add it back into the forest */
struct node* add_to_forest(struct tree* new_tree, struct node* forest) {
	struct node* new_head = malloc(sizeof(struct node));
	new_head->leaf = new_tree;
	new_head->next = forest;

	return new_head;
}

/* huffman_build should take a forest of leaves and return a built huffman tree */
struct tree* huffman_build(struct node* forest) {
	if(forest->next == NULL) { /* if there's only one giant tree left in the forest */
		return forest->leaf; /* then this is the huffman tree we're looking for */
	}


	struct tree* right = find_min(forest);
	forest = remove_target(forest, right);

	struct tree* left = find_min(forest);
	forest = remove_target(forest, left);
	
	struct tree* newly_built = combine_two_trees(left, right);

	forest = add_to_forest(newly_built, forest);


	/* return the result of the build */
	return huffman_build(forest);

}

void print_tree(struct tree* huffman_tree) {
	if(huffman_tree->is_leaf) {
		printf("(%d,%d)", huffman_tree->c, huffman_tree->freq);
	} else {
		printf("(");
		print_tree(huffman_tree->left);
		printf(")(");
		print_tree(huffman_tree->right);
		printf(")");
	}

}

/* encode the original message */
void encode(unsigned char to_encode, struct tree* huffman_tree, struct buffer *buff, FILE *fp) {
	if(huffman_tree->is_leaf) {
		if(huffman_tree->c == to_encode) {
			return;
		} else {
			printf("character %c (as char) %x (as hex) %d (as integer) not found in huffman tree\n", to_encode, to_encode, to_encode);
		}
	} else {
			if (huffman_tree->left->children[to_encode]) {
				/* write 0 to the buffer */
				write_bit(0, buff, fp);
				encode(to_encode, huffman_tree->left, buff, fp);
			} else if (huffman_tree->right->children[to_encode]) {
				/* write 1 to the buffer */
				write_bit(1, buff,fp);
				encode(to_encode, huffman_tree->right, buff, fp);
			} else {
				printf("error");
			}
		}
}