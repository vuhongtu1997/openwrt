#include <iostream>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <fstream>
#include <sstream>
using namespace std;

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

static string hex_print(const void *pv, size_t len)
{
    const unsigned char *p = (const unsigned char *)pv;
    if (NULL == pv)
    {
        cout << "NULL" << endl;
        return "NULL";
    }
    else
    {
        char Temp[65];

        string msg = "";
        size_t i = 0;
        for (i = 0; i < len; i++)
        {
            sprintf(Temp + i * 2, "%02X", *p++);
        }
        cout << Temp << endl;
        return Temp;
    }
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        handleErrors();
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        handleErrors();
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

void WriteIntoPassWd(string msg)
{
    FILE *file;
    string passwd_mqtt = "RD:"+msg;
    string FilePath = "/etc/mosquitto/passwd.txt";
    char *filePath = new char[FilePath.length() + 1];
    strcpy(filePath, FilePath.c_str());
    char *Msg = new char[passwd_mqtt.length() + 1];
    file = fopen(filePath, "a");
    strcpy(Msg, passwd_mqtt.c_str());
    fputs(Msg, file);
    fclose(file);
}

void WriteIntoPassWd_MQTT(string msg)
{
    FILE *file;
    string FilePath = "/etc/PassMqtt.txt";
    char *filePath = new char[FilePath.length() + 1];
    strcpy(filePath, FilePath.c_str());
    char *Msg = new char[msg.length() + 1];
    file = fopen(filePath, "w");
    strcpy(Msg, msg.c_str());
    fputs(Msg, file);
    fclose(file);
}

int execute(string key_in, string plaintext_in)
{
    unsigned char *key = (unsigned char *)key_in.c_str();
    unsigned char *plaintext = (unsigned char *)plaintext_in.c_str();
    unsigned char ciphertext[128];
    int ciphertext_len = 32;
    ERR_load_crypto_strings();
    ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, ciphertext);
//    printf("Ciphertext is:\n");
//    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);
    string msg = hex_print((const char *)ciphertext, ciphertext_len);
    WriteIntoPassWd(msg);
    WriteIntoPassWd_MQTT(msg);
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}

void getMacAddress(unsigned char *Mac)
{
    struct ifreq s;
    unsigned char *mac = NULL;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, "eth0");
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
    {
        mac = (unsigned char *)s.ifr_addr.sa_data;
    }
    sprintf((char *)Mac, (const char *)"2804%.2x%.2x%.2x%.2x%.2x%.2x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int main(void)
{
    unsigned char Text[20] = {0};
    string Key = "RANGDONGRALSMART";
    getMacAddress(Text);
    cout << Text << endl;
    std::string Textt(reinterpret_cast<char*>(Text));
    execute(Key, Textt);
    return 0;
}

