#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include <Guid/AcpiS3Context.h>
#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>


UINTN  OUTPUT_CHAR_FORMAT = 1;
UINTN  OUTPUT_INT_FORMAT = 2;

#define CUSTOM_BUF_SIZE 128
#define DATA_ROW_FORMAT 16
#define DATA_ROW_FORMAT_CHAR 80



UINTN StringLength(CHAR16 *Str)
{
  if(Str == NULL){
    return 0;
  }
  UINTN length = 0;
  while(1){
    if(Str[length] != L'\0'){
      length++;
    }else{
      return length + 1;
    }
  }
}


BOOLEAN CompareString(CHAR16 *Str1, CHAR16 *Str2)
{
  if(Str1 == NULL || Str2 == NULL){
    return FALSE;
  }
  if(StringLength(Str1) != StringLength(Str2)){
    return FALSE;
  }
  for(UINTN i = 0; i < StringLength(Str1) - 1; i++){
    if(Str1[i] != Str2[i]){
      return FALSE;
    }
  }
  return TRUE;
  
}


CHAR16 *Signature2String(UINT64 Signature, UINTN Size)
{ 
  CHAR16 *Str_Signature = AllocatePool(sizeof(CHAR16) * 9);
  for (UINTN i = 0; i < Size; i++)
  {
    //Print(L"char: %c\n", (CHAR16)(signature & 0xFF));
    Str_Signature[i] = (CHAR16)(Signature & 0xFF);
    Signature = Signature >> 8;
  }
  Str_Signature[8] = '\0';
  //Print (L"string: %s   addr: %x\n", Str_Signature, Str_Signature);
  return Str_Signature;
}

void PrintFromArray(UINT8 *OemId, UINTN Length, UINTN OutputFormat)
{
  
  for (UINTN i = 0; i < Length; i++)
  {
    if(OutputFormat == OUTPUT_CHAR_FORMAT){
      Print (L"%c", (CHAR8)(*OemId));
    }else if(OutputFormat == OUTPUT_INT_FORMAT){
      Print (L"0x%x", (CHAR8)(*OemId));
    } 
    OemId++;
  }
  Print(L"\n");
}

void WaitForAnyKey(){
/*
  Print (L"Press any key for continue................\n");
  EFI_STATUS Status = 0;
  UINTN Index = 0;
  EFI_EVENT Event[2] = {0};
  Event[0] = gST->ConIn->WaitForKey;
  Status = gBS->CreateEvent(EVT_TIMER, TPL_CALLBACK, (EFI_EVENT_NOTIFY) NULL, (VOID *)NULL, &Event[1]);
  Status = gBS->SetTimer(Event[1], TimerPeriodic, 600 * 1000 * 1000);
  Status = gBS->WaitForEvent(2, Event, &Index);
  EFI_INPUT_KEY Key;
  
  UINTN Count = 0;
  while(1){
    if(Status == EFI_SUCCESS){
    if(Index == 0){
      Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
      break;
    }else if(Index == 1){
      //Status = gBS->WaitForEvent(2, Event, &Index);
      break;
    }  
  }
  }
  gBS->CloseEvent(Event);
  */
}







VOID *PrintRSDPStruct(EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *Rsdp)
{
  Print (L"\n");
  Print (L"===================RSDP==================\n");
  CHAR16 *Signature = Signature2String(Rsdp->Signature, sizeof(Rsdp->Signature));
  Print (L"Signature: %s\n", Signature);
  FreePool(Signature);
  Print (L"Checksum: 0x%x\n", Rsdp->Checksum);
  Print (L"oem id: ");
  PrintFromArray(Rsdp->OemId, sizeof(Rsdp->OemId), OUTPUT_CHAR_FORMAT);
  Print (L"Revision: 0x%x\n", Rsdp->Revision);
  Print (L"RsdtAddress: 0x%x\n", Rsdp->RsdtAddress);
  Print (L"Length: 0x%x\n", Rsdp->Length);
  Print (L"XsdtAddress: 0x%x\n", Rsdp->XsdtAddress);
  Print (L"Extended Checksum: 0x%x\n", Rsdp->ExtendedChecksum);
  Print (L"Reserved: ");
  PrintFromArray(Rsdp->Reserved, sizeof(Rsdp->Reserved), OUTPUT_INT_FORMAT);
  Print (L"===================RSDP==================\n");
  Print (L"\n");
}


