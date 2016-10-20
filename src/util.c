#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "../inc/compress.h"
#include "../inc/decompress.h"
#include "../inc/util.h"

#define ASCII_SIZE 256

int get_extesion_size(char *src_filename){
	int size = strlen(src_filename);
	int i, size_ext=0;
	for (i = size-1; i >= 0; --i, size_ext++){
		if(src_filename[i] == '.'){
			return size_ext;
		}
	}
}

int get_name_size(char *src_filename){
	int size = strlen(src_filename), size_ext=0;
	size_ext = get_extesion_size(src_filename);
	return size-size_ext-1;
}

unsigned char get_byte(int size_ext, int size_password){
	unsigned char byte;
	byte=(size_ext<<5);
	byte|=size_password;
	return byte;
}

unsigned char *get_file_name(char *src_filename){
	int size = get_name_size(src_filename);
	char *filename = (char*)malloc(sizeof(char)*size+1);
	int i;
	for(i = 0; i < size; i++){
		filename[i] = src_filename[i];
	}
	filename[i] = '\0';	
	return filename;
}

unsigned char *get_extension_name(char *src_filename){
	int size_e = get_extesion_size(src_filename);
	int size_f = get_name_size(src_filename);
	char *extension = (char*)malloc(sizeof(char)*size_e+1);
	int i, j;
	for(i = size_f+1, j = 0; j < size_e; i++, j++){
		extension[j] = src_filename[i]; 
	}
	extension[j] = '\0';
	return extension;
}



//recebe um string com a senha, retorna uma string no formato MD5
unsigned char *string_to_md5(const char *str, int str_size){
    int i;
    unsigned char digest[16];
    unsigned char *out = (unsigned char*)malloc(33);
    //O código cria uma variável de “contexto de MD5” e o inicia. Esta variável irá conter todo o conteúdo do arquivo passado como parâmetro para então poder gerar o MD5.
    MD5_CTX c;
    MD5_Init(&c);


    while (str_size > 0) {
        if (str_size > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, str_size);
        }
        str_size -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (i = 0; i < 16; ++i) {
        snprintf(&(out[i*2]), 16*2, "%02x", (unsigned int)digest[i]);
    }

    return out;
}


void simple_compress(unsigned char *file_content, long int file_size, char *dest_filename) {
  int i, frequency[ASCII_SIZE] = {0};
  Heap* heap = create_heap(ASCII_SIZE);
  Node* bt = NULL;
  int tree_size, trash_size;
  unsigned char byte1;
  unsigned char byte2;

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
  //remove(dest_filename);
  FILE *dest_file = fopen(dest_filename, "w");                            /* Creates the destination file */

  putc(0, dest_file);                                                     /* Prints first byte in the destination file as 0, since we don't have the trash size yet */
  putc(byte2, dest_file);                                                 /* Prints second byte in the destination file */

  print_tree_in_file(bt, dest_file);                                      /* Prints the tree in the destination file */

  free_tree(bt);

  trash_size = write_in_file(file_content, file_size, dest_file, table);  /* Prints the compressed content and returns the trash size */
  free_table(table, ASCII_SIZE);

  rewind(dest_file);                                                      /* Rewinds the dest_file pointer to the beginning of the destination file */

  trash_size = trash_size<<5;                                             /* Sets trash size to the first three bits */
  byte1 |= trash_size;                                                    /* Adds trash size to the start of the first byte */
  putc(byte1, dest_file);                                                 /* Prints first byte in the destination file */
  fclose(dest_file);

}

void simple_decompress(unsigned char *file_content, long int file_size, char *dest_filename) {
  /* Analizes the first two bytes to get the tree size and the trash size */
  int trash_size = 0, tree_size = file_content[1];          // Set the trash size to 0 and the tree size to the byte1
  unsigned char byte1 = file_content[0];
  unsigned char byte2 = file_content[1];

  int i, j;
  for(i = 0, j = 8; i < 5; ++i, ++j) {                      // Traverses the byte1 from right to left to calculate the rest of the tree size,
    if(is_bit_i_set(byte1, i))                              // starting the byte index with 8, if the bit is set, calculates de decimal 
      tree_size += pow(2, j);                               // value of the bit and sum to the tree size
  }

  trash_size = byte1 >> 5;                                  // Get the first 3 bits of byte1 for the trash size

  char tree_str[tree_size+1];
  for(i = 2; i < tree_size+2; ++i) {                        // Get the the tree nodes of the file, starting from the byte 2 to tree_size+2
    tree_str[i-2] = file_content[i];
  }
  tree_str[tree_size] = '\0';

  i = 0;
  Node *tree = str_to_tree(tree_str, &i);                   // Rebuild the tree from the string
  Node *aux_tree = tree;                                    // Creates an aux node to keep the root node on the tree pointer

  FILE *dest_file = fopen(dest_filename, "w");              // Opens the destination file


  for(i = tree_size + 2; i < file_size - 1; ++i) {          // Traverses each bit of the remaining text of the file until the before last byte
    for(j = 7; j >= 0; --j) {                               
      if(is_bit_i_set(file_content[i], j)) {                // If the bit is set go to the right child
        aux_tree = get_right_tree(aux_tree);
      } else {                                              // If not go to the right
        aux_tree = get_left_tree(aux_tree);
      }

      if(is_leaf(aux_tree)) {
        putc(get_tree_item(aux_tree), dest_file);          // Put the char on the destination file if reach a leaf
        aux_tree = tree;                                   // Rollback the aux_tree to the root of the  tree to start the search of the next byte
      }
    }
  }

  /* The last byte is analysed  */
  for(i = 7; i >= trash_size; --i) {
    if(is_bit_i_set(file_content[file_size - 1], i)) {
      aux_tree = get_right_tree(aux_tree);
    } else {
      aux_tree = get_left_tree(aux_tree);
    }

    if(is_leaf(aux_tree)) {
      putc(get_tree_item(aux_tree), dest_file);
      aux_tree = tree;
    }
  }

  free_tree(tree);
  fclose(dest_file);
}



/**
int main(){
	char string[15] = "teste.xxtxxt";
	char *saida = get_extension_name(string);
	printf("%s\n", saida);
	printf("tamanho nome: %d\n", get_file_name_size(string));
	printf("tamanho extensao: %d\n", get_file_extesion_size(string));
	return 0;
}
**/