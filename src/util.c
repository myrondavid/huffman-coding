#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "../inc/table.h"
#include "../inc/huffman_tree.h"

int get_file_extesion_size(char *src_filename){
	int size = strlen(src_filename);
	int i, size_ext=0;
	for (i = size-1; i >= 0; --i, size_ext++){
		if(src_filename[i] == '.'){
			return size_ext;
		}
	}
}

int get_file_name_size(char *src_filename){
	int size = strlen(src_filename), size_ext=0;
	size_ext=get_file_extesion_size(src_filename);
	return size-size_ext-1;
}

char *get_file_name(char *src_filename){
	int size = get_file_name_size(src_filename);
	char *filename = (char*)malloc(sizeof(char)*size+1);
	int i;
	for(i = 0; i < size; i++){
		filename[i] = src_filename[i];
	}
	filename[i] = '\0';	
	return filename;
}

char *get_extension_name(char *src_filename){
	int size_e = get_file_extesion_size(src_filename);
	int size_f = get_file_name_size(src_filename);
	char *extension = (char*)malloc(sizeof(char)*size_e);
	int i;
	for(i = size_f; i < strlen(src_filename); i++){
		extension[i-size_f] = src_filename[i]; 
	}
	extension[i-size_f+1] = '\0';
	return extension;
}


//recebe um string com a senha, retorna uma string no formato MD5
/**
char *string_to_md5(const char *str, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
}
**/
int main(){
	char string[15] = "teste.xxtxxt";
	char *saida = get_extension_name(string);
	printf("%s\n", saida);
	printf("tamanho nome: %d\n", get_file_name_size(string));
	printf("tamanho extensao: %d\n", get_file_extesion_size(string));
	return 0;
}