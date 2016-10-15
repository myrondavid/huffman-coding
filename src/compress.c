#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/compress.h"
#include "../inc/heap.h"
#include "../inc/huffman_tree.h"
#include "../inc/table.h"
#include "../inc/util.h"

#define ASCII_SIZE 256

void compress(unsigned char *file_content, long int file_size, char *dest_filename, char* src_filename) {
  int i, frequency[ASCII_SIZE] = {0};
  Heap* heap = create_heap(ASCII_SIZE);
  Node* bt = NULL;
  int tree_size, trash_size;
  int ext_size, filename_size;
  unsigned char byte1;
  unsigned char byte2;
  unsigned char byte3;


  ext_size = get_extesion_size(src_filename); //recupera o tamanho da extensão
  filename_size = get_name_size(src_filename); //recupera o tamanho do nome do arquivo

  for(i = 0; i < file_size; i++){

    ++frequency[file_content[i]];                                         /* Counts the frequency of every character in the file */

  }

  for(i = 0; i < ASCII_SIZE; i++){
    if(frequency[i] > 0){

      enqueue(heap, create_node(i, frequency[i]));                        /* Uses the index of the array as the char and the value of each index as the frequency when creating nodes */

    }
  }

  bt = heap_to_tree(heap);                                                /* Makes the huff tree out of a heap */
  tree_size = calculate_tree_size(bt, 0);                                 /* Calculates the number of nodes in the huff tree */
  free(heap);

  Table* table = create_table(ASCII_SIZE);                                       /* Creates the table that will contain the value of each bit of the characters present in the text, according to the huff tree */

  char* empty_string = (char*)calloc(17, sizeof(char));                   /* Empty string used to allocate the memory space needed for the average huff binary code of a letter */

  fill_table(bt, table, "", empty_string);                                /* Transforms binary tree "bt" into the coding for each character */

  free(empty_string);

  byte1 = (tree_size>>8);                                                 /* Sets first three bits of the tree size in the first byte */
  byte2 = tree_size;                                                      /* Sets the remainder of the tree size in the second byte */

  FILE *dest_file = fopen(dest_filename, "w");                            /* Creates the destination file */

  putc(0, dest_file);                                                     /* Prints first byte in the destination file as 0, since we don't have the trash size yet */
  putc(byte2, dest_file);                                                 /* Prints second byte in the destination file */

  print_tree_in_file(bt, dest_file);                                      /* Prints the tree in the destination file */
  free_tree(bt);


  byte3 = (ext_size<<5); //seta os 3 primeiros bits do byte com o tamanho da extensão
  putc(byte3, dest_file);  //salva o byte com o tamanho da extensão

  char *ext = get_extension_name(src_filename); //recupera a extensão do nome do arquivo
  int cont;
  for(cont = 0; cont < ext_size; cont++){
    putc(ext[cont], dest_file); //salva cada byte da extensão no arquivo
  }

  trash_size = write_in_file(file_content, file_size, dest_file, table);  /* Prints the compressed content and returns the trash size */
  free_table(table, ASCII_SIZE);

  rewind(dest_file);                                                      /* Rewinds the dest_file pointer to the beginning of the destination file */

  trash_size = trash_size<<5;                                             /* Sets trash size to the first three bits */
  byte1 |= trash_size;                                                    /* Adds trash size to the start of the first byte */
  putc(byte1, dest_file);                                                 /* Prints first byte in the destination file */
  fclose(dest_file);

}
