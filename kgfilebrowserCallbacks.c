#include <kulina.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "icons.c"


static char Folder1[300],Home1[300]="";
static char Folder2[300],Home2[300]="";
static char Trash[300];
static void *folderimg=NULL;
static void *fileimg=NULL,*musicimg=NULL,*videoimg=NULL,
	    *cimg=NULL,*tarimg=NULL,
	    *gzipimg=NULL,*exeimg=NULL,*binaryimg=NULL;

static DIT *T1=NULL,*T2=NULL;
static DIY *X1=NULL,*X2=NULL;
static DIY *Y1=NULL,*Y2=NULL;
static DIY *TR=NULL;
int SetThumbNailImages(ThumbNail **th,int size);
char *kgCheckFileType(char *name);
static ThumbNail **GetFolderThumbNails(char *Folder,int size);
static ThumbNail **GetFileThumbNails(char *Folder,int size);
int RunkgPopUpMenu(void *arg,int xo,int yo,char **menu);

char *FMenu1[8]={"Copy selected files","Move selected files","Selected files to Trash","Delete selected files","Rename this","Backup this","Close",NULL};
char *FMenu2[8]={"Copy selected files","Move selected files","Selected files to Trash","Delete selected files","Close",NULL};
char *DMenu1[5]= {"Backup","Rename","Make new folder","Close",NULL};
char *DMenu2[5]= {"Make new folder","Close",NULL};
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
	char wrk[500];
	char *pt;
	char *ret=NULL;
	sprintf(wrk,"file %s",name);
	printf("%s\n",wrk);
	pp = popen(wrk,"r");
	while (fgets(wrk,499,pp) != NULL) {
	        printf("%s\n",wrk);
		pt = strstr(wrk,"cannot open");
		if(pt != NULL) return NULL;
		ret = MakeFileToken(wrk,"executable","Exe");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," image data","Image");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," MPEG","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," MP4","Video");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," WAVE","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," Stereo","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," stereo","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," audio","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," Audio","Music");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk," directory","Folder");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"gzip compressed","Gzip");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"tar archive","Tar");
		if (ret!= NULL) return ret;
		ret = MakeFileToken(wrk,"C source","Csource");
		if (ret!= NULL) return ret;
		ret = malloc(strlen("Unknown")+1);
		strcpy(ret,"Unknown");
		return ret;
	}
}
	       
static void Update1(void *Tmp) {
	    void **th;
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
	    kgSetList(Y1,th);
	    kgUpdateWidget(X1);
	    kgUpdateWidget(Y1);
	    kgUpdateWidget(T1);
	    kgUpdateOn(Tmp);
}
static void Update2(void *Tmp) {
	    void **th;
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
  int sid,did;
  int same=0;
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
	       if(sid == did ) return 0;
               des = kgGetWidgetName(Tmp,did);
               if( (strcmp(des,"Xbox1")==0)||(strcmp(des,"Ybox1")==0)) ddir=Folder1;
               if( (strcmp(des,"Xbox2")==0)||(strcmp(des,"Ybox2")==0)) ddir=Folder2;
               if( ((strcmp(des,"Trash")==0))) ddir=Trash;
	       if(strcmp(des,"Imagebox")==0) {
		       ThumbNail *th;
		       char *ret1;
		       th = kgGetThumbNail(fw,item);
		       sprintf(job,"%-s/%-s",sdir,th->name);
		       remove(job);
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
		       sprintf(job,"mv %-s/%-s %s",sdir,th->name,Trash);
		       system(job);
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
		       sprintf(flname,"%-s/%-s",sdir,th->name);
		       ret=kgCheckFileType(flname);
		       sprintf(job,"mv %-s/%-s %s",sdir,th->name,ddir);
		       system(job);
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
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir1,kgGetThumbNailName(fw,item),Folder2);
		 sprintf(destloc,"%-s/%-s",
			 Folder2,kgGetThumbNailName(fw,item));
		         goto jump;
	       }
	       if( (strcmp(src,"Ybox1")==0) &&(strcmp(des,"Ybox2")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir1,kgGetThumbNailName(fw,item),Folder2);
		 sprintf(destloc,"%-s/%-s",
			 Folder2,kgGetThumbNailName(fw,item));
		         goto jump;
	       }
	       if( (strcmp(src,"Xbox2")==0) &&(strcmp(des,"Xbox1")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir2,kgGetThumbNailName(fw,item),Folder1);
		 sprintf(destloc,"%-s/%-s",
			 Folder1,kgGetThumbNailName(fw,item));
		         goto jump;
	       }
	       if( (strcmp(src,"Ybox2")==0) &&(strcmp(des,"Ybox1")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir2,kgGetThumbNailName(fw,item),Folder1);
		 sprintf(destloc,"%-s/%-s",
			 Folder1,kgGetThumbNailName(fw,item));
		         goto jump;
	       }
	       return 1;
     jump:
