/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"
#include <stdlib.h>
#include <string.h>
#include "lc4_loader.h"
#include "lc4_disassembler.h"

int reverse_assemble (row_of_memory* memory)
{
  /* binary constants should be proceeded by a 0b as in 0b011 for decimal 3 */
  while(1){
    row_of_memory* node = search_opcode (memory, 1);
    if (!node){
      return 0;
    }

    if ((node -> address <= 0x7FFF && node -> address >= 0x2000) || (node -> address <= 0xFFFF && node -> address >= 0xA000)){
      node -> assembly = malloc(1);
      node->assembly[0] = '\0';
      continue;
    }

    char* binary_string = convert(node-> contents);

    if (binary_string[10] == '1'){
      node -> assembly = malloc(sizeof(unsigned char) * 20);
      char* instruction = "ADD ";
      add_assemblyImm_list(node, instruction);
     
    }else{
       // allocate space for string   
      node -> assembly = malloc(sizeof(unsigned char) * 20);
      char code[4];
      memcpy(code, &binary_string[10], 3);
      code[3] = '\0';
      //
      if (strcmp(code, "000") == 0){
        char* instruction = "ADD ";
        add_assembly_list(node, instruction);
        
      }
      else if (strcmp(code, "001") == 0){
        char* instruction = "MUL ";
        add_assembly_list(node, instruction);

      }
      else if (strcmp(code, "010") == 0) {
        char* instruction = "SUB ";
        add_assembly_list(node, instruction);

      } 
      else if (strcmp(code, "011") == 0) {
        char* instruction = "DIV ";
        add_assembly_list(node, instruction);

      }

    }

  }
	return 0 ;
}


char* convert(short unsigned int contents){
  // convert short unsigned int to binary string
  static char string [17];
  int bit;
  for (int i = 15; i >= 0; i--){
    bit = (contents >> i) & 1;
    string[15-i] = bit + '0';
  }
  string[16] = '\0';
  return string;

}


void add_assembly_list (row_of_memory* node, char* instruction){
       
        char srd[10], srs[10], srt[10];
        char assembly[50];
        int rd = (node-> contents >> 9) & 0x7;
        int rs = (node-> contents >> 6) & 0x7;
        int rt = (node-> contents ) & 0x7;

        sprintf(srd, "R%d, ", rd);
        sprintf(srs, "R%d, ", rs);
        sprintf(srt, "R%d", rt);
        sprintf(assembly, "%s%s%s%s", instruction, srd, srs, srt);
        strcpy( node -> assembly, (const char *)assembly);

}


void add_assemblyImm_list (row_of_memory* node, char* instruction){
        char srd[10], srs[10], simm[10];
        char assembly[50];
        int rd = (node-> contents >> 9) & 0x7;
        int rs = (node-> contents >> 6) & 0x7;
        signed short imm = (node-> contents ) & 0x1F;

        if (imm & 0x10) {
          imm |= 0xFFE0; 
        }
     
        sprintf(srd, "R%d, ", rd);
        sprintf(srs, "R%d, ", rs);
        sprintf(simm, "#%d", imm);
        sprintf(assembly, "%s%s%s%s", instruction, srd, srs, simm);
        strcpy( node -> assembly, (const char *)assembly);

}


void write_asm_file(row_of_memory* node, char* filename) {
    FILE *file;
    char new_filename[100];
    strcpy(new_filename, filename);
    int len = strlen(filename);
    strcpy(&new_filename[len - 3], "asm");
    printf("%s", new_filename);
    file = fopen(new_filename, "w"); 

    if (!file) {
        printf("Failed to create asm file\n");
        return;
    }

    row_of_memory* current = node;

    // .CODE
    fprintf(file, ".CODE\n");

    while (current){
        if (current->assembly) {
            fprintf(file, "%s\n", current->assembly);
        }
        current = current->next;
    }

    //.DATA
    fprintf(file, ".DATA\n");

    fclose(file);

}
