/** @file
  Header file for UserAuthenticationSmm.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __USER_AUTHENTICATION_MM_H__
#define __USER_AUTHENTICATION_MM_H__

#include <PiMm.h>

#include <Protocol/SmmVariable.h>

#include <Guid/UserAuthentication.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/MmServicesTableLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/PlatformPasswordLib.h>

#include "KeyService.h"

#define PASSWORD_SALT_SIZE  32
#define PASSWORD_HASH_SIZE  32  // SHA256_DIGEST_SIZE

#define PASSWORD_MAX_TRY_COUNT        5
#define PASSWORD_HISTORY_CHECK_COUNT  5

//
// Name of the variable
//
#define USER_AUTHENTICATION_VAR_NAME               L"Password"
#define USER_AUTHENTICATION_HISTORY_LAST_VAR_NAME  L"PasswordLast"

//
// Variable storage
//
typedef struct {
  UINT8    PasswordHash[PASSWORD_HASH_SIZE];
  UINT8    PasswordSalt[PASSWORD_SALT_SIZE];
} USER_PASSWORD_VAR_STRUCT;

#endif
