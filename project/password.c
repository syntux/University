#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <crypt.h>
//

typedef struct parse {
	char *username;
	int id;
	char *salt;
	char *encrypted;
	char *combine;
	char *password;
} parse;

void initParse(parse *txt) {
	txt->username = malloc(40*sizeof(char));
	txt->salt = malloc(40*sizeof(char));
	txt->encrypted = malloc(40*sizeof(char));
	txt->combine = "$1$ab$";
	txt->password = malloc(40*sizeof(char));
}

void freeParse(parse *txt) {
	free(txt->username);
	free(txt->salt);
	free(txt->encrypted); 
	free(txt->password); 
}

int main(int argc, char **argv) {


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

	FILE *save;
	int saveState[12];

	char *testing;
	int saveCount = 0;
	int z = 0;
	int found[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	/*
			char saved[255];
			while (fgets(saved, 255, save)){
				char *string = strtok(saved, ",");
				while (string != NULL) {
					printf("z: %d\n", z);
					saveState[z++] = atoi(string);	
					string = strtok(NULL, ",");
				}	
			}
			saveCount = saveState[0];
	
	*/
	
	if((save = fopen("save.txt", "rw+")) == NULL) {
		fprintf(stderr, "Need save.txt data\n");
		MPI_Finalize();
		return 1;
	}

	char saved[255];
	while (fgets(saved, 255, save)){
		char *string = strtok(saved, ",");
		while (string != NULL) {
			//printf("z: %d\n", z);
			saveState[z++] = atoi(string);	
			string = strtok(NULL, ",");
		}	
	}
	printf("s: %d\n", saveState[0]);
	saveCount = saveState[0];
	for (int i = 1; i <= 11; i++) {
		found[i-1] = saveState[i];
	}

	//for (int i = 0; i < 11; i++) printf("found %d  ", found[i]);

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
				//sprintf(shadowList[count].combine, "$%d$%s$", shadowList[count].id, shadowList[count].salt); 
				//printf("C: %s\n", shadowList[count].combine);
			}
			else if (i == 2) {
				if ( tmp[strlen(tmp)-1] == '\n') {
					tmp[strlen(tmp)-1] = '\0';
				}
				strcpy(shadowList[count].encrypted, tmp);
			//	shadowList[count].encrypted = tmp; 
				//printf("EN: %s\n", shadowList[count].encrypted);
				sprintf(shadowList[count].password, "%s%s", shadowList[count].combine, shadowList[count].encrypted);
				//printf("pas: %s\n", shadowList[count].password);
			}
			
		//	printf("C: %s\n", shadowList[count].combine);
			i++;	
		}
		count++;
	}

	//int wordCount = 235888;	
	//int amount = worldCount / worldSize;	
	//int tmp = amount;
	//if (rank == worldSize-1) amount += wordCount % worldSize;

	
	count = 0;
	//int end = amount * rank;
	//int end = amount * (rank+1);
	//char buf[255];
	//while (fgets(buf, 255, fileWords)) {
	
	MPI_File ans;
	const char *fname = "answers.data";

	/*
	MPI_File_open(
		world,
		fname,
		MPI_MODE_CREATE | MPI_MODE_RDWR | MPI_MODE_APPEND,
		MPI_INFO_NULL,
		&ans
	);
	int offset = 0;
	MPI_File_set_view(
		ans,
		offset*255,
		MPI_CHAR,
		MPI_CHAR,
		"native",
		MPI_INFO_NULL
	);	
	*/
	FILE *res;
	res = fopen("fileAnswer.txt", "a");
	MPI_Request rqst[11];

	for (int i = 0; i < 11; i++) {
		MPI_Irecv(&found[i], 1, MPI_INT, MPI_ANY_SOURCE, i, world, &rqst[i]); 
	}

	MPI_File fh;
	//const char *fname = "results.data";
	MPI_File_open(
		world,
		fname,
		MPI_MODE_CREATE | MPI_MODE_RDWR,
		MPI_INFO_NULL,
		&fh	
	);
	char r[100];

	while (fscanf(fileWords, "%s", buf)) {
		/*
		if (rank != worldSize-1) {
			if (count < end && count >= start) {

			}
			
		}
		else  {
			
			break;
		}
		count++;
		*/
/*
	char password[] = "123test";

	char salt[] = "$1$ab$";

	char *encrypt = crypt(password, salt);

	printf("%s\n", encrypt);
	*/

	//parse shadowList [11];
	//	for (int i = 0; i < 11; i++) {
	//		MPI_Irecv(&found[i], 1, MPI_INT, MPI_ANY_SOURCE, i, world, &rqst[i]); 
	//	}
		
		char tmp[255];
		int one = 1;
		if (count >= saveCount && count % 30 == 0 && count != 0 && rank == 0) {
			 saveCount = count;
			fseek(save, 0, SEEK_SET);
			fprintf(save, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
											saveCount, found[0], found[1], found[2],
										     found[3], found[4], found[5], found[6],	
											found[7], found[8], found[9], found[10]);
			fseek(save, 0, SEEK_SET);
			//fprintf(save, "%d", saveCount);
		}
		if (count % worldSize == rank && count >= saveCount) {
			for (int i = 0; i < 11; i++) {
				MPI_Test(&rqst[i], &found[i], MPI_STATUS_IGNORE);
			}
			//MPI_Irecv(&found[tag], 1, MPI_INT, MPI_ANY_SOURCE, tag, world, &req);
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					for (int k = 0; k < 10; k++) {
						for (int v = 0; v < 10; v++) {
							for (int scount = 0; scount < 11; scount++) {	
								if (found[scount] == 1) {
									continue;
								}
								char *encrypt;
								if (k < 1 && j < 1 && i < 1) {
									//if (v == 9) printf("num: %d\n", strcmp(crypt(tmp, shadowList[scount].combine), shadowList[count].password));
									//if (v == 9) printf("%s tmp %s ANS: %s", encrypt, tmp, shadowList[scount].password);
									//if (v == 9) printf("tmp: %s enc %s her %s\n", shadowList[count].password, crypt(tmp, shadowList[scount].combine), tmp);
									//encrypt = crypt(tmp, shadowList[scount].combine);	
									sprintf(tmp, "%d%s", v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);
										//printf("true : user %s\n", shadowList[scount].username);	
										//printf("user: %s ENCRY: %s\n", shadowList[scount].password, encrypt);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d", buf, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%d%d%s", k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}	

									sprintf(tmp, "%s%d%d", buf, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}


									sprintf(tmp, "%d%d%d%s", j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d%d%d", buf, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%d%d%d%d%s", i, j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d%d%d%d", buf, i, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}
								}	
								if (k >= 1 && j < 1 && i < 1) {
									sprintf(tmp, "%d%d%s", k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d%d", buf, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%d%d%d%s", j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d%d%d", buf, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%d%d%d%d%s", i, j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d%d%d%d", buf, i, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}
								}	
								if (j >= 1 && i < 1) {
									sprintf(tmp, "%d%d%d%s", j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%s%d%d%d", buf, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}

									sprintf(tmp, "%d%d%d%d%s", i, j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}
									sprintf(tmp, "%s%d%d%d%d", buf, i, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}
								}	
								if (i >= 1) {
									sprintf(tmp, "%d%d%d%d%s", i, j, k, v, buf);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}
									//printf("%s\n", tmp);
									sprintf(tmp,"%s%d%d%d%d", buf, i, j, k, v);
									encrypt = crypt(tmp, shadowList[scount].combine);	
									if ((strcmp(encrypt, shadowList[scount].password)) == 0) {
										found[scount] = 1;
										fprintf(res, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										for (int nodes = 0; nodes < worldSize; nodes++) {
											MPI_Send(&one, 1, MPI_INT, nodes, scount, world);
										}
										memset(r, ' ', 100);
										r[99] = '\n';
										sprintf(r, "Username: %s and password %s\n", shadowList[scount].username, tmp);
										int offset = (scount)*100*sizeof(char);

										MPI_File_write_at(
											fh,
											offset,
											r,
											100,
											MPI_CHAR,
											MPI_STATUS_IGNORE);
									}
								}
							}
						}
					}
				}
			}
			//printf("rank %d and word %s\n", rank, buf);
			
		}
	//	if (count == 50) break;
		count++;
		if (count >= saveCount) saveCount++;
	}
/*
	for (i = 0; i < 235888; i++) {
		//printf("%s\n", words[i]);
		free(words[i]);
	}
*/

	/*
	printf("Rank %d - ", rank);
	for (int i = 0; i < 11; i++) {
		printf("%d ", found[i]);
	}
	puts("");
	*/

//	if (rank == 0) printf("test: %s\n", shadowList[1].salt);

	//printf("testttt %s\n", shadowList[1].encrypted);
	fclose(fileWords);
	fclose(fileShadow);
	
	fclose(save);
	fclose(res);
	MPI_File_close(&fh);

	for (int i = 0; i < 11; i++) freeParse(&shadowList[i]);
	MPI_Finalize();
	return 0;
}
