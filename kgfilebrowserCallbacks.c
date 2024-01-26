#include <kulina.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "icons.c"
#define Free(x) if(x!=NULL) free(x);
static void *Dtmp;
void *Runinfobox(void *parent,void *arg);
void *Runstatbox(void *parent,void *arg);
int kgFilterString(char *s, char *fltr);


static char Folder1[300],Home1[300]="";
static char Folder2[300],Home2[300]="";
static char Trash[300];
static void *folderimg=NULL;
static void *fileimg=NULL,*musicimg=NULL,*videoimg=NULL,
	    *cimg=NULL,*tarimg=NULL,*linkimg=NULL,
	    *gzipimg=NULL,*exeimg=NULL,*binaryimg=NULL;

static DIT *T1=NULL,*T2=NULL;
static DIY *X1=NULL,*X2=NULL;
static DIY *Y1=NULL,*Y2=NULL;
static DIY *TR=NULL;
static char Filter[200];
int SetThumbNailImages(ThumbNail **th,int size);
char *kgCheckFileType(char *name);
static ThumbNail **GetFolderThumbNails(char *Folder,int size);
static ThumbNail **GetFileThumbNails(char *Folder,int size);
int RunkgPopUpMenu(void *arg,int xo,int yo,char **menu);
static char *MakeFileToken(char *src,char *check,char *token);
static ThumbNail *MakeThumbNail(char *name,int size);
#if 0
char *FMenu1[8]={"Copy selected files","Move selected files","Selected files to Trash","Delete selected files","Rename this","Backup this","Create Link in Other",NULL};
char *FMenu2[8]={"Copy selected files","Move selected files","Selected files to Trash","Delete selected files",NULL};
char *DMenu1[5]= {"Make new folder","Rename","Backup","Create Link in Other",NULL};
char *DMenu2[5]= {"Make new folder",NULL};
#else
char *FMenu1[5]={"Rename this","Backup this","Create Link in Other","Info",NULL};
#endif
#define kgFree(pt) {if(pt!=NULL) free(pt);pt=NULL;}
#define CHECKDELETESAME {\
	 if(same) {\
		 if(strcmp(src,"Xbox1")==0 ){ \
                  kgDeleteThumbNail(X2,item); \
                  kgUpdateWidget(X2); \
		 } \
		 if(strcmp(src,"Ybox1")==0 ){ \
                  kgDeleteThumbNail(Y2,item); \
                  kgUpdateWidget(Y2); \
		 } \
		 if(strcmp(src,"Xbox2")==0 ){ \
                  kgDeleteThumbNail(X1,item); \
                  kgUpdateWidget(X1); \
		 } \
		 if(strcmp(src,"Ybox2")==0 ){ \
                  kgDeleteThumbNail(Y1,item); \
                  kgUpdateWidget(Y1); \
		 } \
	 } \
}

#define CHECKADDSAME {\
	 if(same) {\
		 if(strcmp(des,"Xbox1")==0 ){ \
                  kgAddThumbNail(X2,kgCopyThumbNail(th),0); \
			 kgSortList(X2);\
			 kgListRemoveDup(X2);\
                  kgUpdateWidget(X2); \
		 } \
		 if(strcmp(des,"Ybox1")==0 ){ \
                  kgAddThumbNail(Y2,kgCopyThumbNail(th),0); \
			 kgSortList(Y2);\
			 kgListRemoveDup(Y2);\
                  kgUpdateWidget(Y2); \
		 } \
		 if(strcmp(des,"Xbox2")==0 ){ \
                  kgAddThumbNail(X1,kgCopyThumbNail(th),0); \
			 kgSortList(X1);\
			 kgListRemoveDup(X1);\
                  kgUpdateWidget(X1); \
		 } \
		 if(strcmp(des,"Ybox2")==0 ){ \
                  kgAddThumbNail(Y1,kgCopyThumbNail(th),0); \
			 kgSortList(Y1);\
			 kgListRemoveDup(Y1);\
                  kgUpdateWidget(Y1); \
		 } \
	 } \
}
#define ProcessRename(Folder,W) {\
  sprintf(fname,"%-s/%-s",Folder,Th->name); \
  strcpy(from,Th->name); \
  sprintf(job,"New name for %-s ",Th->name); \
  strcat(job,"%30s"); \
  strcpy(to,Th->name); \
  gscanf(Tmp,job,to); \
  sprintf(job,"rename \\%-s\\ \\%-s\\ \\%-s\\",Th->name,to,fname); \
  kgRunJob(job,NULL); \
  sprintf(fname,"%-s/%-s",Folder,to); \
  if(kgCheckFileType(fname)!= NULL) { \
    free(Th->name); \
    kgSetThumbNailName(W,item,to); \
    if(same) {\
  	  void *Owid=NULL; \
  	  ThumbNail *Oth=NULL;\
          char *wname=NULL;\
	  wname = kgGetWidgetName(Tmp,kgGetWidgetId(Tmp,W));\
  	  if(strcmp(wname,"Xbox1")==0) {  Owid =X2; }\
	    else if(strcmp(wname,"Xbox2")==0) {  Owid =X1; }\
	         else if(strcmp(wname,"Ybox1")==0) {  Owid =Y2; }\
  	              else if(strcmp(wname,"Ybox2")==0) {  Owid =Y1; }\
  	  Oth= (ThumbNail *)kgGetThumbNail(Owid,item);\
          free(Oth->name); \
          kgSetThumbNailName(Owid,item,to); \
          kgSortList(Owid); \
          kgUpdateWidget(Owid);\
    }\
    kgSortList(W); \
    kgUpdateWidget(W);\
    kgUpdateOn(Tmp);\
  }\
}
#define ProcessBackup(Folder,W) {\
  int exist=0;\
  ThumbNail *Thnew=NULL;\
  void *bs;\
  sprintf(fname,"%-s/%-s",Folder,Th->name); \
  strcpy(from,Th->name); \
  sprintf(job," Backup/copy name for %-s ",Th->name); \
  strcat(job,"%30s"); \
  strcpy(to,Th->name); \
  strcat(to,".bak");\
  gscanf(Tmp,job,to); \
  if(kgCheckMenu(Tmp,400,400,"Make Backup",0)){ \
  sprintf(job,"cp -rf  \\%-s/%-s\\ \\%-s/%-s\\",Folder,from,Folder,to); \
  sprintf(fname,"%-s/%-s",Folder,to); \
  if( kgCheckFileType(fname) != NULL) exist=1;\
  bs = kgOpenBusy(Tmp,600,250);\
  kgRunJob(job,NULL);\
  kgCloseBusy(bs);\
  if(kgCheckFileType(fname)!= NULL) { \
    if(!exist) {\
      Thnew = (ThumbNail *)kgCopyThumbNail(Th);\
      free(Thnew->name); \
      Thnew->name = (char *)malloc(strlen(to)+1);\
      strcpy(Thnew->name,to);\
      kgAddThumbNail(W,Thnew,0);\
      if(same) {\
  	  void *Owid=NULL; \
  	  ThumbNail *Oth=NULL;\
          char *wname=NULL;\
	  wname = kgGetWidgetName(Tmp,kgGetWidgetId(Tmp,W));\
  	  if(strcmp(wname,"Xbox1")==0) {  Owid =X2; }\
	    else if(strcmp(wname,"Xbox2")==0) {  Owid =X1; }\
	         else if(strcmp(wname,"Ybox1")==0) {  Owid =Y2; }\
  	              else if(strcmp(wname,"Ybox2")==0) {  Owid =Y1; }\
  	  Oth= (ThumbNail *)kgCopyThumbNail(Thnew);\
          kgAddThumbNail(Owid,Oth,0);\
          kgSortList(Owid); \
          kgUpdateWidget(Owid);\
      }\
      kgSortList(W); \
      kgUpdateWidget(W);\
      kgUpdateOn(Tmp);\
    }\
  }\
  }\
}
#define ProcessMakeFolder(Folder,W) {\
  int exist=0;\
  int count=0;\
  ThumbNail *Thnew=NULL;\
  sprintf(fname,"%-s/NewFolder%-3.3d",Folder,count); \
  while ( kgCheckFileType(fname) != NULL) { \
      count++; \
      sprintf(fname,"%-s/NewFolder%-3.3d",Folder,count); \
  }\
  sprintf(job," New Folder Name  "); \
  strcat(job,"%30s"); \
  sprintf(to,"NewFolder%3.3d",count);\
  gscanf(Tmp,job,to); \
  sprintf(job,"mkdir   \\%-s/%-s\\",Folder,to); \
  sprintf(fname,"%-s/%-s",Folder,to); \
  if( kgCheckFileType(fname) != NULL) exist=1;\
  kgRunJob(job,NULL);\
  if(kgCheckFileType(fname)!= NULL) { \
    if(!exist) {\
      Thnew = (ThumbNail *)MakeThumbNail(to,20);\
      kgAddThumbNail(W,Thnew,0);\
      if(same) {\
  	  void *Owid=NULL; \
  	  ThumbNail *Oth=NULL;\
          char *wname=NULL;\
	  wname = kgGetWidgetName(Tmp,kgGetWidgetId(Tmp,W));\
  	  if(strcmp(wname,"Xbox1")==0) {  Owid =X2; }\
	  else if(strcmp(wname,"Xbox2")==0) {  Owid =X1; }\
  	  Oth= (ThumbNail *)kgCopyThumbNail(Thnew);\
          kgAddThumbNail(Owid,Oth,0);\
          kgSortList(Owid); \
          kgUpdateWidget(Owid);\
      }\
      kgSortList(W); \
      kgUpdateWidget(W);\
      kgUpdateOn(Tmp);\
    }\
  }\
}
#define ProcessCreateLink(W,Sfolder,Dfolder) {\
  int exist=0;\
  int count=0;\
  ThumbNail *Thnew=NULL;\
  sprintf(job," Link Name in other side "); \
  strcat(job,"%30s"); \
  sprintf(to,"%-s",Th->name);\
  gscanf(Tmp,job,to); \
  sprintf(fname,"%-s/%-s",Dfolder,to);\
  sprintf(from,"%-s/%-s",Sfolder,Th->name);\
  if(kgCheckFileType(fname)== NULL) { \
     CreateLink (from,fname);\
     if(kgCheckFileType(fname)!= NULL) { \
  	  void *Owid=NULL; \
  	  ThumbNail *Oth=NULL;\
          char *wname=NULL;\
	  wname = kgGetWidgetName(Tmp,kgGetWidgetId(Tmp,W));\
  	  if(strcmp(wname,"Xbox1")==0) {  Owid =X2; }\
	  if(strcmp(wname,"Xbox2")==0) {  Owid =X1; }\
  	  if(strcmp(wname,"Ybox1")==0) {  Owid =Y2; }\
	  if(strcmp(wname,"Ybox2")==0) {  Owid =Y1; }\
  	  Oth= (ThumbNail *)malloc(sizeof(ThumbNail));\
          Oth->name=(char *)malloc(strlen(to)+1);\
          strcpy(Oth->name,to);\
          Oth->state=1; Oth->sw=0;\
	  if(linkimg == NULL) \
      	        linkimg = kgGetImageCopy(NULL,(void *)&link_str);\
	  if( (Owid==X1)||(Owid==X2) ) {\
	       Oth->img = kgThumbNailImage(linkimg,48,48);\
	       if(same) {\
                 kgAddThumbNail(X1,Oth,0);\
                 kgAddThumbNail(X2,kgCopyThumbNail(Oth),0);\
                 kgSortList(X1); \
                 kgUpdateWidget(X1);\
                 kgSortList(X2); \
                 kgUpdateWidget(X2);\
	       }\
	       else {\
                  kgAddThumbNail(Owid,Oth,0);\
                  kgSortList(Owid); \
                  kgUpdateWidget(Owid);\
	       }\
	  }\
	  else {\
	       Oth->img = kgThumbNailImage(linkimg,20,20);\
	       if(same) {\
                 kgAddThumbNail(Y1,Oth,0);\
                 kgAddThumbNail(Y2,kgCopyThumbNail(Oth),0);\
                 kgSortList(Y1); \
                 kgUpdateWidget(Y1);\
                 kgSortList(Y2); \
                 kgUpdateWidget(Y2);\
	       }\
	       else {\
                  kgAddThumbNail(Owid,Oth,0);\
                  kgSortList(Owid); \
                  kgUpdateWidget(Owid);\
	       }\
	  }\
      }\
      kgUpdateOn(Tmp);\
  }\
}
#define ProcessOtherButtonClick(W,Menu1,Menu2) {\
		  rval =-1; \
		  kgGetClickedPosition(Tmp,&x,&y); \
		  item = kgGetThumbNailItem(W,x,y); \
		  List=(ThumbNail **)kgGetList(W); \
		  xpos = x; \
		  ypos = y; \
		  if(ypos >(yl-250) )ypos= yl-250; \
		  if(xpos >(xl-250) )xpos= xl-250; \
	          Th=NULL; \
		  if(item>= 0)  { \
		    Th= (ThumbNail *)List[item]; \
		    rval = RunkgPopUpMenu(Tmp,xpos,ypos,Menu1); \
		  } \
		  else rval = -1; \
}
#define ProcessInfo(Folder) { \
	strcpy(job,"stat ");\
	strcat(job,"\\");\
	strcat(job,Folder);\
	strcat(job,"/");\
	strcat(job,Th->name);\
	strcat(job,"\\");\
	kgRunJob(job,ProcessStatData);\
}

