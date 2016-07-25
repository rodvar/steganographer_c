/************************************************************************
                        Password.h

**************************************************************************/

#ifndef PASSWORD_H
#define PASSWORD_H
#include <string>
#include "Almacenable.h"
#include <vector>


/**
  * Clase que representa un clave con la cual contrastar para el acceso al sistema
  */

class Password : public Almacenable
{
private:

	 vector<unsigned long int> hashPassword;

public:
	// Constructors/Destructors
	/**
	 * Construye el password
	 * @param password
	 */
	Password (vector<unsigned long int> hashPassword);

	/**
	 * Destruye el password
	 */
	virtual ~Password ();

	/**
	 * @return el hashing correpondiente al password.
	 */
	 vector<unsigned long int> getHashPassword ( );

	 /**
	 *setea el hashing del password.
	 */
	 void setHashPassword ( vector<unsigned long int>  hash);

};

#endif // PASSWORD_H
