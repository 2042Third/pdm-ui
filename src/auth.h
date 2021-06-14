#pragma once
#include "cMain.h"

class auth
{
public:
	auth();
	~auth();
	// Decrypt the given file
	bool file_auth(const char* key);
private:
};

