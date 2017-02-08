#include "huffman.h"

/* helper function for decoding - checks if the current bit is 1 or 0 */
char next_bit(struct buffer *decoding, FILE *to_read) {
	if(decoding->i > 7) {		
		decoding->buffer = fgetc(to_read);
		decoding->i = 0;
	} 

	int bit_mask = 1 << (7 - decoding->i);

	decoding->i++;

	if(bit_mask & decoding->buffer) {

		return 1;

	} else {

		return 0;
	}
	
}


/* decode the encoded message by traversing the tree - if the currrent bit is 0, go to the left tree; if it's 1, go to the right tree */
char decode(struct tree *forest, struct buffer *decoding, FILE *to_read) {
	struct tree *traverse = forest;
	char to_return;
	if(traverse->is_leaf) {
		to_return = traverse->c;
		
	} else {
		char next = next_bit(decoding, to_read);
		if(next == 0) {
			to_return = decode(traverse->left, decoding, to_read);

		} else if(next == 1) {
			to_return = decode(traverse->right, decoding, to_read);

		}
	}

	return to_return;
}


