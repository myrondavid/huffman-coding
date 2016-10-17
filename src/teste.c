#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "../inc/compress.h"
#include "../inc/heap.h"
#include "../inc/huffman_tree.h"
#include "../inc/table.h"
#include "../inc/util.h"


int main() {
    char string[100] = "senha";
	char *saida = string_to_md5(string, strlen(string));
	printf("%s\n", saida);
	return 0;
}