#include "crsa.h"

struct RSA {
	mpz_t n;
	mpz_t e;
	mpz_t d;
};

RSA newkey(int size) {
	//p
	mpz_t p;
	mpz_init(p);
	random_prime_mpz2(&p, size / 2);
	//q
	mpz_t q;
	mpz_init(q);
	random_prime_mpz2(&q, size / 2);
	//n
	mpz_t n;
	mpz_init(n);
	mpz_mul(n, p, q);
	
	//p-1
	mpz_t p1;
	mpz_init(p1);
	mpz_sub_ui(p1, p, 1);
	//q-1
	mpz_t q1;
	mpz_init(q1);
	mpz_sub_ui(q1, q, 1);
	//phiN
	mpz_t phiN;
	mpz_init(phiN);
	mpz_mul(phiN, p1, q1);
	
	//e (public)
	mpz_t e;
	mpz_init(e);
	mpz_set_ui(e, 65537);
	//d (private)
	mpz_t d;
	mpz_init(d);
	bezout_mpz(&e, &phiN, &d);
	
	RSA rsa;
	mpz_init(rsa.n); mpz_init(rsa.d); mpz_init(rsa.e);
	mpz_set(rsa.n, n); mpz_set(rsa.d, d); mpz_set(rsa.e, e);
	mpz_clear(n); mpz_clear(d); mpz_clear(e); mpz_clear(p); mpz_clear(q); mpz_clear(phiN); mpz_clear(p1); mpz_clear(q1);
	
	return rsa;
}

RSA setRSAFromKey(char* privateKeyBase64, char* publicKeyBase64) {
	RSA rsa;
	mpz_init(rsa.n); mpz_init(rsa.d); mpz_init(rsa.e);
	char privateKey[10000]; char publicKey[10000];
	char str_n[10000] = ""; char str_d[10000] = ""; char str_e[10000] = "";
	if (strcmp(privateKeyBase64, "") != 0) {
		//privateKey enter
		strremove(privateKeyBase64, "====== BEGIN CRYPTORIA RSA PRIVATE KEY ======");
		strremove(privateKeyBase64, "======= END CRYPTORIA RSA PRIVATE KEY =======");
		strremove(privateKeyBase64, "\n");
		strremove(privateKeyBase64, "\t");
		strremove(privateKeyBase64, " ");
		Base64decode(privateKey, privateKeyBase64);
		getStrBetween(str_n, privateKey, "<n>", "</n>");
		getStrBetween(str_d, privateKey, "<d>", "</d>");
		getStrBetween(str_e, privateKey, "<e>", "</e>");
		mpz_set_str(rsa.n, str_n, 10); mpz_set_str(rsa.d, str_d, 10); mpz_set_str(rsa.e, str_e, 10);
	} else if (strcmp(publicKeyBase64, "") != 0) {
		//publicKey enter
		strremove(publicKeyBase64, "====== BEGIN CRYPTORIA RSA PUBLIC KEY ======");
		strremove(publicKeyBase64, "======= END CRYPTORIA RSA PUBLIC KEY =======");
		strremove(publicKeyBase64, "\n");
		strremove(publicKeyBase64, "\t");
		strremove(publicKeyBase64, " ");
		Base64decode(publicKey, publicKeyBase64);
		getStrBetween(str_n, publicKey, "<n>", "</n>");
		getStrBetween(str_e, publicKey, "<e>", "</e>");
		mpz_set_str(rsa.n, str_n, 10); mpz_set_str(rsa.e, str_e, 10);
	} else {
		printf("error : function setRSAFromKey has nos key in parameter\n");
	}
	return rsa;
}

void getPublicKey(RSA rsa, char* publicKey) {
	char str_file[5000];
	char str_e[5000];
	char str_n[5000];
	char str_balise[5000];
	char str_balise_base64[5000];
	char str_balise_base64_n[5000];
	char begin[] = "====== BEGIN CRYPTORIA RSA PUBLIC KEY ======\n";
	char end[] = "\n======= END CRYPTORIA RSA PUBLIC KEY =======";
	
	mpz_get_str(str_e, 10, rsa.e);
	mpz_get_str(str_n, 10, rsa.n);
	sprintf(str_balise, "<e>%s</e><n>%s</n>", str_e, str_n);
	Base64encode(str_balise_base64, str_balise, strlen(str_balise));
	unsigned int i = 0;
	unsigned int l = 0;
	while(i != strlen(str_balise_base64))
	{
		if(i % strlen(begin) == 0 && i != 0)
		{
			str_balise_base64_n[i + l] = '\n';
			l++;
		}
		str_balise_base64_n[i + l] = str_balise_base64[i];
		i++;
	}
	str_balise_base64_n[strlen(str_balise_base64) + l] = '\0';
	sprintf(str_file, "%s%s%s", begin, str_balise_base64_n, end);
	strcpy(publicKey, str_file);
}