VOID *GetRsdpFromConfigurationTables ()
{ 
  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Rsdt;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE     *Fadt;
  EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *Facs;
  UINTN Index;
  for (Index = 0; Index < gST->NumberOfTableEntries; Index++)
  {
    //Print(L"%03d    %g\n", Index, &(gST->ConfigurationTable[Index].VendorGuid));
    if (CompareGuid(&(gST->ConfigurationTable[Index].VendorGuid), gEfiAcpi20TableGuid))
    {
      //Print (L"Found ACPI Table \n");  
      Rsdp = gST->ConfigurationTable[Index].VendorTable;
      break;
    }
  }

  if (Rsdp != NULL){
    PrintRSDPStruct(Rsdp);
    return Rsdp;
  }
}


EFI_STATUS PrintXsdtStruct(EFI_ACPI_DESCRIPTION_HEADER        *Xsdt)
{
  Print (L"\n");
  Print (L"===================XSDT==================\n");
  CHAR16 *Signature = Signature2String(Xsdt->Signature, sizeof(Xsdt->Signature));
  Print (L"Signature: %s\n", Signature);
  Print (L"Length: 0x%x\n", Xsdt->Length);
  Print (L"Revision: 0x%x\n", Xsdt->Revision);
  Print (L"Checksum: 0x%x\n", Xsdt->Checksum);
  Print (L"Oem id: ");
  PrintFromArray(Xsdt->OemId, sizeof(Xsdt->OemId), OUTPUT_CHAR_FORMAT);
  CHAR16 *OemTableId = Signature2String(Xsdt->OemTableId, sizeof(Xsdt->OemTableId));
  Print (L"OemTableId: %s\n", OemTableId);
  Print (L"OemRevision: 0x%x\n", Xsdt->OemRevision);
  CHAR16 *CreatorId = Signature2String(Xsdt->CreatorId, sizeof(Xsdt->CreatorId));
  Print (L"CreatorId: %s\n", CreatorId);
  Print (L"CreatorRevision: 0x%x\n", Xsdt->CreatorRevision);
  Print (L"===================XSDT==================\n");
  Print (L"\n");
  FreePool(Signature);
  FreePool(OemTableId);
  FreePool(CreatorId);

}

VOID *GetTableFromXsdt(EFI_ACPI_DESCRIPTION_HEADER        *Xsdt, UINT32 Signature)
{
  UINT64 BaseAdddr = Xsdt + 1;
  UINT32 EntryCount = (Xsdt->Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER)) / sizeof(UINT64);
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  UINT32 EntryAddr;
  for(UINTN i = 0; i< EntryCount; i++){
    CopyMem (&EntryAddr, (VOID *)(BaseAdddr + i * sizeof(UINT64)), sizeof(UINT64));
    Table = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)(EntryAddr));
    if (Table->Signature == Signature) {
      return Table;
    }
  }

}