static int _check_for_string(char *s,char *chk) {
  int ls,lchk,i,k,count,ret=0,j,ch;
  ls = strlen(s);
  k=0;while( (chk[k]!='\0')&&(chk[k]!='*')&&(chk[k]!='?')) k++;
  ch = chk[k];chk[k]='\0';
  lchk = strlen(chk);
  if(lchk>ls) return 0;
  for(i=0;i<(ls-lchk+1);i++) {
   count=0;
#ifdef ZORDOS
   for(j=0;j<lchk;j++) if( s[i+j] == toupper(chk[j]) )count++;
#else
   for(j=0;j<lchk;j++) if( s[i+j] == chk[j] )count++;
#endif
   if(count==lchk ) {ret=i+lchk; break;}
  }
  chk[k]=ch;
  return ret;
}
static int _check_for_start_string(char *s,char *chk) {
  int ls,lchk,i,count,ret=0,j,ch;
  ls = strlen(s);
  i=0;while( (chk[i]!='\0')&&(chk[i]!='*')&&(chk[i]!='?')) i++;
  ch = chk[i];chk[i]='\0';
  lchk = strlen(chk);
  if(lchk<=ls) {
    count=0;
    for(j=0;j<lchk;j++) if( s[j] == chk[j] )count++; else break;
    if(count==lchk ) {ret=lchk; }
  }
  chk[i]=ch;
  return ret;
}
static int _skip_string(char *chk) {
  int i=0;
  while( (chk[i]!='\0')&&(chk[i]!='*')&&(chk[i]!='?')) i++;
  return i;
}
/* checking for character
   if found position is returned
*/
static int _check_for_char(char *s,char ch) {
 int i=0;
 while( (s[i]!= ch)&&(s[i]!='\0')) i++;
 if(s[i]=='\0') return 0;
 else return i;
}
static int uiFilterOtherOptions(char *s, char *fltr){
  int pos,skip;
  if(fltr[0]==' ') return 1;
  
  if(fltr[0]=='\0'){
     if(s[0]=='\0') return 1;
     else return 0;
  }
  if(fltr[0]=='*') {
    if(fltr[1]=='.') {
      pos = _check_for_char(s,'.');
      if(pos != 0) return uiFilterOtherOptions(s+pos+1,fltr+2);
      else return 0;
    }
    else {
      if(fltr[1]=='\0') return 1;
      else if(fltr[1]=='?'){
              return uiFilterOtherOptions(s+1,fltr+2);
           }
           else {
              pos = _check_for_string(s,fltr+1);
              if(pos == 0) {
                 return 0;
              }
              else {
                 skip = _skip_string(fltr+1);
                 return uiFilterOtherOptions(s+pos,fltr+1+skip);
              }
           }
    }
  }
  else {
      if(fltr[0]=='?'){
              return uiFilterOtherOptions(s+1,fltr+1);
      }
      else {
              pos = _check_for_start_string(s,fltr);
              if(pos == 0) {
                 return 0;
              }
              else {
                 skip = _skip_string(fltr);
                 return uiFilterOtherOptions(s+pos,fltr+skip);
              }
      }
  }
}
int kgFilterString(char *s, char *fltr){
  int i,j,ret=0;
  char *buf;
  if(strcmp("*",fltr)==0) return 1;
  if(strcmp("*.*",fltr)==0){
     i=0;
     while(s[i]!='\0') if(s[i++]=='.') return 1;
     return 0;
  }
  else{
    buf= (char *)Malloc(strlen(fltr)+1);
//    strcpy(buf,fltr);
    i=0;
    while(1) {
      j=0;
      while(fltr[i]==' ') i++;
      if(fltr[i] < ' ') break;
      while(fltr[i]>' ') buf[j++]=fltr[i++];
      buf[j]='\0';
      ret =  uiFilterOtherOptions(s,buf);
      if(ret) break;
    }
    Free(buf);
    return ret;
  }
} 
static int kgGetLine(int pip0,char *buff){
     unsigned char ch;
     fd_set rfds;
     struct timeval tv;
     int retval,chnl,status,i;
     int ret,val;
     chnl = pip0;
     i=0;
     FD_ZERO(&rfds);
     FD_SET(pip0,&rfds);
     while(1) {
       tv.tv_sec = 0;
       tv.tv_usec =200000;
       ret=0;
       retval = select(chnl+1,&rfds,NULL,NULL,&tv);
       if(retval < 0) {ret=-1;break;}
       val=1;
       if((retval> 0)&&(FD_ISSET(chnl,&rfds))){
         if(read(chnl,&ch,1) != 1){
           ret=0;
           break;
         }
         buff[i++]=ch;
         if(i>490) i=490;
         if( (ch=='\n')||(ch=='\r')) {ret=ch;break;}
       }
       else {ret=0;break;}
     }
     buff[i]='\0';
     return ret;
}
int kgRunFunction(int (*function)(void *),void *arg,int (*ProcessOut)(int,int,int)){
   int ret =0;
   FILE *fp,*fp1;
   int pip[2],pid,status,pip2[2],argc;
   char *args[100],buff[1000],pt[300];
   char *ptr;
   char *pgrpath=NULL;
   int i=0,pos=0;
   if( pipe(pip) < 0) return 0;
   if( pipe(pip2) < 0) return 0;
   pid = fork();
   if(pid == 0) { /* child process */
//     if(fork()!=0) exit(0); /* to avoid zombie */

     close(pip2[1]);
     close(pip[0]);
     close(0);
     dup(pip2[0]);
//     close(pip2[0]);
#if 1
     close(1);
     dup(pip[1]);
#endif
     close(2);
     dup(pip[1]);
  //   close(pip[1]);
     if(function != NULL) function(arg);
     fflush(stderr);
     fflush(stdout);
     exit(0);
   }
   else {   /* parent process */
//     printf("Waiting pid: %d\n",pid);
     close(pip2[0]);
     close(pip[1]);
     if(ProcessOut != NULL){
        ret=ProcessOut(pip[0],pip2[1],pid);
        printf("Killing %d\n",pid);
        kill(pid,9);
     }
     waitpid(pid,&status,0);
     return ret;
   }
}
static int ProcessData(int rid,int wid,int pid) {
	int pvals[4];
	int incr=100;
	pvals[0]=rid;
	pvals[1] = pid;
	pvals[2]= incr;
	pvals[3]= wid;
	Runinfobox(Dtmp,pvals);
	return 1;
}
static int* MonitorProgress(void *Tmp,int pid) {
	int pvals[4];
	int incr=1;
        int Pip1[2],Pip2[2];
	int *Pips;
        if( pipe(Pip1) < 0) return NULL;
        if( pipe(Pip2) < 0) return NULL;
	Pips= (int *) malloc(sizeof(int)*2);
        if(fork()==0) {
            close(Pip2[0]);
            close(Pip1[1]);
//	    MonitorProgress(Pip1[0],Pip2[1],0);
     	    pvals[0]=Pip1[0];
	    pvals[1] = pid;
	    pvals[2]= incr;
	    pvals[3]= Pip2[1];
	    Runinfobox(Tmp,pvals);
            close(Pip2[1]);
            close(Pip1[0]);
	    exit(0);
        }
        close(Pip2[1]);
        close(Pip1[0]);
        Pips[0]=Pip2[0];
        Pips[1]=Pip1[1];
	return Pips;
}
static int CloseMonitor(int *Pips) {
    close(Pips[0]);
    close(Pips[1]);
    free(Pips);
    return 1;
}
static int ProcessStatData(int rid,int wid,int pid) {
	int pvals[2];
	pvals[0]=rid;
	pvals[1] = pid;
	Runstatbox(Dtmp,pvals);
	return 1;
}

