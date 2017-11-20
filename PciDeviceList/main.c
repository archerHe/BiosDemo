#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "PciDeviceListByProtocol.h"


INTN ShellAppMain(IN UINTN Argc, IN CHAR16 * * Argv)
{
  ListPciDeviceByProtocol();
  
  return 0;
}




