// -*- coding: utf-8 -*-
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define KEY_LENGTH 2048

int generate_key_pair(const char *pub_key_file, const char *priv_key_file) {
    RSA *rsa_key = NULL;
    FILE *pub_key = NULL;
    FILE *priv_key = NULL;

    if ((pub_key = fopen(pub_key_file, "wb")) == NULL) {
        perror("Unable to open public key file");
        return 1;
    }

    if ((priv_key = fopen(priv_key_file, "wb")) == NULL) {
        perror("Unable to open private key file");
        fclose(pub_key);
        return 1;
    }

    rsa_key = RSA_generate_key(KEY_LENGTH, RSA_F4, NULL, NULL);
    if (rsa_key == NULL) {
        fprintf(stderr, "Error generating RSA key pair\n");
        ERR_print_errors_fp(stderr);
        fclose(pub_key);
        fclose(priv_key);
        return 1;
    }

    PEM_write_RSAPublicKey(pub_key, rsa_key);
    PEM_write_RSAPrivateKey(priv_key, rsa_key, NULL, NULL, 0, NULL, NULL);

    RSA_free(rsa_key);
    fclose(pub_key);
    fclose(priv_key);
    return 0;
}

int encrypt_file(const char *input_file, const char *output_file, const char *pub_key_file) {
    FILE *infile = fopen(input_file, "rb");
    FILE *outfile = fopen(output_file, "wb");
    FILE *pub_key = fopen(pub_key_file, "rb");

    if (infile == NULL || outfile == NULL || pub_key == NULL) {
        perror("Unable to open files");
        return 1;
    }

    RSA *rsa_key = PEM_read_RSAPublicKey(pub_key, NULL, NULL, NULL);
    if (rsa_key == NULL) {
        perror("Unable to read public key");
        fclose(infile);
        fclose(outfile);
        fclose(pub_key);
        return 1;
    }

    int rsa_size = RSA_size(rsa_key);
    unsigned char *in = (unsigned char *)malloc(rsa_size);
    unsigned char *out = (unsigned char *)malloc(rsa_size);

    int bytesRead;
    while ((bytesRead = fread(in, 1, rsa_size - 11, infile)) > 0) {
        if (RSA_public_encrypt(bytesRead, in, out, rsa_key, RSA_PKCS1_PADDING) == -1) {
            perror("Encryption error");
            fclose(infile);
            fclose(outfile);
            fclose(pub_key);
            free(in);
            free(out);
            RSA_free(rsa_key);
            return 1;
        }
        fwrite(out, 1, rsa_size, outfile);
    }

    fclose(infile);
    fclose(outfile);
    fclose(pub_key);
    free(in);
    free(out);
    RSA_free(rsa_key);
    return 0;
}

int decrypt_file(const char *input_file, const char *output_file, const char *priv_key_file) {
    FILE *infile = fopen(input_file, "rb");
    FILE *outfile = fopen(output_file, "wb");
    FILE *priv_key = fopen(priv_key_file, "rb");

    if (infile == NULL || outfile == NULL || priv_key == NULL) {
        perror("Unable to open files");
        return 1;
    }

    RSA *rsa_key = PEM_read_RSAPrivateKey(priv_key, NULL, NULL, NULL);
    if (rsa_key == NULL) {
        perror("Unable to read private key");
        fclose(infile);
        fclose(outfile);
        fclose(priv_key);
        return 1;
    }

    int rsa_size = RSA_size(rsa_key);
    unsigned char *in = (unsigned char *)malloc(rsa_size);
    unsigned char *out = (unsigned char *)malloc(rsa_size);

    int bytesRead;
    while ((bytesRead = fread(in, 1, rsa_size, infile)) > 0) {
        if (RSA_private_decrypt(bytesRead, in, out, rsa_key, RSA_PKCS1_PADDING) == -1) {
            perror("Decryption error");
            fclose(infile);
            fclose(outfile);
            fclose(priv_key);
            free(in);
            free(out);
            RSA_free(rsa_key);
            return 1;
        }
        fwrite(out, 1, bytesRead, outfile);
    }

    fclose(infile);
    fclose(outfile);
    fclose(priv_key);
    free(in);
    free(out);
    RSA_free(rsa_key);
    return 0;
}

int main() {
    const char *pub_key_file = "public.pem";
    const char *priv_key_file = "private.pem";
    const char *input_file = "input.txt";
    const char *encrypted_file = "encrypted.dat";
    const char *decrypted_file = "decrypted.txt";

    if (generate_key_pair(pub_key_file, priv_key_file) != 0) {
        fprintf(stderr, "Key pair generation failed\n");
        return 1;
    }

    if (encrypt_file(input_file, encrypted_file, pub_key_file) != 0) {
        fprintf(stderr, "Encryption failed\n");
        return 1;
    }

    if (decrypt_file(encrypted_file, decrypted_file, priv_key_file) != 0) {
        fprintf(stderr, "Decryption failed\n");
        return 1;
    }

    printf("Encryption and decryption completed successfully.\n");

    return 0;
}

