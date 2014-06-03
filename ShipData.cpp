// ShipData.cpp: implementation of the CShipData class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
//#include "mypub.h"
#include "ShipData.h"
#include "string.h"

void reverse(void *data, int len)
{
    register unsigned char *pb=(unsigned char *)data;
    register unsigned char *pe=pb+len-1;
    register unsigned char b;
    while (pb<pe)
    {
      b = *pb;
      *pb=*pe;
      *pe=b;
      pb++;pe--;
    }
}

#ifdef NOREV
#define REVERSE_(a) 
#define REVERSEX(pa,bytes) 
#else
#define REVERSE_(a) reverse(&(a),sizeof(a))
#define REVERSEX(pa,bytes) reverse(pa,bytes)
#endif

bool IsNumberString(const char *str)
{
   const char *p;
   for (p=str;(*p)!='\0';++p)
   {
      if ((*p)<'0'||(*p)>'9')
         return(false);
   }
   return(true);
}

void ExportUChar(FILE *ofp, char *ucname, int uclen, unsigned char *uc);
void ExportPChar(FILE *ofp, char *fname, int fvlen, char *fvalue);
void ExportStr(FILE *ofp, char *fname, unsigned char *str);

void ExportInt(FILE *ofp,const char *fname, int ivalue)
{
   fprintf(ofp,"%s=",fname);
   if (ivalue==0x7FFFFFFF)
      fprintf(ofp,"-\n");
   else
      fprintf(ofp,"%d\n",ivalue);
}

void ExportShort(FILE *ofp,const char *fname, short svalue)
{
   fprintf(ofp,"%s=",fname);
   if (svalue==0x7FFF)
      fprintf(ofp,"-\n");
   else
      fprintf(ofp,"%d\n",svalue);
}

