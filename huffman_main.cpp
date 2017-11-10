/************************ huffman_main.cpp ************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include "huffman.h"
#include "call_huffman.h"

void main(int argc, char *argv[]) {

    // create file pointer for source file and output file
    int run = TRUE;
    int runsub;
    FILE *fptr;
    char file_name[STRSIZE];
    char cdq[STRSIZE];

    while (run == TRUE) {

        fflush(stdin);
        printf("\nChoose an option:\n");
        printf("[C]ompress file\n");
        printf("[D]ecompress file\n");
        printf("[Q]uit\n");
        printf("\nEnter C, D, or Q: ");

        fflush(stdin);

        fgets(cdq, STRSIZE, stdin);

        switch (cdq[0]){
        case 'c':
        case 'd':
            runsub = TRUE;
            while (runsub == TRUE)
            {
                char choice[3] = { "de" };

                // compress/decompress menu
                printf("\nEnter the filename to ");
                if (cdq[0] == 'd') printf("de");
                printf("compress (with extension)\n");
                printf("or enter [B] to go back: ");

                fflush(stdin);
                
                // fgets captures newline after string input, while sscanf removes it
                fgets(file_name, STRSIZE, stdin);
                sscanf(file_name, "%s", file_name);

                if (tolower(file_name[0]) == 'b' && strlen(file_name) == 1 )
                {
                    // file_name[1] == '\0'
                    runsub = FALSE;
                    fflush(stdin);
                }
                else
                {
                    fopen_s(&fptr, file_name, "rb");

                    //fptr = fopen(file_name, "rb");

                    if (fptr != NULL)
                    {
                        run_file(fptr, cdq[0], file_name);
                    }
                    else
                    {
                        printf("\nThat is not a valid file.");
                        printf("\nCheck the filename and path and try again.\n");
                    }
                }
            } //end while sub menu
            break;
        case 'q':
            run = FALSE;
            break;
        default:
            printf("\nError: That option is not valid\n\n");
            break;
        }       
    } // end while main menu
}