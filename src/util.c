#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "../inc/util.h"

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
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    unsigned char *out = (unsigned char*)malloc(33);

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

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
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