static int CreateLink(char *src,char *des){
	char *pt=NULL;
	char *spt=NULL,*dpt=NULL;
	char *pts=NULL,*ptd=NULL;
	

	
	int scount=0,dcount=0,n=0,i;
	char job[500];
	char target[500];

	if(strcmp(src,des)==0) return 0;
	strcpy(target,des);
	scount =0;
	pt =src;
	spt = NULL;
	while(strstr(pt,"/") != NULL) {spt=pt;pt++;scount++;}
	*spt='\0';
	spt++;
	dcount =0;
	pt =des;
	dpt = NULL;
	while(strstr(pt,"/") != NULL) {dpt=pt;pt++;dcount++;}
	*dpt='\0';
	dpt++;
	if(strcmp(src,des)==0) {
		sprintf(job,"ln -sf \\%-s\\ \\%-s/%-s\\",spt,des,dpt);
//		system(job);
		kgRunJob(job,NULL);
		return 1;
	}
	pt = strstr(src,des);
	if (pt!= NULL) { // destination is a top directory/self
		pt += strlen(des);
		if(*pt=='/')pt++;
		sprintf(job,"ln -sf \\./%-s/%-s\\ \\%-s/%-s\\",pt,spt,des,dpt);
//		system(job);
		kgRunJob(job,NULL);
		return 1;
	}
	pt = strstr(des,src);
	if (pt!= NULL) { // destination is a sub directory
		pt += strlen(src);
		if (*pt=='/') pt++;
		n =0;
		while( (pt=strstr(pt,"/"))!= NULL) {
			pt++;
			if(*pt=='\0') break;
			n++;
		}
		strcpy(job,"ln -sf \\../");
		i=0;
		while(i< n) {i++;strcat(job,"../");}
		pt= job + strlen(job);
		sprintf(pt,"%-s\\ \\%-s/%s\\",spt,des,dpt);
		kgRunJob(job,NULL);
                return 1;
	}
	n=0;
	pts = strstr(src+1,"/");
	ptd = strstr(des+1,"/");
	while( (pts!=NULL) && (ptd != NULL) ) {
		*pts = '\0';
		*ptd = '\0';
		if(strcmp(src,des)==0) {
			n++;
			src=pts+1;
			des=ptd+1;
	                pts = strstr(src,"/");
                   	ptd = strstr(des,"/");
			continue;
		}
		else {
		  *pts = '/';
		  *ptd = '/';
		  break;
		}

	}
	if(n==0) {
	   sprintf(job,"ln -sf \\%-s/%-s\\ \\%-s/%-s\\",src,spt,des,dpt);
	   kgRunJob(job,NULL);
           return 1;
	}
	else { // that is, some parts of paths are matching
	  strcpy(job,"ln -sf \\../");
	  n =0;
	  pt =des;
	  while( (pt=strstr(pt,"/"))!= NULL) {
			pt++;
			if(*pt=='\0') break;
			strcat(job,"../");
			n++;
          }
          pt= job + strlen(job);
	  sprintf(pt,"%-s/%-s\\ \\%s\\",src,spt,target);
	  kgRunJob(job,NULL);
          return 1;
	}

}
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
char *kgCheckFileType(char *name) {
	FILE *pp;
	char wrk[500],tmp[500];
	char *pt;
	char *ret=NULL;
	sprintf(wrk,"file \'%s\'",name);
//	printf("%s\n",wrk);
	pp = popen(wrk,"r");
	if(pp == NULL) return NULL;
	while (fgets(tmp,499,pp) != NULL) {
//	        printf("%s\n",tmp);
//		fflush(stdout);
		pt = strstr(tmp,"cannot open");
		if(pt != NULL) {pclose(pp);return NULL;}
		ret = MakeFileToken(tmp,"executable","Exe");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," image data","Image");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," MPEG","Music");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," MP4","Video");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," WAVE","Music");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," Stereo","Music");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," stereo","Music");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," audio","Music");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," Audio","Music");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp," directory","Folder");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp,"gzip compressed","Gzip");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp,"tar archive","Tar");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp,"C source","Csource");
		if (ret!= NULL) {pclose(pp);return ret;}
		ret = MakeFileToken(tmp,"symbolic link ","Link");
		if (ret!= NULL) {pclose(pp);return ret;}
//		ret = MakeFileToken(tmp," SVG ","Image");
//		if (ret!= NULL) {pclose(pp);return ret;}
		ret = malloc(strlen("Unknown")+1);
		strcpy(ret,"Unknown");
		pclose(pp);
		return ret;
	}
}
	       
