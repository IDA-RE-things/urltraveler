#include "stdafx.h"
#include "Decoder.h"
#include <stdio.h>
#include <wincrypt.h>

#define PAGE_SIZE 1024

HCRYPTPROV g_hProv = NULL;

BOOL Init()
{
	BOOL bResult; // eax@2

	if ( g_hProv )
	{
		bResult = TRUE;
	}
	else
	{
		if ( CryptAcquireContextA(&g_hProv, 0, "Microsoft Base Cryptographic Provider v1.0", 1u, 0)
			|| CryptAcquireContextA(&g_hProv, 0, "Microsoft Base Cryptographic Provider v1.0", 1u, 8u)
			|| (bResult = CryptAcquireContextA(&g_hProv, 0, "Microsoft Base Cryptographic Provider v1.0", 1u, 0xF0000000u)) != 0 )
			bResult = TRUE;
	}

	return bResult;
}

HCRYPTKEY CreateKey(const BYTE *a1, DWORD a2)
{
	HCRYPTKEY result;
	HCRYPTHASH v3; 
	HCRYPTKEY phKey;

	v3 = 0;
	if ( a1 && a2 )
	{
		if ( Init() )
		{
			if ( CryptCreateHash(g_hProv, 0x8004u, 0, 0, &v3) )
			{
				if ( CryptHashData(v3, a1, a2, 0) )
					CryptDeriveKey(g_hProv, 0x6801u, v3, 0, &phKey);
				CryptDestroyHash(v3);
			}
			result = phKey;
		}
		else
		{
			result = -1;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}



size_t get_file_size(FILE *pFile)
{
	long old_pos = ftell(pFile);

	fseek(pFile, 0, SEEK_END);

	size_t file_size = ftell(pFile);

	fseek(pFile, old_pos, SEEK_SET);

	return file_size;
}

int decode(std::string file_path, std::string &decode_content)
{
	BYTE init_array[16] = {0}; 
	FILE *data_file = fopen(file_path.c_str(), "rb");

	if (data_file == NULL)
	{
		return -1;
	}

	size_t file_size = get_file_size(data_file);
	BYTE *file_buffer = new BYTE[file_size];
	fread(file_buffer, 1, file_size, data_file);
	fclose(data_file);

	BYTE encode_page[0x800] = {0};
	BYTE *pt = encode_page;
	size_t key_size = file_buffer[20];
    //_maxthon3_default_storage_
	//Maxthon3_MxCmpUrl_Mood
	//Maxthon__WebSIteBooster
	//guestmaxthon3_favdb_txmood
	//guestmaxthon3_config_txmood
	//username+maxthon3_favdb_txmood

	HCRYPTKEY hCryptKey = CreateKey((BYTE *)"guestmaxthon3_favdb_txmood", 0x1a);

	for (int page_index = 0; page_index < file_size / PAGE_SIZE; page_index++)
	{
		DWORD dwPageSize = PAGE_SIZE;
		BYTE *page_content = &file_buffer[page_index * PAGE_SIZE];

		CryptDecrypt(hCryptKey, 0, 1, 0, page_content, &dwPageSize);
	
	}

	FILE *fOut = fopen("maxthon_a.db", "wb");

	fwrite(file_buffer, 1, file_size, fOut);

	decode_content.assign((char *)file_buffer, file_size);

	fclose(fOut);

	return 0;
}

int encode(std::string encode_content, std::string save_path)
{
	BYTE init_array[16] = {0}; 
	size_t encode_size = encode_content.size();

	if (encode_size == 0)
	{
		return -1;
	}

	BYTE *encode_buffer = new BYTE[encode_size];

	memcpy(encode_buffer, encode_content.c_str(), encode_size);

	BYTE encode_page[0x800] = {0};
	BYTE *pt = encode_page;
	size_t key_size = encode_buffer[20];

	for (int page_index = 0; page_index < encode_size / PAGE_SIZE; page_index++)
	{
		BYTE *page_content = &encode_buffer[page_index * PAGE_SIZE];

		
	}

	FILE *fOut = fopen(save_path.c_str(), "wb");

	fwrite(encode_buffer, 1, encode_size, fOut);

	fclose(fOut);

	return 0;
}



