/************************************************************************
                        Password.cpp

**************************************************************************/

#include "Password.h"

// Constructors/Destructors
//

Password::~Password ( ) {

}

Password::Password (vector<unsigned long int> hashPassword) {
    this->hashPassword = hashPassword;
}

/**
 * Get the value of this->hashPassword
 * @return the value of this->hashPassword
 */
vector<unsigned long int> Password::getHashPassword ( ){
	return this->hashPassword;
}

void Password::setHashPassword ( vector<unsigned long int>  hash){
        this->hashPassword = hash;
    }