void ExportRawData(FILE *ofp,const char *fname, int datasize, void *pdata)
{
   unsigned char *pc=(unsigned char *)pdata;
   int i;
   fprintf(ofp,"%s=",fname);
   for (i=0;i<datasize;i++)
   {
      fprintf(ofp,"%02X ",pc[i]);
   }
   for (i=0;i<datasize;i++)
   {
      switch (pc[i])
      {
      case 0:
         fprintf(ofp,".");
         break;
      case 0x0A:
      case 0x0D:
      case 0x09:
         fprintf(ofp," ");
         break;
      default:
         fprintf(ofp,"%c",pc[i]);
         break;
      }
   }
   fprintf(ofp,"\n");
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipData::CShipData()
{
   m_pStack = NULL;
   m_pVBlock = NULL;
   m_pStackDef = NULL;
   m_pTierDef = NULL;
   m_pSNL1Data = NULL;
   m_pBLKNotes = NULL;
   m_pShipNote = NULL;
   m_pHatchs = NULL;
   m_pSTKAData = NULL;
   m_pSTKSums = NULL;
   m_pSTKAXXX = NULL;
   m_pSTKATiers = NULL;
    m_pBaySums = NULL;

   memset(&m_head,0,sizeof(T_NSD_HEAD));
   memset(m_Bays,0,MAXBBLOCKS*sizeof(T_NSD_BAY));
   memset(&m_StackHead,0,sizeof(T_NSD_STKHEAD));
   usHatchs=0;
   usBaySums=0;
}

CShipData::~CShipData()
{
   if (m_pStack!=NULL)
   {
      delete []m_pStack;
   }
   if (m_pVBlock!=NULL)
   {
      delete []m_pVBlock;
   }
   if (m_pStackDef!=NULL)
   {
      delete []m_pStackDef;
   }
   if (m_pTierDef!=NULL)
   {
      delete []m_pTierDef;
   }
   if (m_pSNL1Data!=NULL)
   {
      delete []m_pSNL1Data;
   }
   if (m_pBLKNotes!=NULL)
   {
      delete []m_pBLKNotes;
   }
   if (m_pShipNote!=NULL)
   {
      delete []m_pShipNote;
   }
   if (m_pHatchs!=NULL)
   {
      delete []m_pHatchs;
   }
   if (m_pSTKAData!=NULL)
   {
      delete []m_pSTKAData;
   }
   if (m_pSTKSums!=NULL)
   {
      delete []m_pSTKSums;
   }
   if (m_pSTKAXXX!=NULL)
   {
      delete []m_pSTKAXXX;
   }
   if (m_pSTKATiers!=NULL)
   {
      delete []m_pSTKATiers;
   }
   if (m_pBaySums!=NULL)
   {
      delete []m_pBaySums;
   }
}

int CShipData::ImportShipData(FILE *fp, char *eMsg)
{
   int rtn=0;
   //g_LogFile.WriteLogEx(201,"\nImport Head of Ship Data @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportShipHead(fp,eMsg);
   if (rtn<0)
      return(rtn);
   //g_LogFile.WriteLogEx(202,"\nImport Bays of Ship Data @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportBays(fp,eMsg);
   if (rtn<0)
      return(rtn);
   //g_LogFile.WriteLogEx(203,"\nImport Head of Ship Stacks @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportStackHead(fp,eMsg);
   if (rtn<0)
      return(rtn);
   //g_LogFile.WriteLogEx(204,"\nImport Data of Ship Stacks @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportStackData(fp,eMsg);
   if (rtn<0)
      return(rtn);
   //g_LogFile.WriteLogEx(205,"\nImport Data of Ship Blocks @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportVBlocks(fp,eMsg);
   if (rtn<0)
      return(rtn);
   //g_LogFile.WriteLogEx(206,"\nImport Data of Ship StackDefinations @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportStackDefinations(fp,eMsg);
   if (rtn<0)
      return(rtn);
   //g_LogFile.WriteLogEx(207,"\nImport Data of Ship Tier Definitions @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportTierDefinitions(fp,eMsg);
   if (rtn<0)
      return(rtn);
   
   //g_LogFile.WriteLogEx(208,"\nImport Data of Ship Block Data @ %X - %ld",ftell(fp),ftell(fp));
   rtn = ImportBlockData(fp,eMsg);
   if (rtn<0)
      return(rtn);

   //g_LogFile.WriteLogEx(206,"End importing @ %X - %ld",ftell(fp),ftell(fp));   
   return(rtn);
}

void pstr2pchar(const char *pstr,char *pchar)
{
   unsigned char clen=((unsigned char *)pstr)[0];
   unsigned int i;
   ++pstr;
   for (i=0;i<clen;++i,++pstr)
   {
      pchar[i]=*pstr;
   }
   pchar[i]='\0';
}

#define STR2PCHAR(a) pstr2pchar(a,a)

int CShipData::ImportShipHead(FILE *fp, char *eMsg)
{
   if (fread(&m_head,sizeof(m_head),1,fp)<1)
   {
      //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read the Head of Ship Data:errno=%d",errno);
      return(-1);
   }
   if (m_head.NSDMark[0]!=0x72||m_head.NSDMark[1]!=0x7F)
   {
      //g_LogFile.WriteLogEx(LEV_WARN,"Warning: NSDMark[0]=%02X NSDMark[1]=%02X",m_head.NSDMark[0],m_head.NSDMark[1]);
      //return(-2);
   }
   pstr2pchar(m_head.sVesselClass,m_head.sVesselClass);
   pstr2pchar(m_head.sVesselISO,m_head.sVesselISO);

   REVERSE_(m_head.usNStks);
   REVERSE_(m_head.usVBlocks);
   REVERSE_(m_head.usTiers);
   REVERSE_(m_head.usStacks);
   REVERSE_(m_head.usBays);
   //g_LogFile.WriteLogEx(LEV_INFO,"Ship Sum: # Stacks:%u, # Blocks:%u, # Bays:%u",
     // m_head.usNStks, m_head.usVBlocks, m_head.usBays);
   return(1);
}


int CShipData::ImportBays(FILE *fp, char *eMsg)
{
   int i;
   for (i=0;i<MAXBBLOCKS;i++)
   {
      if (fread(m_Bays+i,sizeof(T_NSD_BAY),1,fp)<1)
      {
         //sprintf(eMsg,"Fail to read the Bay[%d]'s Data:errno=%d",i,errno);
         return(-2);
      }
      pstr2pchar(m_Bays[i].sTierName,m_Bays[i].sTierName);
      pstr2pchar(m_Bays[i].sBayNo1,m_Bays[i].sBayNo1);
      pstr2pchar(m_Bays[i].sBayNo2,m_Bays[i].sBayNo2);
      REVERSE_(m_Bays[i].usTier);
   }
   return(i);
}

int CShipData::ImportStackHead(FILE *fp, char *eMsg)
{
   if (fread(&m_StackHead,sizeof(T_NSD_STKHEAD),1,fp)<1)
   {
      //sprintf(eMsg,"Fail to read the Data of StackHead:errno=%d",errno);
      return(-3);
   }
   return(1);
}


int CShipData::ImportStackData(FILE *fp, char *eMsg)
{
   int i=0;
   long errat;
   if (m_pStack!=NULL)
   {
      delete []m_pStack;
   }
  // g_LogFile.WriteLogEx(LEV_INFO,"\tNumber of Stocks:%u",m_head.usNStks);
   if (m_head.usNStks>0)
   {
      m_pStack = new T_NSD_STK[m_head.usNStks];
      if (m_pStack==NULL)
      {
         sprintf(eMsg,"Fail to new T_NSD_STK[%u]",m_head.usNStks);
         return(-4);
      }
      for (i=0;i<m_head.usNStks;i++)
      {
         errat = ftell(fp);
         if (fread(m_pStack+i,sizeof(T_NSD_STK),1,fp)<1)
         {
            //sprintf(eMsg,"Fail to read T_NSD_STK[%d] @ %x :errno=%d",i,errat,errno);
            return(-4);
         }
         REVERSE_(m_pStack[i].usBay);
         REVERSE_(m_pStack[i].usLayer);
         REVERSE_(m_pStack[i].usStack);
         REVERSE_(m_pStack[i].usBTier);
         REVERSE_(m_pStack[i].usTTier);
         REVERSE_(m_pStack[i].usMaxHeight);
         REVERSE_(m_pStack[i].iStkLCG20);
         REVERSE_(m_pStack[i].iStkLCG40);
         REVERSE_(m_pStack[i].iStkLCG45);

         REVERSE_(m_pStack[i].sStkWt20);
         REVERSE_(m_pStack[i].sStkWt40);
         REVERSE_(m_pStack[i].sStkWt45);

         REVERSE_(m_pStack[i].usVCG);

		 if (m_pStack[i].usLayer>2){}
/*         {
            g_LogFile.WriteLogEx(LEV_WARN,"Invalid Stack Data[%d] @ %x, usLayer(%u)>2", i,errat,
               m_pStack[i].usLayer);
         }
         if (m_pStack[i].usStack>26)
         {
            g_LogFile.WriteLogEx(LEV_WARN,"Invalid Stack Data[%d] @ %x, usStack(%u)>26", i,errat,
               m_pStack[i].usStack);
         }
         if (m_pStack[i].usBTier<1||m_pStack[i].usBTier>16)
         {
            g_LogFile.WriteLogEx(LEV_WARN,"Invalid Stack Data[%d] @ %x, usBTier(%u) (<1 or >16)",i, errat,
               m_pStack[i].usBTier);
         }
         if (m_pStack[i].usTTier<1||m_pStack[i].usTTier>16)
         {
            g_LogFile.WriteLogEx(LEV_WARN,"Invalid Stack Data[%d] @ %x, usTTier(%u) (<1 or >16)",
               i,errat,m_pStack[i].usTTier);
         }
*/
         // 2009-5-17 22:58:35
         else if (m_pStack[i].usTTier>11)
         {
            int j;
            for (j=11;j<m_pStack[i].usTTier&&j<13;j++)
            {
               m_pStack[i].ucType[j]=m_pStack[i].ucType[j+3];
               m_pStack[i].ucType[j+3]=0;
            }
         }
      }
   }
   return(i);
}


int CShipData::ImportVBlocks(FILE *fp, char *eMsg)
{
   int i;
   long errat;
   if (m_pVBlock!=NULL)
   {
      delete []m_pVBlock;
   }
   //g_LogFile.WriteLogEx(LEV_INFO,"\tNumber of Valid Blocks:%u",m_head.usVBlocks);
   if (m_head.usVBlocks>0)
   {
      m_pVBlock = new T_NSD_VBLOCK[m_head.usVBlocks];
      if (m_pVBlock==NULL)
      {
         sprintf(eMsg,"Fail to new T_NSD_VBLOCK[%u]",m_head.usVBlocks);
         return(-5);
      }
      for (i=0;i<m_head.usVBlocks;i++)
      {
         errat = ftell(fp);
         if (fread(m_pVBlock+i,sizeof(T_NSD_VBLOCK),1,fp)<1)
         {
            //sprintf(eMsg,"Fail to read T_NSD_VBLOCK[%d] @ %x:errno=%d",i,errat,errno);
            return(-5);
         }
         pstr2pchar(m_pVBlock[i].StkName,m_pVBlock[i].StkName);
         REVERSE_(m_pVBlock[i].usBay);
         REVERSE_(m_pVBlock[i].usLayer);
         REVERSE_(m_pVBlock[i].iStkLCG20);
         REVERSE_(m_pVBlock[i].iStkLCG40);
         REVERSE_(m_pVBlock[i].iStkLCG45);
         REVERSE_(m_pVBlock[i].iStkWt20);
         REVERSE_(m_pVBlock[i].iStkWt40);
         REVERSE_(m_pVBlock[i].iStkWt45);
         REVERSE_(m_pVBlock[i].usStack);
         int j;
         for (j=0;j<48;j++)
         {
            REVERSE_(m_pVBlock[i].usYX[j]);
         }
         if (m_pVBlock[i].usLayer>2)
         {
            //g_LogFile.WriteLogEx(LEV_WARN,"Invalid Stack Data[%d] @ %x, usLayer(%u)>2", i,errat,
            //   m_pVBlock[i].usLayer);
         }
      }
   }
   return(i);
}

void CShipData::ExportShipData(FILE *fp)
{
   ExportShipHead(fp);
   ExportBays(fp);
   ExportStackHead(fp);
   ExportStackData(fp);
   ExportVBlocks(fp);
   ExportStackDefinitions(fp);
   ExportTierDefinitions(fp);
   ExportBlockData(fp);
}

void CShipData::ExportShipHead(FILE *fp)
{
   ExportRawData(fp,"\nNSD_Head",sizeof(m_head),&m_head);
   ExportRawData(fp,"{\n\tNSDMark[2]",sizeof(m_head.NSDMark),m_head.NSDMark);
   fprintf(fp,"\tsVesselClass[6]=%s\n",m_head.sVesselClass);
   fprintf(fp,"\tusNStks=%u\n",m_head.usNStks);
   fprintf(fp,"\tusVBlocks=%u\n",m_head.usVBlocks);
   //ExportRawData(fp,"\tusTiers",sizeof(m_head.usTiers),&(m_head.usTiers));
   fprintf(fp,"\tusTiers=%u\n",m_head.usTiers);
   //ExportRawData(fp,"\tusStacks",sizeof(m_head.usStacks),&(m_head.usStacks));
   fprintf(fp,"\tusStacks=%u\n",m_head.usStacks);
   fprintf(fp,"\tusBays=%u\n",m_head.usBays);
   ExportRawData(fp,"\tuc12",sizeof(m_head.ucBytes),m_head.ucBytes);
   fprintf(fp,"\tsVesselISO=%s\n",m_head.sVesselISO);
   fprintf(fp,"}\n");
}

void CShipData::ExportBays(FILE *fp)
{
   int i;
   for (i=0;i<MAXBBLOCKS;i++)
   {
      fprintf(fp,"\nBAY[%d]\n{\n",i);
      fprintf(fp,"\tsTierName=%s\n",m_Bays[i].sTierName);
      fprintf(fp,"\tusTier=%u\n",m_Bays[i].usTier);
      fprintf(fp,"\tsBayNo1=%s\n",m_Bays[i].sBayNo1);
      fprintf(fp,"\tsBayNo2=%s\n",m_Bays[i].sBayNo2);
      ExportRawData(fp,"\tcReserved",sizeof(m_Bays[i].cReserved),m_Bays[i].cReserved);
      fprintf(fp,"}");
   }
   fprintf(fp,"\n");
}

void CShipData::ExportStackHead(FILE *fp)
{
   ExportRawData(fp,"\nSTKHead",sizeof(m_StackHead),&m_StackHead);
   fprintf(fp,"\n");
}


void CShipData::ExportStackData(FILE *fp)
{
   unsigned int i;
   if (m_pStack==NULL)
   {
      fprintf(fp,"\nNULL Stack Data\n");
      return;
   }
   for (i=0;i<m_head.usNStks;i++)
   {
      fprintf(fp,"\nStack[%u]\n{\n",i);
      fprintf(fp,"\tusBay=%u\n",m_pStack[i].usBay);
      fprintf(fp,"\tusLayer=%u\n",m_pStack[i].usLayer);
      fprintf(fp,"\tusStack=%u\n",m_pStack[i].usStack);
      fprintf(fp,"\tusBTier=%u\n",m_pStack[i].usBTier);
      fprintf(fp,"\tusTTier=%u\n",m_pStack[i].usTTier);
      if (m_pStack[i].usMaxHeight==0x7FFF)
         fprintf(fp,"\tusMaxHeight=-\n");
      else
         fprintf(fp,"\tusMaxHeight=%u\n",m_pStack[i].usMaxHeight);
      ExportInt(fp,"\tiStkLCG20",m_pStack[i].iStkLCG20);
      ExportInt(fp,"\tiStkLCG40",m_pStack[i].iStkLCG40);
      ExportInt(fp,"\tiStkLCG45",m_pStack[i].iStkLCG45);
      ExportRawData(fp,"\tuc18",sizeof(m_pStack[i].uc18),m_pStack[i].uc18);
      ExportShort(fp,"\tsStkWt20",m_pStack[i].sStkWt20);
      ExportShort(fp,"\tsStkWt40",m_pStack[i].sStkWt40);
      ExportShort(fp,"\tsStkWt45",m_pStack[i].sStkWt45);
      ExportRawData(fp,"\tuc2A",sizeof(m_pStack[i].uc2A),m_pStack[i].uc2A);
      ExportRawData(fp,"\tus30",sizeof(m_pStack[i].us30),&(m_pStack[i].us30));
      fprintf(fp,"\tusVCG=%u\n",m_pStack[i].usVCG);
      ExportRawData(fp,"\tuc34=",sizeof(m_pStack[i].uc34),m_pStack[i].uc34);
      ExportUChar(fp,"\tucType[16]=",sizeof(m_pStack[i].ucType),m_pStack[i].ucType);
      fprintf(fp,"}");
   }
   fprintf(fp,"\n");
}


void CShipData::ExportVBlocks(FILE *fp)
{
   unsigned int i;
   int j;
   if (m_pVBlock==NULL)
   {
      fprintf(fp,"\nNULL VBlock Data\n");
      return;
   }
   for (i=0;i<m_head.usVBlocks;i++)
   {
      fprintf(fp,"\nVBlock[%u]\n{\n",i);
      fprintf(fp,"\tusBay=%u\n",m_pVBlock[i].usBay);
      fprintf(fp,"\tusLayer=%u\n",m_pVBlock[i].usLayer);
      ExportUChar(fp,"\tusAttr[2]",2,m_pVBlock[i].ucAttr);
      ExportInt(fp,"\tiStkLCG20",m_pVBlock[i].iStkLCG20);
      ExportInt(fp,"\tiStkLCG40",m_pVBlock[i].iStkLCG40);
      ExportInt(fp,"\tiStkLCG45",m_pVBlock[i].iStkLCG45);
      ExportRawData(fp,"\tui12",12,m_pVBlock[i].ui12);
      ExportInt(fp,"\tiStkWt20",m_pVBlock[i].iStkWt20);
      ExportInt(fp,"\tiStkWt40",m_pVBlock[i].iStkWt40);
      ExportInt(fp,"\tiStkWt45",m_pVBlock[i].iStkWt45);
      char fname[20];
      for (j=0;j<14;j++)
      {
         sprintf(fname,"\tui%02X",0x2A+j*4);
         ExportRawData(fp,fname,4,m_pVBlock[i].ui2A+j);
      }
      for (j=0;j<4;j++)
      {
         sprintf(fname,"\tus%02X",0x62+j*2);
         ExportRawData(fp,fname,2,m_pVBlock[i].us62+j);
      }
      for (j=0;j<8;j++)
      {
         sprintf(fname,"\tui%02X",0x6A+j*4);
         ExportRawData(fp,fname,4,m_pVBlock[i].ui6A+j);
      }
      fprintf(fp,"\tStkName=%s\n",m_pVBlock[i].StkName);
      fprintf(fp,"\tusStack=%u\n",m_pVBlock[i].usStack);
      ExportUChar(fp,"\tucDoors",2,m_pVBlock[i].ucDoors);
      ExportUChar(fp,"\tucAthWarShip",2,m_pVBlock[i].ucAthWarShip);
      fprintf(fp,"\tusYX=");
      for (j=0;j<48;j++)
      {
         fprintf(fp,"%u",m_pVBlock[i].usYX[j]);
         if (j&0x01)
            fprintf(fp,",");
         else
            fprintf(fp," ");
      }
      //ExportUChar(fp,"\tusYX",48*2,(unsigned char *)m_pVBlock[i].usYX);
      ExportUChar(fp,"\n\tucCAFVN",2,m_pVBlock[i].ucCAFVN);
      ExportUChar(fp,"\tucFA",6,m_pVBlock[i].ucFA);
      fprintf(fp,"}");
   }
   fprintf(fp,"\n");
}

int CShipData::ImportStackDefinations(FILE *fp, char *eMsg)
{
   unsigned int i=0;
   long errat;
   if (m_pStackDef!=NULL)
   {
      delete []m_pStackDef;
   }
   m_pStackDef = NULL;
   //g_LogFile.WriteLogEx(LEV_INFO,"\tNumber of Stack Definitions:%u",m_head.usStacks);
   if (m_head.usStacks>0)
   {
      m_pStackDef = new T_NSD_STKDEF[m_head.usStacks];
      if (m_pStackDef==NULL)
      {
         //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to new T_NSD_STKDEF[%u]",m_head.usStacks);
         return(-6);
      }
      for (i=0;i<m_head.usStacks;i++)
      {
         errat = ftell(fp);
         if (fread(m_pStackDef+i,sizeof(T_NSD_STKDEF),1,fp)<1)
         {
            //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read T_NSD_STKDEF[%u] @ %x:errno=%d",i,errat,errno);
            return(-6);
         }
         STR2PCHAR(m_pStackDef[i].StackName);
         int j;
         for (j=0;j<NUMSTKCODES;j++)
         {
            STR2PCHAR(m_pStackDef[i].StackCode[j]);
         }
      }
   }
   return(i);
}

void CShipData::ExportStackDefinitions(FILE *fp)
{
   unsigned int i;
   if (m_pStackDef==NULL)
   {
      fprintf(fp,"\nNULL Stack Definition\n");
      return;
   }
   for (i=0;i<m_head.usStacks;i++)
   {
      fprintf(fp,"\nStackDef[%u]\n{",i);
      fprintf(fp,"\n\tStackName=%s",m_pStackDef[i].StackName);
      int j;
      for (j=0;j<NUMSTKCODES;j++)
      {
         fprintf(fp,"\n\tStackCode[%d]=%s",j,m_pStackDef[i].StackCode[j]);
         if (!IsNumberString(m_pStackDef[i].StackCode[j]))
         {
            //g_LogFile.WriteLogEx(LEV_WARN,"Warning:StackName=%s StackCode[%d]=%s",m_pStackDef[i].StackName,j,m_pStackDef[i].StackCode[j]);
         }
      }
      fprintf(fp,"\n}");
   }
   fprintf(fp,"\n");
}

int CShipData::ImportTierDefinitions(FILE *fp, char *eMsg)
{
   unsigned int i=0;
   long errat;
   if (m_pTierDef!=NULL)
   {
      delete []m_pTierDef;
   }
   m_pTierDef = NULL;
   //g_LogFile.WriteLogEx(LEV_INFO,"\tNumber of Tier Definitions:%u",m_head.usTiers);
   if (m_head.usTiers>0)
   {
      m_pTierDef = new T_NSD_TIERDEF[m_head.usTiers];
      if (m_pTierDef==NULL)
      {
         //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to new T_NSD_TIERDEF[%u]",m_head.usTiers);
         return(-7);
      }
      for (i=0;i<m_head.usTiers;i++)
      {
         errat = ftell(fp);
         if (fread(m_pTierDef+i,sizeof(T_NSD_TIERDEF),1,fp)<1)
         {
            //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read T_NSD_TIERDEF[%u] @ %x:errno=%d",i,errat,errno);
            return(-7);            
         }
         STR2PCHAR(m_pTierDef[i].TierName);
         int j;
         for (j=0;j<NUMTIERCODES;j++)
         {
            STR2PCHAR(m_pTierDef[i].TierCode[j]);
         }
      }
   }
   return(i);
}

void CShipData::ExportTierDefinitions(FILE *fp)
{
   unsigned int i;
   if (m_pTierDef==NULL)
   {
      fprintf(fp,"\nNULL Tier Definition\n");
      return;
   }
   for (i=0;i<m_head.usTiers;i++)
   {
      fprintf(fp,"\nTierDef[%u]\n{",i);
      fprintf(fp,"\n\tTierName=%s",m_pTierDef[i].TierName);
      int j;
      for (j=0;j<NUMTIERCODES;j++)
      {
         fprintf(fp,"\n\tTierCode[%d]=%s",j,m_pTierDef[i].TierCode[j]);
         if (!IsNumberString(m_pTierDef[i].TierCode[j]))
         {
            //g_LogFile.WriteLogEx(LEV_WARN,"Warning:TierName=%s TierCode[%d]=%s",m_pTierDef[i].TierName,j,m_pTierDef[i].TierCode[j]);
         }
      }
      fprintf(fp,"\n}");
   }
   fprintf(fp,"\n");
}

int CShipData::ImportBlockData(FILE *fp, char *eMsg)
{
   T_NSD_BDHEAD bdhead;
   if (m_pSNL1Data!=NULL)
   {
      delete []m_pSNL1Data;
      m_pSNL1Data = NULL;
   }
   if (m_pBLKNotes!=NULL)
   {
      delete []m_pBLKNotes;
      m_pBLKNotes = NULL;
   }
   if (m_pShipNote!=NULL)
   {
      delete []m_pShipNote;
      m_pShipNote = NULL;
   }
   if (m_pHatchs!=NULL)
   {
      delete []m_pHatchs;
      m_pHatchs = NULL;
   }
   if (m_pSTKAData!=NULL)
   {
      delete []m_pSTKAData;
      m_pSTKAData = NULL;
   }
   if (m_pSTKSums!=NULL)
   {
      delete []m_pSTKSums;
      m_pSTKSums = NULL;
   }
   if (m_pSTKAXXX!=NULL)
   {
      delete []m_pSTKAXXX;
      m_pSTKAXXX = NULL;
   }
   if (m_pSTKATiers!=NULL)
   {
      delete []m_pSTKATiers;
      m_pSTKATiers = NULL;
   }
   if (m_pSTKAData!=NULL)
   {
      delete []m_pSTKAData;
      m_pSTKAData = NULL;
   }
   if (m_pBaySums!=NULL)
   {
      delete []m_pBaySums;
      m_pBaySums = NULL;
   }

   if (fread(&usHatchs,sizeof(usHatchs),1,fp)<1)
   {
      //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read usHatchs");
      return(-8);
   }
   REVERSE_(usHatchs);
   int i;
   if (usHatchs>0)
   {
      m_pHatchs = new T_NSD_HATCH[usHatchs];
      for (i=0;i<usHatchs;i++)
      {
         fread(m_pHatchs+i,sizeof(T_NSD_HATCH),1,fp);
         REVERSE_(m_pHatchs[i].us1);
         REVERSE_(m_pHatchs[i].us2);
         REVERSE_(m_pHatchs[i].us3);
         STR2PCHAR(m_pHatchs[i].HatchName);
      }
      //fseek(fp,usHatchs*0x18,SEEK_CUR);
   }
   i=0;
   while (fread(&bdhead,sizeof(bdhead),1,fp)>=1)
   {
      char name[8];
      REVERSE_(bdhead.length);
      memcpy(name,bdhead.name,4); name[4]='\0';
      //g_LogFile.WriteLogEx(LEV_INFO,"BLKName:%s, Length:%u @%X",name,bdhead.length,ftell(fp));
      if (memcmp(bdhead.name,"Bays",4)==0 && m_pBaySums==NULL)
      {
         long pf = ftell(fp);
         fread(&usBaySums,sizeof(usBaySums),1,fp);
         REVERSE_(usBaySums);
         if (usBaySums>0)
         {
            int j;
            m_pBaySums = new T_NSD_BAYSUMS [usBaySums];
            for (j=0;j<usBaySums;++j)
            {
               fread(m_pBaySums+j,sizeof(T_NSD_BAYSUMS),1,fp);
               STR2PCHAR(m_pBaySums[j].sTierName);
               REVERSE_(m_pBaySums[j].us);
            }
         }
         fseek(fp,pf+bdhead.length,SEEK_SET);
      }
      else if (memcmp(bdhead.name,"SNL1",4)==0 && m_pSNL1Data==NULL)
      {
         memcpy(&m_SNL1Head,&bdhead,sizeof(bdhead));
         if (bdhead.length>0)
         {
            m_pSNL1Data = new char [bdhead.length];
            if (fread(m_pSNL1Data,bdhead.length,1,fp)<1)
            {
               //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read Block Notes Data");
               return(-9);
            }
            m_pBLKNotes = new P_NSD_BLKNOTES [m_head.usVBlocks];
            int j;
            unsigned int offset=2;
            for (j=0;j<m_head.usVBlocks;j++)
            {
               if (offset>bdhead.length)
               {
                  //g_LogFile.WriteLogEx(LEV_ERROR,"Invalid Block Notes Data @ %u (>%u)",offset,bdhead.length);
                  return(-10);
               }
               m_pBLKNotes[j]=(T_NSD_BLKNOTES *)(m_pSNL1Data+offset);
               REVERSE_(m_pBLKNotes[j]->usBay);
               REVERSE_(m_pBLKNotes[j]->usLayer);
               unsigned char len = ((unsigned char *)(m_pBLKNotes[j]->Notes))[0];
               STR2PCHAR(m_pBLKNotes[j]->Notes);
               if ((len&0x01)==0)
               {
                  // 两字节对齐
                  ++len;
               }
               offset += sizeof(T_NSD_BLKNOTES)+len; 
            }
         }
      }
      // 2009-5-17 23:23:42
      else if (memcmp(bdhead.name,"STKA",4)==0)
      {
         m_pSTKAData = new char [bdhead.length];
         if (fread(m_pSTKAData,bdhead.length,1,fp)<1)
         {
            //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read Ship STKA Data");
            return(-12);
         }
         m_pSTKSums = new P_NSD_STKSUMS [m_head.usNStks];
         int j;
         unsigned int offset=2;
         for (j=0;j<m_head.usNStks;j++)
         {
            m_pSTKSums[j] = (P_NSD_STKSUMS)(m_pSTKAData+offset);
            REVERSE_(m_pSTKSums[j]->usBay);
            REVERSE_(m_pSTKSums[j]->usLayer);
            REVERSE_(m_pSTKSums[j]->usStack);
            REVERSE_(m_pSTKSums[j]->usTTier);
            offset += sizeof(T_NSD_STKSUMS);
            if (m_pSTKSums[j]->usTTier>13)
            {
               int jj;
               for (jj=13;jj<m_pSTKSums[j]->usTTier;jj++)
               {
                  m_pStack[j].ucType[jj]=m_pSTKSums[j]->resevers[jj-13];
               }
               m_pStack[j].usTTier = jj;
            }
         }
         m_pSTKAMark = m_pSTKAData+offset; 
         offset += 2;
         m_pSTKAXXX = new P_NSD_STKAXXX [m_head.usNStks];
         for (j=0;j<m_head.usNStks;j++)
         {
            m_pSTKAXXX[j] = (P_NSD_STKAXXX)(m_pSTKAData+offset);
            REVERSE_(m_pSTKAXXX[j]->usBay);
            REVERSE_(m_pSTKAXXX[j]->usLayer);
            REVERSE_(m_pSTKAXXX[j]->usStack);
            int jj;
            for (jj=0;jj<6;jj++)
               REVERSE_(m_pSTKAXXX[j]->usTIERS6[jj]);
            offset += sizeof(T_NSD_STKAXXX);
         }
         offset += 2;
         m_pSTKATiers = new P_NSD_STKATIERS [m_head.usNStks];
         for (j=0;j<m_head.usNStks;j++)
         {
            m_pSTKATiers[j] = (P_NSD_STKATIERS)(m_pSTKAData+offset);
            REVERSE_(m_pSTKATiers[j]->usBay);
            REVERSE_(m_pSTKATiers[j]->usLayer);
            REVERSE_(m_pSTKATiers[j]->usStack);
            REVERSE_(m_pSTKATiers[j]->usTTier);
            offset += sizeof(T_NSD_STKATIERS)+5*m_pSTKATiers[j]->usTTier-5;
         }         
      }
      else if (memcmp(bdhead.name,"TEXT",4)==0 && m_pShipNote==NULL)
      {
         m_pShipNote = new char [bdhead.length];
         if (fread(m_pShipNote,bdhead.length,1,fp)<1)
         {
            //g_LogFile.WriteLogEx(LEV_ERROR,"Fail to read Ship Note Data");
            return(-11);
         }
         STR2PCHAR((m_pShipNote+2));
      }
      else
      {
         fseek(fp,bdhead.length,SEEK_CUR); // 跳过去
      }
      i++;
   }
   return(i);
}

const char * CShipData::GetShipNote()
{
   if (m_pShipNote==NULL)
      return(NULL);
   else
      return(m_pShipNote+2);
}

void CShipData::ExportBlockData(FILE *fp)
{
   fprintf(fp,"\nusHatchs=%u",usHatchs);
   if (m_pHatchs==NULL)
   {
      fprintf(fp,"\nNo Hatch Cover Data");
   }
   else
   {
      int i;
      for (i=0;i<usHatchs;i++)
      {
         fprintf(fp,"\nHatch[%d] us1=%u us2=%u us3=%u HatchName=%s",
            i,m_pHatchs[i].us1,m_pHatchs[i].us2,m_pHatchs[i].us3,
            m_pHatchs[i].HatchName);
         ExportRawData(fp," reserve",sizeof(m_pHatchs[i].reserve),m_pHatchs[i].reserve);
      }
   }

   if (m_pSNL1Data==NULL || m_pBLKNotes==NULL)
   {
      fprintf(fp,"\nNo SNL1 data");
   }
   else
   {
      int i;
      for (i=0;i<m_head.usVBlocks;i++)
      {
         fprintf(fp,"\nBlock[%d](usBay=%u,usLayer=%u) Note:%s",
            i,m_pBLKNotes[i]->usBay,m_pBLKNotes[i]->usLayer,m_pBLKNotes[i]->Notes);
      }
   }
   const char *psnote = GetShipNote();
   if (psnote==NULL)
   {
      fprintf(fp,"\nNo Ship Note!");
   }
   else
   {
      fprintf(fp,"\nShip Note:%s",psnote);
   }
   fprintf(fp,"\n");

}

T_NSD_BLKNOTES * CShipData::GetBlockNote(int blocknumber)
{
   if (m_pBLKNotes==NULL)
      return(NULL);
   if (blocknumber<0||blocknumber>=m_head.usVBlocks)
      return(NULL);
   T_NSD_BLKNOTES *pbn = m_pBLKNotes[blocknumber];
   if (pbn->usLayer<1 || pbn->usLayer>2)
      return(NULL);
   if (pbn->usBay<1 || pbn->usBay>m_head.usBays)
      return(NULL);
   return(pbn);   
}

T_NSD_STKDEF * CShipData::GetStackDefinition(unsigned short usStack)
{
   if (usStack<1||usStack>m_head.usStacks)
      return(NULL);
   if (m_pStackDef==NULL)
      return(NULL);
   return(m_pStackDef+usStack-1);
}

T_NSD_TIERDEF * CShipData::GetTierDefinition(unsigned short usTier)
{
   if (m_pTierDef==NULL)
      return(NULL);
   if (usTier<1||usTier>m_head.usTiers)
      return(NULL);
   return(m_pTierDef+usTier-1);
}
