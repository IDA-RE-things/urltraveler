#include <windows.h>
#include <wincrypt.h>


#include "../../sqliteInt.h"
#include "../../pager.h"
#include "crypt.h"

#ifdef SQLITE_HAS_CODEC
void sqlite3pager_free_codecarg(void *pArg);
#endif

//#undef  SQLITE_OMIT_DISKIO
#ifndef SQLITE_OMIT_DISKIO
#ifdef SQLITE_HAS_CODEC

// Extra padding before and after the cryptographic buffer
#define CRYPT_OFFSET 8

typedef struct _CRYPTBLOCK
{
  // 采用BlowFish加密算法读和写的两个加密/解密对象 [8/27/2008 温辉敏]
  void *ptrBlowFishCryptReadObj;	// Key used to read from the database and write to the journal
  void *ptrBlowFishCryptWriteObj;	// Key used to write to the database

  DWORD     dwPageSize;   // Size of pages
  LPVOID    pvCrypt;      // A buffer for encrypting/decrypting (if necessary)
  DWORD     dwCryptSize;  // Equal to or greater than dwPageSize.  If larger, pvCrypt is valid and this is its size
} CRYPTBLOCK, *LPCRYPTBLOCK;

HCRYPTPROV g_hProvider = 0; // Global instance of the cryptographic provider

#define SQLITECRYPTERROR_PROVIDER "Cryptographic provider not available"

// Needed for re-keying
static void * sqlite3pager_get_codecarg(Pager *pPager)
{
  return (pPager->xCodec) ? pPager->pCodecArg: NULL;
}

