#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
//#include <crypt.h>
//

typedef struct parse {
	char *username;
	int id;
	char *salt;
	char *encrypted;
} parse;

void initParse(parse *txt) {
	txt->username = malloc(40*sizeof(char));
	txt->salt = malloc(40*sizeof(char));
	txt->encrypted = malloc(40*sizeof(char));
}

void freeParse(parse *txt) {
	free(txt->username);
	free(txt->salt);
	free(txt->encrypted); 
}

int main(int argc, char **argv) {
/*
	char password[] = "123test";

	char salt[] = "$1$ab$";

	char *encrypt = crypt(password, salt);

	printf("%s\n", encrypt);
	*/

	MPI_Init(&argc, &argv);
	MPI_Comm world = MPI_COMM_WORLD;	

	int rank, worldSize;

	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &rank);



	/*
	if (rank == 0) {
		FILE *fh;

		fh = fopen("words.txt", "r");	

		char *words[235888];
		int i = 0;
		char buf[255];
		while (fgets(buf, 255, fh)) {
			words[i] = malloc(46 * sizeof(char));
			strcpy(words[i], buf);
			i++;
		}


		for (i = 0; i < 235888; i++) {
			//printf("%s\n", words[i]);
			free(words[i]);
		}

	}
	*/


	FILE *fileWords, *fileShadow;

	if ((fileWords = fopen("words.txt", "r")) == NULL) {
		fprintf(stderr, "No words.txt\n");
		MPI_Finalize();
		return 1;
	}
	if ((fileShadow = fopen("shadow", "r")) == NULL) {
		fprintf(stderr, "No shadow\n");
		MPI_Finalize();
		return 2;
	}

/*
typdef struct parse {
	char username[255];
	int id;
	char salt[255];
	char encrypted[255];
} parse;
*/

	parse shadowList [11];
	int count = 0;
	char buf[255];
	while (fgets(buf, 255, fileShadow)) {
		char *tmp = malloc(40*sizeof(char));
		tmp = strtok(buf, ":");
		initParse(&shadowList[count]);
		//shadowList[count].username = tmp;
		strcpy(shadowList[count].username, tmp);
		int i = 0;
		while(tmp != NULL) {
			//tmp = malloc(40*sizeof(char));
			tmp = strtok(NULL, "$");
//			printf("%s\n", tmp);
			if (i == 0) {
				shadowList[count].id = atoi(tmp);
				//free(tmp);
			}
			else if (i == 1) {
				strcpy(shadowList[count].salt, tmp);
				//shadowList[count].salt = tmp;
			}
			else if (i == 2) {
				strcpy(shadowList[count].encrypted, tmp);
			//	shadowList[count].encrypted = tmp; 
				printf("EN: %s\n", shadowList[count].encrypted);
			}
			i++;	
		}
		count++;
	}

//	if (rank == 0) printf("test: %s\n", shadowList[1].salt);

	printf("testttt %s\n", shadowList[1].encrypted);
	fclose(fileWords);
	fclose(fileShadow);

	for (int i = 0; i < 11; i++) freeParse(&shadowList[i]);
	MPI_Finalize();
	return 0;
}
