#include "auth.h"

auth::auth() 
{
	
}

auth::~auth()
{
}

bool auth::file_auth(const char* key)
{
	// To implement decryption, and file system...

	//wxPasswordEntryDialog passwordEntryDialog(this, "User password", "user: admin", label->GetLabel());
	if (key == "123456") {
		return true;
	}
	return false;
}