#if 0
               printf("%s\n",job);
#endif
               system(job);
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
static int DragItem(void *Tmp,void *fw,int item) {
  DIY *FY,*TY;
  FY = (DIY *)fw;
  int x=-1,y=-1;
  void *tw=NULL;
  char *src,*des;
  char job[500];
  char Dir1[200]="",Dir2[200]="";
  int sid,did;
  int same=0;
  if(strcmp(Folder1,"/") !=0) strcpy(Dir1,Folder1);
  if(strcmp(Folder2,"/") !=0) strcpy(Dir2,Folder2);
  sid = kgGetWidgetId(Tmp,fw);
  src = kgGetWidgetName(Tmp,sid);
  same =0;
  if(strcmp(Folder1,Folder2)==0) same=1;
  if(kgDragThumbNail(FY,item,&x,&y)) {
     tw = (DIT *)kgGetLocationWidget(Tmp,x,y);
     if(tw != NULL) {
               did = kgGetWidgetId(Tmp,tw);
	       if(sid == did ) return 0;
               des = kgGetWidgetName(Tmp,did);
	       if(strcmp(des,"Imagebox")==0) {
		       ThumbNail *th;
		       th = kgPickThumbNail(fw,item);
		       remove(th->name);
		       kgFreeThumbNail(th);
		       kgUpdateWidget(fw);
		       kgUpdateOn(Tmp);
		       return 1;
	       }
//	       printf("%s | %s\n",src,des);
	       if( (strcmp(src,"Xbox1")==0) &&(strcmp(des,"Xbox2")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir1,kgGetThumbNailName(fw,item),Folder2);
		         goto jump;
	       }
	       if( (strcmp(src,"Ybox1")==0) &&(strcmp(des,"Ybox2")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir1,kgGetThumbNailName(fw,item),Folder2);
		         goto jump;
	       }
	       if( (strcmp(src,"Xbox2")==0) &&(strcmp(des,"Xbox1")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir2,kgGetThumbNailName(fw,item),Folder1);
		         goto jump;
	       }
	       if( (strcmp(src,"Ybox2")==0) &&(strcmp(des,"Ybox1")==0)) {
		 if(same) return 1;
                 sprintf(job,"cp -r %-s/%-s %-s",
			 Dir2,kgGetThumbNailName(fw,item),Folder1);
		         goto jump;
	       }
	       if( (strcmp(src,"Xbox1")==0) &&(strcmp(des,"Trash")==0)) {
                 sprintf(job,"mv  %-s/%-s %-s",
			 Dir1,kgGetThumbNailName(fw,item),Trash);
		         goto jump;
	       }
	       if( (strcmp(src,"Ybox1")==0) &&(strcmp(des,"Trash")==0)) {
                 sprintf(job,"mv %-s/%-s %-s",
			 Dir1,kgGetThumbNailName(fw,item),Trash);
		         goto jump;
	       }
	       if( (strcmp(src,"Xbox2")==0) &&(strcmp(des,"Trash")==0)) {
                  sprintf(job,"mv %-s/%-s %-s",
			 Dir2,kgGetThumbNailName(fw,item),Trash);
		         goto jump;
	       }
	       if( (strcmp(src,"Ybox2")==0) &&(strcmp(des,"Trash")==0)) {
                  sprintf(job,"mv %-s/%-s %-s",
			 Dir2,kgGetThumbNailName(fw,item),Trash);
		         goto jump;
	       }
	       if( (strcmp(src,"Trash")==0) &&(strcmp(des,"Xbox1")==0)) {
                 sprintf(job,"mv  %-s/%-s %-s",
			 Trash,kgGetThumbNailName(fw,item),Folder1);
		         goto jump;
	       }
	       if( (strcmp(src,"Trash")==0) &&(strcmp(des,"Xbox2")==0)) {
                 sprintf(job,"mv  %-s/%-s %-s",
			 Trash,kgGetThumbNailName(fw,item),Folder2);
		         goto jump;
	       }
	       if( (strcmp(src,"Trash")==0) &&(strcmp(des,"Ybox1")==0)) {
                 sprintf(job,"mv  %-s/%-s %-s",
			 Trash,kgGetThumbNailName(fw,item),Folder1);
		         goto jump;
	       }
	       if( (strcmp(src,"Trash")==0) &&(strcmp(des,"Ybox2")==0)) {
                 sprintf(job,"mv  %-s/%-s %-s",
			 Trash,kgGetThumbNailName(fw,item),Folder2);
		         goto jump;
	       }
	       return 1;
     jump:
#if 0
               printf("%s\n",job);
#endif
               system(job);
	       kgAddThumbNail(tw,kgCopyThumbNail(kgGetThumbNail(fw,item)),0);
	       if((strcmp(src,"Trash")==0)||(strcmp(des,"Trash")==0) ) {
		       kgDeleteThumbNail(fw,item);
	       }
	       if((strcmp(src,"Trash")==0)&&((strcmp(des,"Ybox1")==0)||(strcmp(des,"Xbox1")==0)))  {
		       Update1(Tmp);
		       if(same) Update2(Tmp);
	       }
	       if((strcmp(src,"Trash")==0)&&((strcmp(des,"Ybox2")==0)||(strcmp(des,"Xbox2")==0)))  {
		       Update2(Tmp);
		       if(same) Update1(Tmp);
	       }
	       if((strcmp(des,"Trash")==0)&&((strcmp(src,"Ybox1")==0)||(strcmp(src,"Xbox1")==0)))  {
		       if(same) Update2(Tmp);
	       }
	       if((strcmp(des,"Trash")==0)&&((strcmp(src,"Ybox2")==0)||(strcmp(src,"Xbox2")==0)))  {
		       if(same) Update1(Tmp);
	       }
	       kgSortList(tw);
	       kgListRemoveDup(tw);
	       kgUpdateWidget(tw);
	       kgUpdateWidget(fw);
	       kgUpdateOn(Tmp);
               Update1(Tmp);
     }
     return 1;
  }
  else return 0;
}
#endif
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

