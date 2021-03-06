#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef BSWABE_DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include <openssl/sha.h>
#include <glib.h>
#include <pbc.h>
#include <gmp.h>

#include "bswabe.h"
#include "private.h"

#define TYPE_A_PARAMS \
"type a\n" \
"q 87807107996633125224377819847540498158068831994142082" \
"1102865339926647563088022295707862517942266222142315585" \
"8769582317459277713367317481324925129998224791\n" \
"h 12016012264891146079388821366740534204802954401251311" \
"822919615131047207289359704531102844802183906537786776\n" \
"r 730750818665451621361119245571504901405976559617\n" \
"exp2 159\n" \
"exp1 107\n" \
"sign1 1\n" \
"sign0 1\n"

char last_error[256];

char*
bswabe_error()
{
	return last_error;
}

void
raise_error(char* fmt, ...)
{
	va_list args;

#ifdef BSWABE_DEBUG
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
#else
	va_start(args, fmt);
	vsnprintf(last_error, 256, fmt, args);
	va_end(args);
#endif
}

void
element_from_string( element_t h, char* s )
{
	unsigned char* r;

	r = malloc(SHA_DIGEST_LENGTH);
	SHA1((unsigned char*) s, strlen(s), r);
	element_from_hash(h, r, SHA_DIGEST_LENGTH);

	free(r);
}

/*bswabe_sigver_t* sigkeygen(bswabe_pub_t* pub){
	bswabe_sigver_t* pair = malloc(sizeof(bswabe_sigver_t));
	pair -> a = malloc(sizeof(bswabe_verification_t));
	pair -> b = malloc(sizeof(bswabe_sig_t));
	element_init_Zr(pair -> b -> x,pub -> p);
	element_random(pair -> b ->x);
				//single pairing initialization for all element_t type initialisation====> computed from above string s
				//exponentiation function used is element_pow_mpz
				//element_t g is initialized as G1 member instead of Zr
				//the char* member of bswabe_verification_t is uninitialised
	element_init_Zr(pair ->a -> y,pub -> p);
	element_random(pair -> a ->y);

	element_init_G2(pair -> a ->g,pub -> p);
	element_random(pair -> a->g);
	element_init_G2(pair -> a->g_y, pub -> p);
	element_pow_zn(pair -> a->g_y,pair -> a->g,pair -> a->y);
	element_init_G2(pair -> a->g_xy, pub -> p);
	element_pow_zn(pair -> a->g_xy,pair -> a->g_y,pair -> b -> x);//see here 
	pair -> a -> y_s = random_binaryString();
	return pair;
}

char* random_binaryString(){
	srand(time(0));
	char *s = (char*)malloc(128 * sizeof(char));
	int count = 0;
	while(count < 128){
		int x = rand();
		if(x % 2 == 0){
			s[count] = '0';
		}else{
			s[count] = '1';
		}
		count++;
	}
	return s;
}*/


/*bswabe_signature_t* sign(bswabe_sig_t *sig, bswabe_cph_t *cp, bswabe_pub_t *pub){

	bswabe_signature_t* signa;
	signa = malloc(sizeof(bswabe_signature_t));
	element_t tempp;
	element_t temp;
	// sigma = (C1 * C3 + C2)^x 	
	element_init_G1(signa -> sigma, pub -> p);
	element_init_G1(tempp, pub -> p);
	element_init_G1(temp, pub -> p);

	element_mul(tempp, cp -> c, cp -> c3);
	//element_add(tempp, temp, cp -> c);
	element_pow_zn(signa -> sigma, tempp, sig -> x);
 	return signa;
}*/


/*int verify(bswabe_verification_t *ver, bswabe_signature_t *signa, bswabe_cph_t *cp, bswabe_pub_t *pub){
	
	//element_printf("verification key is %B", ver -> g_xy);
	element_t pair1;
	element_t pair2;
	element_t temp;
	element_t tempp;
	element_init_GT(pair1, pub -> p);
	element_init_GT(pair2, pub -> p);
	//element_init_G1(signa -> sigma, pub -> p);
	element_init_G1(tempp, pub -> p);
	element_init_G1(temp, pub -> p);

	element_mul(tempp, cp -> c, cp -> c3);
	//element_add(tempp, temp, cp -> c);

	pairing_apply(pair1, signa -> sigma, ver -> g_y, pub -> p); 
	pairing_apply(pair2, tempp, ver -> g_xy, pub -> p);

	if(element_cmp(pair1,pair2) == 0){
		printf("\n-----verification MATCHES----------");
		return 1;
	}else{
		printf("\n-----verfication does not matches-----");
		printf("\n-----cannot decrypt--------");
		return 0;
	}

}*/

int isPrime(int x){
    
    int i;
    for(i = 2;i < x ;i++){
        if(x % i == 0){
            return 0;
        }
    }
    return 1;
}

int powe(unsigned long int x, unsigned long int y, unsigned long int p) 
{ 
    unsigned long int res = 1;      // Initialize result 
  
    x = x % p;  // Update x if it is more than or  
                // equal to p 
  
    while (y > 0) 
    { 
        // If y is odd, multiply x with result 
        if (y & 1) 
            res = (res*x) % p; 
  
        // y must be even now 
        y = y>>1; // y = y/2 
        x = (x*x) % p;   
    } 
    return res; 
} 

