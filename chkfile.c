#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *MakeFileToken(char *src,char *check,char *token) {
	char *ret=NULL;
	char *pt;
	if(src == NULL) return NULL;
	pt = strstr(src,check);
	if(pt == NULL) return NULL;
	ret = (char *)malloc(strlen(token)+1);
	strcpy(ret,token);
	return ret;
}
char *kgCheckFile(char *name) {
	FILE *pp;
	char wrk[300];
	char *pt;
	char *ret=NULL;
	sprintf(wrk,"file %s",name);
	printf("%s\n",wrk);
	pp = popen(wrk,"r");
	while (fgets(wrk,299,pp) != NULL) {
	        printf("%s\n",wrk);
		pt = strstr(wrk,"cannot open");
		if(pt != NULL) return NULL;
		ret = MakeFileToken(wrk,"executable","Exe");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"image data","Image");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"MPEG","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"MP4","Video");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"WAVE","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"Stereo","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"audio","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"Audio","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"directory","Folder");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"gzip","Gzip");
		if (ret!= NULL) return ret;
		ret = malloc(8);
		strcpy(ret,"Unknown");
		return ret;
	}
}
	       
int main(int argc,char **argv) {
	printf("%s :%s\n",argv[1],kgCheckFile(argv[1]));
	return 0;
}
