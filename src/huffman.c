#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/heap.h"

void help();

int main(int argc, char const *argv[]) {
  FILE *file;
  char *src_filename, *dest_filename, option[3];
  char *file_content;
  size_t file_size;
  int i, frequency[256] = {0};
  Heap* queue = create_heap(256);
  Node* emptyNode = NULL;
  

  if(argc == 1) {
    help();
  } else {
    if(argc == 2 && strcmp(argv[1], "-help") == 0) {
      help();
      return 0;
    } else if(argc != 4) {
      printf("Invalid arguments! See 'huffman -help' for more information. \n");
      exit(1);
    }

    src_filename = (char*)malloc(strlen(argv[1]) * sizeof(char));
    dest_filename = (char*)malloc(strlen(argv[2]) * sizeof(char));

    strcpy(src_filename, argv[1]);
    strcpy(dest_filename, argv[2]);
    strcpy(option, argv[3]);

    if(strcmp(option, "-c") != 0 && strcmp(option, "-d") != 0) {
      printf("Invalid option! See 'huffman -help' for more information.\n");
      exit(1);
    }

    file = fopen(src_filename, "r");

    if(file == NULL) {
      printf("The source file does not exist!\n");
      exit(1);
    }

    fseek(file, 0, SEEK_END);                             /* Set the pointer to the end of the file */
    file_size = ftell(file);                              /* Return the position of the pointer on the file */
    rewind(file);                                         /* Rollback the pointer to the beginning of thee file */
    file_content = (char*)malloc((file_size + 1) * (sizeof(char)));
    fread(file_content, sizeof(char), file_size, file);   /* Read the content of the file */
    fclose(file);
    file_content[file_size] = 0;
  }

  for(i = 0; i < file_size; i++){

    ++frequency[file_content[i]];                          /* Counts the frequency of every character in the file */

  }

  for(i = 0; i < 256; i++){
    if(frequency[i] > 0){

      enqueue(queue, create_node(i, frequency[i]));

    }
  }
  /*
  i = 0;

  while(queue->size > 1){

    if(i%2 == 0){
      enqueue(queue, emptyNode);
      emptyNode = create_node('*', 0);
      emptyNode->left = dequeue(queue);
      emptyNode->frequency += emptyNode->left->frequency;
    }

    else{
      emptyNode->right = dequeue(queue);
      emptyNode->frequency += emptyNode->right->frequency;
    }

    i++;

  }
  */

  /* Use the index of the array as the char and the value of each index as the frequency when creating nodes */

  return 0;
}

void help() {
  printf("Huffman Help\n\n");

  printf("Usage: huffman [source_filename_path] [destination_filename_path] [options]\n\n");

  printf("Options:\n");
  printf("%s\n", "-c: Compress the source file to the destination file");
  printf("%s\n", "-d: Decompress the source file to the destination file");

  exit(0);
}

