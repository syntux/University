#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <crypt.h>

int main() {
	char password[] = "123test";

	char salt[] = "$1$ab$";

	char *encrypt = crypt(password, salt);

	printf("%s\n", encrypt);
	return 0;
}
