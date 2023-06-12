#include <kulina.h>
#include "kgfilebrowserCallbacks.h"
#include "Gclrkgfilebrowser.c"
#include "images.c"
#define R 120
#define G 140
#define B 120

int kgfilebrowserGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIY y0 = { 
    'y',
    5,95,  
    188,605,   
    72,32,  
    24, 
    24, 
    1,9808587, 
    0,8, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,kgfilebrowserbrowser1callback, /* *args, callback */
    4,  /* Border Offset  */
     20,  /* Scroll width  */
     2,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y0.Wid,(char *)"Xbox1");
  y0.item = -1;
  DIY y1 = { 
    'y',
    192,95,  
    734,605,   
    72,44,  
    48, 
    48, 
    1,9808587, 
    0,5, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,kgfilebrowserbrowser2callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y1.Wid,(char *)"Ybox1");
  y1.item = -1;
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"");
  butn2[0].xpmn=kgPowerdownImage(22,180,0,60);
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
#if 0
#if 1
  butn2[0].xpmn= (char *)malloc(100); 
  strcpy(butn2[0].xpmn,(char *)"##/usr/share/icons/CratOS_lion-icons/actions/scalable/application-exit.png");
#else
  butn2[0].xpmn= kgGetProcessedImage("##/usr/share/icons/CratOS_lion-icons/actions/scalable/application-exit.png",40,0.8,0,0,0);
#endif
#endif
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  DIL h2 = { 
    'h',
    1396,42,  
    1446,90,
    2,0,  
    40, 
    40, 
    1,1, 
    2,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    kgfilebrowsersplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h2.Wid,(char *)"Quit");
  h2.item = -1;
  DIY y3 = { 
    'y',
    1269,95,  
    1444,605,   
    72,32,  
    24, 
    24, 
    1,40, 
    0,8, 
    (int *)v[2], 
    NULL, 
    NULL, 
    NULL,kgfilebrowserbrowser3callback, /* *args, callback */
    4,  /* Border Offset  */
     20,  /* Scroll width  */
     2,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y3.Wid,(char *)"Xbox2");
  y3.item = -1;
  DIY y4 = { 
    'y',
    739,95,  
    1269,605,   
    72,44,  
    48, 
    48, 
    1,40, 
    0,5, 
    (int *)v[3], 
    NULL, 
    NULL, 
    NULL,kgfilebrowserbrowser4callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y4.Wid,(char *)"Ybox2");
  y4.item = -1;
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"");
  butn5[0].xpmn=kgHomeImage(22,R,G,B);
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=-1;
  butn5[0].butncode='';
  butn5[1].sw=1;
  strcpy(butn5[1].title,(char *)"");
  butn5[1].xpmn=kgUpdirImage(22,R,G,B);
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=-1;
  butn5[1].butncode='';
  DIN b5 = { 
    'n',
    13,11,  
    105,61,
    2,2,  
    40, 
    40, 
    2,1, 
    6,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    kgfilebrowserbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"Butbox1");
  b5.item = -1;
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(5);
  strcpy(e6[0].fmt,(char *)"%40s");
  e6[0].v=(void *)v[4];
  e6[0].sw=1;
  e6[0].noecho=0;
  e6[0].img=NULL;
  DIT t6 = { 
    't',
    105,21,  
    504,53,
    20, 
    1,1, 
    e6,
    1,1,
    NULL,kgfilebrowsertextbox1callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t6.Wid,(char *)"Tbox1");
  t6.pt=NULL;
  t6.type = 0;
  t6.item = -1;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"");
  butn7[0].xpmn=kgHomeImage(22,R,G,B);
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"");
  butn7[1].xpmn=kgUpdirImage(22,R,G,B);
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-1;
  butn7[1].butncode='';
  DIN b7 = { 
    'n',
    1301,9,  
    1393,59,
    2,2,  
    40, 
    40, 
    2,1, 
    6,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    kgfilebrowserbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"Butbox2");
  b7.item = -1;
  T_ELMT *e8  ; 
  e8 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e8[0].fmt = (char *)malloc(5);
  strcpy(e8[0].fmt,(char *)"%40s");
  e8[0].v=(void *)v[5];
  e8[0].sw=1;
  e8[0].noecho=0;
  e8[0].img=NULL;
  DIT t8 = { 
    't',
    897,20,  
    1297,53,
    20, 
    1,1, 
    e8,
    1,1,
    NULL,kgfilebrowsertextbox2callback,0,0,18,9 /* args,Call back */
  };
  strcpy(t8.Wid,(char *)"Tbox2");
  t8.pt=NULL;
  t8.type = 0;
  t8.item = -1;
  DIY y9 = { 
    'y',
    116,607,  
    1323,737,   
    72,44,  
    48, 
    48, 
    1,1, 
    0,1, 
    (int *)v[6], 
    NULL, 
    NULL, 
    NULL,kgfilebrowserbrowser5callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y9.Wid,(char *)"Trash");
  y9.item = -1;
  BUT_STR  *butn10=NULL; 
  butn10= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn10[0].sw=1;
  strcpy(butn10[0].title,(char *)"Trash");
  butn10[0].xpmn=NULL;
  butn10[0].xpmp=NULL;
  butn10[0].xpmh=NULL;
