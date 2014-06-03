// ShipData.h: interface for the CShipData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPDATA_H__58EA76E3_4F92_4AA9_8ACF_403FB24E0CD2__INCLUDED_)
#define AFX_SHIPDATA_H__58EA76E3_4F92_4AA9_8ACF_403FB24E0CD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* ���´����������ֽڶ���ʹ�õ� */
#ifdef WIN32
#define __PACKED__ 
#pragma pack(push,1)
#else
#ifdef __GNUC__
#define __PACKED__	__attribute__ ((packed))
//#pragma pack(push,1)
#else
#ifdef HP_UX
#define __PACKED__
#pragma pack 1
#else
#define __PACKED__
#pragma options align=packed
#endif
#endif
#endif

struct __PACKED__ T_NSD_HEAD 
{
   unsigned char NSDMark[2]; // 0x72,0x7F
   char sVesselClass[6];   // str:length+value,e.g. 0x04 B P R 0x20 - "BPR"
   unsigned short usNStks;     // MAR.761.GosportMaersk: 0x02 0xE7-743 ���ٸ�NSD_STK���ݿ���
   unsigned short usVBlocks;   // 0x00 0x41-65; 0x00 0x2B-43 ��Ч�Ŀ�װ�صĿ飬�װ����·ֱ����(�±�ע�ߵ����һ��bay)
   unsigned short usTiers;     // 0x00 0x02-2 ; 0x00 0x01-1  Tier������
   unsigned short usStacks;     // 0x00 0x01-1 ; 0x00 0x01-1  Stack������
   unsigned short usBays;     // 0x00 0x24-36; 0x00 0x17-23 bay�ĸ�������Ҫ���ĸ���
   unsigned char ucBytes[4]; // 0x01 0x01 0x01 0x01; 0x00 0x02 0x02 0x02
   char sVesselISO[8]; // 0x03 I S O 0x20 0x20 0x20 0x00 ����bay������
};

#define MAXBBLOCKS 40 
struct __PACKED__ T_NSD_BAY
{
   char sTierName[8];     // str: length+value, e.g. 0x03 I S O 0x20 0x20 0x20 0x??
   unsigned short usTier; // 0x00 0x01  Tier���ͱ�� 1,2,3...
   char sBayNo1[4];  // 0x02 0 5 0x20;
   char sBayNo2[4];  // 0x02 0 6 0x20;
   unsigned char cReserved[16]; // ����� 
};

//0x56E 122�ֽڵ����ݿ� - ???
struct __PACKED__ T_NSD_STKHEAD
{
   unsigned char cReserved[122];
};

//0x5E8��ʼ��ÿ��74�ֽ� 16X4+10 �� ����λ�������й�ϵ����һ��������ÿ����λ����Ϣ
struct __PACKED__ T_NSD_STK 
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1���²�� below��2���װ��ϲ� above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... ��
   unsigned short usBTier;  //6 00 01 ������һ��Ĳ�ţ���01��ʼ
   unsigned short usTTier;  //8 00 03; 00 02; 00 02; �������һ���ţ���01��ʼ
   unsigned short usMaxHeight;     //10 ��������ʽ��ʾ���ѵ����߶�:03 2A=810(8.10);02 26=550(5.50);01 22=290(2.90)
   int   iStkLCG20;     //12 00 00 65 32=25906 (259.06)
   int   iStkLCG40;     //16-10 00 00 63 FF=25599 (255.99)
   int   iStkLCG45;     //20-14 7F FF FF FF=���ֵ (-)
   unsigned char uc18[12]; //24-18
   short sStkWt20;      //36 Size 20��Weight Limit����: ���7F FE �� ���32766 ��ʾΪ3276.6; ����Ϊ00 00��ʾΪ0.0; 02 58=600-60.0
   short sStkWt40;      //38 Size 40��Weight Limit����: 03 84=900 - 90.0
   short sStkWt45;      //40 Size 45��Weight Limit����: 03 84=900 - 90.0
   unsigned char  uc2A[6]; //42 3x(00 00)
   unsigned short us30;    //48 ? 02 EE=750 ?
   unsigned short usVCG;   //50 Stack VCG:09 63=2403-24.03
   unsigned char  uc34[6];
   unsigned char ucType[16]; //58 ����ÿ����������ͣ�01-20;02-40;03-20/40;04-45;08-48,07-20/40/45;15-20/40/45/48
   // 1x-air/prt;2x-rst(Restricted Use); 4x-hzp(Hazards Prohibited); 8x-csp(Cool Stow Prohibited)
};