VOID *PrintTableHeader(EFI_ACPI_DESCRIPTION_HEADER *Table){
  CHAR16 *Signature_Tmp = Signature2String(Table->Signature, sizeof(Table->Signature));
  CHAR16 Signature[5] = L"abcd";
    for(UINTN j = 0; j < 5 - 1; j++){
      Signature[j] = Signature_Tmp[j];
    }
  Print (L"\n");
  Print (L"\n");
  Print (L"===================%s Header start==================\n", Signature);
  Print (L"Signature: %s\n", Signature);
  Print (L"Length: 0x%x\n", Table->Length);
  Print (L"Revision: 0x%x\n", Table->Revision);
  Print (L"Checksum: 0x%x\n", Table->Checksum);
  Print (L"Oem id: ");
  PrintFromArray(Table->OemId, sizeof(Table->OemId), OUTPUT_CHAR_FORMAT);
  CHAR16 *OemTableId = Signature2String(Table->OemTableId, sizeof(Table->OemTableId));
  Print (L"OemTableId: %s\n", OemTableId);
  Print (L"OemRevision: 0x%x\n", Table->OemRevision);
  CHAR16 *CreatorId = Signature2String(Table->CreatorId, sizeof(Table->CreatorId));
  Print (L"CreatorId: %s\n", CreatorId);
  Print (L"CreatorRevision: 0x%x\n", Table->CreatorRevision);
  Print (L"===================%s Header  end==================\n");
  Print (L"\n");
  Print (L"\n");
  FreePool(Signature);
  FreePool(OemTableId);
  FreePool(CreatorId);
}


VOID *PrintAcpi20GenericAddrStuct(EFI_ACPI_2_0_GENERIC_ADDRESS_STRUCTURE *GenAddrStruct){ 
  Print (L"AddressSpaceId:  0x%x ", GenAddrStruct->AddressSpaceId);
  Print (L"RegisterBitWidth:  0x%x ", GenAddrStruct->RegisterBitWidth);
  Print (L"RegisterBitOffset: 0x%x  ", GenAddrStruct->RegisterBitOffset);
  Print (L"Reserved:  0x%x  ", GenAddrStruct->Reserved);
  Print (L"Address: 0x%x  ", GenAddrStruct->Address);
  Print (L"\n");
}

VOID *PrintFacsFromXsdt(EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *Facs)
{
  Print (L"\n");
  CHAR16 *Signature = Signature2String(Facs->Signature, sizeof(Facs->Signature));
  Print (L"===================%s==================\n", Signature);
  Print (L"Signature:   %s\n", Signature);
  Print (L"Length:    0x%x\n", Facs->Length);
  CHAR16 *HardwareSignature = Signature2String(Facs->HardwareSignature, sizeof(Facs->HardwareSignature));
  Print (L"HardwareSignature:   %s\n", HardwareSignature);
  FreePool(HardwareSignature);
  Print (L"FirmwareWakingVector:    0x%x\n", Facs->FirmwareWakingVector);
  Print (L"GlobalLock:    0x%x\n", Facs->GlobalLock);
  Print (L"Flags:     0x%x\n", Facs->Flags);
  Print (L"XFirmwareWakingVector:   0x%x\n", Facs->XFirmwareWakingVector);
  Print (L"Version:     0x%x\n", Facs->Version);
  PrintFromArray(Facs->Reserved, sizeof(Facs->Reserved), OUTPUT_INT_FORMAT);
  Print (L"===================%s==================\n", Signature);
  FreePool(Signature);
  Print (L"\n");
}

VOID *PrintCommonTableContentAsHex(EFI_ACPI_DESCRIPTION_HEADER *Table)
{
  UINT64 BaseAddr = Table + 1;
  Print (L"Data Area(Hex): \n");
  for(UINTN i = 0; i < Table->Length - sizeof(*Table); i++){
    Print (L"%02x ", *(UINT8*)BaseAddr);
    if((i + 1) % DATA_ROW_FORMAT == 0){
      Print (L"\n");
    }
    BaseAddr = (UINT8*)BaseAddr + 1;
  }
}

VOID *PrintCommonTableContentAsChar(EFI_ACPI_DESCRIPTION_HEADER *Table)
{
  UINT64 BaseAddr = Table + 1;
  Print (L"Data Area(Char): \n");
  for(UINTN i = 0; i < Table->Length - sizeof(*Table); i++){
    Print (L"%c ", *(UINT8*)BaseAddr);
    if((i + 1) % DATA_ROW_FORMAT_CHAR == 0){
      Print (L"\n");
    }
    BaseAddr = (UINT8*)BaseAddr + 1;
  }
}


