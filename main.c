/* SPDX-License-Identifier: BSD-3-Clause */

#include <Uefi.h>
#include <GlobalTable.h>
#include <Guid.h>

#include <Pi/PiMultiPhase.h>
#include <Protocol/MpService.h>

#include "utils.h"

EFI_STATUS
EFIAPI
GetNumberOfProcessorsTest (
  IN EFI_MP_SERVICES_PROTOCOL *MpServices
  )
{
  UINTN NumberOfProcessors;
  UINTN NumberOfEnabledProcessors;
  EFI_STATUS Status;

  if (!MpServices)
    return EFI_UNSUPPORTED;

  printf ("[+] GetNumberOfProcessors test\r\n");
  Status = MpServices->GetNumberOfProcessors (MpServices, &NumberOfProcessors,
                                              &NumberOfEnabledProcessors);
  if (EFI_ERROR (Status)) {
    printf ("[!] GetNumberOfProcessors: 0x%x\r\n", Status);
    return Status;
  }

  printf ("    NumberOfProcessors: %d\r\n", NumberOfProcessors);
  printf ("    NumberOfEnabledProcessors: %d\r\n", NumberOfEnabledProcessors);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SwitchBSPTest (
  IN EFI_MP_SERVICES_PROTOCOL *MpServices
  )
{
  UINTN ProcessorNumber;
  UINTN SwitchToProcessorNumber;
  EFI_STATUS Status;

  if (!MpServices)
    return EFI_UNSUPPORTED;

  printf ("[+] SwitchBSP test\r\n");
  Status = MpServices->WhoAmI (MpServices, &ProcessorNumber);
  if (EFI_ERROR (Status)) {
    printf ("[!] WhoAmI: 0x%x\r\n", Status);
    return Status;
  }
  printf ("    Current Executing Processor: %d\r\n", ProcessorNumber);

  SwitchToProcessorNumber = 1;
  printf ("    Switching BSP from %d to %d\r\n", ProcessorNumber,
                                                SwitchToProcessorNumber);
  Status = MpServices->SwitchBSP (MpServices, SwitchToProcessorNumber, TRUE);
  if (EFI_ERROR (Status)) {
    printf ("[!] SwitchBSP: 0x%x\r\n", Status);
    return Status;
  }

  Status = MpServices->WhoAmI (MpServices, &ProcessorNumber);
  if (EFI_ERROR (Status)) {
    printf ("[!] WhoAmI: 0x%x\r\n", Status);
    return Status;
  }
  printf ("    Current Executing Processor: %d\r\n", ProcessorNumber);

  SwitchToProcessorNumber = 0;
  printf ("    Switching BSP from %d to %d\r\n", ProcessorNumber,
                                                SwitchToProcessorNumber);
  Status = MpServices->SwitchBSP (MpServices, 0, TRUE);
  if (EFI_ERROR (Status)) {
    printf ("[!] SwitchBSP: 0x%x\r\n", Status);
    return Status;
  }

  Status = MpServices->WhoAmI (MpServices, &ProcessorNumber);
  if (EFI_ERROR (Status)) {
    printf ("[!] WhoAmI: 0x%x\r\n", Status);
    return Status;
  }
  printf ("    Current Executing Processor: %d\r\n", ProcessorNumber);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EfiMain (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_MP_SERVICES_PROTOCOL *MpServices;
  EFI_STATUS Status;

  gImageHandle = ImageHandle;
  gST = SystemTable;
  gBS = gST->BootServices;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL,
                                (VOID **)&MpServices);
  if (EFI_ERROR (Status)) {
    printf ("[!] LocateProtocol: 0x%x\r\n", Status);
    return Status;
  }

  Status = GetNumberOfProcessorsTest (MpServices);
  if (EFI_ERROR (Status))
    return Status;

  Status = SwitchBSPTest (MpServices);
  if (EFI_ERROR (Status))
    return Status;

  return EFI_SUCCESS;
}
