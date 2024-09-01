#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/evp.h>

static const unsigned char aes_key[] = "example_key1234";
static const unsigned char aes_iv[] = "example_iv_1234";

void handle_decryption(const unsigned char *ciphertext, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, aes_key, aes_iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, strlen((char *)ciphertext));
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    EVP_CIPHER_CTX_free(ctx);
}

void handle_encryption(const unsigned char *plaintext, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, aes_key, aes_iv);
    EVP_EncryptUpdate(ctx,ciphertext,&len , plaintext , strlen((char *)plaintext));
    EVP_EncryptFinal_ex(ctx,ciphertext + len,&len);

   EVP_CIPHER_CTX_free(ctx);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8090);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 5);

    printf("Server listening on port 8080...\n");

    client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &addr_len);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    unsigned char buffer[1024];
    unsigned char decrypted_text[1024];
    unsigned char encrypted_text[1024];
    unsigned char plain_text[1024];
    unsigned char cipher_text[1024];
    unsigned char decrypted_text2[1024];
    unsigned char decrypted_text3[1024];
    char newline = '\n';
    int count=0;
    int flag=0;
    int count2=0;
    char ch;
    
    FILE * database=fopen("databaseServer.txt","a+");
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(decrypted_text, 0, sizeof(decrypted_text));
        memset(encrypted_text, 0, sizeof(encrypted_text));
        memset(plain_text,0,sizeof(plain_text));
        memset(cipher_text,0,sizeof(cipher_text));
        fseek(database,0,SEEK_SET);
	count=0;
	count2=0;
	flag=0;

        // Receive message from client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Connection closed by client.\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Encrypted message received: %s\n", buffer);
        
        while(fgets(cipher_text,sizeof(cipher_text),database)!=NULL){
		count++;
		if(count%2==0){
		strcpy(decrypted_text3,decrypted_text);
		strcpy(decrypted_text2,decrypted_text);
		strcpy(decrypted_text,cipher_text);
		}
		if(strcmp(cipher_text,buffer)==0){
		flag = 1;
		printf("Already decrypted, reading from the database\n");
		
		
		//fgets(encrypted_text,sizeof(encrypted_text),database);
		printf("Decrypted message: %s\n", decrypted_text3);
		
		
		//while(fgets(decrypted_text,sizeof(decrypted_text),database)!=NULL){
		//	count2++;
		//	if(count2==count){
				//fgets(encrypted_text,sizeof(encrypted_text),database);
		//		fgets(encrypted_text,sizeof(encrypted_text),database);
		//		printf("Decrypted message: %s\n", decrypted_text);
		//	}
		//}
		}
	
        
	}

        
	if(flag == 0){
        handle_decryption(buffer, decrypted_text);
        fseek(database,0,SEEK_END);
	fwrite(&newline,sizeof(char),1,database);
	fwrite(decrypted_text,sizeof(decrypted_text),1,database);
	fwrite(&newline,sizeof(char),1,database);
	fwrite(buffer,sizeof(buffer),1,database);
        printf("Decrypted message: %s\n", decrypted_text);
        }
        
        count =0;
        count2=0;
        flag=0;
        fseek(database,0,SEEK_SET);
        
        memset(buffer, 0, sizeof(buffer));
        memset(encrypted_text, 0, sizeof(encrypted_text));
        memset(decrypted_text, 0, sizeof(decrypted_text));
        memset(decrypted_text2, 0, sizeof(decrypted_text));
        memset(plain_text,0,sizeof(plain_text));
        memset(cipher_text,0,sizeof(cipher_text));

        // Send response to client
        printf("Enter message to send: ");
        fgets((char*)buffer,sizeof(buffer),stdin);
        buffer[strcspn((char*)buffer,"\n")]='\0';
        
        while(fgets(plain_text,sizeof(plain_text),database)!=NULL){
        count++;
        if(strcmp(plain_text,buffer)==0){
        	flag=1;
        	printf("Already encrypted, sending the cipher text from database\n");
        	fgets(encrypted_text,sizeof(encrypted_text),database);
        	fgets(encrypted_text,sizeof(encrypted_text),database);
        	printf("Encrypted message%s\n",encrypted_text);
        	send(client_socket , encrypted_text , strlen((char *)encrypted_text) , 0 );
        	
        }
        }
        
        if(flag == 0){
        fseek(database,0,SEEK_END);
	
	fwrite(&newline,sizeof(char),1,database);
        fwrite(buffer,sizeof(buffer),1,database);
        fwrite(&newline,sizeof(char),1,database);
        handle_encryption(buffer, encrypted_text);

        printf("Encrypted message: %s\n", encrypted_text);
        fwrite(encrypted_text,sizeof(encrypted_text),1,database);
               

        send(client_socket , encrypted_text , strlen((char *)encrypted_text) , 0 );
        }

       // handle_encryption(buffer, encrypted_text);

        //printf("Encrypted message: %s\n", encrypted_text);

        //send(client_socket , encrypted_text , strlen((char *)encrypted_text) , 0 );
        
     }
    
     close(client_socket);
     close(server_socket);

     return 0;
}