VOID *PrintFadtTable(EFI_ACPI_DESCRIPTION_HEADER *Fadt){
  //Print (L"Fadt header sizeof:  0x%x    addr:0x%x\n",sizeof(*Fadt), Fadt);
  WaitForAnyKey();
  
  //EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *FadtBody = (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *)(Fadt + 8 * 36);
  //UINTN            BasePtr;
  //BasePtr = (UINTN)(Fadt + 1);
  //Print (L"baseptr: %x\n", BasePtr);
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *FadtBody = (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *) Fadt;
  
  Print (L"Firmware Control   0x%x\n", FadtBody->FirmwareCtrl);
  Print (L"DSDT Address:    0x%x\n", FadtBody->Dsdt);
  
  Print (L"Reserved0:   0x%x\n", FadtBody->Reserved0);
  Print (L"PreferredPmProfile:    0x%x\n", FadtBody->PreferredPmProfile);
  Print (L"SciInt:    0x%x\n", FadtBody->SciInt);
  Print (L"SmiCmd:    0x%x\n", FadtBody->SmiCmd);
  Print (L"AcpiEnable:    0x%x\n", FadtBody->AcpiEnable);
  Print (L"AcpiDisable:   0x%x\n", FadtBody->AcpiDisable);
  Print (L"S4BiosReq:   0x%x\n", FadtBody->S4BiosReq);
  Print (L"PstateCnt:   0x%x\n", FadtBody->PstateCnt);
  Print (L"Pm1aEvtBlk:    0x%x\n", FadtBody->Pm1aEvtBlk);
  Print (L"Pm1bEvtBlk:    0x%x\n", FadtBody->Pm1bEvtBlk);
  Print (L"Pm1aCntBlk:    0x%x\n", FadtBody->Pm1aCntBlk);
  Print (L"Pm1bCntBlk:    0x%x\n", FadtBody->Pm1bCntBlk);
  Print (L"Pm2cntBlk:   0x%x\n", FadtBody->Pm2CntBlk);
  Print (L"PmTmrBlk:    0x%x\n", FadtBody->PmTmrBlk);
  WaitForAnyKey();
  Print (L"Gpe0Blk:   0x%x\n", FadtBody->Gpe0Blk);
  Print (L"Gpe1Blk:   0x%x\n", FadtBody->Gpe1Blk);
  Print (L"Pm1EvtLen:   0x%x\n", FadtBody->Pm1EvtLen);
  Print (L"Pm1CntLen:   0x%x\n", FadtBody->Pm1CntLen);
  Print (L"Pm2CntLen:   0x%x\n", FadtBody->Pm2CntLen);
  Print (L"PmTmrLen:    0x%x\n", FadtBody->PmTmrLen);
  Print (L"Gpe0BlkLen:    0x%x\n", FadtBody->Gpe0BlkLen);
  Print (L"Gpe1BlkLen:    0x%x\n", FadtBody->Gpe1BlkLen);
  Print (L"Gpe1Base:    0x%x\n", FadtBody->Gpe1Base);
  Print (L"CstCnt:    0x%x\n", FadtBody->CstCnt);
  Print (L"Plvl2Lat:    0x%x\n", FadtBody->PLvl2Lat);
  Print (L"Plvl3Lat:    0x%x\n", FadtBody->PLvl3Lat);
  Print (L"FlushSize:   0x%x\n", FadtBody->FlushSize);
  Print (L"FlushStride:   0x%x\n", FadtBody->FlushStride);
  Print (L"DutyOffset:    0x%x\n", FadtBody->DutyOffset);
  Print (L"DutyWidth:   0x%x\n", FadtBody->DutyWidth);
  WaitForAnyKey();
  Print (L"DayAlrm:   0x%x\n", FadtBody->DayAlrm);
  Print (L"MonAlrm:   0x%x\n", FadtBody->MonAlrm);
  Print (L"Century:   0x%x\n", FadtBody->Century);
  Print (L"IaPcBootArch:    0x%x\n", FadtBody->IaPcBootArch);
  Print (L"Reserved1:   0x%x\n", FadtBody->Reserved1);
  Print (L"Flags:   0x%x\n", FadtBody->Flags);
  Print (L"ResetReg:  ");
  PrintAcpi20GenericAddrStuct(&FadtBody->ResetReg);
  Print (L"ResetValue:  0x%x\n", FadtBody->ResetValue);
  Print (L"Reserved2: 0x%x  0x%x  0x%x\n", FadtBody->Reserved2[0],FadtBody->Reserved2[1],FadtBody->Reserved2[2]);
  Print (L"XFirmwareCtrl:   0x%x\n", FadtBody->XFirmwareCtrl);
  Print (L"XDsdt:   0x%x\n", FadtBody->XDsdt);

  PrintFacsFromXsdt((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE*)FadtBody->XFirmwareCtrl);
  PrintTableHeader((EFI_ACPI_DESCRIPTION_HEADER*) FadtBody->XDsdt);
  EFI_ACPI_DESCRIPTION_HEADER *XDsdt = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)FadtBody->XDsdt);
  PrintCommonTableContentAsHex(XDsdt);
  PrintCommonTableContentAsChar((EFI_ACPI_DESCRIPTION_HEADER*) FadtBody->XDsdt);
  
  
  
}

