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
    char string[15] = "teste.xxtxxt";
	char *saida = get_extension_name(string);
	printf("%s\n", saida);
	printf("tamanho nome: %d\n", get_name_size(string));
	printf("tamanho extensao: %d\n", get_extesion_size(string));
	return 0;
}