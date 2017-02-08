#include "huffman.h"

/* decompress the message file by reading in the tree, rebuilding the huffman tree and decoding bit by bit */
void decompress(char *filename) {
	struct tree *huffman_from_saved = malloc(sizeof(struct tree));

	char *txt = malloc(sizeof(char) * 4);
	txt[0] = '.';
	txt[1] = 't';
	txt[2] = 'x';
	txt[3] = 't';

	FILE *tree_rebuild = fopen(filename, "r");
	if(tree_rebuild == NULL) {
		printf("File %s is not opened correctly. Terminating.\n", filename);
		exit(0);
	}
	huffman_from_saved = decode_tree(tree_rebuild);
	printf("Saving the deecompressed file at %s!\n\n\n", save_file_name(filename, txt));
	FILE *to_write = fopen(save_file_name(filename, txt), "w");
	if(to_write == NULL) {
		printf("File %s is not opened correctly. Terminating.\n", filename);
		exit(0);
	}

	struct buffer *decoding = malloc(sizeof(struct buffer));
	decoding->buffer = fgetc(tree_rebuild);
	/* deal with the junk in the decoding file (it has ` and a newline) */
	if(decoding->buffer == '\r') {
		decoding->buffer = fgetc(tree_rebuild);
	}

	if(decoding->buffer == '\n') {
		decoding->buffer = fgetc(tree_rebuild);
	}

	decoding->i = 0;

	char next = decode(huffman_from_saved, decoding, tree_rebuild);
	while(next != '\a') {
		fprintf(to_write, "%c", next);
		next = decode(huffman_from_saved, decoding, tree_rebuild);
	}

	/* close files and free memories */
	fclose(tree_rebuild);
	fclose(to_write);
	free(decoding);
	free(huffman_from_saved);
	free(txt);
}