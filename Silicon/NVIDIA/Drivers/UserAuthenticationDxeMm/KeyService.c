/** @file
  Password key service.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseCryptLib.h>
#include "KeyService.h"

/**
  Compares the contents of two buffers with slow algorithm

  This function compares Length bytes of SourceBuffer to Length bytes of DestinationBuffer.
  If all Length bytes of the two buffers are identical, then 0 is returned.  Otherwise, the
  value returned is the first mismatched byte in SourceBuffer subtracted from the first
  mismatched byte in DestinationBuffer.

  If Length > 0 and DestinationBuffer is NULL, then ASSERT().
  If Length > 0 and SourceBuffer is NULL, then ASSERT().
  If Length is greater than (MAX_ADDRESS - DestinationBuffer + 1), then ASSERT().
  If Length is greater than (MAX_ADDRESS - SourceBuffer + 1), then ASSERT().

  @param  DestinationBuffer The pointer to the destination buffer to compare.
  @param  SourceBuffer      The pointer to the source buffer to compare.
  @param  Length            The number of bytes to compare.

  @return 0                 All Length bytes of the two buffers are identical.
  @retval -1                The SourceBuffer is not identical to DestinationBuffer.

**/
INTN
EFIAPI
KeyLibSlowCompareMem (
  IN CONST VOID  *DestinationBuffer,
  IN CONST VOID  *SourceBuffer,
  IN UINTN       Length
  )
{
  UINT8  Delta;
  UINTN  Index;
  UINT8  *Destination;
  UINT8  *Source;

  Destination = (UINT8 *)DestinationBuffer;
  Source      = (UINT8 *)SourceBuffer;
  Delta       = 0;
  for (Index = 0; Index < Length; Index++) {
    Delta |= Destination[Index] ^ Source[Index];
  }

  if (Delta == 0) {
    return 0;
  } else {
    return -1;
  }
}

/**
  Generate Salt value.

  @param[in, out]   SaltValue           Points to the salt buffer
  @param[in]        SaltSize            Size of the salt buffer

  @retval      TRUE           Salt is generated.
  @retval      FALSE          Salt is not generated.
**/
BOOLEAN
EFIAPI
KeyLibGenerateSalt (
  IN OUT UINT8  *SaltValue,
  IN UINTN      SaltSize
  )
{
  if (SaltValue == NULL) {
    return FALSE;
  }

  RandomSeed (NULL, 0);
  RandomBytes (SaltValue, SaltSize);
  return TRUE;
}

/**
  Hash the password with PBKDF2.

  @param[in]   HashType         Hash type
  @param[in]   Key              Points to the key buffer
  @param[in]   KeySize          Key buffer size
  @param[in]   SaltValue        Points to the salt buffer
  @param[in]   SaltSize         Size of the salt buffer
  @param[out]  KeyHash          Points to the hashed result
  @param[in]   KeyHashSize      Size of the hash buffer

  @retval      TRUE           Hash the data successfully.
  @retval      FALSE          Failed to hash the data.

**/
BOOLEAN
EFIAPI
KeyLibGeneratePBKDF2Hash (
  IN   UINT32  HashType,
  IN   VOID    *Key,
  IN   UINTN   KeySize,
  IN   UINT8   *SaltValue,
  IN   UINTN   SaltSize,
  OUT  UINT8   *KeyHash,
  IN   UINTN   KeyHashSize
  )
{
  BOOLEAN  Result;

  if (HashType != HASH_TYPE_SHA256) {
    return FALSE;
  }

  if (KeyHashSize != SHA256_DIGEST_SIZE) {
    return FALSE;
  }

  Result = Pkcs5HashPassword (
             KeySize,
             Key,
             SaltSize,
             SaltValue,
             DEFAULT_PBKDF2_ITERATION_COUNT,
             SHA256_DIGEST_SIZE,
             KeyHashSize,
             KeyHash
             );
  return Result;
}
