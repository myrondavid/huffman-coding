#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "../inc/table.h"
#include "../inc/huffman_tree.h"

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
	size_ext=get_extesion_size(src_filename);
	return size-size_ext-1;
}

char get_byte(int size_ext, int size_password){
	unsigned char byte;
	byte=(size_ext<<5);
	byte|=size_password;
	return byte;
}

int main(){
		
	
	
	return 0;
}