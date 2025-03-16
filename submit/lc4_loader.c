/************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"
#include <string.h>
#include <stdlib.h>
#include "lc4_loader.h"

/* declarations of functions that must defined in lc4_loader.c */

FILE* open_file(char* file_name)
{
  FILE* file = fopen(file_name, "rb");
  if (file != NULL){
    return file;
  }
  printf("Error1: unable to open the file");
	return NULL ;
}


int parse_file(FILE* my_obj_file, row_of_memory** memory) {
    if (my_obj_file == NULL || memory == NULL) {
        return -1;
    }
    unsigned short type, address, n;
    while (fread(&type, sizeof(unsigned short), 1, my_obj_file) == 1) {
        type = reverse_endian(type);
        if (fread(&address, sizeof(unsigned short), 1, my_obj_file) != 1 ||
            fread(&n, sizeof(unsigned short), 1, my_obj_file) != 1) {
                fclose(my_obj_file);
            return -1;
        }
        address = reverse_endian(address);
        n = reverse_endian(n);

        if (type == 0xCADE || type == 0xDADA) { // CODE and DATA part
            for (int i = 0; i < n; ++i) {
                unsigned short contents;
                if (fread(&contents, sizeof(unsigned short), 1, my_obj_file) != 1) {
                    fclose(my_obj_file);
                    return -1;
                }
                contents = reverse_endian(contents);
                if (add_to_list(memory, address + i, contents) == -1) {
                    fclose(my_obj_file);
                    return -1;
                }
            }

        } else if (type == 0xC3B7) { // SYMBOL part
            char label[n + 1];
            if (fread(label, sizeof(char), n, my_obj_file) != n) {
                fclose(my_obj_file);
                return -1;
            }
            label[n] = '\0';

            row_of_memory* node = search_address(*memory, address);
            if (node == NULL) {
                if (add_to_list(memory, address, 0) == -1) {
                    fclose(my_obj_file);
                    return -1;
                }
                node = search_address(*memory, address);
            }
            if (node != NULL) {
                free(node->label); // Free existing label if any
                node->label = strdup(label);
            }
        }
    }
    fclose(my_obj_file);
    return 0;
}

short unsigned int reverse_endian(short unsigned int num) {
    return (num >> 8) | (num << 8);
}