static void Update1(void *Tmp) {
	    void **th;
	    void *bs=NULL;
	    bs=kgOpenBusy(Tmp,600,250);
	    kgSetString(T1,0,Folder1);
	    th = (void **)kgGetList(X1);
	    if(th != NULL) kgFreeThumbNails((ThumbNail **)th);
//	    th = (void **)kgFolderThumbNails(Folder1);
//            SetThumbNailImages((ThumbNail **)th,20);
            th = (void **)GetFolderThumbNails(Folder1,20);
	    kgSetList(X1,th);
	    th = (void **)kgGetList(Y1);
	    if(th != NULL) kgFreeThumbNails((ThumbNail **)th);
            th = (void **)GetFileThumbNails(Folder1,48);
	    kgCloseBusy(bs);
	    kgSetList(Y1,th);
	    kgUpdateWidget(X1);
	    kgUpdateWidget(Y1);
	    kgUpdateWidget(T1);
	    kgUpdateOn(Tmp);
}
static void Update2(void *Tmp) {
	    void **th;
	    void *bs=NULL;
	    bs=kgOpenBusy(Tmp,900,250);
	    kgSetString(T2,0,Folder2);
	    th = (void **)kgGetList(X2);
	    if(th != NULL) kgFreeThumbNails((ThumbNail **)th);
//	    th = (void **)kgFolderThumbNails(Folder2);
//            SetThumbNailImages((ThumbNail **)th,20);
            th = (void **)GetFolderThumbNails(Folder2,20);
	    kgSetList(X2,th);
	    th = (void **)kgGetList(Y2);
	    if(th != NULL) kgFreeThumbNails((ThumbNail **)th);
            th = (void **)GetFileThumbNails(Folder2,48);
	    kgSetList(Y2,th);
	    kgCloseBusy(bs);
	    kgUpdateWidget(X2);
	    kgUpdateWidget(Y2);
	    kgUpdateWidget(T2);
	    kgUpdateOn(Tmp);
}
#if 1
static int DragItem(void *Tmp,void *fw,int item) {
  DIY *FY,*TY;
  FY = (DIY *)fw;
  int x=-1,y=-1;
  void *tw=NULL;
  char *src,*des;
  char job[500];
  char Dir1[200]="",Dir2[200]="";
  char destloc[500];
  char *sdir,*ddir;;
  char *fret=NULL;
  void *bs=NULL;
  int sid,did;
  int same=0;
  int positem;
  if(strcmp(Folder1,"/") !=0) strcpy(Dir1,Folder1);
  if(strcmp(Folder2,"/") !=0) strcpy(Dir2,Folder2);
  sid = kgGetWidgetId(Tmp,fw);
  src = kgGetWidgetName(Tmp,sid);
  same =0;
  if( (strcmp(src,"Xbox1")==0)||(strcmp(src,"Ybox1")==0)) sdir=Folder1;
  if( (strcmp(src,"Xbox2")==0)||(strcmp(src,"Ybox2")==0)) sdir=Folder2;
  if( (strcmp(src,"Trash")==0)) sdir=Trash;
  if(strcmp(Folder1,Folder2)==0) same=1;
  if(kgDragThumbNail(FY,item,&x,&y)) {
     tw = (DIT *)kgGetLocationWidget(Tmp,x,y);
     if(tw != NULL) {
               did = kgGetWidgetId(Tmp,tw);
	       if(sid == did ) {
		       positem = kgGetThumbNailItem(fw,x,y);
		       if( item != positem ) return 1;
		       return 0;
	       }
               des = kgGetWidgetName(Tmp,did);
               if( (strcmp(des,"Xbox1")==0)||(strcmp(des,"Ybox1")==0)) ddir=Folder1;
               if( (strcmp(des,"Xbox2")==0)||(strcmp(des,"Ybox2")==0)) ddir=Folder2;
               if( ((strcmp(des,"Trash")==0))) ddir=Trash;
	       if(strcmp(des,"Imagebox")==0) {
		       ThumbNail *th;
		       char *ret1;
		       th = kgGetThumbNail(fw,item);
		       sprintf(job,"rm -rf \\%-s/%-s\\",sdir,th->name);
                       bs = kgOpenBusy(Tmp,600,250);
		       kgRunJob(job,NULL);
                       kgCloseBusy(bs);
		       sprintf(job,"%-s/%-s",sdir,th->name);
		       if((ret1=kgCheckFileType(job))== NULL) {
			 CHECKDELETESAME;
		         kgDeleteThumbNail(fw,item);
		         kgUpdateWidget(fw);
		         kgUpdateOn(Tmp);
		       }
		       else free(ret1);
		       return 1;
	       }
	       if(strcmp(des,"Trash")==0) {
		       ThumbNail *th;
		       char *ret1;
		       th = kgGetThumbNail(fw,item);
		       sprintf(job,"%-s/%-s",Trash,th->name);
		       if( kgCheckFileType(job) != NULL) {
                         if(kgCheckMenu(Tmp,400,400,"Do you want to overwrite",0)){
		            sprintf(job,"bash -c \\mv -f \"%-s/%-s/*\" -t \"%-s/%-s\"\\",sdir,th->name,Trash,th->name);
			 }
			 else {
		            sprintf(job,"bash -c \\mv -n \"%-s/%-s/*\" -t \"%s/%-s\"\\",sdir,th->name,Trash,th->name);
			 }
		       }
		       else sprintf(job,"mv \\%-s/%-s\\ -t \\%s\\",sdir,th->name,Trash);
//		       printf("%s\n",job);
                       bs = kgOpenBusy(Tmp,600,250);
		       kgRunJob(job,NULL);
                       kgCloseBusy(bs);
		       sprintf(job,"rm -rf \\%-s/%-s\\",sdir,th->name);
		       kgRunJob(job,NULL);
		       sprintf(job,"%-s/%-s",sdir,th->name);
		       if((ret1=kgCheckFileType(job))== NULL) {
		         th = kgPickThumbNail(fw,item);
//		         kgDeleteThumbNail(fw,item);
		         kgAddThumbNail(TR,th,0);
			 kgSortList(TR);
			 kgListRemoveDup(TR);
			 CHECKDELETESAME;
		         kgUpdateWidget(fw);
		         kgUpdateWidget(TR);
		         kgUpdateOn(Tmp);
		       }
		       else free(ret1);
		       return 1;
	       }
	       if(strcmp(src,"Trash")==0) {
		       ThumbNail *th;
		       char flname[300];
		       char *ret,*ret1;
		       th = kgGetThumbNail(fw,item);
		       sprintf(job,"%-s/%-s",ddir,th->name);
		       if( (ret=kgCheckFileType(job)) != NULL) {
                         if(kgCheckMenu(Tmp,400,400,"Do you want to overwrite",0)){
		            sprintf(job,"bash -c \\mv -f \"%-s/%-s/*\" -t \"%-s/%-s\"\\",sdir,th->name,ddir,th->name);
			    printf("%s\n",job);

			 }
			 else {
		            sprintf(job,"bash -c \\mv -n \"%-s/%-s/*\" -t \"%s/%-s\"\\",sdir,th->name,ddir,th->name);
			 }
		       }
		       else sprintf(job,"mv \\%-s/%-s\\ -t \\%s\\",sdir,th->name,ddir);
		       sprintf(flname,"%-s/%-s",sdir,th->name);
		       ret=kgCheckFileType(flname);
#if 0
		       sprintf(job,"mv %-s/%-s %s",sdir,th->name,ddir);
#endif
                       bs = kgOpenBusy(Tmp,600,250);
	     	       printf("%s\n",job);
#if 1
		       kgRunJob(job,NULL); // need bash since '*'
#else
		       system(job);
#endif
		       kgCloseBusy(bs);
		       sprintf(job,"rm -rf \\%-s/%-s\\",sdir,th->name);
		       kgRunJob(job,NULL);
		       sprintf(job,"%-s/%-s",sdir,th->name);
		       if((ret1=kgCheckFileType(job))== NULL) {
		         th = kgPickThumbNail(fw,item);
		         kgUpdateWidget(fw);
			 if (ret[0]=='F') {
			    if( strcmp(des,"Ybox1")==0) tw=X1;
			    if( strcmp(des,"Ybox2")==0) tw=X2;
			 }
			 else {
			    if( strcmp(des,"Xbox1")==0) tw=Y1;
			    if( strcmp(des,"Xbox2")==0) tw=Y2;
			 }
                         did = kgGetWidgetId(Tmp,tw);
                         des = kgGetWidgetName(Tmp,did);
			 free(ret);
		         kgAddThumbNail(tw,th,0);
			 kgSortList(tw);
			 kgListRemoveDup(tw);
			 CHECKADDSAME;
		         kgUpdateWidget(tw);
		         kgUpdateOn(Tmp);
		       }
		       else free(ret1);
		       return 1;
	       }
//	       printf("%s | %s\n",src,des);
	       if( (strcmp(src,"Xbox1")==0) &&(strcmp(des,"Xbox2")==0)) {
		 if(same) return 1;
		 sprintf(job,"%-s/%-s",Folder2,kgGetThumbNailName(fw,item));
		 if(kgCheckFileType(job)!= NULL) {
                   if(!kgCheckMenu(Tmp,400,400,"Copy Folder;may overwrite",0))return 1;
		 }
                 sprintf(job,"cp -rvf  \\%-s/%-s\\ \\%-s\\",
			 Dir1,kgGetThumbNailName(fw,item),Folder2);
		 sprintf(destloc,"%-s/%-s",
			 Folder2,kgGetThumbNailName(fw,item));
		         goto jump;
		 return 1;
	       }
	       if( (strcmp(src,"Ybox1")==0) &&(strcmp(des,"Ybox2")==0)) {
		 if(same) return 1;
//                 if(kgCheckMenu(Tmp,400,400,"Copy File;may overwrite",0)){ 
		 sprintf(job,"%-s/%-s",Folder2,kgGetThumbNailName(fw,item));
		 if(kgCheckFileType(job)!= NULL) {
                   if(!kgCheckMenu(Tmp,400,400,"Copy File;may overwrite",0))return 1;
		 }
                 sprintf(job,"cp -rfv \\%-s/%-s\\ \\%-s\\",
			 Dir1,kgGetThumbNailName(fw,item),Folder2);
		 sprintf(destloc,"%-s/%-s",
			 Folder2,kgGetThumbNailName(fw,item));
		         goto jump;
		 return 1;
	       }
	       if( (strcmp(src,"Xbox2")==0) &&(strcmp(des,"Xbox1")==0)) {
   	         void *bs;
		 if(same) return 1;
		 sprintf(job,"%-s/%-s",Folder1,kgGetThumbNailName(fw,item));
		 if(kgCheckFileType(job)!= NULL) {
                   if(!kgCheckMenu(Tmp,400,400,"Copy Folder;may overwrite",0))return 1;
		 }
                 sprintf(job,"cp -rfv \\%-s/%-s\\ \\%-s\\",
			 Dir2,kgGetThumbNailName(fw,item),Folder1);
		 sprintf(destloc,"%-s/%-s",
			 Folder1,kgGetThumbNailName(fw,item));
		         goto jump;
		 return 1;
	       }
	       if( (strcmp(src,"Ybox2")==0) &&(strcmp(des,"Ybox1")==0)) {
		 if(same) return 1;
		 sprintf(job,"%-s/%-s",Folder1,kgGetThumbNailName(fw,item));
		 if(kgCheckFileType(job)!= NULL) {
                   if(!kgCheckMenu(Tmp,400,400,"Copy File;may overwrite",0))return 1;
		 }
                 sprintf(job,"cp -rfv \\%-s/%-s\\ \\%-s\\",
			 Dir2,kgGetThumbNailName(fw,item),Folder1);
		 sprintf(destloc,"%-s/%-s",
			 Folder1,kgGetThumbNailName(fw,item));
		         goto jump;
		 return 1;
	       }
	       return 1;
     jump:
#if 0
               printf("%s\n",job);
#endif
//               bs = kgOpenBusy(Tmp,600,250);
               kgRunJob(job,ProcessData);
//	       kgCloseBusy(bs);
	       if(kgCheckFileType(destloc)==NULL) return 1;
	       kgAddThumbNail(tw,kgCopyThumbNail(kgGetThumbNail(fw,item)),0);
	       kgSortList(tw);
	       kgListRemoveDup(tw);
	       kgUpdateWidget(tw);
	       kgUpdateWidget(fw);
	       kgUpdateOn(Tmp);
        //       Update1(Tmp);
     }
     return 1;
  }
  else return 0;
}
#else
#endif
static int CopyItems(void *Tmp,void *fw) {
  DIY *FY,*TY;
  int item=0,i=0;
  ThumbNail **th;
  FY = (DIY *)fw;
  int x=-1,y=-1;
  int Pip1[2],Pip2[2],*Pips=NULL;
  void *tw=NULL;
  char *src,*des;
  char job[500],Msg[200];
  char Dir1[200]="",Dir2[200]="";
  char destloc[500];
  char *sdir,*ddir;;
  char *fret=NULL;
  int sid,did;
  int same=0;
  void *bs;
  if(!kgCheckMenu(Tmp,400,400,"Copy Selected",0)) return 0;
  if(strcmp(Folder1,"/") !=0) strcpy(Dir1,Folder1);
  if(strcmp(Folder2,"/") !=0) strcpy(Dir2,Folder2);
  sid = kgGetWidgetId(Tmp,fw);
  src = kgGetWidgetName(Tmp,sid);
  same =0;
  if( (strcmp(src,"Ybox1")==0)){ sdir=Folder1;ddir=Folder2;tw= kgGetNamedWidget(Tmp,"Ybox2");}
  if( (strcmp(src,"Ybox2")==0)) { sdir=Folder2; ddir=Folder1;tw= kgGetNamedWidget(Tmp,"Ybox1");}
  if(strcmp(Folder1,Folder2)==0) same=1;
  if(same) return 0;
  th = (ThumbNail **)kgGetList(fw);
  bs = kgOpenBusy(Tmp,700,250);
  if(th != NULL) {
    i =0;
#if 0
    if( pipe(Pip1) < 0) return 0;
    if( pipe(Pip2) < 0) return 0;
    if(fork()==0) {
            close(Pip2[0]);
            close(Pip1[1]);
	    MonitorProgress(Pip1[0],Pip2[1],0);
            close(Pip2[1]);
            close(Pip1[0]);
	    exit(0);
    }
    close(Pip2[1]);
    close(Pip1[0]);
#endif
    Pips=    MonitorProgress(NULL,0);
    if(Pips==NULL) return 0;
    while(th[i]!= NULL) {
      if(kgGetSwitch(fw,i)) {
        sprintf(job,"cp \\%-s/%-s\\ \\%-s\\",sdir,th[i]->name,ddir);
	sprintf(Msg,"%-s\n",th[i]->name);
	write(Pips[1],Msg,strlen(Msg));
        kgRunJob(job,NULL);
	sprintf(destloc,"%-s/%-s",ddir,th[i]->name);
        if(kgCheckFileType(destloc)!=NULL)  {
          kgAddThumbNail(tw,kgCopyThumbNail(th[i]),0);
	}
      }
      if(kgGetLine(Pips[0],Msg)) break;
      i++;
    }
    CloseMonitor(Pips);
  }
  kgSortList(tw);
  kgListRemoveDup(tw);
  kgCloseBusy(bs);
  kgUpdateWidget(tw);
  kgUpdateOn(Tmp);
  return 1;
}
static int MoveItems(void *Tmp,void *fw) {
  DIY *FY,*TY;
  int item=0,i=0;
  ThumbNail **th,*thitem=NULL;
  FY = (DIY *)fw;
  int *Pips=NULL;
  int x=-1,y=-1;
  void *tw=NULL;
  char *src,*des;
  char job[500];
  char Dir1[200]="",Dir2[200]="";
  char destloc[500],Msg[200];
  char *sdir,*ddir;;
  char *fret=NULL;
  int sid,did;
  int same=0;
  void *bs;
  if(!kgCheckMenu(Tmp,400,400,"Move Selected",0)) return 0;
  if(strcmp(Folder1,"/") !=0) strcpy(Dir1,Folder1);
  if(strcmp(Folder2,"/") !=0) strcpy(Dir2,Folder2);
  sid = kgGetWidgetId(Tmp,fw);
  src = kgGetWidgetName(Tmp,sid);
  same =0;
  if( (strcmp(src,"Ybox1")==0)){ sdir=Folder1;ddir=Folder2;tw= kgGetNamedWidget(Tmp,"Ybox2");}
  if( (strcmp(src,"Ybox2")==0)) { sdir=Folder2; ddir=Folder1;tw= kgGetNamedWidget(Tmp,"Ybox1");}
  if(strcmp(Folder1,Folder2)==0) same=1;
  if(same) return 0;
  th = (ThumbNail **)kgGetList(fw);
  bs = kgOpenBusy(Tmp,700,250);
  if(th != NULL) {
    Pips = MonitorProgress(NULL,0);
    if(Pips == NULL) return 0;
    i =0;
    while(th[i]!= NULL) {
      if(kgGetSwitch(fw,i)) {
        sprintf(job,"mv \\%-s/%-s\\ \\%-s\\",sdir,th[i]->name,ddir);
	sprintf(Msg,"%-s\n",th[i]->name);
	write(Pips[1],Msg,strlen(Msg));
        kgRunJob(job,NULL);
	sprintf(destloc,"%-s/%-s",ddir,th[i]->name);
        if(kgCheckFileType(destloc)!=NULL)  {
	  thitem = kgPickThumbNail(fw,i);
          kgAddThumbNail(tw,thitem,0);
	  th = (ThumbNail **)kgGetList(fw);
	  continue;
	}
      }
      if(kgGetLine(Pips[0],Msg)) break;
      i++;
    }
    CloseMonitor(Pips);
  }
  kgSortList(tw);
  kgListRemoveDup(tw);
  kgCloseBusy(bs);
  kgUpdateWidget(tw);
  kgUpdateWidget(fw);
  kgUpdateOn(Tmp);
  return 1;
}
static int RemoveItems(void *Tmp,void *fw) {
  DIY *FY,*TY,*ow=NULL;
  int item=0,i=0;
  ThumbNail **th,*thitem=NULL;
  FY = (DIY *)fw;
  int x=-1,y=-1;
  void *tw=NULL;
  char *src,*des;
  char job[500];
  char Dir1[200]="",Dir2[200]="";
  char destloc[500];
  char *sdir,*ddir;;
  char *fret=NULL;
  int sid,did;
  int same=0;
  void *bs;
  if(!kgCheckMenu(Tmp,400,400,"Remove Selected",0)) return 0;
  if(strcmp(Folder1,"/") !=0) strcpy(Dir1,Folder1);
  if(strcmp(Folder2,"/") !=0) strcpy(Dir2,Folder2);
  sid = kgGetWidgetId(Tmp,fw);
  src = kgGetWidgetName(Tmp,sid);
  same =0;
  if( (strcmp(src,"Ybox1")==0)){ 
	  sdir=Folder1;ddir=Trash;
	  tw= kgGetNamedWidget(Tmp,"Trash");
	  ow = kgGetNamedWidget(Tmp,"Ybox2");
  }
  if( (strcmp(src,"Ybox2")==0)) {
	  sdir=Folder2; 
	  ddir=Trash;
	  tw= kgGetNamedWidget(Tmp,"Trash");
	  ow = kgGetNamedWidget(Tmp,"Ybox1");
  }
  if(strcmp(Folder1,Folder2)==0) {
	  same=1;
  }
  bs = kgOpenBusy(Tmp,700,250);
  th = (ThumbNail **)kgGetList(fw);
  if(th != NULL) {
    i =0;
    while(th[i]!= NULL) {
      if(kgGetSwitch(fw,i)) {
        sprintf(job,"mv \\%-s/%-s\\ \\%-s\\",sdir,th[i]->name,ddir);
        kgRunJob(job,NULL);
	sprintf(destloc,"%-s/%-s",ddir,th[i]->name);
        if(kgCheckFileType(destloc)!=NULL)  {
	  thitem = kgPickThumbNail(fw,i);
          kgAddThumbNail(tw,thitem,0);
	  th = (ThumbNail **)kgGetList(fw);
	  if(same) kgDeleteThumbNail(ow,i);
	  continue;
	}
      }
      i++;
    }
  }
  kgSortList(tw);
  kgListRemoveDup(tw);
  kgCloseBusy(bs);
  kgUpdateWidget(tw);
  kgUpdateWidget(fw);
  if(same) kgUpdateWidget(ow);
  kgUpdateOn(Tmp);
  return 1;
}
int SetThumbNailImages(ThumbNail **th,int size) {
	int i=0;
	void *img;
	if(th==NULL) return 0;
        img = kgHomeImage(size,160,180,160);
	while(th[i] != NULL) {
		th[i]->img = kgCopyImage(img);
		i++;
	}
	return 1;
}
static ThumbNail *MakeThumbNail(char *name,int size) {
	ThumbNail *Th=NULL;
	void *imgo=NULL,*img=NULL;
	Th = (ThumbNail *) malloc(sizeof(ThumbNail));
	if(folderimg == NULL) {
          imgo = (void *)kgGetImageCopy(NULL,(void *)&folder_str);
          if(imgo != NULL) {
            folderimg = kgThumbNailImage(imgo,size,size);
            kgFreeImage(imgo);
          }
        }
	Th->img = kgCopyImage(folderimg);
	Th->name = (char *) malloc(sizeof(strlen(name)+1));
	strcpy(Th->name,name);
	Th->sw = 1;
	Th->id=1;
	Th->state = 1;
	return Th;
}