void concatenate(char ans[],char* s1, char* s2, char* s3){
    //printf("String obtained on concatenation: %s\n", s1);
    //printf("String obtained on concatenation: %s\n", s2);
    //int size = strlen(s1) + strlen(s2) + strlen(s3) + 1;
    //char ans[size];
    //printf("%d", size);

    int i = 0;
	if(s1[0] != 'A'){
    while(s1[i] != '\0'){
        ans[i] = s1[i];
        i++;
    }
	}
    int j = i; 
    i = 0;
	if(s2[0] != 'A'){
    while(s2[i] != '\0'){
        ans[j] = s2[i];
        j++;
        i++;
    }
}

    i = 0;
	if(s3[0] != 'A'){
    while(s3[i] != '\0'){
        ans[j] = s3[i];
        j++;
        i++;
    }
}

    // strcat(ans,s1);
    printf("String obtained on excuse me concatenation: %s\n", ans);
    // strcat(ans,s2);
    // printf("String obtained on concatenation: %s\n", ans);
    // strcat(ans, s3);
    // printf("String obtained on concatenation: %s\n", ans);
    return;
}

void take_Concatenate(char sol[], mpz_t m1, mpz_t m2, mpz_t m3){

    char s1[100];
    char s2[100];
    char s3[100];
    
	int flag = 0;
	int flag2 =0;
	int flag3 = 0;
     if(mpz_cmp_ui(m1, 0) == 0){
        s1[0] = "A";
	
    }else{
        mpz_get_str(s1, 10, m1);
	flag = strlen(s1);
    }

    if(mpz_cmp_ui(m2, 0) == 0){
        s2[0] = "A";

    }else{
        mpz_get_str(s2, 10, m2);
	flag2 = strlen(s2);
    }

    if(mpz_cmp_ui(m3, 0) == 0){
        s3[0] = "A"; 
    }else{
        mpz_get_str(s3, 10, m3);
	flag3 = strlen(s3);
    }


    int size = flag + flag2 + flag3;
    char ans[size];
    concatenate(ans, s1, s2, s3);
    int i;
    for( i = 0;i<size;i++){
        sol[i] = ans[i];
    }
}



unsigned long int generate_random(mpz_t rand_Num,int limit)
{   
    
    unsigned long int  seed= rand() % limit;
    mpz_init_set_ui(rand_Num, seed);

	return seed;
} 

void compute_hash(mpz_t hash,char str[]) 
{ 
 

   mpz_init(hash);
   mpz_t p;
   mpz_init_set_ui(p, 2);
   mpz_t mod;
   mpz_init_set_ui(mod,100);
   //printf("working");
   char c;
   
     for( c=0;c<strlen(str);c++)
   {    mpz_t m;
        mpz_init(m);
        mpz_t ans;
        mpz_init(ans);
       
        mpz_pow_ui (m, p, c); 
	mpz_mod(m,m,mod);
        mpz_t vall;
        int x = str[c] - '0';

        mpz_init_set_ui(vall, x);

        mpz_mul(ans, vall, m);
	mpz_mod(ans,ans,mod);
        mpz_add(hash,hash,ans);
	mpz_mod(hash,hash,mod);
   }
 
	gmp_printf("//////////////////%Zd//////////////////",hash); 
   return;

}

void compute_hash2(mpz_t hash,unsigned long int x) 
{ 
 
	unsigned long int num = x;
   mpz_init(hash);
   mpz_t p;
   mpz_init_set_ui(p, 2);
   mpz_t mod;
   mpz_init_set_ui(mod,100);
   //printf("working");
   int c = 0;
   
     while(num != 0)
   {    
	mpz_t m;
        mpz_init(m);
        mpz_t ans;
        mpz_init(ans);
       
        mpz_pow_ui (m, p, c); 
	mpz_mod(m,m,mod);
        mpz_t vall;
        unsigned long int x_ = num % 10;

        mpz_init_set_ui(vall, x_);

        mpz_mul(ans, vall, m);
	mpz_mod(ans,ans,mod);
        mpz_add(hash,hash,ans);
	mpz_mod(hash,hash,mod);
	c++;
	num = num / 10;
   }
 
	gmp_printf("//////////////////%Zd//////////////////",hash);
   return;

}

int *delete_subarr(int arr_primes[],int arr_remove[], int n,int m)
{  
  // int arr_remove[m]; // size = m 
  int* ans_array;
  ans_array = malloc(sizeof(int) * (n - m));
   int i,j=0;
    int k = 0;
   for( i=0;i<n;i++)
   {
       int flag = 0;
       for(j=0;j<m;j++)
       {   
           if(arr_remove[j] ==arr_primes[i])
             {
                flag = 1;
              
             }
            
       }
       
       if(flag == 0){
           ans_array[k]= arr_primes[i]; 
           k++;
       }
   }
  return ans_array; 
}