VOID *PrintBdatTable(EFI_ACPI_DESCRIPTION_HEADER *Bdat)
{
  UINT64 BdatBodyAddr = Bdat + 1;
  Print (L"Data:  ");
  for(UINTN i = 0; i < Bdat->Length - sizeof(*Bdat); i++){
    Print (L"0x%02x   ", *((CHAR8*)BdatBodyAddr));
    BdatBodyAddr = (CHAR8*)BdatBodyAddr + 1;
  }
  Print (L"\n");
}

VOID *PrintSsdtTable(EFI_ACPI_DESCRIPTION_HEADER *Ssdt)
{
/*
  Print (L"SSDT Data: ");
  UINT64 SsdtBodyAddr = Ssdt + 1;
  Print (L"ssdt Header addr: 0x%x   body addr: 0x%x\n", Ssdt, SsdtBodyAddr);
  WaitForAnyKey();
  CHAR8 *Buf = AllocatePool(CUSTOM_BUF_SIZE + 1);
  ZeroMem(Buf, CUSTOM_BUF_SIZE + 1);
  UINTN Count = (Ssdt->Length - sizeof(*Ssdt)) / CUSTOM_BUF_SIZE;
  for(UINTN i = 0; i < Count; i++){
    CopyMem(Buf, SsdtBodyAddr, CUSTOM_BUF_SIZE);
    Print (L"%s", Buf);
    SsdtBodyAddr = SsdtBodyAddr + CUSTOM_BUF_SIZE;
  }
  UINTN ResidueLen = (Ssdt->Length - sizeof(*Ssdt)) - Count * CUSTOM_BUF_SIZE;
  Print (L"Count: %x  ResiduLen: %x\n", Count, ResidueLen);
  WaitForAnyKey();
  if(ResidueLen != 0){
    ZeroMem(Buf, CUSTOM_BUF_SIZE + 1);
    CopyMem(Buf, SsdtBodyAddr, ResidueLen);
    Print (L"%s", Buf);
  }
  Print (L"\n");
  FreePool(Buf);
  */
  Print (L"SSDT Data(HEX): \n");
  UINT64 SsdtBodyAddr = Ssdt + 1;
  UINTN SsdtBodyLen = Ssdt->Length - sizeof(*Ssdt);
  for(UINTN Count = 1; Count <= SsdtBodyLen; Count++){
    Print (L"%02x ", *(UINT8*)SsdtBodyAddr);
    if((Count % DATA_ROW_FORMAT) == 0){
      Print (L"\n");
    }
    SsdtBodyAddr = (CHAR8 *)SsdtBodyAddr + 1;
  }

}