static ThumbNail **GetFolderThumbNails(char *Folder,int size) {
    ThumbNail **th=NULL;
    char fullname[300];
    char *fret;
    int i;
    void *imgo=NULL,*img=NULL;
    th = (ThumbNail **)kgFolderThumbNails(Folder);
    if(th==NULL) return NULL;
    if(folderimg == NULL) {
      imgo = (void *)kgGetImageCopy(NULL,(void *)&folder_str);
      if(imgo != NULL) {
	    folderimg = kgThumbNailImage(imgo,size,size);
	    kgFreeImage(imgo);
      }
    }
    if(linkimg == NULL) {
      imgo = (void *)kgGetImageCopy(NULL,(void *)&link_str);
      if(imgo != NULL) {
	    linkimg = kgThumbNailImage(imgo,size,size);
	    kgFreeImage(imgo);
      }
    }
    i=0;
    while(th[i] != NULL ) {
	    sprintf(fullname,"%-s/%-s",Folder,th[i]->name);
            fret = (char *)kgCheckFileType(fullname);
	    switch (fret[0]) {
	        case 'L':
		  if(linkimg == NULL) 
      		       linkimg = kgGetImageCopy(NULL,(void *)&link_str);
	          th[i]->img = kgThumbNailImage(linkimg,size,size);
		  break;
		default:
	          th[i]->img = kgCopyImage(folderimg);
		  break;
	    }
	    i++;
    }
    return th;
}