void
bswabe_setup( bswabe_pub_t** pub, bswabe_msk_t** msk ,int n) 
{
	// #################Odelu Implementation##################### 


    int arr_use[168];
    int counter_use = 0;
    int x_use = 2;
    while(counter_use < 168){

        if(isPrime(x_use)){
            arr_use[counter_use] = x_use;
            counter_use++;
        }
        x_use++;
    }

   

	int remove[3]={2,5,29};
    int n_use=168;
    int m_use=3;
    int *p_use; 
    p_use = delete_subarr(arr_use,remove,n_use,m_use); 
    int i_use=0;
    /*for ( i_use = 0; i_use < 165; i_use++ ) {
      printf( "*(p + %d) : %d\n", i_use, *(p_use + i_use));
   }*/
    
        mpz_t x;
	mpz_init_set_ui(x,10);
	mpz_t HYPER_MOD;
	mpz_init(HYPER_MOD);
	mpz_pow_ui(HYPER_MOD, x , 100000);         

	*pub = malloc(sizeof(bswabe_pub_t)); 
	*msk = malloc(sizeof(bswabe_msk_t));	
	mpz_init_set_ui((*pub) -> n, n);// 
	mpz_init_set_ui((*msk) -> n, n);
	
	mpz_init_set_ui((*msk) -> p ,41); 
	mpz_init_set_ui((*msk) -> q,59);
 	
	mpz_mul((*pub) -> N, (*msk) -> p, (*msk) -> q);
	mpz_t p_1;
	mpz_t q_1;
	mpz_init(p_1);
	mpz_init(q_1);
	mpz_sub_ui(p_1, (*msk) -> p, 1);
	mpz_sub_ui(q_1, (*msk) -> q, 1);
      //  printf("working");
	mpz_t totient;
	mpz_init(totient);
	mpz_t vax;
	mpz_init_set_ui(vax,895); 
	mpz_mul(totient, p_1, q_1);


	mpz_t khaali;
	mpz_init(khaali);
       

	//mpz_init_set_ui((*pub) -> p_i[0],7);;
	//mpz_init_set_ui((*pub) -> p_i[1],21);
	//mpz_init_set_ui((*pub) -> p_i[2],39);
	 int i;
      for(i = 0; i < n;i++){
		
		unsigned long int selector = generate_random(khaali,165);
		/*mpz_t inter;
                generate_random(inter,15); 
		mpz_t gcd; // 
		mpz_init(gcd);

		// &&&&&changes made....
		mpz_mul(gcd, inter, totient);
		mpz_add_ui(gcd, gcd, 1);*/
		mpz_init_set_ui((*pub) -> p_i[i],p_use[selector]);
		mpz_t inter2;
		mpz_init(inter2); 
                mpz_powm(inter2,((*pub) -> p_i[i]), vax, totient);
		
		

		// &&&&&changes made....
		
		
		//gmp_printf("value of pub -> p1 == %Zd\n and value of msk -> q_i = %Zd\n", ((*pub) -> p_i[i]), inter2); 

		//if(mpz_cmp_ui(ans, 0) != 0){
		//	mpz_init_set((*pub) -> p_i[i], gcd); 
			// working on q_i[s]

			mpz_init_set((*msk) -> q_i[i], inter2);
			// l * totient  + 1 / p_i[i]
			
		//}else{
		//	i = i - 1;
		//} 
		mpz_clear(inter2);
		//mpz_clear(inter);
	}
	// now we have to generate k and x and g

       // printf("working");
	mpz_t lcm;
	mpz_init_set_ui(lcm, 1);
         int z;
	for( z = 0;z<n;z++){
		mpz_lcm(lcm, lcm, (*msk) -> q_i[z]);
	}


      //  printf("working");
	
	mpz_t random_x; 


	int selector_x = generate_random(random_x,165);
	
	mpz_init_set_ui((*msk) -> x, p_use[selector_x]); // add randomnesss 
	
	

	// work on g now 
	mpz_t gcd_g;
	mpz_init(gcd_g);
	int atrr_counter_g = 0;
	mpz_t random_g; 
	
	generate_random(random_g,30);
       //mpz_init_set_ui(random_g,13);
//	mpz_init_set((*pub) -> g, random_g);
        printf("working");
/*
	mpz_sub_ui(random_g,(*pub) -> N,1); 
	mpz_cdiv_q_ui(random_g, random_g,2);
*/
	
	// working on the k // this is actually computationally ineffecient
	mpz_t lcm_tot; 
	mpz_init(lcm_tot);
	mpz_lcm(lcm_tot, lcm, totient);
	
	mpz_t random_k; 
	int selector_k = generate_random(random_k,165);
	mpz_init_set_ui((*msk) -> k, p_use[selector_k]); // add randomness
	
	// working on Y and R;
	mpz_t inter_R;
	mpz_t inter_Y;
        mpz_t mod;
        printf("working");
	mpz_init(inter_R);
	mpz_init(inter_Y);
        mpz_init_set_ui(mod, 10000000000);
	mpz_powm(inter_R, random_g, (*msk) -> k, (*pub) -> N);
	mpz_powm(inter_Y, random_g, (*msk) -> x,(*pub) -> N);

	mpz_init_set((*pub) -> R, inter_R);
	mpz_init_set((*pub) -> Y, inter_Y);
        
	// work on d_u ///

	mpz_t mul;
	mpz_init_set_ui(mul, 1);

	int universal_attr_counter = 0;

	int o;
	for( o = 0;o<n;o++){
                //printf("%c", universal_attr[0]);
			mpz_mul(mul, mul, (*msk) -> q_i[universal_attr_counter]);
		universal_attr_counter++;
		
	}

	// mul is same as d_u // 
	mpz_init((*pub) -> D_u);
	mpz_powm((*pub) -> D_u, random_g, mul,(*pub) -> N); 
	
}


