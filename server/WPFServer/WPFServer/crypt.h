#include "sha512.h"

#define CHAR_SIZE 8
#define SALT_BITS 256
#define SALT_LENGTH (SALT_BITS/CHAR_SIZE)
#define HASH_BITS 512
#define HASH_LENGTH (HASH_BITS/CHAR_SIZE)
#define DECRYPT_FAILURE false
#define DECRYPT_SUCCESS true

/* Debug */
void Print(char* name, std::string str, int len){
	std::cout << name << ": ";
	for(int i=0; i<len; i++){
		std::cout << str[i];
	}
	std::cout << std::endl;
}

/* Generate 256-bit salt. 255^32 combinations. */
void GenerateRandom256BitSalt(char* s, const int len) {

	// fill salt from random ASCII [1,255] code set collection
    for (int i = 0; i < len; i++) {
        s[i] = (char)((rand() % 255) + 1); // ASCII code 0 is not included, to work with C strings.
    }

}

/* Generate 512-bit salted Sha512 hash */
/* If fourth argument is not NULL, the crypt will not store a salt in the third argument */
void Crypt(char* hash, const char* digits, char* returnedSalt, char* precomputedSalt = NULL){

	char* salt = NULL;
	std::string computedHash = "";
	std::string hashKey = "";

	// if computing a new salt is requested
	if(!precomputedSalt){
		
		//generate and store salt
		GenerateRandom256BitSalt(returnedSalt, SALT_LENGTH);

		//use generated salt
		salt=returnedSalt;

	}else{

		//use precomputed salt
		salt=precomputedSalt;

	}

	//form hash key
	hashKey.append(digits);
	hashKey.append(salt);

	//create hash
	computedHash = sha512(hashKey);

	//convert string to char* for return parameter
	for(int i=0; i<HASH_LENGTH; i++){
		hash[i]=computedHash[i];
	}

}

/* Compare server stored hash against user's attempted password with server stored salt */
bool Decrypt(char const* attemptedPassword, char* retrievedSalt, const char* retrievedPasswordHash){

	char* hash = new char[HASH_LENGTH];
	bool decryptionSuccessful=false;

	//generate hash with percomputed salt
	Crypt(hash, attemptedPassword, NULL, retrievedSalt);

	//compare computed hash against server-stored hash
	if(!strcmp(retrievedPasswordHash,hash)){
		decryptionSuccessful=true;
	}

	//if the hashes are equal
	if(decryptionSuccessful){
		return DECRYPT_SUCCESS;
	}else{
		return DECRYPT_FAILURE;
	}

}