#if 1
  butn10[0].xpmn= (char *)malloc(100); 
  strcpy(butn10[0].xpmn,(char *)"##/usr/share/icons/CratOS_lion-icons/apps/scalable/gnome-fs-trash-empty.png");
#else
  butn10[0].xpmn= kgGetProcessedImage("##/usr/share/icons/CratOS_lion-icons/apps/scalable/gnome-fs-trash-empty.png",72,0.8,0,0,0);
#endif
#if 1
  butn10[0].xpmp= (char *)malloc(100); 
  strcpy(butn10[0].xpmp,(char *)"##/usr/share/icons/CratOS_lion-icons/apps/scalable/clanbomber.png");
#else
  butn10[0].xpmp= kgGetProcessedImage("##/usr/share/icons/CratOS_lion-icons/apps/scalable/clanbomber.png",72,0.8,0,0,0);
#endif
  butn10[0].bkgr=-1;
  butn10[0].butncode='';
  DIN b10 = { 
    'n',
    32,617,  
    112,727,
    2,2,  
    72, 
    96, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn10, 
    kgfilebrowserbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b10.Wid,(char *)"TrashButn");
  b10.item = -1;
  char *xpm11=   (char *)&trash_empty_str;
  DIP p11 = { 
    'p',
    1327,608,  
    1443,734,  
    (void *)xpm11,
    -1, /* bkgr colour */ 
      -1,0,0.000000 /* border hide transparency*/ 
  };
  strcpy(p11.Wid,(char *)"Imagebox");
  p11.item = -1;
  BUT_STR  *butn12=NULL; 
  butn12= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn12[0].sw=1;
  strcpy(butn12[0].title,(char *)"New Folder");
  butn12[0].xpmn=NULL;
  butn12[0].xpmp=NULL;
  butn12[0].xpmh=NULL;
  butn12[0].bkgr=-1;
  butn12[0].butncode='';
  DIN b12 = { 
    'n',
    3,61,  
    93,95,
    2,2,  
    80, 
    30, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn12, 
    kgfilebrowserbutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b12.Wid,(char *)"NewFolderL");
  b12.item = -1;
  BUT_STR  *butn13=NULL; 
  butn13= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn13[0].sw=1;
  strcpy(butn13[0].title,(char *)"New Folder");
  butn13[0].xpmn=NULL;
  butn13[0].xpmp=NULL;
  butn13[0].xpmh=NULL;
  butn13[0].bkgr=-1;
  butn13[0].butncode='';
  DIN b13 = { 
    'n',
    1266,62,  
    1356,102,
    2,2,  
    80, 
    30, 
    1,1, 
    5,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn13, 
    kgfilebrowserbutton5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b13.Wid,(char *)"NewFolderR");
  b13.item = -1;
  BUT_STR  *butn14=NULL; 
  butn14= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn14[0].sw=1;
  strcpy(butn14[0].title,(char *)"Select all");
  butn14[0].xpmn=NULL;
  butn14[0].xpmp=NULL;
  butn14[0].xpmh=NULL;
  butn14[0].bkgr=-1;
  butn14[0].butncode='';
  butn14[1].sw=1;
  strcpy(butn14[1].title,(char *)"Clear");
  butn14[1].xpmn=NULL;
  butn14[1].xpmp=NULL;
  butn14[1].xpmh=NULL;
  butn14[1].bkgr=-1;
  butn14[1].butncode='';
  butn14[2].sw=1;
  strcpy(butn14[2].title,(char *)"Copy selected");
  butn14[2].xpmn=NULL;
  butn14[2].xpmp=NULL;
  butn14[2].xpmh=NULL;
  butn14[2].bkgr=-1;
  butn14[2].butncode='';
  butn14[3].sw=1;
  strcpy(butn14[3].title,(char *)"Move selected");
  butn14[3].xpmn=NULL;
  butn14[3].xpmp=NULL;
  butn14[3].xpmh=NULL;
  butn14[3].bkgr=-1;
  butn14[3].butncode='';
  butn14[4].sw=1;
  strcpy(butn14[4].title,(char *)"To trash selected");
  butn14[4].xpmn=NULL;
  butn14[4].xpmp=NULL;
  butn14[4].xpmh=NULL;
  butn14[4].bkgr=-1;
  butn14[4].butncode='';
  DIN b14 = { 
    'n',
    191,62,  
    659,102,
    0,0,  
    90, 
    30, 
    5,1, 
    5,0.250000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn14, 
    kgfilebrowserbutton6callback, /*  Callbak */
    NULL  /* any args */
  };
  strcpy(b14.Wid,(char *)"OptionsL");
  b14.item = -1;
  BUT_STR  *butn15=NULL; 
  butn15= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn15[0].sw=1;
  strcpy(butn15[0].title,(char *)"Select all");
  butn15[0].xpmn=NULL;
  butn15[0].xpmp=NULL;
  butn15[0].xpmh=NULL;
  butn15[0].bkgr=-1;
  butn15[0].butncode='';
  butn15[1].sw=1;
  strcpy(butn15[1].title,(char *)"Clear");
  butn15[1].xpmn=NULL;
  butn15[1].xpmp=NULL;
  butn15[1].xpmh=NULL;
  butn15[1].bkgr=-1;
  butn15[1].butncode='';
  butn15[2].sw=1;
  strcpy(butn15[2].title,(char *)"Copy selected");
  butn15[2].xpmn=NULL;
  butn15[2].xpmp=NULL;
  butn15[2].xpmh=NULL;
  butn15[2].bkgr=-1;
  butn15[2].butncode='';
  butn15[3].sw=1;
  strcpy(butn15[3].title,(char *)"Move selected");
  butn15[3].xpmn=NULL;
  butn15[3].xpmp=NULL;
  butn15[3].xpmh=NULL;
  butn15[3].bkgr=-1;
  butn15[3].butncode='';
  butn15[4].sw=1;
  strcpy(butn15[4].title,(char *)"To trash selected");
  butn15[4].xpmn=NULL;
  butn15[4].xpmp=NULL;
  butn15[4].xpmh=NULL;
  butn15[4].bkgr=-1;
  butn15[4].butncode='';
  DIN b15 = { 
    'n',
    738,62,  
    1206,102,
    0,0,  
    90, 
    30, 
    5,1, 
    5,0.250000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn15, 
    kgfilebrowserbutton7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b15.Wid,(char *)"OptionsR");
  b15.item = -1;
  T_ELMT *e16  ; 
  e16 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e16[0].fmt = (char *)malloc(19);
  strcpy(e16[0].fmt,(char *)"Filter(simple)%30s");
  e16[0].v=(void *)v[7];
  e16[0].sw=1;
  e16[0].noecho=0;
  e16[0].img=NULL;
  DIT t16 = { 
    't',
    502,18,  
    895,54,
    20, 
    1,1, 
    e16,
    1,1,
    NULL,kgfilebrowsertextbox3callback,1,1,18,9 /* args,Call back */
  };
  strcpy(t16.Wid,(char *)"FilterBox");
  t16.pt=NULL;
  t16.type = 0;
  t16.item = -1;
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+18));
  d =dtmp+i; 
  d[17].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIY));
  *d[0].y = y0;
  d[0].y->item = -1;
  kgfilebrowserbrowser1init(d[0].y,pt) ;
  d[1].t = (DIT *)malloc(sizeof(DIY));
  *d[1].y = y1;
  d[1].y->item = -1;
  kgfilebrowserbrowser2init(d[1].y,pt) ;
  d[2].t = (DIT *)malloc(sizeof(DIL));
  *d[2].h = h2;
  d[2].h->item = -1;
  kgfilebrowsersplbutton1init(d[2].h,pt) ;
  d[3].t = (DIT *)malloc(sizeof(DIY));
  *d[3].y = y3;
  d[3].y->item = -1;
  kgfilebrowserbrowser3init(d[3].y,pt) ;
  d[4].t = (DIT *)malloc(sizeof(DIY));
  *d[4].y = y4;
  d[4].y->item = -1;
  kgfilebrowserbrowser4init(d[4].y,pt) ;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  kgfilebrowserbutton1init(d[5].N,pt) ;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  kgfilebrowserbutton2init(d[7].N,pt) ;
  d[8].t = (DIT *)malloc(sizeof(DIT));
  *d[8].t = t8;
  d[8].t->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIY));
  *d[9].y = y9;
  d[9].y->item = -1;
  kgfilebrowserbrowser5init(d[9].y,pt) ;
  d[10].t = (DIT *)malloc(sizeof(DIN));
  *d[10].N = b10;
  d[10].N->item = -1;
  kgfilebrowserbutton3init(d[10].N,pt) ;
  d[11].t = (DIT *)malloc(sizeof(DIP));
  *d[11].p = p11;
  d[11].p->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIN));
  *d[12].N = b12;
  d[12].N->item = -1;
  kgfilebrowserbutton4init(d[12].N,pt) ;
  d[13].t = (DIT *)malloc(sizeof(DIN));
  *d[13].N = b13;
  d[13].N->item = -1;
  kgfilebrowserbutton5init(d[13].N,pt) ;
  d[14].t = (DIT *)malloc(sizeof(DIN));
  *d[14].N = b14;
  d[14].N->item = -1;
  kgfilebrowserbutton6init(d[14].N,pt) ;
  d[15].t = (DIT *)malloc(sizeof(DIN));
  *d[15].N = b15;
  d[15].N->item = -1;
  kgfilebrowserbutton7init(d[15].N,pt) ;
  d[16].t = (DIT *)malloc(sizeof(DIT));
  *d[16].t = t16;
  d[16].t->item = -1;
  d[17].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakekgfilebrowserGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    ThumbnailBrowser1  1 data value
    ThumbnailBrowser2  1 data value
    ThumbnailBrowser3  1 data value
    ThumbnailBrowser4  1 data value
    Text_Box1  1 data values
    Text_Box2  1 data values
    ThumbnailBrowser5  1 data value
    Text_Box3  1 data values

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   int  *v2 ;
   v2 = (int *)malloc(sizeof(int));
   *v2 = 1;
   int  *v3 ;
   v3 = (int *)malloc(sizeof(int));
   *v3 = 1;
   char  *v4 ;
   v4 = (char *)malloc(sizeof(char)*500);
   v4[0] = '\0';
   char  *v5 ;
   v5 = (char *)malloc(sizeof(char)*500);
   v5[0] = '\0';
   int  *v6 ;
   v6 = (int *)malloc(sizeof(int));
   *v6 = 1;
   char  *v7 ;
   v7 = (char *)malloc(sizeof(char)*500);
   v7[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*9);
   v[8]=NULL;
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=(void *)(v2);
   v[3]=(void *)(v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(v6);
   v[7]=(void *)(v7);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = kgfilebrowserGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v; // kulina will double free this; you may modify
   return GrpId;
}