bswabe_cph_t*
bswabe_enc( bswabe_pub_t* pub, bswabe_msk_t* msk,element_t m, int attrib[]) 
{

//#######################################ODELU####################################################
        pairing_t px;
	char* pairing_desc;
	pairing_desc = strdup(TYPE_A_PARAMS);
	pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
        element_init_GT(m,px); 
        element_random(m);
	element_printf("%B", m);
        
        mpz_t x;
	mpz_init_set_ui(x,10);
	mpz_t HYPER_MOD;
	mpz_init(HYPER_MOD);
	mpz_pow_ui(HYPER_MOD, x , 100000);
	mpz_t p_1;
	mpz_t q_1;
	mpz_init(p_1);
	mpz_init(q_1);
	mpz_sub_ui(p_1, msk -> p, 1);
	mpz_sub_ui(q_1, msk -> q, 1);
        printf("working");
	mpz_t totient;
	mpz_init(totient);
	mpz_mul(totient, p_1, q_1);


	bswabe_cph_t* cph;

	cph = malloc(sizeof(bswabe_cph_t));

	

	
	mpz_t mod;
	mpz_init_set_ui(mod, 1000000000); 

	mpz_t blank1;
	mpz_init(blank1);

	mpz_t blank2;
	mpz_init(blank2);

	
	mpz_t r_m;
        generate_random(r_m,30);                  
        //mpz_init_set_ui(r_m,1);
        gmp_printf("\nvalue of pub->Y is %Zd and r_m in encrypt is = %Zd\n",pub ->Y, r_m);
       // fully correct ab misc.c ki ek copy bana
	//mpz_init(r_m);
	//char ans[100];
	//printf("FIRST HASHING");
	//take_Concatenate(ans, M, sigma,blank1);
	// working on attrib and string concatenation
	
	//printf("%s\n", ans);
//	int size = pub -> n;
//	int j = strlen(ans);
//	int final_length = j + size;
//	char final_ans[final_length];
/* fix it later
	for(int i = 0;i < final_length;i++){

	if(i < j){
		final_ans[i] = ans[i]; 

	}else{

		if(attrib[i] == 1){
			final_ans[i] = '1';
		}
		else{
			final_ans[i] = '0'; 
		} 
	}
	}*/

	
	//printf("######## %s\n",ans); 
	
	//compute_hash(r_m, ans);

	//gmp_printf("%Zd",r_m);
	
	mpz_t e_u;
	mpz_init_set_ui(e_u, 1);

	mpz_t e_p;
	mpz_init_set_ui(e_p, 1);
	//int n = 4;
	int n = mpz_get_ui(pub -> n); 
      //  printf("\n\\\\%d\\\\n",n);
	 int i;
	for(i = 0;i<n;i++){
		if(attrib[i] == 1){
			mpz_mul(e_p, e_p, pub -> p_i[i]);
		}
		mpz_mul(e_u, e_u, pub -> p_i[i]);
	}
	mpz_init_set(cph -> e_p,e_p);

	// working on C_sigma and C_m and S_m
	mpz_t inter;
	mpz_init_set(inter, pub -> D_u);

	mpz_cdiv_q(e_u, e_u, e_p);
	//mpz_mod(e_u,e_u,totient);
	//mpz_mod(r_m,r_m,totient);
	//mpz_t x_;
	//mpz_t y;
	//mpz_init(x_);
	//mpz_init(y);
	//mpz_mul(x_, e_u, r_m);//eu*ru
	//mpz_mod(x_,x_,totient); 
	//mpz_mul(y,x_,pub -> d_u);
	//mpz_mod(y, y, totient);
	mpz_powm(inter,inter,r_m,pub -> N);
	mpz_powm(inter,inter,e_u,pub -> N);
	
        /*mpz_t modx;
	mpz_init(modx);
	mpz_mod(modx, e_u, totient);*/
	gmp_printf("!!!!!!!!%Zd$$$$$$$$$$$$$$$%Zd!!!!!!!!!!!!!!!\n", e_u,inter); 

	//mpz_powm(inter, inter, r_m, mod);
	//mpz_powm(inter, inter, e_u, mod);

	mpz_t H2_K_m;
	mpz_init(H2_K_m);


	mpz_t sigma;
	generate_random(sigma,30);
	//gmp_printf("^^^^^^ %Zd" , sigma);
	
	// -----------

	printf("\nSECOND HASH");
      //  gmp_printf("%Zd ******************* %Zd", blank1,blank2);
	char ans2[100];

	mpz_t blank3;
	mpz_t blank4;
	mpz_init(blank3);
	mpz_init(blank4);
	//take_Concatenate(ans2, inter,blank3,blank4);
	mpz_get_str(ans2, 10 ,inter); // chala
	//gmp_printf("%Zd ******************* %Zd", blank1,blank2);

	printf("%s---------------",ans2); 
	compute_hash(H2_K_m, ans2);
	gmp_printf("k_m in encrypt is %Zd", H2_K_m);


	mpz_init(cph -> C_sigma); 
	mpz_xor(cph -> C_sigma, H2_K_m, sigma);
	
	gmp_printf("\n\nvalue of cph -> C_sigma = %Zd\n\n", cph -> C_sigma);
	mpz_t H3_sigma;
	mpz_init(H3_sigma); // 

	//char ans3[1000];
	//take_Concatenate(ans3, sigma, blank1 , blank2);
	//mpz_get_str(ans3, 10, sigma);
	unsigned long int ans3 = mpz_get_ui(sigma);
	//gmp_printf("\n\nvalue of sigma is %Zd and blank1 is %Zd\n\n",sigm);

	printf("((((((((((((((((%d))))))))))))))", ans3);
	compute_hash2(H3_sigma, ans3);

	mpz_t M;
	generate_random(M,30);
	gmp_printf("\n\nvalue of M is %Zd and value of H3_sigma is %Zd\n\n", M, H3_sigma); 
	mpz_init(cph -> C_m); 
	//unsigned long int value_M = mpz_get_ui(M);
	//unsigned long int value_H3 = mpz_get_ui(H3_sigma);
	//unsigned long int value_ans = value_M ^ value_H3;
	//printf("M = %d and H3 = %d and ans = %d", value_M, value_H3, value_ans); 
	//mpz_init_set_ui(cph -> C_m, value_ans);
	
	mpz_xor(cph -> C_m, M,H3_sigma); 

	
	gmp_printf("\n\n3rd hash completed and value of cph -> C_m is %Zd\n\n",cph -> C_m); 
	char ans4[4];
	unsigned long int M_v = mpz_get_ui(M);
	unsigned long int sigma_v = mpz_get_ui(sigma);
	
	unsigned long int final_ans = sigma_v * 100 + M_v;
	//take_Concatenate(ans4, sigma, M, blank1); sigma = 15 && M = 23 sigma * 100 + 23 + 523

	
	// 1627 + 1 + 12 + 16 + 56 
	printf("\n\n value of ans4 is %s\n\n",ans4);
	
	compute_hash2(cph -> S_m, final_ans);
	printf("4th Hash Completed");
	// working on Y_m and R_m

//	mpz_t x_mod;
//	mpz_init(x_mod);
//	mpz_mod(x_mod, msk -> x, totient);

//	mpz_t k_mod;
//	mpz_init(k_mod);
//	mpz_mod(k_mod, msk -> k, totient);

//	mpz_t z;
//	mpz_init(z);
//	mpz_mul(z,x_mod, r_m);
//	mpz_mod(z,z,totient);
  //      gmp_printf("z is = %Zd\n",z);

//	mpz_t z1;
//	mpz_init(z1);
//	mpz_mul(z1,k_mod, r_m); 
//	mpz_mod(z1,z1,totient);

	mpz_init(cph ->Y_m);
	mpz_init(cph -> R_m);
	mpz_powm(cph -> Y_m, pub -> Y, r_m, pub -> N);
        gmp_printf("Y_m in encrypt is = %Zd\n", cph -> Y_m); 
	unsigned long int pub_R = mpz_get_ui(pub -> R); 
	printf("pub_R in encrypt is = %lu\n", pub_R); 
	unsigned long int ir_m = mpz_get_ui(r_m);
        printf("ir_m in encrypt is = %lu\n", ir_m); 
	unsigned long int pub_N = mpz_get_ui(pub -> N);
        printf("pub_N in encrypt is = %lu\n", pub_N); // specifier 
	unsigned long int cph_Rm = powe(pub_R, ir_m, pub_N);
	mpz_init_set_ui(cph -> R_m, cph_Rm);
	//mpz_powm(cph -> R_m, pub -> R, r_m, pub -> N);
        gmp_printf("R_m in encrypt is = %Zd\n", cph -> R_m); 
	return cph;

}



