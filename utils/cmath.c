#include "cmath.h"

void random_mpz(mpz_t *mpz, int size) {
	char number[10] = {"0123456789"};
	char number_first[10] = {"123456789"};
	char str[4000];
	int a = randombytes_uniform(9);
	str[0] = number_first[a];
	int i;
	for(i = 1; i != size; i++) {
		a = randombytes_uniform(10);
		str[i] = number[a];
	}
	mpz_set_str(*mpz, str, 10);
}

void random_prime_mpz(mpz_t *mpz, int size) {
	/*
	char n[1024] = "";
	for(int i = 0; i != size; i++) {
		int random = randombytes_uniform(2);
		if(random == 0) {
			n[i] = '0';
		} else {
			n[i] = '1';
		}
	}
	n[size] = '\0';
	mpz_set_str(*mpz, n, 2);
	mpz_nextprime(*mpz, *mpz);
	*/
	char number[10] = {"0123456789"};
	char number_first[10] = {"123456789"};
	char str[4000];
	int a = randombytes_uniform(9);
	str[0] = number_first[a];
	int i;
	for(i = 1; i != size; i++) {
		a = randombytes_uniform(10);
		str[i] = number[a];
	}
	mpz_set_str(*mpz, str, 10);
	mpz_nextprime(*mpz, *mpz);
}

void random_prime_mpz2(mpz_t *mpz, int size) {
	char n[5000] = "";
	for(int i = 0; i != size; i++) {
		int random = randombytes_uniform(2);
		if(random == 0) {
			n[i] = '0';
		} else {
			n[i] = '1';
		}
	}
	n[size] = '\0';
	mpz_set_str(*mpz, n, 2);
	mpz_nextprime(*mpz, *mpz);
}

void bezout_mpz(mpz_t *a, mpz_t *b, mpz_t *d) {
	//a0
	mpz_t a0;
	mpz_init(a0);
	mpz_set(a0, *a);
	//b0
	mpz_t b0;
	mpz_init(b0);
	mpz_set(b0, *b);
	
	//p
	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p, 1);
	//q
	mpz_t q;
	mpz_init(q);
	mpz_set_ui(q, 0);
	//r
	mpz_t r;
	mpz_init(r);
	mpz_set_ui(r, 0);
	//s
	mpz_t s;
	mpz_init(s);
	mpz_set_ui(s, 1);
	
	//c
	mpz_t c;
	mpz_init(c);
	//quotient
	mpz_t quotient;
	mpz_init(quotient);
	//nouveau_r
	mpz_t nouveau_r;
	mpz_init(nouveau_r);
	//nouveau_s
	mpz_t nouveau_s;
	mpz_init(nouveau_s);
	
	while(mpz_cmp_ui(*b, 0) != 0) {
		mpz_mod(c, *a, *b);
		mpz_fdiv_q(quotient, *a, *b);
		
		mpz_set(*a, *b);
		mpz_set(*b, c);
		
		mpz_mul(nouveau_r, quotient, r);
		mpz_sub(nouveau_r, p, nouveau_r);
		
		mpz_mul(nouveau_s, quotient, s);
		mpz_sub(nouveau_s, q, nouveau_s);
		
		mpz_set(p, r);
		mpz_set(q, s);
		mpz_set(r, nouveau_r);
		mpz_set(s, nouveau_s);
	}
	
	mpz_set(*d, p);
	mpz_set(*a, a0);
	mpz_set(*b, b0);
	while(mpz_cmp_ui(*d, 0) < 0) {
		mpz_add(*d,*d, *b);
	}
}