int kgfilebrowser( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=2107030000;
  kgInitUi(&D);
  D.d=NULL;
#if 1
  GrpId = kgfilebrowserGroup(&D,v,pt);
#else 
  GrpId = MakekgfilebrowserGroup(&D,pt); // can try this also
#endif 
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 15;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 37;   /* Position of Dialog */ 
  D.yo = 37;
  D.xl = 1453;    /*  Length of Dialog */
  D.yl = 740;    /*  Width  of Dialog */
  D.Initfun = kgfilebrowserinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgfilebrowsercleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = kgfilebrowserCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgfilebrowserResizeCallBack;  /*  Resize callback */
#if 0 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = kgfilebrowserWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.NoWinMngr = 0;    /*  1 for no Window Manager*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina File Browser ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
  Modifykgfilebrowser(&D,GrpId);    /*  add extras to  gui*/
  ModifykgfilebrowserGc(&D);    /*  set colors for gui if don't like default*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *Runkgfilebrowser(void *arg) {
/*************************************************

    ThumbnailBrowser1  1 data value
    ThumbnailBrowser2  1 data value
    ThumbnailBrowser3  1 data value
    ThumbnailBrowser4  1 data value
    Text_Box1  1 data values
    Text_Box2  1 data values
    ThumbnailBrowser5  1 data value
    Text_Box3  1 data values

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   char  v4[500]="" ;
   char  v5[500]="" ;
   int   v6 = 1;
   char  v7[500]="" ;
   void* v[8];
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(v4);
   v[5]=(void *)(v5);
   v[6]=(void *)(&v6);
   v[7]=(void *)(v7);
   void *pt=NULL; /* pointer to send any extra information */
   kgfilebrowser(NULL,v,pt );
   return NULL;
}
