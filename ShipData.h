// ShipData.h: interface for the CShipData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPDATA_H__58EA76E3_4F92_4AA9_8ACF_403FB24E0CD2__INCLUDED_)
#define AFX_SHIPDATA_H__58EA76E3_4F92_4AA9_8ACF_403FB24E0CD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* 以下代码是用来字节对齐使用的 */
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
   unsigned short usNStks;     // MAR.761.GosportMaersk: 0x02 0xE7-743 多少个NSD_STK数据块数
   unsigned short usVBlocks;   // 0x00 0x41-65; 0x00 0x2B-43 有效的可装载的块，甲板上下分别计算(下标注线的最后一个bay)
   unsigned short usTiers;     // 0x00 0x02-2 ; 0x00 0x01-1  Tier的种类
   unsigned short usStacks;     // 0x00 0x01-1 ; 0x00 0x01-1  Stack的种类
   unsigned short usBays;     // 0x00 0x24-36; 0x00 0x17-23 bay的个数，即要画的个数
   unsigned char ucBytes[4]; // 0x01 0x01 0x01 0x01; 0x00 0x02 0x02 0x02
   char sVesselISO[8]; // 0x03 I S O 0x20 0x20 0x20 0x00 船舶bay的命名
};

#define MAXBBLOCKS 40 
struct __PACKED__ T_NSD_BAY
{
   char sTierName[8];     // str: length+value, e.g. 0x03 I S O 0x20 0x20 0x20 0x??
   unsigned short usTier; // 0x00 0x01  Tier类型编号 1,2,3...
   char sBayNo1[4];  // 0x02 0 5 0x20;
   char sBayNo2[4];  // 0x02 0 6 0x20;
   unsigned char cReserved[16]; // 任意的 
};

//0x56E 122字节的数据块 - ???
struct __PACKED__ T_NSD_STKHEAD
{
   unsigned char cReserved[122];
};

//0x5E8开始：每块74字节 16X4+10 － 与箱位和列数有关系，但一定是描述每个箱位的信息
struct __PACKED__ T_NSD_STK 
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1－下层仓 below；2－甲板上层 above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... 列
   unsigned short usBTier;  //6 00 01 最下面一层的层号，从01开始
   unsigned short usTTier;  //8 00 03; 00 02; 00 02; 最上面的一层层号，从01开始
   unsigned short usMaxHeight;     //10 以整数方式表示本堆的最大高度:03 2A=810(8.10);02 26=550(5.50);01 22=290(2.90)
   int   iStkLCG20;     //12 00 00 65 32=25906 (259.06)
   int   iStkLCG40;     //16-10 00 00 63 FF=25599 (255.99)
   int   iStkLCG45;     //20-14 7F FF FF FF=最大值 (-)
   unsigned char uc18[12]; //24-18
   short sStkWt20;      //36 Size 20的Weight Limit数据: 最大7F FE － 最大32766 显示为3276.6; 常规为00 00显示为0.0; 02 58=600-60.0
   short sStkWt40;      //38 Size 40的Weight Limit数据: 03 84=900 - 90.0
   short sStkWt45;      //40 Size 45的Weight Limit数据: 03 84=900 - 90.0
   unsigned char  uc2A[6]; //42 3x(00 00)
   unsigned short us30;    //48 ? 02 EE=750 ?
   unsigned short usVCG;   //50 Stack VCG:09 63=2403-24.03
   unsigned char  uc34[6];
   unsigned char ucType[16]; //58 本列每层的箱体类型：01-20;02-40;03-20/40;04-45;08-48,07-20/40/45;15-20/40/45/48
   // 1x-air/prt;2x-rst(Restricted Use); 4x-hzp(Hazards Prohibited); 8x-csp(Cool Stow Prohibited)
};

// 在StackData后接下来为每块16X16=256的数据块
struct __PACKED__ T_NSD_VBLOCK  // 共256字节
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1－下层仓 below；2－甲板上层 above
   unsigned char ucAttr[2];  //4 似乎和PairBay等相关01-Fore;02-Aft;03-Fore/Aft
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
   char StkName[8]; //-8A 03 ISO 20 20 20 03 Stack名称
   //unsigned char uc92[2]; //-92 00 01
   unsigned short usStack;  // 似乎是Stack类型编号
   unsigned char ucDoors[2]; //-94 00 00; Doors: 00 01-Fore; 00 02-Aft | Reefer Plugs: 0x 00 - 1Fore,2Aft
   unsigned char ucAthWarShip[2]; //-96 00 00; AthWarShip 01 00; 
   //unsigned int ui98[24];     //-98 
   unsigned short usYX[48];
   unsigned char ucCAFVN[2]; //-F8 00 42; Ventilated 00 04; Fore Hatch 00 02; 
   unsigned char ucFA[6]; //-FA ?? 前四字节是Bulkhead Fore LCG:00 00 66 63=26211 - 262.11
};

