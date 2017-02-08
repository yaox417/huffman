#include "huffman.h"

int main() {
	int i = 1;
	while(i) {
		int proceed;
		printf("How can I help you today?\nPress 0 to exit. Press 1 to compress. Press 2 to decompress.\n");
		scanf("%d", &proceed);
		fgetc(stdin); /* get rid of the newline in the buffer */

		char filename[MAX_FILE_LENGTH + 1];

		char *to_add = malloc(sizeof(char) * 5);
		to_add[0] = '.';
		to_add[1] = 'h';
		to_add[2] = 'u';
		to_add[3] = 'f';
		to_add[4] = 'f';


		if(proceed == 1) {

			printf("Which file would you like to compress? (Please ignore .txt extension)\n");
			fgets(filename, MAX_FILE_LENGTH, stdin);
			filename[strlen(filename) - 1] = '\0';	

			compress(filename, to_add);

		} else if (proceed == 2) {
			printf("Which file would you like to decompress? (Please enter file that ends with .huff and include .txt extension)\n");
			fgets(filename, MAX_FILE_LENGTH, stdin);
			filename[strlen(filename) - 1] = '\0';

			decompress(filename);

		} else if (proceed == 0) {
			exit(0);
		} else {
			printf("I don't understand because you didn't follow the order. Please choose again!\n\n");
		}		
		free(to_add);
	}
	


	return 0;

}