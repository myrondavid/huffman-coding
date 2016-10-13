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

/**
void get_file_name(char *src_filename, char* name){

}
**/

/**
int main(){

	char string[20] = "teste.txtasdasd";
	int size = get_file_name_size(string);
	char filename[size+1];
	int i;
	for(i = 0; i < size; i++){
		filename[i] = string[i];
	}
	filename[i] = '\0';	
	printf("%s\n", filename);

	return 0;
}
**/

char *str2md5(const char *str, int length) {
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

int main(){
	char string[20] = "hello world"
	MD5Context md5;
	MD5Init(&md5);
	MD5Update(&md5, string, strlen(string));
	MD5Final(digest, &md5);	
	return 0;
}