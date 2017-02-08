all:
	gcc -o huffman buffer.c leaf.c tree_file.c encode.c decode.c compress.c decompress.c main.c