bswabe_prv_t* bswabe_keygen( bswabe_pub_t* pub, bswabe_msk_t* msk, int user_attr_set[])
{ 
                    // #################Odelu Implementation#####################
         mpz_t x;
	mpz_init_set_ui(x,10);
	mpz_t HYPER_MOD;
	mpz_init(HYPER_MOD);
	mpz_pow_ui(HYPER_MOD, x , 100000);

   bswabe_prv_t* prv;
   prv = malloc(sizeof(bswabe_prv_t)); 
  
   int n = mpz_get_ui(pub -> n); //search
  //*pub = malloc(sizeof(bswabe_pub_t)); k
  // *msk = malloc(sizeof(bswabe_msk_t));	
  // (*pub) -> n = n;
  // (*msk) -> n = n;

  // mpz_init_set_ui((*msk) -> p ,1031);
  // mpz_init_set_ui((*msk) -> q,1409);
 	
   //mpz_mul((*pub) -> N, (*msk) -> p, (*msk) -> q);
   mpz_t p_1;
   mpz_t q_1;
   mpz_init(p_1);
   mpz_init(q_1);
   mpz_sub_ui(p_1, msk -> p, 1);
   mpz_sub_ui(q_1, msk -> q, 1);

   mpz_t totient;
   mpz_init(totient);
   mpz_mul(totient, p_1, q_1); 
   gmp_printf("value of totient is equal to %Zd",totient);
   printf("working");

  //WORKING ON K1
  mpz_t d_A;
  mpz_init_set_ui(d_A, 1);
   
  //mpz_t b;                 //a_i (0 or 1)
  //mpz_init_set_ui(b, 0);
  //mpz_t a;
  //mpz_init_set_ui(a, 1);

  printf("working");
        int universal_attr_counter = 0;
        printf("%d", user_attr_set[0]);
        int m;
	for( m = 0;m<n;m++)
       {
                
                //printf("%c", universal_attr[0]);
		
		
                if(user_attr_set[m] == 1)
                {
			//gmp_printf("I am inside the llop == %Zd\n",msk -> q_i[universal_attr_counter]);
			mpz_mul(d_A, d_A, msk -> q_i[universal_attr_counter]);
		}

               /* {
                    mpz_mul(d_A, d_A,1);
                }*/
		universal_attr_counter++;
		
	}
   printf("working");
  gmp_printf("d_a at init = %Zd",d_A);
  mpz_t check;
  mpz_init(check);
  mpz_mod(check,d_A,totient);
  gmp_printf("d_a check = %Zd",check);
  mpz_t e_A;
  mpz_init_set_ui(e_A, 1);
  int universal_attr_count = 0;
         int k;
         for( k = 0;k< n;k++)
       {
		
                if(user_attr_set[k] == 1)
                {
			mpz_mul(e_A, e_A, pub-> p_i[universal_attr_count]);  
		}// e_a / e_p = 1 

               /* else
                {
                    mpz_mul(e_A, e_A,1);
                }*/
		universal_attr_count++;
		
	}

  mpz_init_set(prv -> e_a, e_A);
   gmp_printf("value of k1  before init set is %Zd \n",prv -> k1);
  //WORKING ON K2
  mpz_t random_r_u;
  mpz_init(random_r_u);  //d_A - r_u * x % k == 0 then r_u == iterator otherrwise skip ( i = 1 se (d_A - i * x) % k )
  int i;
   mpz_t s_u;
   mpz_init(s_u);
	mpz_init(prv -> k1);
         gmp_printf("value of k1 at init is %Zd \n",prv -> k1);
	mpz_init(prv -> k2);
  for( i=1;i<=1000000;i++) 
  {
mpz_t df;
  mpz_init(df);
   mpz_t df1;
  mpz_init(df1);
  mpz_mul_ui(df1,msk ->x,i); 
  mpz_sub(df,d_A,df1);
   mpz_t mod1;
  mpz_init(mod1);
  mpz_mod(mod1,df,msk ->k);

     if(mpz_cmp_ui(mod1,0) < 0){ 
		break;
	}
     else if(mpz_cmp_ui(mod1,0) == 0)
       {
              mpz_init_set_ui(prv -> k2,i);
	      mpz_cdiv_q(df, df, msk -> k);
                 gmp_printf("value of k1 in else if before init set is %Zd \n",prv -> k1);
		mpz_init_set(prv -> k1,df); //chala
                  gmp_printf("value of k1 in else if after init set is %Zd \n",prv -> k1);
		break;   
       }


  }






//generate_random(random_r_u,15);
	//gmp_printf("value of random after just initialize = %Zd\n", random_r_u); 
  mpz_t random_t_u;
  mpz_init_set_ui(random_t_u,1);
  //generate_random(random_t_u,15);

  //take k and x     
  //mpz_t random_x;
  //generate_random(random_x);
  //mpz_init_set(msk -> x, random_x);

  //mpz_t random_k;
  //generate_random(random_k);
  //mpz_init_set(msk -> k, random_k);

  //compute s_u from d_A = ks_u + r_u x modtotient 

   /*mpz_t a1;
   mpz_init(a1);
   mpz_mod(a1,d_A,totient);
   mpz_t a2;
   mpz_init(a2);
   mpz_t prod2;    //r_ux
   mpz_init(prod2);
   mpz_mul(prod2, msk -> x, random_r_u); 
   mpz_mod(a2,prod2,totient);

   mpz_t b;
   mpz_init(b);
   mpz_sub(b,a1,a2);
  
   mpz_t a3;
   mpz_init(a3);
   mpz_t m;
   generate_random(m,15);
   mpz_mul(a3,m,totient); 

   mpz_t s_u;
   mpz_init(s_u);

   mpz_add(s_u,a3,b);*/

    gmp_printf("s_u  calculated is %Zd \n",s_u);

  /* mpz_t K;
   mpz_t s_u;
   mpz_init(s_u);
   
   mpz_t rand;
   generate_random(rand);
   mpz_init_set(K, rand);

   mpz_t prod1;    //K*tot
   mpz_init(prod1);
   mpz_mul(prod1, K, totient);

   mpz_t prod2;    //r_ux
   mpz_init(prod2);
   mpz_mul(prod2, msk -> x, random_r_u);
    
   mpz_add(prod1,prod1,prod2);

   mpz_t var;
   mpz_init(var);
   mpz_sub(var,d_A,prod1);
   
  
   mpz_cdiv_q (s_u, var,msk -> k); */       //s_u
   

  mpz_t v1;
   mpz_init(v1);
    mpz_t v2;
   mpz_init(v2);
    mpz_t v3;
   mpz_init(v3);
   mpz_mul(v1,msk -> k,s_u);
   mpz_mul(v2,random_r_u,msk -> x);
   mpz_add(v1,v1,v2);
   mpz_mod(v3,v1,totient);
   gmp_printf("d_A reverse calculated is %Zd \n",v3); 

                                                           
gmp_printf("value of totient in keygen is equal to %Zd",totient);  
   //Compute k1 and k2 from s_u
   // first k1
  // mpz_t k1;
 
   /*mpz_t prod3;
   mpz_init(prod3);
   mpz_init(prv -> k1);
   mpz_mul(prod3, msk-> x, random_t_u);  // x*random
   mpz_t a;
   mpz_init(a);
   mpz_add(a,s_u,prod3);  // s_u + x*random
   mpz_mod (prv -> k1, a, totient);*/// (s_u + x*random)mod(tot) 
   
    
   
   // now k2
  /* mpz_init(prv -> k2);
   mpz_t prod4;
   mpz_init(prod4);
   mpz_mul(prod4, msk -> k, random_t_u);       //k*t_u
   
   gmp_printf("value of random before subtraction = %Zd\n", random_r_u); // 31 * 11 + 11 * 2289  25520 
   
   mpz_t difference ;
   mpz_init(difference);
   
   mpz_sub(difference,random_r_u,prod4);  // r_u  - k*t_u                                 // uncomment this .......
   //mpz_abs(difference,difference);// comment this ..
  
    gmp_printf("\nvalue of difference is equal to %Zd \n",difference); 
   //mpz_init_set_ui(prv -> k2, 68);

   //gmp_printf("value of random_r_u = %Zd\n value of totient = %Zd\n",random_r_u, totient); 
   mpz_t variable;
   mpz_init(variable);
   mpz_mod (variable, difference, totient);
   mpz_init_set(prv ->k2,variable);*/

  mpz_t var1;
  mpz_init(var1);
  mpz_t var2;
  mpz_init(var2); 
  gmp_printf("\nk1 in d_a is equal to %Zd \n",prv -> k1);
  gmp_printf("\nk2 in d_a is equal to %Zd\n",prv -> k2);
  gmp_printf("\nmsk->x in d_a is equal to %Zd\n",msk ->x);
  gmp_printf("\nmsk->k in d_a is equal to %Zd\n",msk ->k);
  mpz_mul(var1,msk ->x,prv ->k2);   // xk2
  mpz_mul(var2,msk ->k,prv ->k1);   // kk1
  gmp_printf("msk ->x = %Zd\n",msk ->x);  
  gmp_printf("msk ->k = %Zd\n",msk ->k);
  mpz_t add1;
  mpz_init(add1);
  mpz_t add2;
  mpz_init(add2);
  mpz_add(add1,var2,var1);  // xk2 + kk1
 // gmp_printf("\nadd1 in d_a is equal to %Zd\n",add1); 
  mpz_mod(add2,add1,totient); 
 gmp_printf("d_A = %Zd",add2);
  
  // mpz_t k2;
   //mpz_t diff;
   //mpz_init(diff);
   //mpz_sub(diff,random_r_u,prod4);
   //mpz_init_set(k2, diff);
   printf("working");

   // TAKE user secret key k_u as  (k1,k2) 
    gmp_printf("value of k1 just before returning prv is %Zd \n",prv -> k1);
	return prv;
}

