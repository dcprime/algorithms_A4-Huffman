#pragma once
/************************ call_huffman.h ************************/

/************************** prototypes **************************/

void run_file(FILE* fptr, char operation, char* file_name);

void compress(FILE* fptr, char* file_name);

void decompress(FILE* fptr);