// ��StackData�������Ϊÿ��16X16=256�����ݿ�
struct __PACKED__ T_NSD_VBLOCK  // ��256�ֽ�
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1���²�� below��2���װ��ϲ� above
   unsigned char ucAttr[2];  //4 �ƺ���PairBay�����01-Fore;02-Aft;03-Fore/Aft
   int iStkLCG20;       //6 00 00 7A AC=31404 314.04 
   int iStkLCG40;       //10 00 00 79 7A=31098 310.98
   int iStkLCG45;       //14 7F FF FF FF=      -
   unsigned int ui12[3];   //18 FF FF FF 7F
/* ****** Updated by CHENYH at 2008-9-14 21:37:31 ****** 
   unsigned int ui16;   //22 FF FF FF 7F
   unsigned int ui1A;   //26 FF FF FF 7F
*/
   int iStkWt20;  //30-1E 00 00 03 2A=810-81.0
   int iStkWt40;  //34-22 00 00 04 BA=1210-121.0
   int iStkWt45;  //38-26 00 00 04 BA=1210-121.0
   unsigned int ui2A[14]; //42-2A 12*00
/* ****** Updated by CHENYH at 2008-9-14 21:21:08 ****** 
   int i36;    //54-36 7F FF FF FF;00 00 0A F1
   int i3A;    //58-3A 7F FF FF FF;00 00 0B F8
   int i3E;    //62-3E 7F FF FF FF;00 00 0C FE
   int i42;    //66-42 7F FF FF FF;00 00 0E 05
   int i46;    //70-46 00 00 05 61;00 00 0F 0B
   int i4A;    //74-4A 00 00 06 65;00 00 10 12
   int i4E;    //78-4E 00 00 07 68;00 00 11 18
   int i52;    //82-52 00 00 08 6B;7F FF FF FF
   int i56;    //86-56 00 00 09 6E;7F FF FF FF
   int i5A;    //90-5A 7F FF FF FF;7F FF FF FF
   int i5E;    //94-5E 7F FF FF FF;7F FF FF FF
*/
   unsigned short us62[4]; //98-62 00 01;
/* ****** Updated by CHENYH at 2008-9-14 21:38:29 ****** 
   unsigned short us64; //100-64 00 06;
   unsigned short us66; //102-66 09 01;0A 02
   unsigned short us68; //104-68 01 01;02 01
*/

   unsigned int ui6A[8]; //-6A 32X00
   char StkName[8]; //-8A 03 ISO 20 20 20 03 Stack����
   //unsigned char uc92[2]; //-92 00 01
   unsigned short usStack;  // �ƺ���Stack���ͱ��
   unsigned char ucDoors[2]; //-94 00 00; Doors: 00 01-Fore; 00 02-Aft | Reefer Plugs: 0x 00 - 1Fore,2Aft
   unsigned char ucAthWarShip[2]; //-96 00 00; AthWarShip 01 00; 
   //unsigned int ui98[24];     //-98 
   unsigned short usYX[48];
   unsigned char ucCAFVN[2]; //-F8 00 42; Ventilated 00 04; Fore Hatch 00 02; 
   unsigned char ucFA[6]; //-FA ?? ǰ���ֽ���Bulkhead Fore LCG:00 00 66 63=26211 - 262.11
};

