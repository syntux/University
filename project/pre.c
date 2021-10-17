#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	//char ptmp1[] = "0hello";
	//char ptmp2[] = "00hello";
	//char ptmp3[] = "000hello";
	char tmp4[] = "0000hello";
	//char tmp1[] = "hello0000";
	char *tmp1 = calloc('\0', 100);
	sprintf(tmp1, "hello0000");

	//char stmp1[] = "hello0";
	//char stmp2[] = "hello00";
	//char stmp3[] = "hello000";
	//char stmp4[] = "hello0000";
	int ind;
	int size = strlen(tmp1);
	for (int i = 0; i <= 9999; i++) {
		if (i == 0) {
			//printf("%s\n", (tmp4)+3);
			//printf("%s\n", (tmp4)+2);
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);
			//
			ind = strlen(tmp1)-3;
			tmp1[ind] = '\0';
			//printf("%s\n", tmp1);
			tmp1[ind] = '0';

			ind = strlen(tmp1)-2;
			tmp1[ind] = '\0';
			//printf("%s\n", (tmp1));
			tmp1[ind] = '0';

			ind = strlen(tmp1)-1;
			tmp1[ind] = '\0';
			//printf("%s\n", (tmp1));
			tmp1[ind] = '0';

			//printf("%s\n", (tmp1));

			continue;
		}	
		
		if (i < 10) {
			tmp4[3]++;	
			//printf("%s\n", (tmp4)+3);
			//printf("%s\n", (tmp4)+2);
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);

		//	printf("%s\n", (tmp1)+3);
		//	printf("%s\n", (tmp1)+2);
		//	printf("%s\n", (tmp1)+1);
		//	printf("%s\n", (tmp1)+0);

			ind = strlen(tmp1)-3;
			tmp1[ind] = '\0';
			tmp1[ind-1]++;
			//printf("%s\n", tmp1);



			tmp1[ind] = tmp1[ind-1];
			tmp1[ind-1] = '0';
			ind = strlen(tmp1)-2;
			tmp1[ind] = '\0';
			//printf("%s\n", tmp1);

			tmp1[ind] = tmp1[ind-1];
			tmp1[ind-1] = '0';
			ind = strlen(tmp1)-1;
			tmp1[ind] = '\0';
			//printf("%s\n", tmp1);



			tmp1[ind] = tmp1[ind-1];
			tmp1[ind-1] = '0';
			//printf("%s\n", tmp1);
			

			tmp1[strlen(tmp1)-4] = tmp1[ind];
			tmp1[ind] = '0';
			

			continue;
		}
		if (i % 10 == 0) {
			tmp4[3] = '0';
			tmp4[2]++;

			if (i == 10) {
				tmp1[size-4] = '1';
				tmp1[size-2] = '\0';
			}
			else {
			//	if (i == 20) printf("% 10 %c\n", tmp1[size-4]);
				if (i < 100) {
					tmp1[size-4]++;
					tmp1[size-3] = '0';
					tmp1[size-2] = '\0';
				}
				else if (i > 100 && i < 1000){
					tmp1[size-3]++;
					tmp1[size-2] = '0';
					tmp1[size-1] = '\0';
				}
				else {
					tmp1[size-2]++;
					tmp1[size-1] = '0';
				}
			}
				
		}
		else {
			tmp4[3]++;

			//if (i == 11) printf("ELSE %c\n", tmp1[size-3]);
			//if (i > 100) tmp1[size-2]++;	
			//else tmp1[size-3]++;
			
			if (i < 100) {
				tmp1[size-3]++;
			}
			else if (i > 100 && i < 1000) {
				tmp1[size-2]++;
			}
			else {
				tmp1[size-1]++;
			}
		}
		if (i >= 10 && i < 100) {
			//printf("%s\n", (tmp4)+2);
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);

			


			printf("%s\n", tmp1);

			tmp1[size-2] = tmp1[size-3];
			tmp1[size-1] = '\0';
			tmp1[size-3] = tmp1[size-4];
			tmp1[size-4] = '0';
			printf("%s\n", tmp1);

			tmp1[size-1] = tmp1[size-2];
			tmp1[size-2] = tmp1[size-3];
			tmp1[size-3] = '0';
			printf("%s\n", tmp1);

			// NO? printf("%s\n", tmp1);
			tmp1[size-3] = tmp1[size-1];
			tmp1[size-1] = '0';
			tmp1[size-4] = tmp1[size-2];
			tmp1[size-2] = '\0';

		}
	
		if (i % 100 == 0) {
			tmp4[2] = '0';
			tmp4[1]++;	

			if (i == 100) {
				tmp1[size-4] = '1';
				tmp1[size-3] = '0';
				tmp1[size-2] = '0';
				tmp1[size-1] = '\0';
			}
			else {
			//	if (i == 20) printf("% 10 %c\n", tmp1[size-4]);
				if (i < 1000) {
					tmp1[size-4]++;
					tmp1[size-3] = '0';
				}
				else {
					tmp1[size-3]++;
					tmp1[size-2] = '0';
				}
			}
		}
		//else {
			//tmp4[3]++;
		//}
		if (i >= 100 && i < 1000) {
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);
			

			//printf("%s\n", tmp1);
			
			tmp1[size-1] = tmp1[size-2];
			tmp1[size-2] = tmp1[size-3];
			tmp1[size-3] = tmp1[size-4];
			tmp1[size-4] = '0';
			//printf("%s\n", tmp1);

			tmp1[size-4] = tmp1[size-3];
			tmp1[size-3] = tmp1[size-2];
			tmp1[size-2] = tmp1[size-1];
			tmp1[size-1] = '\0';
		
		}
		if (i % 1000 == 0) {
			tmp4[1] = '0';
			tmp4[0]++;

			if (i == 1000) {
				tmp1[size-4] = '1';
				tmp1[size-3] = '0';
				tmp1[size-2] = '0';
				tmp1[size-1] = '0';
			}
			else {
				tmp1[size-4]++;
				tmp1[size-3] = '0';
			}

		}
		if (i >= 1000) {
		//	printf("%s\n", (tmp4)+0);
//			if (i > 1950 && i < 2010) printf("%s\n", tmp1);
		//	printf("%s\n", tmp1);
		}
	}

}
