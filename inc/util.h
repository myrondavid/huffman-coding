#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "../inc/table.h"
#include "../inc/huffman_tree.h"

#ifndef UTIL_H
#define UTIL_H

int get_extesion_size(char *src_filename);

int get_name_size(char *src_filename);

char get_byte(int size_ext, int size_password);

char *get_file_name(char *src_filename);

char *get_extension_name(char *src_filename);

char *string_to_md5(const char *str, int str_size);


#endif