#define NUMSTKCODES 19
struct __PACKED__ T_NSD_STKDEF
{
   char StackName[8]; // Stack��������, ��StackNameΪISO��ʱ����Ҫ������ˣ��ƺ�ISO���װ����ִ���׼��00,01,02...26��27��column����ע��
   char StackCode[NUMSTKCODES][4];  // 00, 01, 02, 03...
};

#define NUMTIERCODES 44
struct __PACKED__ T_NSD_TIERDEF
{
   char TierName[8];  // Tier��������
	char TierCode[NUMTIERCODES][4]; // ע���²�32�����ϲ�32�� (��22����)
};

struct __PACKED__ T_NSD_HATCH
{
   unsigned short us1;
   unsigned short us2;
   unsigned short us3;
   char reserve[10];
   char HatchName[8];
};

struct __PACKED__ T_NSD_BDHEAD
{
   unsigned int length;
   char name[4];
   //unsigned int offset;  // ���ļ��е����ݲ�����ʼλ��
};

struct __PACKED__ T_NSD_BLKNOTES
{
   unsigned short usBay;
   unsigned short usLayer;
   char Notes[1];
};

struct __PACKED__ T_NSD_STKSUMS
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1���²�� below��2���װ��ϲ� above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... ��
   unsigned short usTTier;  //�������һ���ţ���01��ʼ
   char resevers[3];
};

struct __PACKED__ T_NSD_STKAXXX
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1���²�� below��2���װ��ϲ� above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... ��
   unsigned short usTIERS6[6];
};

struct __PACKED__ T_NSD_STKATIERS
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1���²�� below��2���װ��ϲ� above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... ��
   unsigned short usTTier;  //�������һ���ţ���01��ʼ
   unsigned char  pucTAttr[1][5];  // 00 03 00 00 00 00��....
};

struct __PACKED__ T_NSD_BAYSUMS  // 2012/7/23 11:12:16 ��Ҫ����40BLOCKS�Ĳ���
{
   char sTierName[8];      // str: length+value, e.g. 0x03 I S O 0x20 0x20 0x20 0x??
   unsigned short us;      // 00,02 or 00,01
};


#ifdef WIN32
#pragma pack(pop)
#else
#ifdef __GNUC__
#else
#ifdef HP_UX
#pragma pack 0
#else
#pragma options align=reset
#endif
#endif
#endif
#undef __PACKED__

typedef T_NSD_BLKNOTES * P_NSD_BLKNOTES;
typedef T_NSD_STKSUMS  * P_NSD_STKSUMS;
typedef T_NSD_STKAXXX  * P_NSD_STKAXXX;
typedef T_NSD_STKATIERS * P_NSD_STKATIERS;
typedef T_NSD_BAYSUMS  * P_NSD_BAYSUMS;

class CShipData  
{
public:
   T_NSD_HEAD m_head;
   T_NSD_BAY m_Bays[MAXBBLOCKS];
   T_NSD_STKHEAD m_StackHead;
   T_NSD_STK *m_pStack;
   T_NSD_VBLOCK *m_pVBlock;

   T_NSD_STKDEF *m_pStackDef; // ����Stack���͵Ķ������ݣ�����Ϊm_head.usStacks
   T_NSD_TIERDEF *m_pTierDef; // ����Tier���͵Ķ������ݣ�����Ϊm_head.usTiers
   unsigned short usHatchs;  // # of Hatch Covers
   T_NSD_HATCH *m_pHatchs;    // Hatch Cover

   unsigned short usBaySums;
   T_NSD_BAYSUMS *m_pBaySums;

private:
   char *m_pShipNote;         // ������Note
   T_NSD_BDHEAD m_SNL1Head;   // SNL1�ε�ͷ
   char *m_pSNL1Data;         // ����ֻ��Ϊ�˴�Ÿ���Block��Notes����
   T_NSD_BLKNOTES **m_pBLKNotes; // ��ָ��m_pBLKNotes[m_head.usVBlocks]�����ݿ飬ע����������ָ�����ݣ�ʵ��ָ��m_pSNL1Data���������λ�ã���Ȼ��������ˣ�����������ô���أ�
   
