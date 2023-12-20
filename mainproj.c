#include <openssl/pkcs12.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/pem.h>


void pass_protect(char* cert_path, char* key_path, char* password, char* out_path){
	// Инициализация OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Создание или загрузка приватного ключа и сертификата
    EVP_PKEY *pkey; // Здесь должен быть ваш приватный ключ
    X509 *cert;     // Здесь должен быть ваш сертификат
    FILE *fp;

    // Получение ключа
    fp = fopen(key_path, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не могу открыть файл приватного ключа %s\n", key_path);
        exit(1);
    }
    pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!pkey) {
        fprintf(stderr, "Ошибка: не могу прочитать приватный ключ\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }


    //чтение сертификата
    fp = fopen(cert_path, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не могу открыть файл сертификата %s\n", cert_path);
        exit(1);
    }
    cert = PEM_read_X509(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!cert) {
        fprintf(stderr, "Ошибка: не могу прочитать сертификат\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    // Создание pkcs8ShroudedKeyBag
    PKCS8_PRIV_KEY_INFO *p8inf = EVP_PKEY2PKCS8(pkey);
    X509_SIG *p8 = PKCS8_encrypt(NID_pbe_WithSHA1And3_Key_TripleDES_CBC, 
                                 EVP_des_ede3_cbc(), 
                                 password, // Пароль для защиты ключа
                                 8, NULL, 0, 0, p8inf);

    PKCS12_SAFEBAG *key_bag = PKCS12_SAFEBAG_create0_p8inf(p8);

    // Создание certBag
    PKCS12_BAGS *cert_bag = PKCS12_BAGS_new();
    cert_bag->type = OBJ_nid2obj(NID_certBag);
    cert_bag->value.bag = M_ASN1_new_of(X509);
    M_ASN1_dup_of(X509, d2i_X509, i2d_X509, &cert, cert_bag->value.bag);

    PKCS12_SAFEBAG *cert_safebag = PKCS12_SAFEBAG_create0(cert_bag);

    // Добавление key_bag и cert_safebag в контейнер
    STACK_OF(PKCS12_SAFEBAG) *bags = sk_PKCS12_SAFEBAG_new_null();
    sk_PKCS12_SAFEBAG_push(bags, key_bag);
    sk_PKCS12_SAFEBAG_push(bags, cert_safebag);

    // Создание и сохранение PKCS12 файла
    PKCS12 *p12 = PKCS12_create("password", // Пароль контейнера
                                "My PKCS12", // Имя контейнера
                                NULL, NULL, bags, 0, 0, 0, 0, 0);

    fp = fopen(out_path, "wb");
    i2d_PKCS12_fp(fp, p12);
    fclose(fp);

    // Освобождение ресурсов
    PKCS12_free(p12);
    sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);

    EVP_PKEY_free(pkey);
    X509_free(cert);

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
}


void diffi(char* password,char* out_file,char* cert_path){
	// Инициализация OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Генерация ключа Диффи-Хеллмана
    DH *dh = DH_new();
    // Настройка параметров DH...

    // Создание keyBag
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_PKEY_assign_DH(pkey, dh);
    X509 *cert;
    FILE *fp;

    //чтение сертификата
    fp = fopen(cert_path, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не могу открыть файл сертификата %s\n", cert_path);
        exit(1);
    }
    cert = PEM_read_X509(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!cert) {
        fprintf(stderr, "Ошибка: не могу прочитать сертификат\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    PKCS8_PRIV_KEY_INFO *p8inf = EVP_PKEY2PKCS8(pkey);
    PKCS12_SAFEBAG *key_bag = PKCS12_SAFEBAG_create0_p8inf(p8inf);

    // Создание certBag (пример с загрузкой сертификата)
    PKCS12_BAGS *cert_bag = PKCS12_BAGS_new();
    cert_bag->type = OBJ_nid2obj(NID_certBag);
    cert_bag->value.cert = cert;

    PKCS12_SAFEBAG *cert_safebag = PKCS12_SAFEBAG_create0(cert_bag);

    // Создание PKCS12 контейнера
    STACK_OF(PKCS12_SAFEBAG) *bags = sk_PKCS12_SAFEBAG_new_null();
    sk_PKCS12_SAFEBAG_push(bags, key_bag);
    sk_PKCS12_SAFEBAG_push(bags, cert_safebag);

    PKCS12 *p12 = PKCS12_create(password, "My PKCS12", NULL, NULL, bags, 0, 0, 0, 0, 0);

    // Сохранение PKCS12 контейнера
    fp = fopen(out_file, "wb");
    i2d_PKCS12_fp(fp, p12);
    fclose(fp);

    // Освобождение ресурсов
    PKCS12_free(p12);
    sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);

    EVP_PKEY_free(pkey);
}

void pfx_to_pem(char *pfx_path, char *pem_key_path, char *pem_cert_path, char* password){

	FILE *fp;
	EVP_PKEY *pkey;
	X509 *cert;
	STACK_OF(509) *ca = NULL;
	PKCS12 *p12;
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
	if (!(fp = fopen(pfx_path, "rb"))) {
		fprintf(stderr, "Ошибка: не могу открыть файл %s\n", pfx_path);
	    exit(1);
	}

	p12 = d2i_PKCS12_fp(fp, NULL);
	fclose(fp);
	if (!p12) {
		fprintf(stderr, "Ошибка: не могу прочитать PFX файл\n");
	    ERR_print_errors_fp(stderr);
	    exit(1);
	}
	/* Извлекаем ключ и сертификат */
    if (!PKCS12_parse(p12, password, &pkey, &cert, &ca)) {
        fprintf(stderr, "Ошибка: не могу извлечь данные из PFX файла\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    PKCS12_free(p12);

	/* Сохраняем приватный ключ в PEM файл */
    if (!(fp = fopen(pem_key_path, "wb")) || !PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL)) {
        fprintf(stderr, "Ошибка: не могу сохранить приватный ключ в %s\n", pem_key_path);
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    fclose(fp);

	/* Сохраняем сертификат в PEM файл */
    if (!(fp = fopen(pem_cert_path, "wb")) || !PEM_write_X509(fp, cert)) {
        fprintf(stderr, "Ошибка: не могу сохранить сертификат в %s\n", pem_cert_path);
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    fclose(fp);

	EVP_PKEY_free(pkey);
    X509_free(cert);
    sk_X509_pop_free(ca, X509_free);
}

void pem_to_pfx(char *pfx_path, char *pem_key_path, char *pem_cert_path, char* password){
	FILE *fp;
    EVP_PKEY *pkey = NULL;
    X509 *cert = NULL;
    PKCS12 *p12 = NULL;

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    /* Чтение приватного ключа из файла PEM */
    fp = fopen(pem_key_path, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не могу открыть файл приватного ключа %s\n", pem_key_path);
        exit(1);
    }
    pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!pkey) {
        fprintf(stderr, "Ошибка: не могу прочитать приватный ключ\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* Чтение сертификата из файла PEM */
    fp = fopen(pem_cert_path, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не могу открыть файл сертификата %s\n", pem_cert_path);
        exit(1);
    }
    cert = PEM_read_X509(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!cert) {
        fprintf(stderr, "Ошибка: не могу прочитать сертификат\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* Создание PFX */
    p12 = PKCS12_create(password, "My Certificate", pkey, cert, NULL, 0, 0, 0, 0, 0);
    if (!p12) {
        fprintf(stderr, "Ошибка: не могу создать PFX\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* Сохранение PFX в файл */
    fp = fopen(pfx_path, "wb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не могу открыть файл %s для записи\n", pfx_path);
        exit(1);
    }
    if (!i2d_PKCS12_fp(fp, p12)) {
        fprintf(stderr, "Ошибка: не могу записать PFX файл\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    fclose(fp);

    EVP_PKEY_free(pkey);
    X509_free(cert);
    PKCS12_free(p12);
}
int main(int argc, char *argv[]){
	const char *mode;
	mode = argv[1];

	//pemtopfx
	if(mode[0] == 1){
		const char *pfx_path, *pem_key_path, *pem_cert_path, *password;
		pfx_path = argv[2];
		pem_key_path = argv[3];
		pem_cert_path = argv[4];
		password = argv[5];
		pem_to_pfx(pfx_path, pem_key_path, pem_cert_path, password);
	}
	//pfxtopem
	if(mode[0] == 2){
		const char *pfx_path, *pem_key_path, *pem_cert_path, *password;
		pfx_path = argv[2];
		pem_key_path = argv[3];
		pem_cert_path = argv[4];
		password = argv[5];
		pfx_to_pem(pfx_path, pem_key_path, pem_cert_path, password);
	}
	//passprotect
	if(mode[0] == 3){
		const char *cert_path, *key_path, *password, *out_path;
		cert_path = argv[2];
		key_path = argv[3];
		password = argv[4];
		out_path = argv[5];
		pass_protect(cert_path, key_path, password, out_path);

	}
	//diffi
	if(mode[0] == 4){
		const char *cert_path, *password, *out_file;
		cert_path = argv[2];
		password = argv[3];
		out_file = argv[4];
		diffi(password,out_file,cert_path);

	}

	

	return 0;
}