VOID *PrintTpm2Table(EFI_ACPI_DESCRIPTION_HEADER *Tpm2)
{
  UINT64 BaseAddr = Tpm2 + 1;
  Print (L"Flags:     0x%08x\n", *(UINT32 *)BaseAddr);
  BaseAddr = BaseAddr + 4;
  Print (L"Control Area:      0x%x\n", *(UINT64 *)BaseAddr);
  BaseAddr = BaseAddr + 8;
  Print (L"Start Method:      0x%08x\n", *(UINT32 *)BaseAddr);
}

VOID *PrintDbgpTable(EFI_ACPI_DESCRIPTION_HEADER *Dbgp)
{
  UINT64 BaseAddr = Dbgp + 1;
  Print (L"Data Area: \n");
  for(UINTN i = 0; i < Dbgp->Length - sizeof(*Dbgp); i++){
    Print (L"0x%2x    ", *(UINT8*)BaseAddr);
    BaseAddr = (UINT8*)BaseAddr + 1;
  }
}




VOID *PrintTablesFromXsdt(EFI_ACPI_DESCRIPTION_HEADER        *Xsdt)
{
  UINT64 BaseAdddr = Xsdt + 1;
  UINT32 EntryCount = (Xsdt->Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER)) / sizeof(UINT64);
  
  UINT32 EntryAddr;
  for(UINTN i = 0; i< EntryCount; i++){
    CopyMem (&EntryAddr, (VOID *)(BaseAdddr + i * sizeof(UINT64)), sizeof(UINT64));
    EFI_ACPI_DESCRIPTION_HEADER *Table;
    Table = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)(EntryAddr));
    CHAR16 *Signature = Signature2String(Table->Signature, sizeof(Table->Signature));
    CHAR16 Signature_Trim[5] = L"abcd";
    for(UINTN j = 0; j < 5 - 1; j++){
      Signature_Trim[j] = Signature[j];
    }
    Print (L"\n");
    Print (L"EntryTable%02d:   0x%x     %s\n", i, BaseAdddr + i * sizeof(UINT64), Signature_Trim);
    FreePool(Signature);
    Print (L"===================%s==================\n", Signature_Trim);
    PrintTableHeader(Table);

    /*
    if(CompareString(Signature_Trim, L"FACP")){
      PrintFadtTable(Table);
    }else if(CompareString(Signature_Trim, L"BDAT")){
      //PrintBdatTable(Table);
      PrintCommonTableContentAsHex(Table)
    }else if(CompareString(Signature_Trim, L"SSDT")){
      //PrintSsdtTable(Table);
    }else if(CompareString(Signature_Trim, L"TPM2")){

    }else if(CompareString(Signature_Trim, L"DBGP")){

    }else if(CompareString(Signature_Trim, L"HPET")){

    }else if(CompareString(Signature_Trim, L"APIC")){

    }else if(CompareString(Signature_Trim, L"NPKT")){

    }else if(Signature_Trim == "WSMT"){

    }else if(Signature_Trim == "SSDT"){

    }
    */
    if(CompareString(Signature_Trim, L"FACP")){
      PrintFadtTable(Table);
      Print (L"===================%s==================\n", Signature_Trim);
      continue;
    }
    PrintCommonTableContentAsHex(Table);
    PrintCommonTableContentAsChar(Table);
    Print (L"===================%s==================\n", Signature_Trim);
    WaitForAnyKey();
    
    
  }

}






EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Rsdt;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE     *Fadt;
  EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *Facs;
  UINTN Index;
  
	UINTN TablesCount = gST->NumberOfTableEntries;
  
	//Print(L"tables count : %d\n", TablesCount);
  //Print (L"acpi table guid   %g\n", &gEfiAcpi20TableGuid);
  EFI_STATUS Status;

	Rsdp = GetRsdpFromConfigurationTables();
  Xsdt = (EFI_ACPI_DESCRIPTION_HEADER*) Rsdp->XsdtAddress;
  PrintXsdtStruct(Xsdt);
  PrintTablesFromXsdt(Xsdt);
  

  return EFI_SUCCESS;
}









