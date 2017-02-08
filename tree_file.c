#include "huffman.h"

/* save the frequency array from the file */
void save_freq_array(int count[], FILE *tree) {
	for(int i = 0; i < 256; i++) {
		if(count[i]) {
			fprintf(tree, "%c", '(');
			fprintf(tree, "'%c", i);
			fprintf(tree, "'%d", count[i]);
			fprintf(tree, "%c", ')');					
		}
	}
}

/* reconstruct the tree from the saved tree file */
struct tree *decode_tree(FILE *freq_file) {
	unsigned char c;
	int freq = 0;
	int count[256] = { 0 };

	unsigned char next;
	next = fgetc(freq_file);
	

	while(next != '\r') {
		if(next == '('){
			next = fgetc(freq_file);
			if(next == '\'') {
				c = fgetc(freq_file);
				next = fgetc(freq_file);
				while(next != ')') {
					next = fgetc(freq_file);
					if((next - '0') >= 0 && (next - '9') <= 0) {
						freq = freq * 10 + (next - '0');
					}
				}
				/* add this entry to the frequency count array */
				count[c] = freq;
				/* reset the frequency count */
				freq = 0;
			} 
		}
		next = fgetc(freq_file);
	}

	struct node *forest = malloc(sizeof(struct node));
	forest = leaves(count);

	struct tree *huffman = huffman_build(forest);	
	
	return huffman;
}