static ThumbNail **GetFileThumbNails(char *Folder,int size) {
    ThumbNail **th=NULL;
    char fullname[300];
    char *fret;
    int i;
    void *imgo=NULL,*img=NULL;
    th = (ThumbNail **)kgFileThumbNails(Folder,"*");
    if(th==NULL) return NULL;
    if(fileimg == NULL) {
      imgo = (void *)kgGetImageCopy(NULL,(void *)&file_str);
      if(imgo != NULL) {
	    fileimg = kgThumbNailImage(imgo,size,size);
	    kgFreeImage(imgo);
      }
    }
    if(linkimg == NULL) {
      imgo = (void *)kgGetImageCopy(NULL,(void *)&link_str);
      if(imgo != NULL) {
	    linkimg = kgThumbNailImage(imgo,size,size);
	    kgFreeImage(imgo);
      }
    }
    i=0;
    while(th[i] != NULL ) {
	    sprintf(fullname,"%-s/%-s",Folder,th[i]->name);
            fret = (char *)kgCheckFileType(fullname);
	    if(fret != NULL) {
	      switch (fret[0]) {
	        case 'I':
		  imgo = kgGetImage(fullname);
                  if(imgo != NULL) {
	            th[i]->img = kgThumbNailImage(imgo,size,size);
		    kgFreeImage(imgo);
		  }
		  else th[i]->img = kgCopyImage(fileimg);
		  break;
	        case 'L':
		  if(linkimg == NULL) 
      		       linkimg = kgGetImageCopy(NULL,(void *)&link_str);
	          th[i]->img = kgThumbNailImage(linkimg,size,size);
		  break;
	        case 'M':
		  if(musicimg == NULL) 
      		       musicimg = kgGetImageCopy(NULL,(void *)&music_str);
	          th[i]->img = kgThumbNailImage(musicimg,size,size);
		  break;
	        case 'V':
		  if(videoimg == NULL) 
      		       videoimg = kgGetImageCopy(NULL,(void *)&video_str);
	          th[i]->img = kgThumbNailImage(videoimg,size,size);
		  break;
	        case 'O':
		  if(binaryimg == NULL) 
      		       binaryimg = kgGetImageCopy(NULL,(void *)&binary_str);
	          th[i]->img = kgThumbNailImage(binaryimg,size,size);
		  break;
	        case 'E':
		  if(exeimg == NULL) 
      		       exeimg = kgGetImageCopy(NULL,(void *)&exec_str);
	          th[i]->img = kgThumbNailImage(exeimg,size,size);
		  break;
	        case 'G':
		  if(gzipimg == NULL) 
      		       gzipimg = kgGetImageCopy(NULL,(void *)&gzip_str);
	          th[i]->img = kgThumbNailImage(gzipimg,size,size);
		  break;
	        case 'T':
		  if(tarimg == NULL) 
      		       tarimg = kgGetImageCopy(NULL,(void *)&tar2_str);
	          th[i]->img = kgThumbNailImage(tarimg,size,size);
		  break;
	        case 'C':
		  if(cimg == NULL) 
      		       cimg = kgGetImageCopy(NULL,(void *)&source_c_str);
	          th[i]->img = kgThumbNailImage(cimg,size,size);
		  break;
	        default:
	          th[i]->img = kgCopyImage(fileimg);
		break;
	      }
	    }
	    else th[i]->img = kgCopyImage(fileimg);
	    i++;
    }
    return th;
}


