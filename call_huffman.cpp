/************************ call_huffman.cpp ************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include "call_huffman.h"
#include "huffman.h"

void run_file(FILE* fptr, char operation, char* file_name) {
    switch (operation){
    case 'c':
        compress(fptr, file_name);
        break;
    case 'd':
        decompress(fptr);
        break;
    default:
        break;
    }
    return;
}

void compress(FILE* fptr, char* file_name) {

    FILE* output_ptr;
    char output_file[15] = "compressed.bin";
    unsigned int file_in_size, file_out_size, max_out_size;    // input, output, and max output file sizes in bytes

    // get and save filesize
    fseek(fptr, 0, SEEK_END);
    file_in_size = ftell(fptr);    

    // calloc a buffer to hold the input file contents
    unsigned char* input_buffer;
    input_buffer = (unsigned char *)calloc(file_in_size, sizeof(unsigned char));

    // read input file into input buffer
    fseek(fptr, 0, SEEK_SET);
    fread(input_buffer, 1, file_in_size, fptr);

    fclose(fptr);

    int fileNameLen = strlen(file_name);

    // set output buffer size as per compression documentation specs:
    // input buffer size * (101/100) + 320 bytes + size of file name in bytes + int (to hold file size)
    max_out_size = file_in_size * (101 / 100) + 320 + fileNameLen + 2 * sizeof(unsigned int);

    printf("max_out_size value is %d\n", max_out_size);

    // calloc a buffer to hold the output compressed file (384 bytes larger than input buffer)
    unsigned char* output_buffer;
    output_buffer = (unsigned char *)calloc(max_out_size, sizeof(unsigned char));

    // compress file and save file size
    file_out_size = Huffman_Compress(input_buffer, output_buffer, file_in_size);

    free(input_buffer);

    printf("\nOutput file is %d bytes\n", file_out_size);

    // write output_buffer to output_file
    fopen_s(&output_ptr, output_file, "wb");

    if (output_ptr != NULL) {
                
        // write input file size to file (unsigned int at start of file)
        fwrite(&file_in_size, sizeof(int), 1, output_ptr);

        // write number of chars for input file name to file (unsigned int)
        fwrite(&fileNameLen, sizeof(int), 1, output_ptr);

        // write file name to file (char array after file size)
        fprintf(output_ptr, "%s", file_name);

        // write buffer to file
        fwrite(output_buffer, 1, file_out_size, output_ptr);

        free(output_buffer);
        fclose(output_ptr);

        // print results of compression
        printf("File %s\n", file_name);
        printf("With original size: %d bytes\n", file_in_size);
        printf("Compressed to size: %d bytes\n", file_out_size);
        printf("Compression factor: %.2f\n\n", (float)file_out_size/(float)file_in_size);
    }
    else {
        printf("\nError writing data to %s\n", output_file);
    }

    return;
}

void decompress(FILE* fptr) {
    FILE* output_ptr;
    unsigned int cfile_end, cfile_size, file_out_size, fileNameLen;    // input, output, and max output file sizes in bytes
    char out_file_name[STRSIZE];

    // get and save cfile_end
    fseek(fptr, 0, SEEK_END);
    cfile_end = ftell(fptr);

    // read output file size from compressed file
    fseek(fptr, 0, SEEK_SET);
    fread(&file_out_size, sizeof(int), 1, fptr);

    // read number of chars for input file name to file (unsigned int)
    fread(&fileNameLen, sizeof(int), 1, fptr);

    // out_file_name must be less than 100 chars so that it can be set as const char instead of char
    if (fileNameLen > STRSIZE) {
        printf("Unable to decompress: Output file name exceeds %d characters\n", STRSIZE);
        fclose(fptr);
        return;
    }
    else {
        // read file name to output_file char array
        fread(&out_file_name, sizeof(unsigned char), fileNameLen, fptr);

        // terminate string of output_file
        out_file_name[fileNameLen] = '\0';
    }

    // get and save cfile_size
    cfile_size = cfile_end - ftell(fptr);

    // calloc a buffer to hold the compressed file data
    unsigned char* cfile;
    cfile = (unsigned char *)calloc(cfile_size, sizeof(unsigned char));

    // write compressed file to cfile buffer
    fread(cfile, 1, cfile_size, fptr);

    fclose(fptr);

    // calloc a buffer to hold the output (uncompressed) file data
    unsigned char* output_buffer;
    output_buffer = (unsigned char *)calloc(file_out_size, sizeof(unsigned char));

    // decompress file and save in output_buffer
    Huffman_Uncompress(cfile, output_buffer, cfile_size, file_out_size);

    free(cfile);

    // write output_buffer to output_file 
    fopen_s(&output_ptr, out_file_name, "wb");

    if (output_ptr != NULL) {

        // write buffer to file
        fwrite(output_buffer, 1, file_out_size, output_ptr);

        fclose(output_ptr);

        // print results of compression
        printf("File %s\n", out_file_name);
        printf("With compressed size: %d bytes\n", cfile_size);
        printf("Decompressed to size: %d bytes\n", file_out_size);
        printf("Decompression factor: %.2f\n\n", (float)file_out_size / (float)cfile_size);
    }
    else {
        printf("\nError writing data to %s\n", out_file_name);
    }

    return;
}