#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_LENGTH 30

/* ***** STRUCTS ***** */

/* 8-bit buffer for writing to file */
struct buffer {
	unsigned char buffer;
	int i; /* the position to write the next bit */
};

/* the leaf */
struct tree {
	unsigned char c;
	int freq;
	struct tree *left, *right;
	int is_leaf;
	char children[256]; /* to keep track of what is in that tree */
};

/* linked list */
struct node {
	struct tree *leaf; /* named 'leaf' to avoid confusion with struct tree */
	struct node *next;
};


/* ***** FUNCTIONS ***** */

/* **leaf.c - build a linked list of leaves according to the frequency of characters from the file** */

/* TURN ONE FREQUENCY COUNT INTO A LEAF */
struct tree *one_leaf(int i, int freq);

/* TURN AN ARRAY OF FREQUENCY COUNTS INTO A LINKED LIST OF TREES */
struct node *leaves (int count[]);


/* **build_tree.c - build a huffman tree (see below)** */

/* BUILDING HUFFMAN TREE
 * in order to build a huffman tree, i need the following functions (all of which are readily available in scheme...):
 * 1. find the tree with the minimum frequency in a forest (do this twice)
 * 2. remove the tree with the minimum frequency from the forest 
 * 3. combine the two trees into one tree, by adding up the frequency, disregarding the character, and placing the two trees as children
 * 4. add this new tree back into the forest 
 * 5. do this recursively until the forest is one big tree - that's our huffman tree
 */

/* find the tree with the lowest frequency and return it as a single-standing tree */ 
struct tree* find_min(struct node* forest);

/* remove tree from forest */
struct node* remove_target(struct node* forest, struct tree* to_remove);

/* combine_two_leaves should take two trees and return a new tree that adds up the frequency and has the two trees as children */
struct tree* combine_two_trees (struct tree* min1, struct tree* min2);

/* add_to_forest takes a newly built tree and add it back into the forest */
struct node* add_to_forest(struct tree* new_tree, struct node* forest);

/* huffman_build should take a forest of leaves and return a built huffman tree */
struct tree* huffman_build(struct node* forest);

/* print out the tree in a stylistic fashion (not useful for final compilation) */
void print_tree(struct tree* huffman_tree);

/* encode the original message; uses buffer.c */
void encode(unsigned char to_encode, struct tree* huffman_tree, struct buffer *buff, FILE *fp);



/* **decode.c - decode the message according to the huffman tree** */

/* helper function for decoding - checks if the current bit is 1 or 0 */
char next_bit(struct buffer *decoding, FILE *to_read);

/* decode the encoded message by traversing the tree - if the currrent bit is 0, go to the left tree; if it's 1, go to the right tree */
char decode(struct tree *forest, struct buffer *decoding, FILE *to_read);


/* **tree_file.c - save and read in frequency array** */

/* save the frequency array from the file */
void save_freq_array(int count[], FILE *tree);

/* reconstruct the tree from the saved tree file */
struct tree *decode_tree(FILE *freq_file);


/* **compress.c - compress a file by counting up frequency of each character, building a huffman tree 
 					and encoding each character traversing the tree */

/* modify filename */
char *save_file_name(char *filename, char *extension);

/* compress the file */
void compress(char *filename, char *to_write); 


/* **decompress.c** */

/* decompress the message file by reading in the tree, rebuilding the huffman tree and decoding bit by bit */
void decompress(char *filename);


/* **buffer.c - write to file using a bit buffer of length 8-bit** */

/* takes in a bit that is either 1 or 0; 
 * write to the buffer, until all 8 bits are used; 
 * if so then flush 
 */
void write_to_file(FILE *fp, struct buffer *buff);

void write_bit(char bit, struct buffer *buff, FILE *fp);