   char *m_pSTKAData;         // �����Ǵ��STKA���ݵĻ���
   P_NSD_STKSUMS *m_pSTKSums;
   char *m_pSTKAMark;
   P_NSD_STKAXXX *m_pSTKAXXX;
   P_NSD_STKATIERS *m_pSTKATiers;

public:

   // ������: GetTierDefinition
   // ����  : ��ѯ��ͼ�е�ָ��Tier�����ͺͶ�������
   // ����  : T_NSD_TIERDEF * : ����ָ����Tier���ͺͶ������ݣ�NULL-��ʾû�����ݻ�usTierֵ�Ƿ�
   // ����  : unsigned short usTier [IN]: ��1-m_head.usTiers, ��T_NSD_BAY(m_Bays[x])::usTier���ֵ
	T_NSD_TIERDEF * GetTierDefinition(unsigned short usTier);

   // ������: GetStackDefinition
   // ����  : ��ѯ��ͼ�е�ָ����Stack�������Լ�����˵��
   // ����  : T_NSD_STKDEF * : ����ָ����Stack�����Լ��������ݣ�NULL����ʾû�����ݻ�usStack�Ƿ�
   // ����  : unsigned short usStack [IN]: ��1-m_head.usStacks����T_NSD_VBLOCK(m_pVBlock[x])::usStackֵ
	T_NSD_STKDEF * GetStackDefinition(unsigned short usStack);

   // ������: GetBlockNote
   // ����  : ��ѯָ��block��Note����
   // ����  : T_NSD_BLKNOTES * : NULL - û�и�block��note���ݣ�����ɹ�����ָ����block��note����
   // ����  : int blocknumber [IN]: block��� 0 - m_head.usVBlocks-1
	T_NSD_BLKNOTES * GetBlockNote(int blocknumber);

   // ������: GetShipNote
   // ����  : ��ѯ����ע��˵��
   // ����  : const char * : ���ظ�ע���ַ�����NULL-û��Ship��Note
	const char * GetShipNote();

	void ExportShipData(FILE *fp);
	int ImportShipData(FILE *fp, char *eMsg);
	CShipData();
	virtual ~CShipData();

private:
	void ExportBlockData(FILE *fp);
	int ImportBlockData(FILE *fp, char *eMsg);
	void ExportTierDefinitions(FILE *fp);
	int ImportTierDefinitions(FILE *fp, char *eMsg);
	void ExportStackDefinitions(FILE *fp);
	int ImportStackDefinations(FILE *fp, char *eMsg);
	void ExportVBlocks(FILE *fp);
	void ExportStackData(FILE *fp);
	void ExportStackHead(FILE *fp);
	void ExportBays(FILE *fp);
	void ExportShipHead(FILE *fp);
	int ImportVBlocks(FILE *fp, char *eMsg);
	int ImportStackHead(FILE *fp, char *eMsg);
	int ImportStackData(FILE *fp, char *eMsg);
	int ImportBays(FILE *fp, char *eMsg);
   int ImportShipHead(FILE *fp, char *eMsg);   

	void ExportUChar(FILE *ofp, char *ucname, int uclen, unsigned char *uc)
	{
		int i;
		fprintf(ofp,"%s=",ucname);
		for (i=0;i<uclen;)
		{
		  fprintf(ofp,"%02x",uc[i++]);
		  if (i<uclen)
			 fprintf(ofp," ");
		  else
			 fprintf(ofp,"\n");
		}
	}
};

#endif // !defined(AFX_SHIPDATA_H__58EA76E3_4F92_4AA9_8ACF_403FB24E0CD2__INCLUDED_)
