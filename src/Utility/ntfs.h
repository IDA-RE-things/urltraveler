#pragma once


#include <Windows.h>
// #include <stdlib.h>
// #include <stdio.h>


#pragma pack(push, curAlignment)

#pragma pack(1)


typedef struct _NTFS_PART_BOOT_SEC
{
	char		chJumpInstruction[3];
	char		chOemID[4];
	char		chDummy[4];

	struct NTFS_BPB
	{
		WORD		wBytesPerSec;
		BYTE		uchSecPerClust;
		WORD		wReservedSec;
		BYTE		uchReserved[3];
		WORD		wUnused1;
		BYTE		uchMediaDescriptor;
		WORD		wUnused2;
		WORD		wSecPerTrack;
		WORD		wNumberOfHeads;
		DWORD		dwHiddenSec;
		DWORD		dwUnused3;
		DWORD		dwUnused4;
		LONGLONG	n64TotalSec;
		LONGLONG	n64MFTLogicalClustNum;
		LONGLONG	n64MFTMirrLogicalClustNum;
		int			nClustPerMFTRecord;
		int			nClustPerIndexRecord;
		LONGLONG	n64VolumeSerialNum;
		DWORD		dwChecksum;
	} bpb;

	char		chBootstrapCode[426];
	WORD		wSecMark;
}NTFS_PART_BOOT_SEC;
////////////////////////////MFT record header and attribute header //////////////////////
typedef struct _NTFS_MFT_FILE
{
	char		szSignature[4];		// Signature "FILE"
	WORD		wFixupOffset;		// offset to fixup pattern
	WORD		wFixupSize;			// Size of fixup-list +1
	LONGLONG	n64LogSeqNumber;	// log file seq number
	WORD		wSequence;			// sequence nr in MFT
	WORD		wHardLinks;			// Hard-link count
	WORD		wAttribOffset;		// Offset to seq of Attributes
	WORD		wFlags;				// 0x01 = NonRes; 0x02 = Dir 低位表是否在使用中 次低位表是否为文件夹
	DWORD		dwRecLength;		// Real size of the record
	DWORD		dwAllLength;		// Allocated size of the record
	LONGLONG	n64BaseMftRec;		// ptr to base MFT rec or 0
	WORD		wNextAttrID;		// Minimum Identificator +1
	WORD		wFixupPattern;		// Current fixup pattern
	DWORD		dwMFTRecNumber;		// Number of this MFT Record
	// followed by resident and
	// part of non-res attributes
}NTFS_MFT_FILE;


typedef struct _RESIDENT
{
	DWORD	dwLength;
	WORD	wAttrOffset;
	BYTE	uchIndexedTag;
	BYTE	uchPadding;
}RESIDENT;

typedef struct _NONRESIDENT
{
	LONGLONG	n64StartVCN;
	LONGLONG	n64EndVCN;
	WORD		wDatarunOffset;
	WORD		wCompressionSize; // compression unit size
	BYTE		uchPadding[4];
	LONGLONG	n64AllocSize;
	LONGLONG	n64RealSize;
	LONGLONG	n64StreamSize;
	// data runs...
}NONRESIDENT;

typedef struct	// if resident then + RESIDENT
{					//  else + NONRESIDENT
	DWORD	dwType;
	DWORD	dwFullLength;
	BYTE	uchNonResFlag;
	BYTE	uchNameLength;
	WORD	wNameOffset;
	WORD	wFlags;
	WORD	wID;

	union ATTR
	{
		RESIDENT Resident;
		NONRESIDENT NonResident;
	}Attr;
} NTFS_ATTRIBUTE;

//////////////////////////////////////////////////////////////////////////////////////////

///////////////////////// Attributes /////////////////////////////////////////////////////
typedef struct
{
	LONGLONG	n64Create;		// Creation time
	LONGLONG	n64Modify;		// Last Modify time
	LONGLONG	n64Modfil;		// Last modify of record
	LONGLONG	n64Access;		// Last Access time
	DWORD		dwFATAttributes;// As FAT + 0x800 = compressed
	DWORD		dwReserved1;	// unknown

} ATTR_STANDARD;   