#define NUMSTKCODES 19
struct __PACKED__ T_NSD_STKDEF
{
   char StackName[8]; // Stack类型名称, 当StackName为ISO的时候，则不要用这个了，似乎ISO彻底按照现代标准的00,01,02...26的27个column来标注了
   char StackCode[NUMSTKCODES][4];  // 00, 01, 02, 03...
};

#define NUMTIERCODES 44
struct __PACKED__ T_NSD_TIERDEF
{
   char TierName[8];  // Tier类型名称
	char TierCode[NUMTIERCODES][4]; // 注意下层32个，上层32个 (是22个吧)
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
   //unsigned int offset;  // 在文件中的数据部分起始位置
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
   unsigned short usLayer;  //2 1－下层仓 below；2－甲板上层 above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... 列
   unsigned short usTTier;  //最上面的一层层号，从01开始
   char resevers[3];
};

struct __PACKED__ T_NSD_STKAXXX
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1－下层仓 below；2－甲板上层 above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... 列
   unsigned short usTIERS6[6];
};

struct __PACKED__ T_NSD_STKATIERS
{
   unsigned short usBay;  //0 00 01-1;00 02-3;...; 00 05-09;... (n*2-1)
   unsigned short usLayer;  //2 1－下层仓 below；2－甲板上层 above
   unsigned short usStack; //4 00 00; 00 01; 00 02;... 列
   unsigned short usTTier;  //最上面的一层层号，从01开始
   unsigned char  pucTAttr[1][5];  // 00 03 00 00 00 00；....
};

struct __PACKED__ T_NSD_BAYSUMS  // 2012/7/23 11:12:16 需要超过40BLOCKS的部分
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

   T_NSD_STKDEF *m_pStackDef; // 各种Stack类型的定义数据，个数为m_head.usStacks
   T_NSD_TIERDEF *m_pTierDef; // 各种Tier类型的定义数据，个数为m_head.usTiers
   unsigned short usHatchs;  // # of Hatch Covers
   T_NSD_HATCH *m_pHatchs;    // Hatch Cover

   unsigned short usBaySums;
   T_NSD_BAYSUMS *m_pBaySums;

private:
   char *m_pShipNote;         // 船舶的Note
   T_NSD_BDHEAD m_SNL1Head;   // SNL1段的头
   char *m_pSNL1Data;         // 这里只是为了存放各个Block的Notes数据
   T_NSD_BLKNOTES **m_pBLKNotes; // 即指向m_pBLKNotes[m_head.usVBlocks]的数据块，注意这样还是指针数据，实际指向m_pSNL1Data里面的数据位置，当然被整理过了，否则，你们怎么用呢？
   
   char *m_pSTKAData;         // 这里是存放STKA数据的缓存
   P_NSD_STKSUMS *m_pSTKSums;
   char *m_pSTKAMark;
   P_NSD_STKAXXX *m_pSTKAXXX;
   P_NSD_STKATIERS *m_pSTKATiers;

public:

   // 函数名: GetTierDefinition
   // 描述  : 查询船图中的指定Tier的类型和定义数据
   // 返回  : T_NSD_TIERDEF * : 返回指定的Tier类型和定义数据；NULL-表示没有数据或usTier值非法
   // 参数  : unsigned short usTier [IN]: 从1-m_head.usTiers, 即T_NSD_BAY(m_Bays[x])::usTier这个值
	T_NSD_TIERDEF * GetTierDefinition(unsigned short usTier);

   // 函数名: GetStackDefinition
   // 描述  : 查询船图中的指定的Stack的类型以及定义说明
   // 返回  : T_NSD_STKDEF * : 返回指定的Stack类型以及定义数据；NULL－表示没有数据或usStack非法
   // 参数  : unsigned short usStack [IN]: 从1-m_head.usStacks，即T_NSD_VBLOCK(m_pVBlock[x])::usStack值
	T_NSD_STKDEF * GetStackDefinition(unsigned short usStack);

   // 函数名: GetBlockNote
   // 描述  : 查询指定block的Note数据
   // 返回  : T_NSD_BLKNOTES * : NULL - 没有该block的note数据；否则成功返回指定的block的note数据
   // 参数  : int blocknumber [IN]: block编号 0 - m_head.usVBlocks-1
	T_NSD_BLKNOTES * GetBlockNote(int blocknumber);

   // 函数名: GetShipNote
   // 描述  : 查询船的注释说明
   // 返回  : const char * : 返回该注释字符串；NULL-没有Ship的Note
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
