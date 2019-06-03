#include "cutils.h"

void addFormMessage(char *msg, char *msg_form) {
	char begin[] = "====== BEGIN CRYPTORIA RSA MESSAGE ======\n";
	char end[] = "\n======= END CRYPTORIA RSA MESSAGE =======";
	
	char msg_base64[5000] = "";
	char msg_base64_n[5000] = "";
	char msg_base64_n_balised[5000] = "";
	Base64encode(msg_base64, msg, strlen(msg));
	
	unsigned int i = 0;
	unsigned int l = 0;
	while(i != strlen(msg_base64))
	{
		if(i % strlen(begin) == 0 && i != 0)
		{
			msg_base64_n[i + l] = '\n';
			l++;
		}
		msg_base64_n[i + l] = msg_base64[i];
		i++;
	}
	msg_base64_n[strlen(msg_base64) + l] = '\0';
	sprintf(msg_base64_n_balised, "%s%s%s", begin, msg_base64_n, end);
	strcpy(msg_form, msg_base64_n_balised);
}

void removeFormMessage(char *msg, char *msg_unform) {
	char begin[] = "====== BEGIN CRYPTORIA RSA MESSAGE ======";
	char end[] = "======= END CRYPTORIA RSA MESSAGE =======";
	
	char msg_base64_balised[5000] = "";
	strcpy(msg_base64_balised, msg);
	
	strremove(msg_base64_balised, begin);
	strremove(msg_base64_balised, end);
	strremove(msg_base64_balised, "\n");
	Base64decode(msg_unform, msg_base64_balised);
}

//fileExist
int fileExist(char nameFile[])
{
	FILE* file = NULL;
    file = fopen(nameFile, "r+");
	if(file != NULL) {
		fclose(file);
		return 1;
	} else {
		return 0;
	}
}

//createFile
void createFile(char nameFile[])
{
	FILE* file = NULL;
    file = fopen(nameFile, "w");
	fclose(file);
}

//writerFile
void writeInFile(char nameFile[], char str[])
{
	/*
	int i = 1;
	while(fileExist(nameFile) == 0) {
		
	}
	*/
	FILE* file = NULL;
    file = fopen(nameFile, "r+");
	fprintf(file, "%s", str);
}

//Source: https://stackoverflow.com/questions/24696113/how-to-find-text-between-two-strings-in-c
void getStrBetween(char* result, char input[], char str1[], char str2[]) {
	const char *s = input;
    const char *PATTERN1 = str1;
    const char *PATTERN2 = str2;

    char *target = NULL;
    char *start, *end;

    if (start = strstr(s, PATTERN1))
    {
        start += strlen( PATTERN1 );
        if (end = strstr(start, PATTERN2))
        {
            target = (char*)malloc(end - start + 1);
            memcpy(target, start, end - start);
            target[end - start] = '\0';
        }
    }
	strcpy(result, target);
    free(target);
}

//Source: https://stackoverflow.com/questions/47116974/remove-a-substring-from-a-string-in-c
char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}