int  kgfilebrowserbrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIY *X;void *pt; 
  int ret=1; 
  ThumbNail *th;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIY *)kgGetWidget(Tmp,i);
  DIT *wid;
  int x,y;
  if(DragItem(Tmp,X1,item-1)) return 1;
  else {
    th = (ThumbNail *) kgPickThumbNail(X,item-1);
    if(strcmp(Folder1,"/") != 0) strcat(Folder1,"/");
    strcat(Folder1,th->name);
    Update1(Tmp);
  }
  return ret;
}
void  kgfilebrowserbrowser1init(DIY *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
    void **th;

    X1=X;
    if(Home1[0]=='\0') getcwd(Home1,299);
#if 0
//    strcpy(Home1,getenv("HOME"));
    th = (void **)kgFolderThumbNails(Home1);
    SetThumbNailImages((ThumbNail **)th,20);
#else
    th = (void **)GetFolderThumbNails(Home1,20);
#endif
    kgSetList(X1,th);
    strcpy(Folder1,Home1);

}
int  kgfilebrowserbrowser2callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  int x,y;
  DIT *wid;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
#if 0
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
     wid = (DIT *)kgGetLocationWidget(Tmp,x,y);
     if(wid != NULL) {
	     if(kgCheckWidgetName(wid,"Ybox2")) {
               char job[400];
               sprintf(job,"cp \\%-s/%-s\\ \\%-s\\",Folder1,kgGetThumbNailName(Y,item-1),Folder2);
               printf("%s\n",job);
               kgRunJob(job,NULL);
	       kgAddThumbNail(Y2,kgCopyThumbNail(kgGetThumbNail(Y,item-1)),0);
	       kgSortList(Y2);
	       kgUpdateWidget(Y2);
	       kgUpdateOn(Tmp);
//               Update2(Tmp);
	     }
     }
  }
#else
  if(DragItem(Tmp,Y1,item-1)) return 1;
  else {
	  return 1;
  }
#endif
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbrowser2init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
    void **list;
    Y1 = Y;
//    strcpy(Home1,getenv("HOME"));
    if(Home1[0]=='\0') getcwd(Home1,299);
    strcpy(Folder1,Home1);
    list = (void **)GetFileThumbNails(Home1,48);
    kgSetList(Y1,list);
}
int  kgfilebrowsersplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowsersplbutton1init(DIL *B,void *pt) {
}
int  kgfilebrowserbrowser3callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIY *X;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  ThumbNail *th;
  pt = D->pt;
  X = (DIY *)kgGetWidget(Tmp,i);
  if(DragItem(Tmp,X2,item-1)) return 1;
  else {
    th = (ThumbNail *) kgPickThumbNail(X,item-1);
    if(strcmp(Folder2,"/") != 0) strcat(Folder2,"/");
    strcat(Folder2,th->name);
    Update2(Tmp);
  }
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbrowser3init(DIY *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
    void **th;
    X2 = X;
    strcpy(Home2,getenv("HOME"));
//    kgSetList(X2,(void **)kgFolderThumbNails(Home2));
//    th = (void **)kgFolderThumbNails(Home2);
    th = (void **)GetFolderThumbNails(Home2,20);
    kgSetList(X2,th);
    strcpy(Folder2,Home2);
}
int  kgfilebrowserbrowser4callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  DIT *wid;
  int x,y;
#if 0
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
     wid = (DIT *)kgGetLocationWidget(Tmp,x,y);
     if(wid != NULL) {
	     if(kgCheckWidgetName(wid,"Ybox1")) {
               char job[400];
               sprintf(job,"cp %-s/%-s %-s",Folder2,kgGetThumbNailName(Y,item-1),Folder1);
               printf("%s\n",job);
               system(job);
	       kgAddThumbNail(Y1,kgCopyThumbNail(kgGetThumbNail(Y,item-1)),0);
	       kgSortList(Y1);
	       kgUpdateWidget(Y1);
	       kgUpdateOn(Tmp);
//               Update1(Tmp);
	     }
     }
  }
#else
  if(DragItem(Tmp,Y2,item-1)) return 1;
  else {
	  return 1;
  }
#endif
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbrowser4init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
    void **list;
    Y2 = Y;
    strcpy(Home2,getenv("HOME"));
    strcpy(Folder2,Home2);
    list = (void **)GetFileThumbNails(Home2,48);
    kgSetList(Y2,list);
}
int  kgfilebrowserbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **th;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
	    strcpy(Folder1,Home1);
	    Update1(Tmp);
      break;
    case 2: 
      if(strcmp(Folder1,"/") != 0) {
	    char *pt,*pt1=NULL;
	    pt = Folder1+1;
	    while((pt = strstr(pt,"/"))!=NULL) {pt1=pt; pt++;}
	    if(pt1!= NULL) *pt1='\0';
	    else strcpy(Folder1,"/");
	    Update1(Tmp);
      }
      break;
  }
  return ret;
}
void  kgfilebrowserbutton1init(DIN *B,void *pt) {
}
int  kgfilebrowsertextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  char *folder;
  char *fret=NULL;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  folder = (char *)kgGetString(T,cellno);
  fret= kgCheckFileType(folder);
  if((fret==NULL)||(fret[0]!='F')) {
	  kgSetString(T,cellno,Folder1);
	  kgUpdateWidget(T);
	  kgUpdateOn(Tmp);
  }
  else {
      strcpy(Folder1,folder);
      Update1(Tmp);
  }
  kgFree(fret);
  return ret;
}
int  kgfilebrowserbutton2callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  void **th=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
	    strcpy(Folder2,Home2);
	    Update2(Tmp);
      break;
    case 2: 
      if(strcmp(Folder2,"/") != 0) {
	    char *pt,*pt1=NULL;
	    pt = Folder2+1;
	    while((pt = strstr(pt,"/"))!=NULL) {pt1=pt; pt++;}
	    if(pt1!= NULL) *pt1='\0';
	    else strcpy(Folder2,"/");
	    Update2(Tmp);
      }
      break;
  }
  return ret;
}
void  kgfilebrowserbutton2init(DIN *B,void *pt) {
}
int  kgfilebrowsertextbox2callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  char *folder;
  char *fret=NULL;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  folder = (char *)kgGetString(T,cellno);
  fret= kgCheckFileType(folder);
  if((fret==NULL)||(fret[0]!='F')) {
	  kgSetString(T,cellno,Folder2);
	  kgUpdateWidget(T);
	  kgUpdateOn(Tmp);
  }
  else {
      strcpy(Folder2,folder);
      Update2(Tmp);
  }
  kgFree(fret);
  return ret;
}
int kgfilebrowserinit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  Dtmp = Tmp;
  pt = D->pt;
  T1 = (DIT *)kgGetNamedWidget(Tmp,"Tbox1");
  T2 = (DIT *)kgGetNamedWidget(Tmp,"Tbox2");
  strcpy(Filter,"*");
#if 1
//  strcpy(Home1,getenv("HOME"));
  if(Home1[0]=='\0') getcwd(Home1,299);
  strcpy(Folder1,Home1);
  if(Home2[0]=='\0') strcpy(Home2,getenv("HOME"));
  strcpy(Folder2,Home2);
#endif
//  printf("%s %s\n",Home1,Home2);
  X1 = (DIY *)kgGetNamedWidget(Tmp,"Xbox1");
  X2 = (DIY *)kgGetNamedWidget(Tmp,"Xbox2");
  Y1 = (DIY *)kgGetNamedWidget(Tmp,"Ybox1");
  Y2 = (DIY *)kgGetNamedWidget(Tmp,"Ybox2");
  TR = (DIY *)kgGetNamedWidget(Tmp,"Trash");