void bswabe_proxy(mpz_t k1, mpz_t C_attr, mpz_t C_user){

	mpz_t random;

	generate_random(random, 30);

	mpz_t random2;
	generate_random(random2, 30);
	mpz_add_ui(random2, random, 61);
	mpz_init(C_attr);
	int option;
	printf("PRESS 0 FOR NO REVOCATION AND 1 FOR USER REVOCATTION "); 
	scanf("%d", &option);
	
	
	mpz_init_set(C_user, random);

	if(option == 0){
            	mpz_mul(C_attr,k1, random);     //NR           
		
	}else{
		
		mpz_mul(C_attr,k1, random2);
	}

}


int bswabe_dec( bswabe_pub_t* pub,bswabe_prv_t* prv, bswabe_cph_t* cph,element_t m)// attrib is P
{
         pairing_t px;
	char* pairing_desc;
	pairing_desc = strdup(TYPE_A_PARAMS);
	pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
         element_init_GT(m,px); 
         element_random(m);
        mpz_t x; 
	mpz_init_set_ui(x,10);
	mpz_t HYPER_MOD;
	mpz_init(HYPER_MOD);
	mpz_pow_ui(HYPER_MOD, x , 100000); // 10^1000
   
	mpz_t mod;
	mpz_init_set_ui(mod, 100000000); 
         gmp_printf("value of k1 in decrypt is %Zd \n",prv -> k1);
   // WORKING ON D1
      mpz_t product1;
      mpz_init(product1);
      mpz_t product2;
      mpz_init(product2);
      mpz_t product3;
      mpz_init(product3);
      mpz_t a;
      mpz_init(a);
      mpz_t expo;
      mpz_init(expo);
      mpz_t K_m;
      mpz_init(K_m);
	mpz_t C_attr;
	mpz_t C_user; // init
	mpz_init(C_attr);
	mpz_init(C_user);
	bswabe_proxy(prv -> k2, C_attr,C_user); 
        gmp_printf("@value of c_attr  is %Zd , %Zd \n", C_attr, C_user);
        mpz_t expo2;
        mpz_init(expo2);
        mpz_cdiv_q(expo2, C_attr, C_user);
	gmp_printf("\nCATTR, CUSER, Exponent 2 : %Zd %Zd %Zd\n",C_attr,C_user,expo2);


	
	unsigned long int K_mi; 
	unsigned long int final_Km = 0;
     
     if( mpz_divisible_p (prv -> e_a, cph -> e_p) !=0)
	{
	mpz_divexact(expo,prv -> e_a, cph -> e_p);
	unsigned long int a_p = mpz_get_ui(expo);
	unsigned long int pub_N = mpz_get_ui(pub -> N);
       gmp_printf("value of pubn before in decrypt == %Zd\n", pub->n);
	unsigned long int cph_Rm = mpz_get_ui(cph -> R_m);
	printf("value of cph_rm in decrypt from integer approach is %lu\n", cph_Rm);
	unsigned long int cph_Ym = mpz_get_ui(cph -> Y_m);
	printf("value of cph_ym in decrypt from integer approach is %lu\n", cph_Ym);
	unsigned long int prv_k1 = mpz_get_ui(prv -> k1); 
	printf("value of prv_k1 in decrypt from integer approach is %lu\n", prv_k1); 
	
	unsigned long int prv_k2 = mpz_get_ui(expo2);
	printf("value of prv_k2 in decrypt from integer approach is %lu\n", prv_k2);
	unsigned long int Rm_k1 = powe(cph_Rm , prv_k1, pub_N);
	printf("value of Rm_k1 in decrypt from integer approach is %lu\n",  Rm_k1);
	unsigned long int Ym_k2 = powe(cph_Ym, prv_k2, pub_N);
	printf("value of Ym_k2 in decrypt from integer approach is %lu\n", Ym_k2); 
	K_mi = (Rm_k1 * Ym_k2) % pub_N; 
	final_Km = powe(K_mi, a_p, pub_N);

	printf("value of Rm_k1 in decrypt from integer approach is %lu\n", Rm_k1);
	printf("value of Ym_k2 in decrypt from integer approach is %lu\n", Ym_k2);
	printf("value of K_mi in decrypt from integer approach is %lu\n", K_mi); 

       gmp_printf("value of Y_m in decrypt == %Zd\n", cph -> Y_m);
        gmp_printf("value of R_m in decrypt == %Zd\n", cph -> R_m);
        
        gmp_printf("value of c_attr 2 is %Zd \n", C_attr);
        mpz_powm(product1, cph -> Y_m, expo2,pub ->N); 
        gmp_printf("value of product1 in decrypt == %Zd\n", product1);
        gmp_printf("value of K1 in decrypt == %Zd\n", prv -> k1);
        mpz_powm(product2, cph -> R_m, prv -> k1,pub ->N);
        gmp_printf("value of product2 in decrypt == %Zd\n", product2); 
        mpz_mul(product3,product1,product2);        
        gmp_printf("value of product3 in decrypt == %Zd\n", product3); 
        mpz_mod(a,product3,pub ->N);
        
       gmp_printf("value of expo in decrypt == %Zd\n", expo);
        mpz_powm(K_m,a, expo,pub ->N);          
	gmp_printf("value of K_m in decrypt == %Zd\n", K_m);   
     }

     else
     {
      printf("computation of K_m is computationally infeasible");
	return 0;
     }



      gmp_printf(" e_a is = %Zd", prv ->e_a);
      gmp_printf(" e_p is = %Zd", cph ->e_p);
  // WORKING ON D2
    
  // Compute σ'm= H2(Km) ⊕ C_σm and M'= Cm ⊕ H3 (σ'm )
    mpz_t sigma_d_m;
    mpz_init(sigma_d_m);
    mpz_t M_d;
    mpz_init(M_d);
    mpz_t blank1;
    mpz_init(blank1);
    mpz_t blank2;
    mpz_init(blank2);

        mpz_t H2_K_m;
	mpz_init(H2_K_m);
	mpz_init_set_ui(K_m, final_Km);
	char str1[10000];
	//take_Concatenate(str1,K_m,blank1,blank2);
	mpz_get_str(str1,10,K_m); 
	//printf("hello i am  == %s", str1);
	compute_hash(H2_K_m, str1);

	mpz_xor(sigma_d_m, H2_K_m,cph -> C_sigma);
	gmp_printf("value of sigma_d_m = %Zd",sigma_d_m);
        mpz_t H3_sigma_m;
	mpz_init(H3_sigma_m);

	//take_Concatenate(str2,sigma_d_m,blank1,blank2);
	int str2 = mpz_get_ui(sigma_d_m);
	compute_hash2(H3_sigma_m, str2); 

	mpz_xor(M_d, cph -> C_m,H3_sigma_m);

	gmp_printf("value of M in decrypt is %Zd",M_d);


        mpz_t S_m;
        mpz_init(S_m);
        
        mpz_t H1;
	mpz_init(H1);

	char str3[10000];
	take_Concatenate(str3,sigma_d_m,M_d,blank1);
	unsigned long int M_v = mpz_get_ui(M_d);
	unsigned long int sigma_v = mpz_get_ui(sigma_d_m);
	
	unsigned long int final_ans = sigma_v * 100 + M_v;
	printf("\n\n str3 is %s\n\n",str3);
	compute_hash2(H1, final_ans);

	
        if(mpz_cmp(cph -> S_m,H1) == 0) 
        {   return 1;}
        
       
            return 0; 
}


