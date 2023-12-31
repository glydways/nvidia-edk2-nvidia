/** @file
*
*  Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include <Uefi.h>
#include <IndustryStandard/Ipmi.h>
#include <Protocol/EdkIIRedfishCredential.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IpmiBaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RedfishCredentialLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#define REDFISH_IPMI_GROUP_EXTENSION                          0x52
#define REDFISH_IPMI_GET_BOOTSTRAP_CREDENTIALS_CMD            0x02
#define REDFISH_IPMI_BOOTSTRAP_CREDENTIAL_ENABLE              0xA5
#define REDFISH_IPMI_BOOTSTRAP_CREDENTIAL_DISABLE             0x00
#define REDFISH_IPMI_COMP_CODE_BOOTSTRAP_CREDENTIAL_DISABLED  0x80

//
// Per Redfish Host Interface Specification 1.3, The maximum lenght of
// username and password is 16 characters long.
//
#define USERNAME_MAX_LENGTH       16
#define PASSWORD_MAX_LENGTH       16
#define USERNAME_MAX_SIZE         (USERNAME_MAX_LENGTH + 1) // NULL terminator
#define PASSWORD_MAX_SIZE         (PASSWORD_MAX_LENGTH + 1) // NULL terminator
#define CREDENTIAL_VARIABLE_NAME  L"Partstooblaitnederc"

#pragma pack(1)
///
/// The definition of IPMI command to get bootstrap account credentials
///
typedef struct {
  UINT8    GroupExtensionId;
  UINT8    DisableBootstrapControl;
} IPMI_BOOTSTRAP_CREDENTIALS_COMMAND_DATA;

///
/// The response data of getting bootstrap credential
///
typedef struct {
  UINT8    CompletionCode;
  UINT8    GroupExtensionId;
  CHAR8    Username[USERNAME_MAX_LENGTH];
  CHAR8    Password[PASSWORD_MAX_LENGTH];
} IPMI_BOOTSTRAP_CREDENTIALS_RESULT_RESPONSE;

///
/// The bootstrap credential keeping in UEFI variable
///
typedef struct {
  CHAR8    Username[USERNAME_MAX_SIZE];
  CHAR8    Password[PASSWORD_MAX_SIZE];
} BOOTSTRAP_CREDENTIALS_VARIABLE;

#pragma pack()

/**
  Function to retrieve temporary user credentials for the UEFI redfish client

  @param[in]  DisableBootstrapControl
                                      TRUE - Tell the BMC to disable the bootstrap credential
                                             service to ensure no one else gains credentials
                                      FALSE  Allow the bootstrap credential service to continue
  @param[out] BootstrapUsername
                                      A pointer to a UTF-8 encoded string for the credential username

  @param[out] BootstrapPassword
                                      A pointer to a UTF-8 encoded string for the credential password

  @retval  EFI_SUCCESS                Credentials were successfully fetched and returned
  @retval  EFI_DEVICE_ERROR           An IPMI failure occurred
**/
EFI_STATUS
GetBootstrapAccountCredentials (
  IN BOOLEAN    DisableBootstrapControl,
  IN OUT CHAR8  *BootstrapUsername,
  IN OUT CHAR8  *BootstrapPassword
  );

/**
  Function to save temporary user credentials into boot time variable. When DeleteVariable is True,
  this function delete boot time variable.

  @param[in] BootstrapUsername       A pointer to a Ascii encoded string for the credential username.
  @param[in] BootstrapPassword       A pointer to a Ascii encoded string for the credential password.
  @param[in] DeleteVariable          True to remove boot time variable. False otherwise.

  @retval  EFI_SUCCESS                Credentials were successfully saved.
  @retval  EFI_INVALID_PARAMETER      BootstrapUsername or BootstrapPassword is NULL
  @retval  Others                     Error occurs
**/
EFI_STATUS
SetBootstrapAccountCredentialsToVariable (
  IN CHAR8 *BootstrapUsername, OPTIONAL
  IN CHAR8  *BootstrapPassword, OPTIONAL
  IN BOOLEAN DeleteVariable
  );