void getPrivateKey(RSA rsa, char* privateKey) {
	char str_file[5000];
	char str_e[5000];
	char str_d[5000];
	char str_n[5000];
	char str_balise[5000];
	char str_balise_base64[5000];
	char str_balise_base64_n[5000];
	char begin[] = "====== BEGIN CRYPTORIA RSA PRIVATE KEY ======\n";
	char end[] = "\n======= END CRYPTORIA RSA PRIVATE KEY =======";
	
	mpz_get_str(str_d, 10, rsa.d);
	mpz_get_str(str_n, 10, rsa.n);
	mpz_get_str(str_e, 10, rsa.e);
	sprintf(str_balise, "<e>%s</e><d>%s</d><n>%s</n>",str_e, str_d, str_n);
	Base64encode(str_balise_base64, str_balise, strlen(str_balise));
	unsigned int i = 0;
	unsigned int l = 0;
	while(i != strlen(str_balise_base64))
	{
		if(i % strlen(begin) == 0 && i != 0)
		{
			str_balise_base64_n[i + l] = '\n';
			l++;
		}
		str_balise_base64_n[i + l] = str_balise_base64[i];
		i++;
	}
	str_balise_base64_n[strlen(str_balise_base64) + l] = '\0';
	sprintf(str_file, "%s%s%s", begin, str_balise_base64_n, end);
	strcpy(privateKey, str_file);
}

int getBlockSize(RSA rsa) {
	char n[5000] = "";
	mpz_get_str(n, 10, rsa.n);
	int size = strlen(n) - 1;
	while(size % 3 != 0) {
		size--;
	}
	return size;
}

void encodeBlock(RSA rsa, char *input, char *output) {
	mpz_t mpz_result;
	mpz_init(mpz_result);
	mpz_t mpz_input;
	mpz_init(mpz_input);
	
	mpz_set_str(mpz_input, input, 10);
	mpz_powm(mpz_result, mpz_input, rsa.e, rsa.n);
	
	mpz_get_str(output, 10, mpz_result);
}

void decodeBlock(RSA rsa, char *input, char *output) {
	mpz_t mpz_result;
	mpz_init(mpz_result);
	mpz_t mpz_input;
	mpz_init(mpz_input);
	
	mpz_set_str(mpz_input, input, 10);
	mpz_powm(mpz_result, mpz_input, rsa.d, rsa.n);
	
	mpz_get_str(output, 10, mpz_result);
}

void padding(int block_size, char *input, char *output) {
	int zero = block_size - strlen(input);
	char char_zero[100000] = "";
	if(strlen(input) != block_size) {
		for(int i = 0; i != zero; i++) {
			char_zero[i] = '0';
		}
		char_zero[zero] = '\0';
	}
	strcat(char_zero, input);
	strcpy(output, char_zero);
}

void encode(RSA rsa, char* input, char* output) {
	char msg[100000] = "";
	for(int i = 0; i < strlen(input); i++) {
		int c = (int)input[i];
		char c_char[4];
		if(c < 100) {
			snprintf(c_char, sizeof(c_char), "0%d", c);
		} else {
			snprintf(c_char, sizeof(c_char), "%d", c);
		}
		strcat(msg, c_char);
	}
	printf("msg ascii: %s\n", msg);
	encodeBlock(rsa, msg, output);
}

void decode(RSA rsa, char* input, char* output) {
	char msg[100000] = "";
	char msg_padding[100000] = "";
	decodeBlock(rsa, input, msg);
	
	if(strlen(msg) % 3 != 0) {
		char zero1[2] = "0";
		sprintf(msg_padding, "%s%s", zero1, msg);
		if(strlen(msg_padding) % 3 != 0) {
			char zero2[3] = "00";
			sprintf(msg_padding, "%s%s", zero2, msg);
		}
	}
	for(int a = 0; a != strlen(msg_padding); a = a + 3) {
		char l[4] = "";
		l[0] = msg_padding[a];
		l[1] = msg_padding[a+1];
		l[2] = msg_padding[a+2];
		l[3] = '\0';
		int ascii = (int)strtol(l, NULL, 10);
		if(ascii != 0) {
			char char_block[2] = "";
			char_block[0] = ascii;
			char_block[1] = '\0';
			strcat(output, char_block);
		}
	}
}

void sign(char *input, RSA rsa) {
		
}

void encrypt_block(RSA rsa, char *block, char *result) {
	
}
