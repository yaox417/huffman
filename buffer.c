#include "huffman.h"


/* takes in a bit that is either 1 or 0; 
 * write to the buffer, until all 8 bits are used; 
 * if so then flush 
 */
void write_bit(char bit, struct buffer *buff, FILE *fp) { 
	// need to pass in the pointer to a struct buffer, instead of a struct buffer
	buff->buffer |= (bit << (7- buff->i));
	// printf("buffer is now %d\n", buff->buffer);
	buff->i++;
	if(buff->i > 7) {
		write_to_file(fp, buff);
	}
}   

void write_to_file(FILE *fp, struct buffer *buff) {
	// need to write to buffer that the pointer points to, so need to deference it
	fwrite(&(buff->buffer), 1, sizeof(buff->buffer), fp);
	buff->buffer = 0;
	buff->i = 0;
}  