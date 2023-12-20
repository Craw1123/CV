#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

// Функция для генерации ключей
void generate_keys(EVP_PKEY **pub_key, EVP_PKEY **priv_key) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    EVP_PKEY_keygen(ctx, pub_key);
    EVP_PKEY_keygen(ctx, priv_key);
    EVP_PKEY_CTX_free(ctx);
}

// Функция для зашифрования файла
int encrypt_file(const char *input_file, const char *output_file, EVP_PKEY *pub_key) {
    FILE *in_file = fopen(input_file, "rb");
    if (!in_file) {
        perror("Failed to open input file");
        return 1;
    }

    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        perror("Failed to open output file");
        fclose(in_file);
        return 1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("Failed to create encryption context");
        fclose(in_file);
        fclose(out_file);
        return 1;
    }

    if (EVP_SealInit(ctx, EVP_aes_256_cbc(), NULL, NULL, &pub_key, 1) != 1) {
        perror("Failed to initialize encryption");
        fclose(in_file);
        fclose(out_file);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    unsigned char buffer_in[1024];
    unsigned char buffer_out[1024 + EVP_MAX_BLOCK_LENGTH];
    int len;
    int out_len;

    while (1) {
        len = fread(buffer_in, 1, 1024, in_file);
        if (len <= 0) break;

        if (EVP_SealUpdate(ctx, buffer_out, &out_len, buffer_in, len) != 1) {
            perror("Failed to encrypt data");
            fclose(in_file);
            fclose(out_file);
            EVP_CIPHER_CTX_free(ctx);
            return 1;
        }

        fwrite(buffer_out, 1, out_len, out_file);
    }

    if (EVP_SealFinal(ctx, buffer_out, &out_len) != 1) {
        perror("Failed to finalize encryption");
        fclose(in_file);
        fclose(out_file);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    fwrite(buffer_out, 1, out_len, out_file);

    fclose(in_file);
    fclose(out_file);
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

// Функция для дешифрования файла
int decrypt_file(const char *input_file, const char *output_file, EVP_PKEY *priv_key) {
    FILE *in_file = fopen(input_file, "rb");
    if (!in_file) {
        perror("Failed to open input file");
        return 1;
    }

    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        perror("Failed to open output file");
        fclose(in_file);
        return 1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("Failed to create decryption context");
        fclose(in_file);
        fclose(out_file);
        return 1;
    }

    if (EVP_OpenInit(ctx, EVP_aes_256_cbc(), NULL, NULL, priv_key) != 1) {
        perror("Failed to initialize decryption");
        fclose(in_file);
        fclose(out_file);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    unsigned char buffer_in[1024 + EVP_MAX_BLOCK_LENGTH];
    unsigned char buffer_out[1024];
    int len;
    int out_len;

    while (1) {
        len = fread(buffer_in, 1, 1024 + EVP_MAX_BLOCK_LENGTH, in_file);
        if (len <= 0) break;

        if (EVP_OpenUpdate(ctx, buffer_out, &out_len, buffer_in, len) != 1) {
            perror("Failed to decrypt data");
            fclose(in_file);
            fclose(out_file);
            EVP_CIPHER_CTX_free(ctx);
            return 1;
        }

        fwrite(buffer_out, 1, out_len, out_file);
    }

    if (EVP_OpenFinal(ctx, buffer_out, &out_len) != 1) {
        perror("Failed to finalize decryption");
        fclose(in_file);
        fclose(out_file);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    fwrite(buffer_out, 1, out_len, out_file);

    fclose(in_file);
    fclose(out_file);
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

int main() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();

    EVP_PKEY *pub_key = NULL;
    EVP_PKEY *priv_key = NULL;

    generate_keys(&pub_key, &priv_key);

    if (pub_key == NULL || priv_key == NULL) {
        fprintf(stderr, "Key generation failed\n");
        return 1;
    }

    if (encrypt_file("input.txt", "encrypted.bin", pub_key) == 0) {
        printf("File encrypted successfully\n");
    }

    if (decrypt_file("encrypted.bin", "decrypted.txt", priv_key) == 0) {
        printf("File decrypted successfully\n");
    }

    EVP_PKEY_free(pub_key);
    EVP_PKEY_free(priv_key);

    EVP_cleanup();
    ERR_free_strings();

    return 0;
}