//  kgSetList(X1,(void **)kgFolderThumbNails(Home1));
//  kgSetList(X2,(void **)kgFolderThumbNails(Home2));
//  kgSetList(Y1,(void **)kgFileThumbNails(Home1,"*"));
//  kgSetList(Y2,(void **)kgFileThumbNails(Home2,"*"));
  kgSetString(T1,0,Home1);
  kgSetString(T2,0,Home2);
  kgUpdateWidget(T1);
  kgUpdateWidget(T2);
//  kgUpdateOn(Tmp);
  return ret;
}
int  kgfilebrowserbrowser5callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  DragItem(Tmp,Y,item-1);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbrowser5init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
  char buf[300];
#if 0
  sprintf(buf,"%-s/.Trash",getenv("HOME"));
  mkdir(buf,0700);
#else
  sprintf(buf,"/tmp");
#endif
  sprintf(Trash,"%-s/%-d",buf,getpid());
  mkdir(Trash,0700);
}
int  kgfilebrowserbutton3callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  DIY *Y;
  void **list;
  void *bs=NULL;
  char job[200];
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
//  printf("Removing Trash %s\n",Trash);
//  rmdir(Trash);
  sprintf(job,"rm -rf %s",Trash);
//  printf("JOB: %s\n",job);
  bs = kgOpenBusy(Tmp,600,250);
  kgRunJob(job,NULL);
  kgCloseBusy(bs);
  mkdir(Trash,0755);
  list = kgGetList(TR);
  if(list != NULL) kgFreeThumbNails((ThumbNail **)list);
  kgSetList(TR,NULL);
  kgUpdateWidget(TR);
  kgUpdateOn(Tmp);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbutton3init(DIN *B,void *pt) {
}
int kgfilebrowsercleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  char job[200];
  void *bs=NULL;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  sprintf(job,"rm -rf %s",Trash);
  kgRunJob(job,NULL);
  return ret;
}
int Modifykgfilebrowser(void *Tmp,int GrpId) {
  DIALOG *D;
  D = (DIALOG *)Tmp;
  DIA *d;
  int i,n;
  d = D->d;
  i=0;while(d[i].t!= NULL) {;
     i++;
  };
  n=1;
  return GrpId;
}

int kgfilebrowserCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  void *wid;
  DIT *dest;
  int x,y,xl,yl,xpos,ypos;
  int same=0;
  char from[300],to[300],fname[300];
  char job[500];
  int rval=-1;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  yl = D->yl;
  xl = D->xl;
  if(strcmp(Folder1,Folder2)==0) same=1;


#if 1
  if((kbe->event ==1)||(kbe->event == 3)) {
    ThumbNail *Th=NULL;
    ThumbNail **List=NULL;
    if(kbe->button !=1) {
	  int item=-1;
	  wid = kgGetClickedWidget(Tmp);

	  if(kgCheckWidgetName(wid,"Xbox1")) {
		  ProcessOtherButtonClick(X1,FMenu1,FMenu1);
		  switch(rval) {
			  case 3:
			    if(Th != NULL) {
			      ProcessCreateLink(X1,Folder1,Folder2);
		            }
			    break;
			  case 1:
			    ProcessRename(Folder1,X1);
			    break;
			  case 2:
			    ProcessBackup(Folder1,X1);
			    break;
			  case 4:
			    ProcessInfo(Folder1);
			    break;
			  default:
			    break;
		  }
	  }
	  if(kgCheckWidgetName(wid,"Ybox1")) {
	          ProcessOtherButtonClick(Y1,FMenu1,FMenu1);
		  switch(rval) {
			  case 1:
			    ProcessRename(Folder1,Y1);
			    break;
			  case 2:
			    ProcessBackup(Folder1,Y1);
			    break;
			  case 3:
			    if(Th != NULL) {
			      ProcessCreateLink(Y1,Folder1,Folder2);
		            }
			    break;
			  case 4:
			    ProcessInfo(Folder1);
			    break;
			  default:
			    break;
		  }

	  }
	  if(kgCheckWidgetName(wid,"Xbox2")) {
		  ProcessOtherButtonClick(X2,FMenu1,FMenu1);
		  switch(rval) {
			  case 3:
			    if(Th != NULL) {
			      ProcessCreateLink(X2,Folder2,Folder1);
		            }
			    break;
			  case 1:
			    ProcessRename(Folder2,X2);
			    break;
			  case 2:
			    ProcessBackup(Folder2,X2);
			    break;
			  case 4:
			    ProcessInfo(Folder2);
			    break;
			  default:
			    break;
		  }
	  }
	  if(kgCheckWidgetName(wid,"Ybox2")) {
	          ProcessOtherButtonClick(Y2,FMenu1,FMenu1);
		  switch(rval) {
			  case 1:
			    ProcessRename(Folder2,Y2);
			    break;
			  case 2:
			    ProcessBackup(Folder2,Y2);
			    break;
			  case 3:
			    if(Th != NULL) {
			      ProcessCreateLink(Y2,Folder2,Folder1);
		            }
			    break;
			  case 4:
			    ProcessInfo(Folder2);
			    break;
			  default:
			    break;
		  }
	  }
    }
  }
#endif
  return ret;
}
int kgfilebrowserResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
int kgfilebrowserWaitCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    Called while waiting for event  
    return value 1 will close the the UI  
   ***********************************/ 
  int ret = 0;
  return ret;
}
int  kgfilebrowserbutton4callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  int same =0;
  char from[300],to[300],fname[300];
  char job[500];
  ThumbNail *Th=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  if(strcmp(Folder1,Folder2)==0) same=1;
  ProcessMakeFolder(Folder1,X1);

  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbutton4init(DIN *B,void *pt) {
}
int  kgfilebrowserbutton5callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  int same =0;
  char from[300],to[300],fname[300];
  char job[500];
  ThumbNail *Th=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  if(strcmp(Folder1,Folder2)==0) same=1;
  ProcessMakeFolder(Folder2,X2);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  kgfilebrowserbutton5init(DIN *B,void *pt) {
}
int  kgfilebrowserbutton6callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,j=0; 
  DIY *Y=NULL;
  char Prompt[200];
  ThumbNail **th=NULL;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Y = (DIY *)kgGetNamedWidget(Tmp,"Ybox1");
  switch(butno) {
    case 1: 
	    th= (ThumbNail **)kgGetList(Y);
	    j=0;
	    strcpy(Prompt,"Simple Files Filter %20s");
	    gscanf(Tmp,Prompt,Filter);
	    while(th[j]!= NULL) {
		    if(kgFilterString(th[j]->name,Filter))
    		       kgSetSwitch(Y,j,1);
		    j++;
	    };
	    strcpy(Filter,"*");
	    kgUpdateWidget(Y);
	    kgUpdateOn(Tmp);
      break;
    case 2: 
	    th= (ThumbNail **)kgGetList(Y);
	    j=0;
	    while(th[j]!= NULL) { kgSetSwitch(Y,j,0);j++;};
	    kgUpdateWidget(Y);
	    kgUpdateOn(Tmp);
      break;
    case 3: 
            CopyItems(Tmp,Y);
      break;
    case 4: 
            MoveItems(Tmp,Y);
      break;
    case 5: 
            RemoveItems(Tmp,Y);
      break;
  }
  return ret;
}
void  kgfilebrowserbutton6init(DIN *B,void *pt) {
}
int  kgfilebrowserbutton7callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,j=0; 
  ThumbNail **th=NULL;
  DIY *Y=NULL;
  char Prompt[200];
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Y = (DIY *)kgGetNamedWidget(Tmp,"Ybox2");
  switch(butno) {
    case 1: 
	    th= (ThumbNail **)kgGetList(Y);
	    j=0;
	    strcpy(Prompt,"Simple Files Filter %20s");
	    gscanf(Tmp,Prompt,Filter);
	    while(th[j]!= NULL) {
		    if(kgFilterString(th[j]->name,Filter))
    		       kgSetSwitch(Y,j,1);
		    j++;
	    };
	    strcpy(Filter,"*");
	    kgUpdateWidget(Y);
	    kgUpdateOn(Tmp);
      break;
    case 2: 
	    th= (ThumbNail **)kgGetList(Y);
	    j=0;
	    while(th[j]!= NULL) { kgSetSwitch(Y,j,0);j++;};
	    kgUpdateWidget(Y);
	    kgUpdateOn(Tmp);
      break;
    case 3: 
            CopyItems(Tmp,Y);
      break;
    case 4: 
            MoveItems(Tmp,Y);
      break;
    case 5: 
            RemoveItems(Tmp,Y);
      break;
  }
  return ret;
}
void  kgfilebrowserbutton7init(DIN *B,void *pt) {
}
int  kgfilebrowsertextbox3callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
  e = T->elmt;
  return ret;
}
