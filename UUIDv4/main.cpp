#include "UUIDv4.h"

#include <iostream>


void main()
{
	
	std::cout << "UUID: " << UUID::New().String() << std::endl;
}