typedef struct
{
	LONGLONG	dwMftParentDir;            // Seq-nr parent-dir MFT entry  父目录记录号(前6个字节)+序列号(与目录相关)
	LONGLONG	n64Create;                  // Creation time
	LONGLONG	n64Modify;                  // Last Modify time
	LONGLONG	n64Modfil;                  // Last modify of record
	LONGLONG	n64Access;                  // Last Access time
	LONGLONG	n64Allocated;               // Allocated disk space
	LONGLONG	n64RealSize;                // Size of the file
	DWORD		dwFlags;					// attribute
	DWORD		dwEAsReparsTag;				// Used by EAs and Reparse
	BYTE		chFileNameLength;
	BYTE		chFileNameType;            // 8.3 / Unicode
	WORD		wFilename[512];             // Name (in Unicode ?)

}ATTR_FILENAME; 
//////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	ULONG EntriesOffset;
	ULONG IndexBlockLength;
	ULONG AllocatedSize;
	ULONG Flags; // 0x00 = Small directory, 0x01 = Large directory
} DIRECTORY_INDEX, *PDIRECTORY_INDEX;


/*
0X00 8 文件的MFT记录号 
0X08 2 索引项大小 
0X0A 2 名称偏移 
0X0C 4 索引标志＋填充 
0X10 8 父目录的MFT文件参考号 
0X18 8 文件创建时间 
0X20 8 文件修改时间 
0X28 8 文件最后修改时间 
0X30 8 文件最后访问时间 
0X38 8 文件分配大小 
0X40 8 文件实际大小 
0X48 8 文件标志 
0X50 1 文件名长度（F） 
0X51 1 文件名命名空间 
0X52 2F 文件名（填充到8字节） 
0X52+2F P 
0X52 +P+2F 8 子节点索引缓存的VCL 
*/
typedef struct {
	ULONGLONG FileReferenceNumber;
	USHORT Length;
	USHORT AttributeLength;
	ULONG Flags; // 0x01 = Has trailing VCN, 0x02 = Last entry
	ATTR_FILENAME FileName;
} DIRECTORY_ENTRY, *PDIRECTORY_ENTRY;

typedef struct {
	DWORD Type;
	ULONG CollationRule;
	ULONG BytesPerIndexBlock;
	ULONG ClustersPerIndexBlock;
	DIRECTORY_INDEX DirectoryIndex;
} INDEX_ROOT, *PINDEX_ROOT;

typedef struct {
	ULONG Index;
	USHORT UsaOffset;
	USHORT UsaCount;
	ULONGLONG Usn;
	ULONGLONG IndexBlockVcn;
	DIRECTORY_INDEX DirectoryIndex;
} INDEX_BLOCK_HEADER, *PINDEX_BLOCK_HEADER;



#pragma pack(pop, curAlignment)




// class CControlNtfs
// {
// private:
// protected:
// 	HANDLE	hDesk;
// 	DWORD dwSectionsPerCluster;
// 	DWORD dwBytesPerSection;
// 	DWORD dwClustersPerIndex;//每条索引的簇数
// 	DWORD dwSectionsPerMFT;//第条MFT记录所需的扇区数
// 	ULONGLONG uMftSection;//第一条MFT记录所在的扇区
// 	ULONGLONG uMftNum;//MFT记录条数（一个文件一条）
// 	ULONGLONG uIndexRootMft;//根目录的MFT序号（0为下标）,一般为5
// 	bool bIsInit;
// 	//NTFS_MFT_FILE NtfsMft;
// 	NTFS_PART_BOOT_SEC PartBoot;
// 
// public:
// 	CControlNtfs(void);
// 	~CControlNtfs(void);
// 
// 	bool OpenDesk(const char Desk);
// 	bool Init();
// 	bool CControlNtfs::ReadSection(LONGLONG n64Section,IN DWORD dwSectionNum, BYTE *pData);
// 	bool CControlNtfs::GetFileSection(const WCHAR *FilePath,LONGLONG *Section,DWORD *OffsetInSection,LONGLONG dwMftParentDir=-1);
// 	bool CControlNtfs::ExtractData(const NTFS_ATTRIBUTE *pNtfsAttr,BYTE *pMft,DWORD dwCurPos,BYTE *&puchData, DWORD &dwDataLen,LONGLONG *Section=NULL,DWORD *OffsetInSection=NULL,bool bIsReadAll=false);
// 	bool CControlNtfs::GetMFTDataByLCV(LONGLONG LCV);
// 
// 	bool CControlNtfs::GetFileSectionByName(const WCHAR *Path,LONGLONG *Section,DWORD *OffsetInSection,ULONGLONG uMftIndex=-1);
// 	bool CControlNtfs::GetDataByMftIndex(ULONGLONG uMftIndex,LONGLONG *Section,DWORD *OffsetInSection,BYTE *pData,DWORD *puDataSize,bool bIsReadAll=false);
// };