static ThumbNail **GetFolderThumbNails(char *Folder,int size) {
    ThumbNail **th=NULL;
    char fullname[300];
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
    i=0;
    while(th[i] != NULL ) {
	    th[i]->img = kgCopyImage(folderimg);
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
               sprintf(job,"cp %-s/%-s %-s",Folder1,kgGetThumbNailName(Y,item-1),Folder2);
               printf("%s\n",job);
               system(job);
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
  pt = D->pt;
  T1 = (DIT *)kgGetNamedWidget(Tmp,"Tbox1");
  T2 = (DIT *)kgGetNamedWidget(Tmp,"Tbox2");
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
  sprintf(buf,"%-s/.Trash",getenv("HOME"));
  mkdir(buf,0700);
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
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  rmdir(Trash);
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
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  rmdir(Trash);
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
  int x,y,yl,xpos,ypos;
  char from[300],to[300],fname[300];
  char job[500];
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  yl = D->yl;

#if 1
  if((kbe->event ==1)||(kbe->event == 3)) {
	  ThumbNail *Th=NULL;
	  ThumbNail **List=NULL;
    if(kbe->button !=1) {
	  int item=-1;
	  wid = kgGetClickedWidget(Tmp);

	  if(kgCheckWidgetName(wid,"Xbox1")) {
		  int rval=-1;
			  printf("Otherbutton on Xbox1\n");
			  kgGetClickedPosition(Tmp,&x,&y);
			  item = kgGetThumbNailItem(X1,x,y);
			  List=(ThumbNail **)kgGetList(X1);
			  if(item>= 0)  {
			    Th= (ThumbNail *)List[item];
			    rval = RunkgPopUpMenu(Tmp,x,y,DMenu1);
			    printf("Name = %s\n",Th->name);
			  }
			  else rval = RunkgPopUpMenu(Tmp,x,y,DMenu2);
			  printf("Rval = %d\n",rval);
	  }
	  if(kgCheckWidgetName(wid,"Ybox1")) {
		  int rval=-1;
			  printf("Otherbutton on Ybox1\n");
			  kgGetClickedPosition(Tmp,&x,&y);
			  item = kgGetThumbNailItem(Y1,x,y);
			  List=(ThumbNail **)kgGetList(Y1);
			  ypos = y;
			  if(ypos >(yl-250) )ypos= yl-250;
			  if(item>= 0)  {
			    Th= (ThumbNail *)List[item];
			    rval = RunkgPopUpMenu(Tmp,x,ypos,FMenu1);
			    printf("Name = %s\n",Th->name);
			  }
			  else rval = RunkgPopUpMenu(Tmp,x,ypos,FMenu2);
			  printf("Rval = %d\n",rval);
			  switch(rval) {
				  case 5:
					  sprintf(fname,"%-s/%-s",Folder1,Th->name);
					  strcpy(from,Th->name);
					  sprintf(job,"New name for %-s ",Th->name);
					  strcat(job,"%30s");
					  strcpy(to,Th->name);
					  gscanf(Tmp,job,to);
					  sprintf(job,"rename %-s %-s %-s",Th->name,to,fname);
					  system(job);
					  free(Th->name);
					  kgSetThumbNailName(Y1,item,to);
					  printf("New Name: %s\n",Th->name);
					  kgSortList(Y1);
					  kgUpdateWidget(Y1);
					  kgUpdateOn(Tmp);
					  break;
				  default:
					  break;
			  }

	  }
	  if(kgCheckWidgetName(wid,"Xbox2")) {
		  int rval=-1;
			  printf("Otherbutton on Xbox2\n");
			  kgGetClickedPosition(Tmp,&x,&y);
			  item = kgGetThumbNailItem(X2,x,y);
			  List=(ThumbNail **)kgGetList(X2);
			  if(item>= 0)  {
			    Th= (ThumbNail *)List[item];
			    printf("Name = %s\n",Th->name);
			    rval = RunkgPopUpMenu(Tmp,X2->x1,y,DMenu1);
			    printf("Name = %s\n",Th->name);
			  }
			  else rval = RunkgPopUpMenu(Tmp,X2->x1,y,DMenu2);
			  printf("Rval = %d\n",rval);
	  }
	  if(kgCheckWidgetName(wid,"Ybox2")) {
		  int rval=-1;
			  printf("Otherbutton on Ybox2\n");
			  kgGetClickedPosition(Tmp,&x,&y);
			  item = kgGetThumbNailItem(Y2,x,y);
			  List=(ThumbNail **)kgGetList(Y2);
			  if(item>= 0)  {
			    Th= (ThumbNail *)List[item];
			    rval = RunkgPopUpMenu(Tmp,x,y,FMenu1);
			    printf("Name = %s\n",Th->name);
			  }
			  else rval = RunkgPopUpMenu(Tmp,x,y,FMenu2);
			  printf("Rval = %d\n",rval);
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