// Create a cryptographic context.  Use the enhanced provider because it is available on
// most platforms
static BOOL InitializeProvider()
{
  if (g_hProvider) return TRUE;

  if (!CryptAcquireContext(&g_hProvider, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
  {
    if(!CryptAcquireContext(&g_hProvider, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
    {
      return FALSE;
    }
  }
  return TRUE;
}

// Create or update a cryptographic context for a pager.
// This function will automatically determine if the encryption algorithm requires
// extra padding, and if it does, will create a temp buffer big enough to provide
// space to hold it.
static LPCRYPTBLOCK CreateCryptBlock(void *ptrBlowFishCryptObj, Pager *pager, LPCRYPTBLOCK pExisting)
{
	LPCRYPTBLOCK pBlock;

	if (!pExisting) // Creating a new cryptblock
	{
		pBlock = sqlite3_malloc(sizeof(CRYPTBLOCK));
		ZeroMemory(pBlock, sizeof(CRYPTBLOCK));
		pBlock->ptrBlowFishCryptReadObj = ptrBlowFishCryptObj;
		pBlock->ptrBlowFishCryptWriteObj = ptrBlowFishCryptObj;
	}
	else // Updating an existing cryptblock
	{
		pBlock = pExisting;
	}

	pBlock->dwPageSize = (DWORD)pager->pageSize;
	pBlock->dwCryptSize = pBlock->dwPageSize;

	// Existing cryptblocks may have a buffer, if so, delete it
	if (pBlock->pvCrypt)
	{
		sqlite3_free(pBlock->pvCrypt);
		pBlock->pvCrypt = NULL;
	}

	// Figure out how big to make our spare crypt block
	pBlock->dwCryptSize = BlowFishCWrapper_GetNecessaryBufLen(pBlock->dwCryptSize);
	pBlock->pvCrypt = sqlite3_malloc(pBlock->dwCryptSize + (CRYPT_OFFSET * 2));

	return pBlock;
}

// Destroy a cryptographic context and any buffers and keys allocated therein
static void DestroyCryptBlock(LPCRYPTBLOCK pBlock)
{
	if (NULL != pBlock->ptrBlowFishCryptReadObj)
	{
		BlowFishCWrapper_DestroyCryptObj(pBlock->ptrBlowFishCryptReadObj);
	}

	if (NULL != pBlock->ptrBlowFishCryptWriteObj && pBlock->ptrBlowFishCryptReadObj != pBlock->ptrBlowFishCryptWriteObj)
	{
		BlowFishCWrapper_DestroyCryptObj(pBlock->ptrBlowFishCryptWriteObj);
	}	

	// If there's extra buffer space allocated, free it as well
	if (pBlock->pvCrypt)
	{
		sqlite3_free(pBlock->pvCrypt);
	}

	// All done with this cryptblock
	sqlite3_free(pBlock);
}


// Encrypt/Decrypt functionality, called by pager.c
void * sqlite3Codec(void *pArg, void *data, Pgno nPageNum, int nMode)
{
	LPCRYPTBLOCK pBlock = (LPCRYPTBLOCK)pArg;
	DWORD dwPageSize;
	LPVOID pvTemp;

	if (!pBlock) return data;

	// Make sure the page size for the pager is still the same as the page size
	// for the cryptblock.  If the user changed it, we need to adjust!
	if (nMode != 2)
	{
		Pager pager = {0};
		pager.pageSize = pBlock->dwPageSize;
		if (pBlock->dwCryptSize != pBlock->dwPageSize)
		{
			// Update the cryptblock to reflect the new page size
			CreateCryptBlock(0, &pager, pBlock);
		}
	}

	switch(nMode)
	{
	case 0: // Undo a "case 7" journal file encryption
	case 2: // Reload a page
	case 3: // Load a page
		///if (!pBlock->hReadKey) break;
		if (!pBlock->ptrBlowFishCryptReadObj)
		{
			break;
		}

		/* Block ciphers often need to write extra padding beyond the 
		data block.  We don't have that luxury for a given page of data so
		we must copy the page data to a buffer that IS large enough to hold
		the padding.  We then encrypt the block and write the buffer back to
		the page without the unnecessary padding.
		We only use the special block of memory if its absolutely necessary. */
		if (pBlock->dwCryptSize != pBlock->dwPageSize)
		{
			CopyMemory(((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET, data, pBlock->dwPageSize);
			pvTemp = data;
			data = ((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET;
		}

		dwPageSize = pBlock->dwCryptSize;
		BlowFishCWrapper_Decode(pBlock->ptrBlowFishCryptReadObj, (LPBYTE)data, (LPBYTE)data, dwPageSize);

		// If the encryption algorithm required extra padding and we were forced to encrypt or
		// decrypt a copy of the page data to a temp buffer, then write the contents of the temp
		// buffer back to the page data minus any padding applied.
		if (pBlock->dwCryptSize != pBlock->dwPageSize)
		{
			CopyMemory(pvTemp, data, pBlock->dwPageSize);
			data = pvTemp;
		}
		break;
	case 6: // Encrypt a page for the main database file
		///if (!pBlock->hWriteKey) break;
		if (!pBlock->ptrBlowFishCryptWriteObj) break;

		CopyMemory(((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET, data, pBlock->dwPageSize);
		data = ((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET;

		dwPageSize = pBlock->dwPageSize;
		BlowFishCWrapper_Encode(
			pBlock->ptrBlowFishCryptWriteObj, 
			((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET, 
			((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET, 
			pBlock->dwCryptSize
			);
		break;
	case 7: // Encrypt a page for the journal file
		/* Under normal circumstances, the readkey is the same as the writekey.  However,
		when the database is being rekeyed, the readkey is not the same as the writekey.
		The rollback journal must be written using the original key for the
		database file because it is, by nature, a rollback journal.
		Therefore, for case 7, when the rollback is being written, always encrypt using
		the database's readkey, which is guaranteed to be the same key that was used to
		read the original data.
		*/
		if (!pBlock->ptrBlowFishCryptReadObj)
		{
			break;
		}

		CopyMemory(((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET, data, pBlock->dwPageSize);
		data = ((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET;

		dwPageSize = pBlock->dwPageSize;
		BlowFishCWrapper_Encode(
			pBlock->ptrBlowFishCryptReadObj, 
			((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET,
			((LPBYTE)pBlock->pvCrypt) + CRYPT_OFFSET, 
			pBlock->dwCryptSize
			);
		break;
	}

	return data;
}

/** 
这个函数好像是 sqlite 3.3.17前不久才加的，以前版本的sqlite里没有看到这个函数
这个函数我还没有搞清楚是做什么的，它里面什么都不做直接返回，对加解密没有影响
**/
void sqlite3_activate_see(const char* right )
{    
	return;
}


// Derive an encryption key from a user-supplied buffer
static HCRYPTKEY DeriveKey(const void *pKey, int nKeyLen)
{
  HCRYPTHASH hHash = 0;
  HCRYPTKEY  hKey;

  if (!pKey || !nKeyLen) return 0;

  if (!InitializeProvider())
  {
    return MAXDWORD;
  }

  if (CryptCreateHash(g_hProvider, CALG_SHA1, 0, 0, &hHash))
  {
    if (CryptHashData(hHash, (LPBYTE)pKey, nKeyLen, 0))
    {
      CryptDeriveKey(g_hProvider, CALG_RC4, hHash, 0, &hKey);
    }
    CryptDestroyHash(hHash);
  }  
  return hKey;
}

// Called by sqlite and sqlite3_key_interop to attach a key to a database.
int sqlite3CodecAttach(sqlite3 *db, int nDb, const void *pKey, int nKeyLen)
{
  int rc = SQLITE_ERROR;
  void *ptrCryptObj = NULL;

  // No key specified, could mean either use the main db's encryption or no encryption
  if (!pKey || !nKeyLen)
  {
    if (!nDb)
    {
      return SQLITE_OK; // Main database, no key specified so not encrypted
    }
    else // Attached database, use the main database's key
    {
      // Get the encryption block for the main database and attempt to duplicate the key
      // for use by the attached database
      LPCRYPTBLOCK pBlock = (LPCRYPTBLOCK)sqlite3pager_get_codecarg(sqlite3BtreePager(db->aDb[0].pBt));

      if (!pBlock) return SQLITE_OK; // Main database is not encrypted so neither will be any attached database
	  if (!pBlock->ptrBlowFishCryptReadObj) return SQLITE_OK; // Not encrypted

	  ptrCryptObj = BlowFishCWrapper_DuplicateCryptObj(pBlock->ptrBlowFishCryptReadObj);
	  if (NULL == ptrCryptObj)
	  {
		  return rc;
	  }
    }
  }
  else // User-supplied passphrase, so create a cryptographic key out of it
  {
	  ptrCryptObj = BlowFishCWrapper_CreateCryptObj(pKey, nKeyLen);
	  if (NULL == ptrCryptObj)
	  {
		  sqlite3Error(db, rc, SQLITECRYPTERROR_PROVIDER);
		  return rc;
	  }
  }

  // Create a new encryption block and assign the codec to the new attached database
  ///if (hKey)
  if(NULL != ptrCryptObj)
  {
    LPCRYPTBLOCK pBlock = CreateCryptBlock(ptrCryptObj, sqlite3BtreePager(db->aDb[nDb].pBt), NULL);
	// 函数名改变 [7/11/2008 温辉敏]
    ///sqlite3pager_set_codec(sqlite3BtreePager(db->aDb[nDb].pBt), sqlite3Codec, pBlock);
	sqlite3PagerSetCodec(sqlite3BtreePager(db->aDb[nDb].pBt), sqlite3Codec, pBlock);
    rc = SQLITE_OK;
  }
  return rc;
}

// Called by our code modification to pager.c to free the cryptblock associated with 
// a pager instance.
void sqlite3pager_free_codecarg(void *pArg)
{
  if (pArg)
    DestroyCryptBlock((LPCRYPTBLOCK)pArg);
}

// Once a password has been supplied and a key created, we don't keep the 
// original password for security purposes.  Therefore return NULL.
void sqlite3CodecGetKey(sqlite3 *db, int nDb, void **ppKey, int *pnKeyLen)
{
  *ppKey = NULL;
  *pnKeyLen = 0;
}

// We do not attach this key to the temp store, only the main database.
int WINAPI sqlite3_key_interop(sqlite3 *db, const void *pKey, int nKeySize)
{
  return sqlite3CodecAttach(db, 0, pKey, nKeySize);
}

// Changes the encryption key for an existing database.
int WINAPI sqlite3_rekey_interop(sqlite3 *db, const void *pKey, int nKeySize)
{
  Btree *pbt = db->aDb[0].pBt;
  Pager *p = sqlite3BtreePager(pbt);
  LPCRYPTBLOCK pBlock = (LPCRYPTBLOCK)sqlite3pager_get_codecarg(p);  

  ///Create 加密BlowFish对象 [8/27/2008 温辉敏]
  void *ptrBlowFishCryptObj = BlowFishCWrapper_CreateCryptObj(pKey, nKeySize);

  int rc = SQLITE_ERROR;

  ///if (hKey == MAXDWORD)
  if(NULL == ptrBlowFishCryptObj)
  {
    sqlite3Error(db, rc, SQLITECRYPTERROR_PROVIDER);
    return rc;
  }

  ///if (!pBlock && !hKey) return SQLITE_OK; // Wasn't encrypted to begin with
  if (!pBlock && !ptrBlowFishCryptObj) return SQLITE_OK; // Wasn't encrypted to begin with

  // To rekey a database, we change the writekey for the pager.  The readkey remains
  // the same
  if (!pBlock) // Encrypt an unencrypted database
  {
    pBlock = CreateCryptBlock(ptrBlowFishCryptObj, p, NULL);
    pBlock->ptrBlowFishCryptReadObj = NULL; // Original database is not encrypted

	// 函数名改变 [7/11/2008 温辉敏]
    //sqlite3pager_set_codec(sqlite3BtreePager(pbt), sqlite3Codec, pBlock);
	sqlite3PagerSetCodec(sqlite3BtreePager(pbt), sqlite3Codec, pBlock);
  }
  else // Change the writekey for an already-encrypted database
  {
	  pBlock->ptrBlowFishCryptWriteObj = ptrBlowFishCryptObj;
    ///pBlock->hWriteKey = hKey;
  }

  // Start a transaction
  rc = sqlite3BtreeBeginTrans(pbt, 1);

  if (!rc)
  {
    // Rewrite all the pages in the database using the new encryption key
	// 函数名变动修改[7/11/2008 温辉敏]
    // Pgno nPage = sqlite3pager_pagecount(p);
	Pgno nPage = sqlite3PagerPagecount(p);
    Pgno nSkip = PAGER_MJ_PGNO(p);
    void *pPage;
    Pgno n;

    for(n = 1; rc == SQLITE_OK && n <= nPage; n ++)
    {
      if (n == nSkip) continue;
	  // 函数名变动修改[7/11/2008 温辉敏]
      // rc = sqlite3pager_get(p, n, &pPage);
	  rc = sqlite3PagerGet(p, n, &pPage);
      if(!rc)
      {
		// 函数名变动修改[7/11/2008 温辉敏]
        //rc = sqlite3pager_write(pPage);
		rc = sqlite3PagerWrite(pPage);

        // 函数名变动修改[7/11/2008 温辉敏]
		//  sqlite3pager_unref(pPage);
		sqlite3PagerUnref(pPage);
      }
    }
  }

  // If we succeeded, try and commit the transaction
  if (!rc)
  {
    rc = sqlite3BtreeCommit(pbt);
  }

  // If we failed, rollback
  if (rc)
  {
    sqlite3BtreeRollback(pbt);
  }

  // If we succeeded, destroy any previous read key this database used
  // and make the readkey equal to the writekey
  if (!rc)
  {
	  if (NULL != pBlock->ptrBlowFishCryptReadObj)
	  {
		 BlowFishCWrapper_DestroyCryptObj(pBlock->ptrBlowFishCryptReadObj);
	  }

	  pBlock->ptrBlowFishCryptReadObj = pBlock->ptrBlowFishCryptWriteObj;

  }
  // We failed.  Destroy the new writekey (if there was one) and revert it back to
  // the original readkey
  else
  {
	  if (NULL != pBlock->ptrBlowFishCryptWriteObj)
	  {
		  BlowFishCWrapper_DestroyCryptObj(pBlock->ptrBlowFishCryptWriteObj);
	  }

	  pBlock->ptrBlowFishCryptWriteObj = pBlock->ptrBlowFishCryptReadObj;
  }

  // If the readkey and writekey are both empty, there's no need for a codec on this
  // pager anymore.  Destroy the crypt block and remove the codec from the pager.
  if (NULL == pBlock->ptrBlowFishCryptReadObj && NULL == pBlock->ptrBlowFishCryptWriteObj)
  {
	// 函数名变动修改 [7/11/2008 温辉敏]
    ///sqlite3pager_set_codec(p, NULL, NULL);
	sqlite3PagerSetCodec(p, NULL, NULL);
    DestroyCryptBlock(pBlock);
  }

  return rc;
}

int sqlite3_key(sqlite3 *db, const void *pKey, int nKey)
{
  return sqlite3_key_interop(db, pKey, nKey);
}

int sqlite3_rekey(sqlite3 *db, const void *pKey, int nKey)
{
  return sqlite3_rekey_interop(db, pKey, nKey);
}

#endif // SQLITE_HAS_CODEC

#endif // SQLITE_OMIT_DISKIO
