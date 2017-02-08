#include "huffman.h"

/* modify filename */
char *save_file_name(char *filename, char *extension) {
	char *save_file = malloc(sizeof(char) * strlen(filename) + strlen(extension)); 


	int i;
	for(i = 0; i < strlen(filename); i++) {
		save_file[i] = filename[i];
	}

	for(i = strlen(filename); i < strlen(filename) + strlen(extension); i++) {		
		save_file[i] = extension[i - strlen(filename)];
	}
	save_file[i] = '\0';
	
	return save_file;
}

/* compress the file */
void compress(char *filename, char *to_write) {	
	char *txt = malloc(sizeof(char) * 4);
	txt[0] = '.';
	txt[1] = 't';
	txt[2] = 'x';
	txt[3] = 't';

	int count[256] = { 0 };
	char *to_compress = save_file_name(filename, txt);
	FILE *fp = fopen(to_compress, "r");
	if(fp == NULL) {
		printf("File %s is not opened correctly. Terminating.\n", to_compress);
		exit(0);
	}

	/* build frequency array */
	int c;
	c = fgetc(fp);
	while(c != EOF) {
		count[c]++;
		c = fgetc(fp);
	}
	fclose(fp);

	/* use the bell character to signal the end of file */
	count[7] = 1; 

	/* create a forest of leaves */
	struct node *forest = malloc(sizeof(struct node));
	forest = leaves(count);

	/* build huffman tree */
	struct tree *huffman = malloc(sizeof(struct tree));
	huffman = huffman_build(forest);

	/* write out the tree */
	char *compressed = save_file_name(save_file_name(filename, to_write), txt);
	printf("Saving the tree and compressed file at %s!\n\n\n", compressed);
	FILE *tree_file = fopen(compressed,"w");
	if(tree_file == NULL) {
		printf("File %s is not opened correctly. Terminating.\n", compressed);
		exit(0);
	}
	save_freq_array(count, tree_file);
	fprintf(tree_file, "%c\n", '\r');
	fclose(tree_file);


	/* initialize buffer */
	struct buffer *to_be_encoded = malloc(sizeof(struct buffer));
	to_be_encoded->buffer = 0;
	to_be_encoded->i = 0;
	
	/* write out the encoded message */
	FILE *fp_read = fopen(to_compress, "r");
	FILE *fp_write = fopen(compressed, "a");

	if(fp_read == NULL) {
		printf("File %s is not opened correctly. Terminating.\n", to_compress);
		exit(0);
	}
	if(fp_write == NULL) {
		printf("File %s is not opened correctly. Terminating.\n", compressed);
		exit(0);
	}

	for((c = fgetc(fp_read)); c != EOF; c = fgetc(fp_read)) {
		encode(c, huffman, to_be_encoded, fp_write);
	}
	encode('\a', huffman, to_be_encoded, fp_write);

	/* flush buffer if there is bits less than 8 written */
	if(to_be_encoded->i) {
		write_to_file(fp_write, to_be_encoded);
	}
		
	/* close files; free pointers */
	fclose(fp_write);
	fclose(fp_read);	
	free(huffman);
	free(to_be_encoded);
	free(txt);
	free(to_compress);
	free(compressed);

}