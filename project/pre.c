#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	//char ptmp1[] = "0hello";
	//char ptmp2[] = "00hello";
	//char ptmp3[] = "000hello";
	char tmp4[] = "0000hello";
	char tmp1[] = "hello0000";

	//char stmp1[] = "hello0";
	//char stmp2[] = "hello00";
	//char stmp3[] = "hello000";
	//char stmp4[] = "hello0000";

	for (int i = 0; i <= 9999; i++) {
		if (i == 0) {
			//printf("%s\n", (tmp4)+3);
			//printf("%s\n", (tmp4)+2);
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);
			//
			tmp1[strlen(tmp1)-2] = '\0';
			printf("test %s\n", tmp1);
			//tmp1[strlen(tmp1)-2] = '\0';
			//printf("%s\n", (tmp1));
			//tmp1[strlen(tmp1)-2] = '0';
			//tmp1[strlen(tmp1)-1] = '\0';
			//printf("%s\n", (tmp1));
			tmp1[strlen(tmp1)-1] = '0';
			tmp1[strlen(tmp1)-0] = '\0';
			//printf("%s\n", (tmp1));
			tmp1[strlen(tmp1)-0] = '0';
			//printf("%s\n", (tmp1));
			continue;
		}	
		
		if (i < 10) {
			tmp4[3]++;	
			//printf("%s\n", (tmp4)+3);
			//printf("%s\n", (tmp4)+2);
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);

			//printf("%s\n", (tmp1)+3);
			//printf("%s\n", (tmp1)+2);
			//printf("%s\n", (tmp1)+1);
			//printf("%s\n", (tmp1)+0);

			continue;
		}
		if (i % 10 == 0) {
			tmp4[3] = '0';
			tmp4[2]++;
		}
		else {
			tmp4[3]++;
		}
		if (i >= 10 && i < 100) {
			//printf("%s\n", (tmp4)+2);
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);
		}
	
		if (i % 100 == 0) {
			tmp4[2] = '0';
			tmp4[1]++;	
		}
		//else {
			//tmp4[3]++;
		//}
		if (i >= 100 && i < 1000) {
			//printf("%s\n", (tmp4)+1);
			//printf("%s\n", (tmp4)+0);
		}
		if (i % 1000 == 0) {
			tmp4[1] = '0';
			tmp4[0]++;
		}
		if (i >= 1000) {
		//	printf("%s\n", (tmp4)+0);
		}
	}

}
