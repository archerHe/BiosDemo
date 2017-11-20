#include "SmbiosReader.h"
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/HiiLib.h>
#include <Uefi/UefiSpec.h>


#define GET_BIT_VAL(x, i)     ((1) & ((x) >> ((i))))
#define Set_BIT_ZERO(x, i)    ((x) & ~((1) << (i)))

EFI_HANDLE HiiHandle;
Type_Table_Addr TypeTableAddr;
UINT32  Table_Max_Size;
UINT8   SMBIOS_VERSION;
CHAR16 *TmpUnicodeBuffer;

UINT8 Table_Count = 0;


EFI_SYSTEM_TABLE  *mSystemTable;

TABLE_ITEM  PortConnectorTypeTable[] = {
  {
    0x00,
    L"None"
  },
  {
    0x01,
    L"Centronics"
  },
  {
    0x02,
    L"Mini Centronics"
  },
  {
    0x03,
    L"Proprietary"
  },
  {
    0x04,
    L"DB-25 pin male"
  },
  {
    0x05,
    L"DB-25 pin female"
  },
  {
    0x06,
    L"DB-15 pin male"
  },
  {
    0x07,
    L"DB-15 pin female"
  },
  {
    0x08,
    L"DB-9 pin male"
  },
  {
    0x09,
    L"DB-9 pin female"
  },
  {
    0x0A,
    L"RJ-11"
  },
  {
    0x0B,
    L"RJ-45"
  },
  {
    0x0C,
    L"50 Pin MiniSCSI"
  },
  {
    0x0D,
    L"Mini-DIN"
  },
  {
    0x0E,
    L"Micro-DIN"
  },
  {
    0x0F,
    L"PS/2"
  },
  {
    0x10,
    L"Infrared"
  },
  {
    0x11,
    L"HP-HIL"
  },
  {
    0x12,
    L"Access Bus (USB)"
  },
  {
    0x13,
    L"SSA SCSI"
  },
  {
    0x14,
    L"Circular DIN-8 male"
  },
  {
    0x15,
    L"Circular DIN-8 female"
  },
  {
    0x16,
    L"On Board IDE"
  },
  {
    0x17,
    L"On Board Floppy"
  },
  {
    0x18,
    L"9 Pin Dual Inline (pin 10 cut)"
  },
  {
    0x19,
    L"25 Pin Dual Inline (pin 26 cut)"
  },
  {
    0x1A,
    L"50 Pin Dual Inline"
  },
  {
    0x1B,
    L"68 Pin Dual Inline"
  },
  {
    0x1C,
    L"On Board Sound Input from CD-ROM"
  },
  {
    0x1D,
    L"Mini-Centronics Type-14"
  },
  {
    0x1E,
    L"Mini-Centronics Type-26"
  },
  {
    0x1F,
    L"Mini-jack (headphones)"
  },
  {
    0x20,
    L"BNC"
  },
  {
    0x21,
    L"1394"
  },
  {
    0x22,
    L"SAS/SATA Plug Receptacle"
  },
  {
    0xA0,
    L"PC-98"
  },
  {
    0xA1,
    L"PC-98Hireso"
  },
  {
    0xA2,
    L"PC-H98"
  },
  {
    0xA3,
    L"PC-98Note"
  },
  {
    0xA4,
    L"PC-98Full"
  },
  {
    0xFF,
    L"Other"
  },
};

TABLE_ITEM  PortTypeTable[] = {
  {
    0x00,
    L"None"
  },
  {
    0x01,
    L"Parallel Port XT/AT Compatible"
  },
  {
    0x02,
    L"Parallel Port PS/2"
  },
  {
    0x03,
    L"Parallel Port ECP"
  },
  {
    0x04,
    L"Parallel Port EPP"
  },
  {
    0x05,
    L"Parallel Port ECP/EPP"
  },
  {
    0x06,
    L"Serial Port XT/AT Compatible"
  },
  {
    0x07,
    L"Serial Port 16450 Compatible"
  },
  {
    0x08,
    L"Serial Port 16550 Compatible"
  },
  {
    0x09,
    L"Serial Port 16550A Compatible"
  },
  {
    0x0A,
    L"SCSI Port"
  },
  {
    0x0B,
    L"MIDI Port"
  },
  {
    0x0C,
    L"Joy Stick Port"
  },
  {
    0x0D,
    L"Keyboard Port"
  },
  {
    0x0E,
    L"Mouse Port"
  },
  {
    0x0F,
    L"SSA SCSI"
  },
  {
    0x10,
    L"USB"
  },
  {
    0x11,
    L"FireWire (IEEE P1394)"
  },
  {
    0x12,
    L"PCMCIA Type II"
  },
  {
    0x13,
    L"PCMCIA Type II"
  },
  {
    0x14,
    L"PCMCIA Type III"
  },
  {
    0x15,
    L"Cardbus"
  },
  {
    0x16,
    L"Access Bus Port"
  },
  {
    0x17,
    L"SCSI II"
  },
  {
    0x18,
    L"SCSI Wide"
  },
  {
    0x19,
    L"PC-98"
  },
  {
    0x1A,
    L"PC-98-Hireso"
  },
  {
    0x1B,
    L"PC-H98"
  },
  {
    0x1C,
    L"Video Port"
  },
  {
    0x1D,
    L"Audio Port"
  },
  {
    0x1E,
    L"Modem Port"
  },
  {
    0x1F,
    L"Network Port"
  },
  {
    0x20,
    L"SATA Port"
  },
  {
    0x21,
    L"SAS Port"
  },
  {
    0xA0,
    L"8251 Compatible"
  },
  {
    0xA1,
    L"8251 FIFO Compatible"
  },
  {
    0xFF,
    L"Other "
  },
};

TABLE_ITEM  SystemSlotTypeTable[] = {
  {
    0x01,
    L"Other"
  },
  {
    0x02,
    L"Unknown"
  },
  {
    0x03,
    L"ISA"
  },
  {
    0x04,
    L"MCA"
  },
  {
    0x05,
    L"EISA"
  },
  {
    0x06,
    L"PCI"
  },
  {
    0x07,
    L"PC Card (PCMCIA)"
  },
  {
    0x08,
    L"VL-VESA"
  },
  {
    0x09,
    L"Proprietary"
  },
  {
    0x0A,
    L"Processor Card Slot"
  },
  {
    0x0B,
    L"Proprietary Memory Card Slot"
  },
  {
    0x0C,
    L"I/O Riser Card Slot"
  },
  {
    0x0D,
    L"NuBus"
  },
  {
    0x0E,
    L"PCI - 66MHz Capable"
  },
  {
    0x0F,
    L"AGP"
  },
  {
    0x10,
    L"AGP 2X"
  },
  {
    0x11,
    L"AGP 4X"
  },
  {
    0x12,
    L"PCI-X"
  },
  {
    0x13,
    L"AGP 8X"
  },
  {
    0x14,
    L"M.2 Socket 1-DP (Mechanical Key A)"
  },
  {
    0x15,
    L"M.2 Socket 1-SD (Mechanical Key E)"
  },
  {
    0x16,
    L"M.2 Socket 2 (Mechanical Key B)"
  },
  {
    0x17,
    L"M.2 Socket 3 (Mechanical Key M)"
  },
  {
    0x18,
    L"MXM Type I"
  },
  {
    0x19,
    L"MXM Type II"
  },
  {
    0x1A,
    L"MXM Type III (standard connector)"
  },
  {
    0x1B,
    L"MXM Type III (HE connector)"
  },
  {
    0x1C,
    L"MXM Type IV"
  },
  {
    0x1D,
    L"MXM 3.0 Type A"
  },
  {
    0x1E,
    L"MXM 3.0 Type B"
  },
  {
    0x1F,
    L"PCI Express Gen 2 SFF-8639"
  },
  {
    0x20,
    L"PCI Express Gen 3 SFF-8639"
  },
  {
    0x21,
    L"PCI Express Mini 52-pin (CEM spec. 2.0) with bottom-side keep-outs"
  },
  {
    0x22,
    L"PCI Express Mini 52-pin (CEM spec. 2.0) without bottom-side keep-outs"
  },
  {
    0x23,
    L"PCI Express Mini 76-pin (CEM spec. 2.0) Corresponds to Display-Mini card"
  },
  {
    0xA0,
    L"PC-98/C20 "
  },
  {
    0xA1,
    L"PC-98/C24 "
  },
  {
    0xA2,
    L"PC-98/E "
  },
  {
    0xA3,
    L"PC-98/Local Bus "
  },
  {
    0xA4,
    L"PC-98/Card "
  },
  {
    0xA5,
    L"PCI Express "
  },
  {
    0xA6,
    L"PCI Express X1"
  },
  {
    0xA7,
    L"PCI Express X2"
  },
  {
    0xA8,
    L"PCI Express X4"
  },
  {
    0xA9,
    L"PCI Express X8"
  },
  {
    0xAA,
    L"PCI Express X16"
  },
  {
    0xAB,
    L"PCI Express Gen 2"
  },
  {
    0xAC,
    L"PCI Express Gen 2 X1"
  },
  {
    0xAD,
    L"PCI Express Gen 2 X2"
  },
  {
    0xAE,
    L"PCI Express Gen 2 X4"
  },
  {
    0xAF,
    L"PCI Express Gen 2 X8"
  },
  {
    0xB0,
    L"PCI Express Gen 2 X16"
  },
  {
    0xB1,
    L"PCI Express Gen 3"
  },
  {
    0xB2,
    L"PCI Express Gen 3 X1"
  },
  {
    0xB3,
    L"PCI Express Gen 3 X2"
  },
  {
    0xB4,
    L"PCI Express Gen 3 X4"
  },
  {
    0xB5,
    L"PCI Express Gen 3 X8"
  },
  {
    0xB6,
    L"PCI Express Gen 3 X16"
  }
};

TABLE_ITEM  SystemSlotDataBusWidthTable[] = {
  {
    0x01,
    L" Other"
  },
  {
    0x02,
    L" Unknown"
  },
  {
    0x03,
    L" 8 bit"
  },
  {
    0x04,
    L" 16 bit"
  },
  {
    0x05,
    L" 32 bit"
  },
  {
    0x06,
    L" 64 bit"
  },
  {
    0x07,
    L" 128 bit"
  },
  {
    0x08,
    L" 1x or x1"
  },
  {
    0x09,
    L" 2x or x2"
  },
  {
    0x0A,
    L" 4x or x4"
  },
  {
    0x0B,
    L" 8x or x8"
  },
  {
    0x0C,
    L" 12x or x12"
  },
  {
    0x0D,
    L" 16x or x16"
  },
  {
    0x0E,
    L" 32x or x32"
  }
};

TABLE_ITEM  SystemSlotCurrentUsageTable[] = {
  {
    0x01,
    L" Other"
  },
  {
    0x02,
    L" Unknown"
  },
  {
    0x03,
    L" Available"
  },
  {
    0x04,
    L" In use"
  },
};

TABLE_ITEM  SystemSlotLengthTable[] = {
  {
    0x01,
    L" Other"
  },
  {
    0x02,
    L" Unknown"
  },
  {
    0x03,
    L" Short length"
  },
  {
    0x04,
    L" Long Length"
  },
};

TABLE_ITEM  SlotCharacteristics1Table[] = {
  {
    0,
    L" Characteristics Unknown"
  },
  {
    1,
    L" Provides 5.0 Volts"
  },
  {
    2,
    L" Provides 3.3 Volts"
  },
  {
    3,
    L" Slot's opening is shared with another slot, e.g. PCI/EISA shared slot."
  },

  {
    4,
    L" PC Card slot supports PC Card-16"
  },
  {
    5,
    L" PC Card slot supports CardBus"
  },
  {
    6,
    L" PC Card slot supports Zoom Video "
  },
  {
    7,
    L" PC Card slot supports Modem Ring Resume "
  }
};

TABLE_ITEM  SlotCharacteristics2Table[] = {
  {
    0,
    L" PCI slot supports Power Management Enable (PME#) signal"
  },
  {
    1,
    L" Slot supports hot-plug devices"
  },
  {
    2,
    L" PCI slot supports SMBus signal"
  }
};

TABLE_ITEM  OnboardDeviceTypesTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  Video"
  },
  {
    0x04,
    L"  SCSI Controller"
  },
  {
    0x05,
    L"  Ethernet"
  },
  {
    0x06,
    L"  Token Ring"
  },
  {
    0x07,
    L"  Sound"
  },
  {
    0x08,
    L"  Pata Controller"
  },
  {
    0x09,
    L"  Sata Controller"
  },
  {
    0x0A,
    L"  Sas Controller"
  },
};

TABLE_ITEM  PMALocationTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  System board or motherboard"
  },
  {
    0x04,
    L"  ISA add-on card"
  },
  {
    0x05,
    L"  EISA add-on card"
  },
  {
    0x06,
    L"  PCI add-on card"
  },
  {
    0x07,
    L"  MCA add-on card"
  },
  {
    0x08,
    L"  PCMCIA add-on card"
  },
  {
    0x09,
    L"  Proprietary add-on card"
  },
  {
    0x0A,
    L"  NuBus"
  },
  {
    0xA0,
    L"  PC-98/C20 add-on card"
  },
  {
    0xA1,
    L"  PC-98/C24 add-on card"
  },
  {
    0xA2,
    L"  PC-98/E add-on card"
  },
  {
    0xA3,
    L"  PC-98/Local bus add-on card"
  }
};

TABLE_ITEM  PMAUseTable[] = {
  {
    0x01,
    L" Other"
  },
  {
    0x02,
    L" Unknown"
  },
  {
    0x03,
    L" System memory"
  },
  {
    0x04,
    L" Video memory"
  },
  {
    0x05,
    L" Flash memory"
  },
  {
    0x06,
    L" Non-volatile RAM"
  },
  {
    0x07,
    L" Cache memory"
  }
};

TABLE_ITEM  PMAErrorCorrectionTypesTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  None"
  },
  {
    0x04,
    L"  Parity"
  },
  {
    0x05,
    L"  Single-bit ECC"
  },
  {
    0x06,
    L"  Multi-bit ECC"
  },
  {
    0x07,
    L"  CRC"
  }
};

TABLE_ITEM  MemoryDeviceFormFactorTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  SIMM"
  },
  {
    0x04,
    L"  SIP"
  },
  {
    0x05,
    L"  Chip"
  },
  {
    0x06,
    L"  DIP"
  },
  {
    0x07,
    L"  ZIP"
  },
  {
    0x08,
    L"  Proprietary Card"
  },
  {
    0x09,
    L"  DIMM"
  },
  {
    0x0A,
    L"  TSOP"
  },
  {
    0x0B,
    L"  Row of chips"
  },
  {
    0x0C,
    L"  RIMM"
  },
  {
    0x0D,
    L"  SODIMM"
  },
  {
    0x0E,
    L"  SRIMM"
  },
  {
    0x0F,
    L"  FB-DIMM"
  }
};

TABLE_ITEM  MemoryDeviceTypeTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  DRAM"
  },
  {
    0x04,
    L"  EDRAM"
  },
  {
    0x05,
    L"  VRAM"
  },
  {
    0x06,
    L"  SRAM"
  },
  {
    0x07,
    L"  RAM"
  },
  {
    0x08,
    L"  ROM"
  },
  {
    0x09,
    L"  FLASH"
  },
  {
    0x0A,
    L"  EEPROM"
  },
  {
    0x0B,
    L"  FEPROM"
  },
  {
    0x0C,
    L"  EPROM"
  },
  {
    0x0D,
    L"  CDRAM"
  },
  {
    0x0E,
    L"  3DRAM"
  },
  {
    0x0F,
    L"  SDRAM"
  },
  {
    0x10,
    L"  SGRAM"
  },
  {
    0x11,
    L"  RDRAM"
  },
  {
    0x12,
    L"  DDR"
  },
  {
    0x13,
    L"  DDR2"
  },
  {
    0x14,
    L"  DDR2 FB-DIMM"
  },
  {
    0x18,
    L"  DDR3"
  },
  {
    0x19,
    L"  FBD2"
  },
  {
    0x1A,
    L"  DDR4"
  },
  {
    0x1B,
    L"  LPDDR"
  },
  {
    0x1C,
    L"  LPDDR2"
  },
  {
    0x1D,
    L"  LPDDR3"
  },
  {
    0x1E,
    L"  LPDDR4"
  }
};

TABLE_ITEM  MemoryDeviceTypeDetailTable[] = {
  {
    0,
    L" Reserved"
  },
  {
    1,
    L" Other"
  },
  {
    2,
    L" Unknown"
  },
  {
    3,
    L" Fast-paged"
  },
  {
    4,
    L" Static column"
  },
  {
    5,
    L" Pseudo-STATIC"
  },
  {
    6,
    L" RAMBUS "
  },
  {
    7,
    L" Synchronous"
  },
  {
    8,
    L" CMOS"
  },
  {
    9,
    L" EDO"
  },
  {
    10,
    L" Window DRAM"
  },
  {
    11,
    L" Cache DRAM"
  },
  {
    12,
    L" Non-volatile"
  },
  {
    13,
    L" Registered(Buffered)"
  },
  {
    14,
    L" Unbuffered(Unregistered)"
  },
  {
    15,
    L" LRDIMM"
  }
};

TABLE_ITEM  MemoryErrorTypeTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  OK"
  },
  {
    0x04,
    L"  Bad read"
  },
  {
    0x05,
    L"  Parity error"
  },
  {
    0x06,
    L"  Single-bit error"
  },
  {
    0x07,
    L"  Double-bit error"
  },
  {
    0x08,
    L"  Multi-bit error"
  },
  {
    0x09,
    L"  Nibble error"
  },
  {
    0x0A,
    L"  Checksum error"
  },
  {
    0x0B,
    L"  CRC error"
  },
  {
    0x0C,
    L"  Corrected single-bit error"
  },
  {
    0x0D,
    L"  Corrected error"
  },
  {
    0x0E,
    L"  Uncorrectable error"
  },
};

TABLE_ITEM  MemoryErrorGranularityTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  Device level"
  },
  {
    0x04,
    L"  Memory partition level"
  },
};

TABLE_ITEM  MemoryErrorOperationTable[] = {
  {
    0x01,
    L"  Other"
  },
  {
    0x02,
    L"  Unknown"
  },
  {
    0x03,
    L"  Read"
  },
  {
    0x04,
    L"  Write"
  },
  {
    0x05,
    L"  Partial Write"
  },
};


VOID WaitForAnyKey(){
  Print (L"Press any key for continue................\n");
  EFI_STATUS Status = 0;
  UINTN Index = 0;
  EFI_EVENT Event[2] = {0};
  Event[0] = gST->ConIn->WaitForKey;
  Status = gBS->CreateEvent(EVT_TIMER, TPL_CALLBACK, (EFI_EVENT_NOTIFY) NULL, (VOID *)NULL, &Event[1]);
  Status = gBS->SetTimer(Event[1], TimerPeriodic, 600 * 1000 * 1000);
  Status = gBS->WaitForEvent(2, Event, &Index);
  EFI_INPUT_KEY Key;
  
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
}

VOID
PrintToken (
  IN UINT16           Token,
  IN EFI_HII_HANDLE   Handle,
  ...
  )
{
  ShellPrintHiiEx(-1,-1,NULL,Token, HiiHandle);
}

CHAR16 *
Ascii2Unicode (
  OUT CHAR16         *UnicodeStr,
  IN  CHAR8          *AsciiStr
  )
{
  CHAR16  *Str;
  Str = UnicodeStr;
  while (TRUE) {
    *(UnicodeStr++) = (CHAR16) *AsciiStr;
    if (*(AsciiStr++) == '\0') {
      return Str;
    }
  }
}

CHAR16 *GetStringFromBiosChar(UINT32 BitVal)
{
  if (BitVal == 1){
    return L"is supported";
  }else{
    return L"is not supported";
  }
}

CHAR16 *GetYesOrNoFromBool(UINT32 BitVal)
{
  if (BitVal == 1){
    return L"Yes";
  }else{
    return L"No";
  }
}

CHAR16 *GetStringsFromType(UINT64 HeaderAddr, UINT8 Index)
{
  SMBIOS_STRUCTURE *Header = (SMBIOS_STRUCTURE*)HeaderAddr;
  UINT64  TextAreaAddr = (UINT64)Header + Header->Length;
  UINT8  *Tmp;
  //CHAR16 *DestString;
  UINT8 Count = 1;
  for (Tmp = TextAreaAddr; Tmp <= TextAreaAddr + Table_Max_Size; Tmp++){
    //CHAR16 *UnicodeStr = AllocatePool(SMBIOS_STRING_MAX_LENGTH);
    if (Index == 1){
      Ascii2Unicode(TmpUnicodeBuffer, (CHAR8 *)(Tmp));
      return TmpUnicodeBuffer;
    }
    if (*Tmp == 0){
      Count++;
      if(Index == Count){
        Ascii2Unicode(TmpUnicodeBuffer, (CHAR8 *)(++Tmp));
        //Print (L"unicode str: %s\n", TmpUnicodeBuffer);
        return TmpUnicodeBuffer;
      }
    }
  } 
  return L"not found String";
}

CHAR16 *GetTypeTableName(UINT8 Index      )
{
  switch(Index){
    case SMBIOS_TYPE_BIOS_INFORMATION:
      return L"BIOS_INFORMATION";
    case SMBIOS_TYPE_SYSTEM_INFORMATION:
      return L"SYSTEM_INFORMATION";
    case SMBIOS_TYPE_BASEBOARD_INFORMATION:
      return L"BASEBOARD_INFORMATION";
    case SMBIOS_TYPE_SYSTEM_ENCLOSURE:
      return L"SYSTEM_ENCLOSURE";
    case SMBIOS_TYPE_PROCESSOR_INFORMATION:
      return L"PROCESSOR_INFORMATION";
    case SMBIOS_TYPE_MEMORY_CONTROLLER_INFORMATION:
      return L"MEMORY_CONTROLLER_INFORMATION";
    case SMBIOS_TYPE_MEMORY_MODULE_INFORMATON:
      return L"MODULE_INFORMATON";
    case SMBIOS_TYPE_CACHE_INFORMATION:
      return L"CACHE_INFORMATION";
    case SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION:
      return L"PORT_CONNECTOR_INFORMATION";
    case SMBIOS_TYPE_SYSTEM_SLOTS:
      return L"SYSTEM_SLOTS";
    case SMBIOS_TYPE_ONBOARD_DEVICE_INFORMATION:
      return L"ONBOARD_DEVICE_INFORMATION";
    case SMBIOS_TYPE_OEM_STRINGS:
      return L"OEM_STRINGS";
    case SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS:
      return L"SYSTEM_CONFIGURATION_OPTIONS";
    case SMBIOS_TYPE_BIOS_LANGUAGE_INFORMATION:
      return L"BIOS_LANGUAGE_INFORMATION";
    case SMBIOS_TYPE_GROUP_ASSOCIATIONS:
      return L"GROUP_ASSOCIATIONS";
    case SMBIOS_TYPE_SYSTEM_EVENT_LOG:
      return L"SYSTEM_EVENT_LOG";
    case SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY:
      return L"PHYSICAL_MEMORY_ARRAY";
    case SMBIOS_TYPE_MEMORY_DEVICE:
      return L"MEMORY_DEVICE";
    default:
      return L"just display Type0~Type17 table name";

  }
}


CHAR16* GetWakeupTypeString(UINT8 Type)
{
  switch(Type){
    case SystemWakeupTypeReserved:
      return L"SystemWakeupTypeReserved";
    case SystemWakeupTypeOther:
      return L"SystemWakeupTypeOther";
    case SystemWakeupTypeUnknown:
      return L"SystemWakeupTypeUnknown";
    case SystemWakeupTypeApmTimer:
      return L"SystemWakeupTypeApmTimer";
    case SystemWakeupTypeModemRing:
      return L"SystemWakeupTypeModemRing";
    case SystemWakeupTypeLanRemote:
      return L"SystemWakeupTypeLanRemote";
    case SystemWakeupTypePowerSwitch:
      return L"SystemWakeupTypePowerSwitch";
    case SystemWakeupTypePciPme:
      return L"SystemWakeupTypePciPme";
    case SystemWakeupTypeAcPowerRestored:
      return L"SystemWakeupTypeAcPowerRestored";
    default:
      return L"not match Type";
  }
}

CHAR16* GetBoardTypeString(UINT8 Type)
{
  switch(Type){
    case BaseBoardTypeUnknown:
      return L"BaseBoardTypeUnknown";
    case BaseBoardTypeOther:
      return L"BaseBoardTypeOther";
    case BaseBoardTypeServerBlade:
      return L"BaseBoardTypeServerBlade";
    case BaseBoardTypeConnectivitySwitch:
      return L"BaseBoardTypeConnectivitySwitch";
    case BaseBoardTypeSystemManagementModule:
      return L"BaseBoardTypeSystemManagementModule";
    case BaseBoardTypeProcessorModule:
      return L"BaseBoardTypeProcessorModule";
    case BaseBoardTypeIOModule:
      return L"BaseBoardTypeIOModule";
    case BaseBoardTypeMemoryModule:
      return L"BaseBoardTypeMemoryModule";
    case BaseBoardTypeDaughterBoard:
      return L"BaseBoardTypeDaughterBoard";
    case BaseBoardTypeMotherBoard:
      return L"BaseBoardTypeMotherBoard";
    case BaseBoardTypeProcessorMemoryModule:
      return L"BaseBoardTypeProcessorMemoryModule";
    case BaseBoardTypeProcessorIOModule:
      return L"BaseBoardTypeProcessorIOModule";
    case BaseBoardTypeInterconnectBoard:
      return L"BaseBoardTypeInterconnectBoard";
    default :
      return L"not match BaseBoard type";
  }
}

CHAR16* GetChassisStateString(UINT8 State)
{
  switch(State){
    case ChassisStateOther:
      return L"Other";
    case ChassisStateUnknown:
      return L"Unknown";
    case ChassisStateSafe:
      return L"Safe";
    case ChassisStateWarning:
      return L"Warning";
    case ChassisStateCritical:
      return L"Critical";
    case ChassisStateNonRecoverable:
      return L"Non-recoverable";
    default:
      return L"not match state";
  }
}

CHAR16* GetChassisSecStateString(UINT8 State)
{
  switch(State){
    case ChassisSecurityStatusOther:
      return L"Other";
    case ChassisSecurityStatusUnknown:
      return L"Unknown";
    case ChassisSecurityStatusNone:
      return L"None";
    case ChassisSecurityStatusExternalInterfaceLockedOut:
      return L"External interface locked out";
    case ChassisSecurityStatusExternalInterfaceLockedEnabled:
      return L"External interface enabled";
    default:
      return L"not match state";
  }
}

VOID PrintProcessorFamily(UINT8 Family)
{
  switch (Family) {
  case 0x01:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_OTHER), HiiHandle);
    break;

  case 0x02:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_UNKNOWN), HiiHandle);
    break;

  case 0x03:
    Print (L"8086\n");
    break;

  case 0x04:
    Print (L"80286\n");
    break;

  case 0x05:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL386_PROCESSOR), HiiHandle);
    break;

  case 0x06:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL486_PROCESSOR), HiiHandle);
    break;

  case 0x07:
    Print (L"8087\n");
    break;

  case 0x08:
    Print (L"80287\n");
    break;

  case 0x09:
    Print (L"80387\n");
    break;

  case 0x0A:
    Print (L"80487\n");
    break;

  case 0x0B:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_PROC_FAMILY), HiiHandle);
    break;

  case 0x0C:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_PRO_PROC), HiiHandle);
    break;

  case 0x0D:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_II_PROC), HiiHandle);
    break;

  case 0x0E:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_PROC_MMX), HiiHandle);
    break;

  case 0x0F:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_CELERON_PROC), HiiHandle);
    break;

  case 0x10:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_XEON_PROC), HiiHandle);
    break;

  case 0x11:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_III_PROC), HiiHandle);
    break;

  case 0x12:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_M1_FAMILY), HiiHandle);
    break;

  case 0x18:
    Print (L"AMD Duron\n");
    break;

  case 0x19:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_K5_FAMILY), HiiHandle);
    break;

  case 0x20:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_POWER_PC_FAMILY), HiiHandle);
    break;

  case 0x21:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_POWER_PC_601), HiiHandle);
    break;

  case 0x22:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_POWER_PC_603), HiiHandle);
    break;

  case 0x23:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_POWER_PC_603_PLUS), HiiHandle);
    break;

  case 0x24:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_POWER_PC_604), HiiHandle);
    break;

  case 0x25:
    Print (L"Power PC 620\n");
    break;

  case 0x26:
    Print (L"Power PC 704\n");
    break;

  case 0x27:
    Print (L"Power PC 750\n");
    break;

  case 0x28:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE_DUO), HiiHandle);
    break;
    
  case 0x29:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE_DUO_MOBILE), HiiHandle);
    break;    

  case 0x2A:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE_SOLO_MOBILE), HiiHandle);
    break;

  case 0x2B:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_ATOM), HiiHandle);
    break;

  case 0x30:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ALPHA_FAMILY_2), HiiHandle);
    break;

  case 0x31:
    Print (L"Alpha 21064\n");
    break;

  case 0x32:
    Print (L"Alpha 21066\n");
    break;

  case 0x33:
    Print (L"Alpha 21164\n");
    break;

  case 0x34:
    Print (L"Alpha 21164PC\n");
    break;

  case 0x35:
    Print (L"Alpha 21164a\n");
    break;

  case 0x36:
    Print (L"Alpha 21264\n");
    break;

  case 0x37:
    Print (L"Alpha 21364\n");
    break;

  case 0x40:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_MIPS_FAMILY), HiiHandle);
    break;

  case 0x41:
    Print (L"MIPS R4000\n");
    break;

  case 0x42:
    Print (L"MIPS R4200\n");
    break;

  case 0x43:
    Print (L"MIPS R4400\n");
    break;

  case 0x44:
    Print (L"MIPS R4600\n");
    break;

  case 0x45:
    Print (L"MIPS R10000\n");
    break;

  case 0x50:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_SPARC_FAMILY), HiiHandle);
    break;

  case 0x51:
    Print (L"SuperSparc\n");
    break;

  case 0x52:
    Print (L"microSparc II\n");
    break;

  case 0x53:
    Print (L"microSparc IIep\n");
    break;

  case 0x54:
    Print (L"UltraSparc\n");
    break;

  case 0x55:
    Print (L"UltraSparc II\n");
    break;

  case 0x56:
    Print (L"UltraSparcIIi\n");
    break;

  case 0x57:
    Print (L"UltraSparcIII\n");
    break;

  case 0x58:
    Print (L"UltraSparcIIIi\n");
    break;

  case 0x60:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_68040_FAMILY), HiiHandle);
    break;

  case 0x61:
    Print (L"68xx\n");
    break;

  case 0x62:
    Print (L"68000\n");
    break;

  case 0x63:
    Print (L"68010\n");
    break;

  case 0x64:
    Print (L"68020\n");
    break;

  case 0x65:
    Print (L"68030\n");
    break;

  case 0x70:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_HOBBIT_FAMILY), HiiHandle);
    break;

  case 0x78:
    Print (L"Crusoe TM5000\n");
    break;

  case 0x79:
    Print (L"Crusoe TM3000\n");
    break;

  case 0x7A:
    Print (L"Efficeon TM8000\n");
    break;

  case 0x80:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_WEITEK), HiiHandle);
    break;

  case 0x82:
    Print (L"Itanium\n");
    break;

  case 0x83:
    Print (L"AMD Athlon64\n");
    break;

  case 0x84:
    Print (L"AMD Opteron\n");
    break;

  case 0x85:
    Print (L"AMD Sempron\n");
    break;

  case 0x86:
    Print (L"AMD Turion64 Mobile\n");
    break;

  case 0x87:
    Print (L"Dual-Core AMD Opteron\n");
    break;

  case 0x88:
    Print (L"AMD Athlon 64X2 DualCore\n");
    break;

  case 0x89:
    Print (L"AMD Turion 64X2 Mobile\n");
    break;

  case 0x8A:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_OPTERON_QUAD_CORE), HiiHandle);
    break;

  case 0x8B:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_OPTERON_THIRD_GENERATION), HiiHandle);
    break;

  case 0x8C:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_PHENOM_FX_QUAD_CORE), HiiHandle);
    break;

  case 0x8D:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_PHENOM_X4_QUAD_CORE), HiiHandle);
    break;

  case 0x8E:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_PHENOM_X2_DUAL_CORE), HiiHandle);
    break;

  case 0x8F:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_ATHLON_X2_DUAL_CORE), HiiHandle);
    break;

  case 0x90:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PA_RISC_FAMILY), HiiHandle);
    break;

  case 0xA0:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_V30_FAMILY), HiiHandle);
    break;

  case 0xA1:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_3200_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xA2:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_3000_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xA3:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5300_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xA4:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5100_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xA5:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5000_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xA6:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_LV_DUAL_CORE), HiiHandle);
    break;

  case 0xA7:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_ULV_DUAL_CORE), HiiHandle);
    break;

  case 0xA8:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7100_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xA9:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5400_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xAA:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_QUAD_CORE), HiiHandle);
    break;

  case 0xAB:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5200_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xAC:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7200_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xAD:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7300_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xAE:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7400_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xAF:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7400_SERIES_MULTI_CORE), HiiHandle);
    break;

  case 0xB0:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_PENTIUM_III_XEON), HiiHandle);
    break;

  case 0xC0:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE2_SOLO), HiiHandle);
    break;

  case 0xC1:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE2_EXTREME), HiiHandle);
    break;

  case 0xC2:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE2_QUAD), HiiHandle);
    break;

  case 0xC3:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE2_EXTREME), HiiHandle);
    break;

  case 0xC4:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE2_DUO_MOBILE), HiiHandle);
    break;

  case 0xC5:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE2_SOLO_MOBILE), HiiHandle);
    break;

  case 0xC6:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CORE_I7), HiiHandle);
    break;

  case 0xC7:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_CELERON_DUAL_CORE), HiiHandle);
    break;

  case 0xC8:
    Print (L"IBM 390\n");
    break;

  case 0xC9:
    Print (L"G4\n");
    break;

  case 0xCA:
    Print (L"G5\n");
    break;

  case 0xCB:
    Print (L"G6\n");
    break;

  case 0xCC:
    Print (L"zArchitectur\n");
    break;

  case 0xD2:
    Print (L"ViaC7M\n");
    break;

  case 0xD3:
    Print (L"ViaC7D\n");
    break;

  case 0xD4:
    Print (L"ViaC7\n");
    break;

  case 0xD5:
    Print (L"Eden\n");
    break;

  case 0xD6:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_MULTI_CORE), HiiHandle);
    break;

  case 0xD7:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_3_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xD8:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_3_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xDA:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xDB:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_5_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xDD:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7_SERIES_DUAL_CORE), HiiHandle);
    break;

  case 0xDE:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7_SERIES_QUAD_CORE), HiiHandle);
    break;

  case 0xDF:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_INTEL_XEON_7_SERIES_MULTI_CORE), HiiHandle);
    break;

  case 0xE6:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_EMBEDDED_OPTERON_QUAD_CORE), HiiHandle);
    break;

  case 0xE7:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_PHENOM_TRIPLE_CORE), HiiHandle);
    break;

  case 0xE8:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_TURION_ULTRA_DUAL_CORE_MOBILE), HiiHandle);
    break;

  case 0xE9:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_TURION_DUAL_CORE_MOBILE), HiiHandle);
    break;

  case 0xEA:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_ATHLON_DUAL_CORE), HiiHandle);
    break;

  case 0xEB:
    PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AMD_SEMPRON_SI), HiiHandle);
    break;

  case 0xFA:
    Print (L"i860\n");
    break;

  case 0xFB:
    Print (L"i960\n");
    break;

  default:
    //
    // In order to reduce code quality notice of
    // case & break not pair, so
    // move multiple case into the else part and
    // use if/else to check value.
    //
    if (Family >= 0x13 && Family <= 0x17) {
      PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_RSVD_FOR_SPEC_M1), HiiHandle);
    } else if (Family >= 0x1A && Family <= 0x1F) {
      PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_RSVD_FOR_SPEC_K5), HiiHandle);
    } else if (Family >= 0xB1 && Family <= 0xBF) {
      PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_RSVD_FOR_SPEC_PENTIUM), HiiHandle);
    } else {
      PrintToken (STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_UNDEFINED_PROC_FAMILY), HiiHandle);
    }
  }
}

CHAR16* GetProcessorTypeDataString(UINT8 Type)
{
  switch(Type){
  case ProcessorOther:
    return L"Other";
  case ProcessorUnknown:
    return L"Unknow";
  case CentralProcessor:
    return L"CentralProcessor";
  case MathProcessor:
    return L"MathProcessor";
  case DspProcessor:
    return L"DspProcessor";
  case VideoProcessor:
    return L"VideoProcessor";
  default:
    return L"no match TypeData";
  }
}

VOID PrintType04ProcessorChar(SMBIOS_TABLE_TYPE4 *Type4)
{
  BOOLEAN BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 1);
  Print (L"Processor Characteristics: 0x%x\n", Type4->ProcessorCharacteristics);
  Print (L"   Bit1:   Unknown - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
  BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 2);
  Print (L"   Bit2:   64-bit Capable  - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
  BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 3);
  Print (L"   Bit3:   Multi-Core  - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
  BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 4);
  Print (L"   Bit4:   Hardware Thread - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
  BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 5);
  Print (L"   Bit5:   Execute Protection  - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
  BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 6);
  Print (L"   Bit6:   Enhanced Virtualization  - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
  BitVal = GET_BIT_VAL(Type4->ProcessorCharacteristics, 7);
  Print (L"   Bit7:   Power/Performance Control  - %d  (%s)\n", BitVal, GetYesOrNoFromBool(BitVal));
}

VOID PrintCacheConf(UINT16 *ValAddr)
{
  Type07CacheConf *CacheConf = (Type07CacheConf*)ValAddr;
  Print (L"Bit2:0 = %d    Cache Level:      0x%x\n", CacheConf->CacheLevel, CacheConf->CacheLevel);
  Print (L"Bit3   = %d    Cache Socketed:    Socketed - (%s)\n", CacheConf->CacheSocked, GetYesOrNoFromBool(CacheConf->CacheSocked));
  Print (L"Bit6:5 = %d    BitLocation, relative to the CPU module:    0x%02x\n", CacheConf->RelativeCpuModule, CacheConf->RelativeCpuModule);
  Print (L"Bit7   = %d    Enabled(at boot time):   %s\n", CacheConf->BootTimeStatus, GetYesOrNoFromBool(CacheConf->BootTimeStatus));
  Print (L"Bit9:8 = %d    Optional Mode:    0x%02x\n", CacheConf->OptionalMode, GetYesOrNoFromBool(CacheConf->OptionalMode));
}

VOID PrintErrorCorrectionType(UINT8 Val)
{
  switch(Val){
    case CacheErrorOther:
      Print (L"Error Correction Type:        0x%02x  - Other\n", Val);
      break;
    case CacheErrorUnknown:
      Print (L"Error Correction Type:        0x%02x  - Unknown\n", Val);
      break;
    case CacheErrorNone:
      Print (L"Error Correction Type:        0x%02x  - None\n", Val);
      break;
    case CacheErrorParity:
      Print (L"Error Correction Type:        0x%02x  - Parity\n", Val);
      break;
    case CacheErrorSingleBit:
      Print (L"Error Correction Type:        0x%02x  - Single-bit ECC\n", Val);
      break;
    case CacheErrorMultiBit:
      Print (L"Error Correction Type:        0x%02x  - Multi-bit ECC\n", Val);
      break;
    default:
      Print (L"Error Correction Type:        no match type\n", Val);
  }
}

VOID PrintSystemCacheType(UINT8 Val)
{
  switch (Val){
    case CacheTypeOther:
      Print (L"System Cache Type:       0x%02x  -  Other\n");
      break;
    case CacheTypeUnknown:
      Print (L"System Cache Type:       0x%02x  -  Unknown\n");
      break;
    case CacheTypeInstruction:
      Print (L"System Cache Type:       0x%02x  -  Instruction\n");
      break;
    case CacheTypeData:
      Print (L"System Cache Type:       0x%02x  -  Data\n");
      break;
    case CacheTypeUnified:
      Print (L"System Cache Type:       0x%02x  -  Unified\n");
      break;
    default:
      Print (L"System Cache Type:       0x%02x  -  no match system cache type\n"); 
  }
}


EFI_STATUS PrintTypeHeader(UINT64 HeaderAddr)
{
  SMBIOS_STRUCTURE *Header = (SMBIOS_STRUCTURE *)HeaderAddr;
  Print (L"\n*************************************************************************\n");
  //ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_QUERYTABLE_STRUCT_TYPE));
  Print (L"Type:                              %s  0x%02x\n", GetTypeTableName(Header->Type), Header->Type);
  Print (L"Length:                            0x%02x\n", Header->Length);
  Print (L"Handle:                            0x%04x\n", Header->Handle);
  return EFI_SUCCESS;
}



EFI_STATUS PrintType00Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE0 *Type0 = (SMBIOS_TABLE_TYPE0 *)HeaderAddr;
  Print (L"Vendor:                        %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"BiosVersion:                   %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"BiosSegment:                   %d\n", Type0->BiosSegment);
  Print (L"BiosReleaseDate:               %s\n", GetStringsFromType(HeaderAddr, 3));
  Print (L"BiosSize:                      %d KB\n", (Type0->BiosSize + 1) * 64);
  if (Type0->BiosCharacteristics.BiosCharacteristicsNotSupported != 1){
    ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_BIOS_CHAR));
    Print (L"BIOS Characteristics: 0x%x\n", Type0->BiosCharacteristics);
    Print (L"   bit0:         Reserved\n");
    Print (L"   bit1:         Reserved\n");
    Print (L"   bit2:         Unknown\n");
    Print (L"   bit3: %d      BIOS Characteristics are not supported  - (%s)\n", Type0->BiosCharacteristics.BiosCharacteristicsNotSupported, GetYesOrNoFromBool(Type0->BiosCharacteristics.BiosCharacteristicsNotSupported));
    Print (L"   bit4: %d      ISA %s\n", Type0->BiosCharacteristics.IsaIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.IsaIsSupported));
    Print (L"   bit5: %d      MCA %s\n", Type0->BiosCharacteristics.McaIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.McaIsSupported));
    Print (L"   bit6: %d      EISA %s\n", Type0->BiosCharacteristics.EisaIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.EisaIsSupported));
    Print (L"   bit7: %d      PCI %s\n", Type0->BiosCharacteristics.PciIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.PciIsSupported));
    Print (L"   bit8: %d      PCMCIA %s\n", Type0->BiosCharacteristics.PcmciaIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.PcmciaIsSupported));
    Print (L"   bit9: %d      Plug and Play %s\n", Type0->BiosCharacteristics.PlugAndPlayIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.PlugAndPlayIsSupported));
    Print (L"   bit10: %d     APM %s\n", Type0->BiosCharacteristics.ApmIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.ApmIsSupported));
    Print (L"   bit11: %d     BiosIsUpgradable %s\n", Type0->BiosCharacteristics.BiosIsUpgradable, GetStringFromBiosChar(Type0->BiosCharacteristics.BiosIsUpgradable));
    Print (L"   bit12: %d     BiosShadowingAllowed %s\n", Type0->BiosCharacteristics.BiosShadowingAllowed, GetStringFromBiosChar(Type0->BiosCharacteristics.BiosShadowingAllowed));
    Print (L"   bit13: %d     VL-VESA %s\n", Type0->BiosCharacteristics.VlVesaIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.VlVesaIsSupported));
    Print (L"   bit14: %d     EscdSupportIsAvailable %s\n", Type0->BiosCharacteristics.EscdSupportIsAvailable, GetStringFromBiosChar(Type0->BiosCharacteristics.EscdSupportIsAvailable));
    Print (L"   bit15: %d     Boot from CD %s\n", Type0->BiosCharacteristics.BootFromCdIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.BootFromCdIsSupported));
    Print (L"   bit16: %d     Selectable boot  %s\n", Type0->BiosCharacteristics.SelectableBootIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.SelectableBootIsSupported));
    Print (L"   bit17: %d     RomBiosIsSocketed %s\n", Type0->BiosCharacteristics.RomBiosIsSocketed, GetStringFromBiosChar(Type0->BiosCharacteristics.RomBiosIsSocketed));
    Print (L"   bit18: %d     Boot from PCMCIA  %s\n", Type0->BiosCharacteristics.BootFromPcmciaIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.BootFromPcmciaIsSupported));
    Print (L"   bit19: %d     EDD specification %s\n", Type0->BiosCharacteristics.EDDSpecificationIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.EDDSpecificationIsSupported));
    Print (L"   bit20: %d     Int 13h -- Japanese floppy for NEC 9800 1.2 MB (3.5\", 1K bytes/sector, 360 RPM)  %s\n", Type0->BiosCharacteristics.JapaneseNecFloppyIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.JapaneseNecFloppyIsSupported));
    Print (L"   bit21: %d     Int 13h -- Japanese floppy for Toshiba 1.2 MB (3.5\", 360 RPM)  %s\n", Type0->BiosCharacteristics.JapaneseToshibaFloppyIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.JapaneseToshibaFloppyIsSupported));
    Print (L"   bit22: %d     Int 13h -- 5.25\" / 360 KB floppy services %s\n", Type0->BiosCharacteristics.Floppy525_360IsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.Floppy525_360IsSupported));
    Print (L"   bit23: %d     Int 13h -- 5.25\" /1.2 MB floppy services %s\n", Type0->BiosCharacteristics.Floppy525_12IsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.Floppy525_12IsSupported));
    Print (L"   bit24: %d     Int 13h — 3.5\" / 720 KB floppy services %s\n", Type0->BiosCharacteristics.Floppy35_720IsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.EisaIsSupported));
    Print (L"   bit25: %d     Int 13h — 3.5\" / 2.88 MB floppy services %s\n", Type0->BiosCharacteristics.Floppy35_288IsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.Floppy35_288IsSupported));
    Print (L"   bit26: %d     Int 5h, print screen Service %s\n", Type0->BiosCharacteristics.PrintScreenIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.PrintScreenIsSupported));
    Print (L"   bit27: %d     Int 9h, 8042 keyboard services %s\n", Type0->BiosCharacteristics.Keyboard8042IsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.Keyboard8042IsSupported));
    Print (L"   bit28: %d     Int 14h, serial services  %s\n", Type0->BiosCharacteristics.SerialIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.SerialIsSupported));
    Print (L"   bit29: %d     Int 17h, printer services %s\n", Type0->BiosCharacteristics.PrinterIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.PrinterIsSupported));
    Print (L"   bit30: %d     Int 10h, CGA/Mono Video Services %s\n", Type0->BiosCharacteristics.CgaMonoIsSupported, GetStringFromBiosChar(Type0->BiosCharacteristics.CgaMonoIsSupported));
    Print (L"   bit31: %d     NEC PC-98 %s\n", Type0->BiosCharacteristics.NecPc98);

    if (SMBIOS_VERSION >= 24) {
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_BIOS_CHAR_EXTENSION));
      Print (L"BIOS Characteristics Extension Byte1: 0x%02x\n", Type0->BIOSCharacteristicsExtensionBytes[0]);
      MBCE_BIOS_RESERVED *ExBiosCharByte1 = (MBCE_BIOS_RESERVED *)(&Type0->BIOSCharacteristicsExtensionBytes[0]);
      Print (L"   bit0: %d   ACPI is supported - (%s)  \n", ExBiosCharByte1->AcpiIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->AcpiIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ACPI_SUPPORTED));
      
      Print (L"   bit1: %d   USB Legacy is supported - (%s)  \n", ExBiosCharByte1->UsbLegacyIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->UsbLegacyIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_USB_LEGACY_SUPPORTED));
      
      Print (L"   bit2: %d   AGP is supported - (%s)  \n", ExBiosCharByte1->AgpIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->AgpIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_AGP_SUPPORTED));
      
      Print (L"   bit3: %d   I2O boot is supported - (%s)  \n", ExBiosCharByte1->I2OBootIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->I2OBootIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_I2O_BOOT_SUPPORTED));
      
      Print (L"   bit4: %d   LS-120 SuperDisk boot is supported - (%s)  \n", ExBiosCharByte1->Ls120BootIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->Ls120BootIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_LS_120_BOOT_SUPPORTED));
      
      Print (L"   bit5: %d   ATAPI ZIP drive boot is supported - (%s)  \n", ExBiosCharByte1->AtapiZipDriveBootIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->AtapiZipDriveBootIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ATAPI_ZIP_DRIVE));
      
      Print (L"   bit6: %d   1394 boot is supported - (%s)  \n", ExBiosCharByte1->Boot1394IsSupported, GetYesOrNoFromBool(ExBiosCharByte1->Boot1394IsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_1394_BOOT_SUPPORTED));
      
      Print (L"   bit7: %d   Smart battery is supported - (%s)  \n", ExBiosCharByte1->SmartBatteryIsSupported, GetYesOrNoFromBool(ExBiosCharByte1->SmartBatteryIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_SMART_BATTERY_SUPPORTED));

      Print (L"BIOS Characteristics Extension Byte2: 0x%02x\n", Type0->BIOSCharacteristicsExtensionBytes[1]);
      MBCE_SYSTEM_RESERVED *ExBiosCharByte2 = (MBCE_SYSTEM_RESERVED*)(&Type0->BIOSCharacteristicsExtensionBytes[1]);

      Print (L"   bit0: %d   BIOS Boot Specification is supported - (%s)  \n", ExBiosCharByte2->BiosBootSpecIsSupported, GetYesOrNoFromBool(ExBiosCharByte2->BiosBootSpecIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_BIOS_BOOT_SPEC_SUPP));

      Print (L"   bit1: %d   Function key-initiated network service boot is supported - (%s)  \n", ExBiosCharByte2->FunctionKeyNetworkBootIsSupported, GetYesOrNoFromBool(ExBiosCharByte2->FunctionKeyNetworkBootIsSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_FUNCTION_KEY_INIT));

      Print (L"   bit2: %d   Enable targeted content distribution - (%s)  \n", ExBiosCharByte2->TargetContentDistributionEnabled, GetYesOrNoFromBool(ExBiosCharByte2->TargetContentDistributionEnabled));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ENABLE_TAR_CONT_DIST));

      Print (L"   bit3: %d   UEFI Specification is supported - (%s)  \n", ExBiosCharByte2->UefiSpecificationSupported, GetYesOrNoFromBool(ExBiosCharByte2->UefiSpecificationSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_UEFI_SPEC_SUPPORT));

      Print (L"   bit4: %d   SMBIOS table describes a virtual machine - (%s)  \n", ExBiosCharByte2->VirtualMachineSupported, GetYesOrNoFromBool(ExBiosCharByte2->VirtualMachineSupported));
      ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_VIRTUAL_MACHINE));
      
    }
  }else{
    ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_BIOS_CHAR_NOT_SUPPORTED));
  }
  
  Print (L"Bios Release: %d.%d\n", Type0->SystemBiosMajorRelease, Type0->SystemBiosMinorRelease);
  Print (L"EC Release: %d.%d\n", Type0->EmbeddedControllerFirmwareMajorRelease, Type0->EmbeddedControllerFirmwareMinorRelease);
  Print (L"Bios Rom Size: 0x%x\n", Type0->ExtendedBiosSize);
  return EFI_SUCCESS;
}

EFI_STATUS PrintType01Info(UINT64       HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE1 *Type1 = (SMBIOS_TABLE_TYPE1 *)HeaderAddr;
  Print (L"Manufacturer:              %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Product Name:              %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"Version:                   %s\n", GetStringsFromType(HeaderAddr, 3));
  Print (L"Serial Number:             %s\n", GetStringsFromType(HeaderAddr, 4));
  Print (L"UUID:                      %g\n", Type1->Uuid);
  Print (L"Wakeup Type:               %s\n", GetWakeupTypeString(Type1->WakeUpType));
  if (SMBIOS_VERSION >= 24){
    Print (L"SKU Number:                %s\n", GetStringsFromType(HeaderAddr, 5));
    Print (L"Family:                    %s\n", GetStringsFromType(HeaderAddr, 6));
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType02Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE2 *Type2 = (SMBIOS_TABLE_TYPE2 *)HeaderAddr;
  Print (L"Manufacturer:              %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Pruduct:                   %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"Version:                   %s\n", GetStringsFromType(HeaderAddr, 3));
  Print (L"Serial Number:             %s\n", GetStringsFromType(HeaderAddr, 4));
  Print (L"Assert Tag:                %s\n", GetStringsFromType(HeaderAddr, 5));
  BASE_BOARD_FEATURE_FLAGS *BoardFeatureFlags = (BASE_BOARD_FEATURE_FLAGS*)(&Type2->FeatureFlag);
  Print (L"Feature Flags:             0x%02x\n", Type2->FeatureFlag);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_BASEBOARD_FEATURE_BIT0), HiiHandle,BoardFeatureFlags->Motherboard, GetYesOrNoFromBool(BoardFeatureFlags->Motherboard));
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_BASEBOARD_FEATURE_BIT1), HiiHandle,BoardFeatureFlags->RequiresDaughterCard, GetYesOrNoFromBool(BoardFeatureFlags->RequiresDaughterCard));
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_BASEBOARD_FEATURE_BIT2), HiiHandle,BoardFeatureFlags->Removable, GetYesOrNoFromBool(BoardFeatureFlags->Removable));
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_BASEBOARD_FEATURE_BIT3), HiiHandle,BoardFeatureFlags->Replaceable, GetYesOrNoFromBool(BoardFeatureFlags->Replaceable));
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_BASEBOARD_FEATURE_BIT4), HiiHandle,BoardFeatureFlags->HotSwappable, GetYesOrNoFromBool(BoardFeatureFlags->HotSwappable));
  Print (L"Location in Chassis:       %s\n", GetStringsFromType(HeaderAddr, 6));
  Print (L"Chassis Handle:            0x%x\n", Type2->ChassisHandle);
  Print (L"Board Type:                %s\n", GetBoardTypeString(Type2->BoardType));
  Print (L"Number of Contained Object Handles:  0x%x\n", Type2->NumberOfContainedObjectHandles);
  Print (L"Contained Object Handles:  0x%x", Type2->ContainedObjectHandles[0]);   
  return EFI_SUCCESS;
}

EFI_STATUS PrintType03Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE3 *Type3 = (SMBIOS_TABLE_TYPE3 *)HeaderAddr;
  Print (L"Manufacturer:                      %s\n", GetStringsFromType(HeaderAddr, 1));
  UINT8 Bit7 = GET_BIT_VAL(Type3->Type, 7);
  UINT8 Bit06;
  if (Bit7 == 0){
    Bit06 = Type3->Type;
  }else{
    Bit06 = Type3->Type / 2;
  }
  Print (L"Type:                              0x%02x\n", Bit06);
  Print (L"   Bit7:%d    Chassis lock is present if 1. Otherwise, either a lock is not present or it is unknown if the enclosure has a lock\n");
  Print (L"Version:                           %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"Serial Number:                     %s\n", GetStringsFromType(HeaderAddr, 3));
  Print (L"Assert Tag Number:                 %s\n", GetStringsFromType(HeaderAddr, 4));
  Print (L"Boot-up State:                     %s\n", GetChassisStateString(Type3->BootupState));
  Print (L"Power Supply State:                %s\n", GetChassisStateString(Type3->PowerSupplyState));
  Print (L"Thermal State:                     %s\n", GetChassisStateString(Type3->ThermalState));
  Print (L"Security State:                    %s\n", GetChassisSecStateString(Type3->SecurityStatus));
  Print (L"OEM-defined:                       0x%08x\n", *(Type3->OemDefined));
  Print (L"Height:                            0x%02x\n", Type3->Height);
  Print (L"Number of Power Cords:             0x%02x\n", Type3->NumberofPowerCords);
  Print (L"Contained Element Counts:          0x%02x\n", Type3->ContainedElementCount);
  Print (L"Contained Element Record length:   0x%02x\n", Type3->ContainedElementRecordLength);
  Print (L"Contained Elements:                0x%x\n", Type3->ContainedElementCount * Type3->ContainedElementRecordLength);
  if (SMBIOS_VERSION >= 27){
    /*
    if (Type3->ContainedElementCount * Type3->ContainedElementRecordLength  == 0){
      Print (L"SKU Number:                        NULL\n");
    }else{
      Print (L"SKU Number:                        %s\n", GetStringsFromType(HeaderAddr, 5));
    }
    */
    Print (L"SKU Number:                        %s\n", GetStringsFromType(HeaderAddr, 5));
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType04Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE4 *Type4 = (SMBIOS_TABLE_TYPE4 *)HeaderAddr;
  Print (L"Socket Dedsignation:                   %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Processor Type:                        %s\n", GetProcessorTypeDataString(Type4->ProcessorType));
  Print (L"Processor Family:  ");
  PrintProcessorFamily(Type4->ProcessorFamily);
  Print (L"Processor Manufacturer:                %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"Processor  ID:                         0x%x\n", *((UINT32 *)(&Type4->ProcessorId)));
  Print (L"Processor Version:                     %s\n", GetStringsFromType(HeaderAddr, 3));
  Print (L"Processor Voltage:                     0x%x\n", Type4->Voltage);
  if (Type4->ExternalClock == 0){
    Print (L"Extenal Clock:                         Unknown\n");
  }else{
    Print (L"Extenal Clock:                         0x%x (%d) MHz\n", Type4->ExternalClock, Type4->ExternalClock);
  }
  Print (L"Max Speed:                             0x%x  (%d) MHz\n", Type4->MaxSpeed,Type4->MaxSpeed);
  Print (L"Current Speed:                         0x%x  (%d) MHz\n", Type4->CurrentSpeed, Type4->CurrentSpeed);
  Print (L"Status:                                0x%x\n", Type4->Status);
  Print (L"Processor Upgrade:                     0x%x\n", Type4->ProcessorUpgrade);
  Print (L"Processor L1 Cache Handle:             0x%x\n", Type4->L1CacheHandle);
  Print (L"Processor L2 Cache Handle:             0x%x\n", Type4->L2CacheHandle);
  Print (L"Processor L3 Cache Handle:             0x%x\n", Type4->L3CacheHandle);
  Print (L"Serial Number:                         %s\n", GetStringsFromType(HeaderAddr, 4));
  Print (L"Assert Tag:                            %s\n", GetStringsFromType(HeaderAddr, 5));
  Print (L"Part Number:                           %s\n", GetStringsFromType(HeaderAddr, 6));
  Print (L"Core Count:                            0x%02x\n", Type4->CoreCount);
  Print (L"Core Enabled:                          0x%02x\n", Type4->EnabledCoreCount);
  Print (L"Thread Count:                          0x%02x\n", Type4->ThreadCount);
  if (SMBIOS_VERSION >= 25){
    PrintType04ProcessorChar(Type4);
  }
  if (SMBIOS_VERSION >= 26){
    Print (L"Processor Family 2:  ");
    PrintProcessorFamily(Type4->ProcessorFamily2);
  }
  if (SMBIOS_VERSION >= 30){
    Print (L"Core Count2:                           0x%02x\n", Type4->CoreCount2);
    Print (L"Core Enabled2:                         0x%02x\n", Type4->EnabledCoreCount2);
    Print (L"Thread Count2:                         0x%02x\n", Type4->ThreadCount2);
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType05Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE5 *Type5 = (SMBIOS_TABLE_TYPE5 *)HeaderAddr;
  Print (L"Error Detecting Method:                    0x%x\n", Type5->ErrDetectMethod);
  Print (L"Error Correcting Capability:               0x%x\n", Type5->ErrCorrectCapability);
  Print (L"Supported Interleave:                      0x%x\n", Type5->SupportInterleave);
  Print (L"Current Interleave:                        0x%x\n", Type5->CurrentInterleave);
  Print (L"Maximum Memory Module Size:                0x%x\n", Type5->MaxMemoryModuleSize);
  Print (L"Supported Speeds:                          %b\n", Type5->SupportSpeed);
  Print (L"Supported Memory Types:                    %b\n", Type5->SupportMemoryType);
  Print (L"Memory Module Voltage:                     %b\n", Type5->MemoryModuleVoltage);
  Print (L"Number of Associated Memory Slots:         0x%x\n", Type5->AssociatedMemorySlotNum);
  Print (L"Memory Module Configuration Handles:       0x%x\n", Type5->MemoryModuleConfigHandles[0]);
  return EFI_SUCCESS;
}

EFI_STATUS PrintType06Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE6 *Type6 = (SMBIOS_TABLE_TYPE6 *)HeaderAddr;
  Print (L"Socket Designation:                        %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Bank Connectiongs:                         0x%x\n", Type6->BankConnections);
  Print (L"Current Speed:                             0x%x\n", Type6->CurrentSpeed);
  Print (L"Current Memory Type:                       %b\n", Type6->CurrentMemoryType);
  Print (L"Installed Size:                            0x%x\n", Type6->InstalledSize);
  Print (L"Enabled Size:                              0x%x\n", Type6->EnabledSize);
  Print (L"Errors Status:                             %b\n", Type6->ErrorStatus);    
  return EFI_SUCCESS;
}

EFI_STATUS PrintType07Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE7 *Type7 = (SMBIOS_TABLE_TYPE7 *)HeaderAddr;
  Print (L"Socket Designation:                        %s", GetStringsFromType(HeaderAddr,1));
  PrintCacheConf(&(Type7->CacheConfiguration));
  Print (L"Max Cache Size:                            0x%x\n", Type7->MaximumCacheSize);
  Print (L"Installed Size:                            0x%x\n", Type7->InstalledSize);
  
  CACHE_SRAM_TYPE_DATA* CacheSramTypeData = (CACHE_SRAM_TYPE_DATA*)(&(Type7->SupportedSRAMType));
  Print (L"Supported SRAM Type:\n");
  Print (L"   Bit0 = %d        Other - (%s)\n", CacheSramTypeData->Other, GetYesOrNoFromBool(CacheSramTypeData->Other));
  Print (L"   Bit1 = %d        Unknown - (%s)\n", CacheSramTypeData->Unknown, GetYesOrNoFromBool(CacheSramTypeData->Unknown));
  Print (L"   Bit2 = %d        Non-Burst - (%s)\n", CacheSramTypeData->NonBurst, GetYesOrNoFromBool(CacheSramTypeData->NonBurst));
  Print (L"   Bit3 = %d        Burst - (%s)\n", CacheSramTypeData->Burst, GetYesOrNoFromBool(CacheSramTypeData->Burst));
  Print (L"   Bit4 = %d        Pipeline Burst  - (%s)\n", CacheSramTypeData->PipelineBurst, GetYesOrNoFromBool(CacheSramTypeData->PipelineBurst));
  Print (L"   Bit5 = %d        Synchronous - (%s)\n", CacheSramTypeData->Synchronous, GetYesOrNoFromBool(CacheSramTypeData->Synchronous));
  Print (L"   Bit6 = %d        Asynchronous  - (%s)\n", CacheSramTypeData->Asynchronous, GetYesOrNoFromBool(CacheSramTypeData->Asynchronous));

  CACHE_SRAM_TYPE_DATA* CurrentSramTypeData = (CACHE_SRAM_TYPE_DATA*)(&(Type7->CurrentSRAMType));
  Print (L"Current SRAM Type:\n");
  Print (L"   Bit0 = %d        Other - (%s)\n", CurrentSramTypeData->Other, GetYesOrNoFromBool(CurrentSramTypeData->Other));
  Print (L"   Bit1 = %d        Unknown - (%s)\n", CurrentSramTypeData->Unknown, GetYesOrNoFromBool(CurrentSramTypeData->Unknown));
  Print (L"   Bit2 = %d        Non-Burst - (%s)\n", CurrentSramTypeData->NonBurst, GetYesOrNoFromBool(CurrentSramTypeData->NonBurst));
  Print (L"   Bit3 = %d        Burst - (%s)\n", CurrentSramTypeData->Burst, GetYesOrNoFromBool(CurrentSramTypeData->Burst));
  Print (L"   Bit4 = %d        Pipeline Burst  - (%s)\n", CurrentSramTypeData->PipelineBurst, GetYesOrNoFromBool(CurrentSramTypeData->PipelineBurst));
  Print (L"   Bit5 = %d        Synchronous - (%s)\n", CurrentSramTypeData->Synchronous, GetYesOrNoFromBool(CurrentSramTypeData->Synchronous));
  Print (L"   Bit6 = %d        Asynchronous  - (%s)\n", CurrentSramTypeData->Asynchronous, GetYesOrNoFromBool(CurrentSramTypeData->Asynchronous));

  Print (L"Cache Speed:                               0x%02x\n", Type7->CacheSpeed);
  PrintErrorCorrectionType(Type7->ErrorCorrectionType);
  PrintSystemCacheType(Type7->SystemCacheType);
  Print (L"Associativity:                             0x%02x\n", Type7->Associativity);
  if(SMBIOS_VERSION >= 31){
    Print (L"Maximum Cache Size 2:                    0x%x\n", Type7->MaximumCacheSize2);
    Print (L"Installed Cache Size 2:                  0x%x\n", Type7->InstalledSize2);
  } 
  return EFI_SUCCESS;
}

EFI_STATUS PrintType08Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE8 *Type8 = (SMBIOS_TABLE_TYPE8 *)HeaderAddr;
  Print (L"Internal Reference Designator:             %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Internal Connector Type:                   0x%02x - %s\n", Type8->InternalConnectorType, PortConnectorTypeTable[Type8->InternalConnectorType].Info);
  Print (L"External Reference Designator:             %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"Enternal Connector Type:                   0x%02x - %s\n", Type8->ExternalConnectorType, PortConnectorTypeTable[Type8->ExternalConnectorType].Info);
  Print (L"Port type:                                 0x%02x - %s\n", Type8->PortType, PortTypeTable[Type8->PortType]); 
  return EFI_SUCCESS;
}

EFI_STATUS PrintType09Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE9 *Type9 = (SMBIOS_TABLE_TYPE9 *)HeaderAddr;
  Print (L"Slot Designation:        %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Slot Type:               0x%02x  - %s\n", Type9->SlotType, SystemSlotTypeTable[Type9->SlotType - 1].Info);
  Print (L"Slot Data Bus Width:     0x%02x  - %s\n", Type9->SlotDataBusWidth, SystemSlotDataBusWidthTable[Type9->SlotDataBusWidth - 1].Info);
  Print (L"Current Usage:           0x%02x  - %s\n", Type9->CurrentUsage, SystemSlotCurrentUsageTable[Type9->CurrentUsage - 1].Info);
  Print (L"Slot Length:             0x%02x  - %s\n", Type9->SlotLength, SystemSlotLengthTable[Type9->SlotLength - 1].Info);
  Print (L"Slot ID:                 0x%04x\n", Type9->SlotID);
  Print (L"Slot Characteristics 1:  0x%02x\n", Type9->SlotCharacteristics1);
  //MISC_SLOT_CHARACTERISTICS1* SlotCharPtr = (MISC_SLOT_CHARACTERISTICS1*)(&Type9->SlotCharacteristics1);
  for (UINT32 Index = 0; Index < 8; Index++){
    UINT32 *SlotChar1Ptr = (UINT32*)(&Type9->SlotCharacteristics1); 
    Print (L"   Bit%d                 %s  - %s\n", Index, SlotCharacteristics1Table[Index].Info, GetYesOrNoFromBool(GET_BIT_VAL(*SlotChar1Ptr, Index)));
  }
  Print (L"Slot Characteristics 2:  0x%02x\n", Type9->SlotCharacteristics2);
  for (UINT32 Index = 0; Index < 3; Index++){
    UINT32 *SlotChar2Ptr = (UINT32*)(&Type9->SlotCharacteristics2);
    Print (L"   Bit%d                 %s  - %s\n", Index, SlotCharacteristics2Table[Index].Info, GetYesOrNoFromBool(GET_BIT_VAL(*SlotChar2Ptr, Index)));
  }
  if (SMBIOS_VERSION >= 26){
    Print (L"Segment Group Number:    0x04x\n", Type9->SegmentGroupNum);
    Print (L"Bus Number:              0x02x\n", Type9->BusNum);
    Print (L"Device/Function Number:  0x02x\n", Type9->DevFuncNum);
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType10Info(UINT64 HeaderAddr)
{
  SMBIOS_TABLE_TYPE10 *Type10 = (SMBIOS_TABLE_TYPE10 *)HeaderAddr;
  PrintTypeHeader(HeaderAddr);
  Print (L"Device Type:             0x%02x\n", Type10->Device->DeviceType);
  Print (L"Description String:      %s\n", GetStringsFromType(HeaderAddr, 1));
  return EFI_SUCCESS;
}

EFI_STATUS PrintType11Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE11 *Type11 = (SMBIOS_TABLE_TYPE11 *)HeaderAddr;
  UINT8 Count = Type11->StringCount;
  Print (L"Count:                   0x%02x\n", Count);
  for (UINT8 Index = 1; Index <= Count; Index++){
    Print (L"   String%d - %s\n", Index, GetStringsFromType(HeaderAddr, Index));
  }

  return EFI_SUCCESS;
}

EFI_STATUS PrintType12Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE12 *Type12 = (SMBIOS_TABLE_TYPE12 *)HeaderAddr;
  UINT8 Count = Type12->StringCount;
  Print (L"Count:                   0x%02x\n", Count);
  for (UINT8 Index = 1; Index <= Count; Index++){
    Print (L"   String%d - %s\n", Index, GetStringsFromType(HeaderAddr, Index));
  }
  
  return EFI_SUCCESS;
}

EFI_STATUS PrintType13Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE13 *Type13 = (SMBIOS_TABLE_TYPE13 *)HeaderAddr;
  Print (L"Installable Languages:             0x%02x\n", Type13->InstallableLanguages);
  Print (L"Flags:                             %d\n", GET_BIT_VAL(Type13->Flags, 0));
  UINT8 Count = Type13->InstallableLanguages;
  for (UINT8 Index = 1; Index <= Count; Index++){
    Print (L"   String%d - %s\n", Index, GetStringsFromType(HeaderAddr, Index));
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType14Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE14 *Type14 = (SMBIOS_TABLE_TYPE14 *)HeaderAddr;
  Print (L"Group Name:                        %s\n", GetStringsFromType(HeaderAddr, 1));
  GROUP_STRUCT *ItemAddr = (GROUP_STRUCT*)Type14->Group;
  Print (L"   Item Type:                      0x%02x\n", ItemAddr->ItemType);
  Print (L"   Item Handle:                    0x%04x\n", ItemAddr->ItemHandle);
  /*
  UINT8 ItemCount = (Type14->Hdr.Length - 5) / 3;
  GROUP_STRUCT *ItemAddr = (GROUP_STRUCT*)Type14->Group;
  for (UINT8 Index = 0; Index < ItemCount; Index++){
    Print (L"   Item Type:                      0x%02x\n", ItemAddr->ItemType);
    Print (L"   Item Handle:                    0x%04x\n", ItemAddr->ItemHandle);
    ItemCount++;
  }
  */
  return EFI_SUCCESS;
}

EFI_STATUS PrintType15Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE15 *Type15 = (SMBIOS_TABLE_TYPE15 *)HeaderAddr;
  Print (L"Log Area Length:                   0x%04x\n", Type15->LogAreaLength);
  Print (L"Log Header Start Offset:           0x%04x\n", Type15->LogHeaderStartOffset);
  Print (L"Log Data Start Offset:             0x%04x\n", Type15->LogDataStartOffset);
  Print (L"Access Method:                     0x%02x\n", Type15->AccessMethod);
  Print (L"Log Status:                        0x%02x\n", Type15->LogStatus);
  Print (L"Log Change Token:                  0x%08x\n", Type15->LogChangeToken);
  Print (L"Access Method Address:             0x%08x\n", Type15->AccessMethodAddress);
  Print (L"Log Header Format:                 0x%02x\n", Type15->LogHeaderFormat);
  UINT32 NumberSupportedLogType = Type15->NumberOfSupportedLogTypeDescriptors;
  UINT32 LengthEachLogType = Type15->LengthOfLogTypeDescriptor;
  Print (L"Number of Supported Log Type:      0x%02x\n", NumberSupportedLogType);
  Print (L"Length of Each Log Type:           0x%02x\n", LengthEachLogType);
  if (NumberSupportedLogType != 0){
    Print (L"List of Supported Event Log Type:  0x%x\n", NumberSupportedLogType * LengthEachLogType);
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType16Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE16 *Type16 = (SMBIOS_TABLE_TYPE16 *)HeaderAddr;
  Print (L"Location:                          0x%02x  - %s\n", Type16->Location, PMALocationTable[Type16->Location - 1].Info);
  Print (L"Use:                               0x%02x  - %s\n", Type16->Use, PMAUseTable[Type16->Use - 1].Info);
  Print (L"Memory Error Correction:           0x%02x  - %s\n", Type16->MemoryErrorCorrection, PMAErrorCorrectionTypesTable[Type16->MemoryErrorCorrection - 1].Info);
  Print (L"Maximum Capacity:                  0x%08x  - %dKB, %dMB\n", Type16->MaximumCapacity, Type16->MaximumCapacity, Type16->MaximumCapacity / 1024);
  Print (L"Memory Error Information Handle:   0x%04x\n", Type16->MemoryErrorInformationHandle);
  Print (L"Number of Memory Devices:          0x%04x\n", Type16->NumberOfMemoryDevices);
  if (SMBIOS_VERSION >= 27){
    Print (L"Extended Maximum Capacity:         0x08x\n", Type16->ExtendedMaximumCapacity);
  }
  return EFI_SUCCESS;
}

EFI_STATUS PrintType17Info(UINT64 HeaderAddr)
{
  PrintTypeHeader(HeaderAddr);
  SMBIOS_TABLE_TYPE17 *Type17 = (SMBIOS_TABLE_TYPE17 *)HeaderAddr;
  Print (L"Physical Memory Array Handle:      0x%04x\n", Type17->MemoryArrayHandle);
  Print (L"Memory Error Information Handle:   0x%04x", Type17->MemoryErrorInformationHandle);
  if (Type17->MemoryErrorInformationHandle == 0xFFFFE){
    Print (L" - system does not provide the error information structure\n");
  }else if (Type17->MemoryErrorInformationHandle == 0xFFFF){
    Print (L" - no error was detected\n");
  }
  Print (L"Total Width:                       0x%04x (%d)bits\n", Type17->TotalWidth, Type17->TotalWidth);
  Print (L"Data Width:                        0x%04x (%d)bits\n", Type17->DataWidth, Type17->DataWidth);
  Print (L"Size:                              0x%04x  - %d", Type17->Size, Type17->Size);
  if (GET_BIT_VAL(Type17->Size, 15) == 0){
    Print (L"MB\n");
  }else if (GET_BIT_VAL(Type17->Size, 15) == 1){
    Print (L"KB\n");
  }
  Print (L"Form Factor:                       0x%02x  - %s\n", Type17->FormFactor, MemoryDeviceFormFactorTable[Type17->FormFactor - 1].Info);
  Print (L"Device Set:                        0x%02x", Type17->DeviceSet);
  if (Type17->DeviceSet == 0){
    Print (L" -  the device is not part of a set\n");
  }else if (Type17->DeviceSet == 0xFF){
    Print (L" - attribute is unknown\n");
  }
  Print (L"Device Locator:                    %s\n", GetStringsFromType(HeaderAddr, 1));
  Print (L"Bank Locator:                      %s\n", GetStringsFromType(HeaderAddr, 2));
  Print (L"Memory Type:                       0x%02x  - %s\n", Type17->MemoryType, MemoryDeviceTypeTable[Type17->MemoryType - 1].Info);
  Print (L"Type Detail:                       0x%04x\n", Type17->TypeDetail);
  UINT16 *TypeDetail = (UINT16*)(&Type17->TypeDetail);
  for (UINT32 Index = 0; Index < sizeof(MemoryDeviceTypeDetailTable) / sizeof(TABLE_ITEM); Index++){
    Print (L"   Bit%d                           %s - %d\n", Index, MemoryDeviceTypeDetailTable[Index].Info, GET_BIT_VAL(*TypeDetail, Index));
  }
  Print (L"Speed:                             0x%04x\n", Type17->Speed);
  Print (L"Manufacture:                       %s\n", GetStringsFromType(HeaderAddr, 3));
  Print (L"Serial Number:                     %s\n", GetStringsFromType(HeaderAddr, 4));
  Print (L"Assert Tag:                        %s\n", GetStringsFromType(HeaderAddr, 5));
  Print (L"Part Number:                       %s\n", GetStringsFromType(HeaderAddr, 6));
  if (SMBIOS_VERSION >= 26){
    Print (L"Attributes:                        0x%02x\n", Type17->Attributes);
  }
  if (SMBIOS_VERSION >= 27){
    Print (L"Extended Size:                     0x%08x\n", Type17->ExtendedSize);
    Print (L"Configured Memory Clock Speed:     0x%04xMHz\n", Type17->ConfiguredMemoryClockSpeed);
  }
  if(SMBIOS_VERSION >= 28){
    Print (L"Minimum Voltage:                   0x%04x\n", Type17->MinimumVoltage);
    Print (L"Maximum Voltage:                   0x%04x\n", Type17->MaximumVoltage);
    Print (L"Configured Voltage:                0x%04x\n", Type17->ConfiguredVoltage);
  }
  return EFI_SUCCESS;
}



EFI_STATUS  PrintTypeContent(UINT64 TypeAddr)
{
  SMBIOS_STRUCTURE *HeaderAddr = (SMBIOS_STRUCTURE *)TypeAddr;
  UINT8 Type = HeaderAddr->Type;
  //Print (L"     Type:   %d", Type);
  switch (Type){
    case SMBIOS_TYPE_BIOS_INFORMATION:
    PrintType00Info(TypeAddr);
    break;
    case SMBIOS_TYPE_SYSTEM_INFORMATION:
    PrintType01Info(TypeAddr);
    break;
    case SMBIOS_TYPE_BASEBOARD_INFORMATION:
    PrintType02Info(TypeAddr);
    break;
    case SMBIOS_TYPE_SYSTEM_ENCLOSURE:
    PrintType03Info(TypeAddr);
    break;
    case SMBIOS_TYPE_PROCESSOR_INFORMATION:
    PrintType04Info(TypeAddr);
    break;
    case SMBIOS_TYPE_MEMORY_CONTROLLER_INFORMATION:
    PrintType05Info(TypeAddr);
    break;
    case SMBIOS_TYPE_MEMORY_MODULE_INFORMATON:
    PrintType06Info(TypeAddr);
    break;
    case SMBIOS_TYPE_CACHE_INFORMATION:
    PrintType07Info(TypeAddr);
    break;
    case SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION:
    PrintType08Info(TypeAddr);
      break;
    case SMBIOS_TYPE_SYSTEM_SLOTS:
    PrintType09Info(TypeAddr);
      break;
    case SMBIOS_TYPE_ONBOARD_DEVICE_INFORMATION:
    PrintType10Info(TypeAddr);
      break;
    case SMBIOS_TYPE_OEM_STRINGS:
    PrintType11Info(TypeAddr);
      break;
    case SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS:
    PrintType12Info(TypeAddr);
      break;
    case SMBIOS_TYPE_BIOS_LANGUAGE_INFORMATION:
    PrintType13Info(TypeAddr);
      break;
    case SMBIOS_TYPE_GROUP_ASSOCIATIONS:
    PrintType14Info(TypeAddr);
      break;
    case SMBIOS_TYPE_SYSTEM_EVENT_LOG:
    PrintType15Info(TypeAddr);
      break;
    case SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY:
    PrintType16Info(TypeAddr);
      break;
    case SMBIOS_TYPE_MEMORY_DEVICE:
    PrintType17Info(TypeAddr);
      break; 
  }
  return EFI_SUCCESS;
}


VOID* GetSmbiosX64EPS()
{
  SMBIOS_TABLE_3_0_ENTRY_POINT *SMBIOS_X64_EPS;
  UINTN Index;
  for (Index = 0; Index < gST->NumberOfTableEntries; Index++)
  {
    if (CompareGuid(&(gST->ConfigurationTable[Index].VendorGuid), gEfiSmbios3TableGuid))
    {
      SMBIOS_X64_EPS = gST->ConfigurationTable[Index].VendorTable;
      break;
    }
  }

  if (SMBIOS_X64_EPS != NULL){
    return SMBIOS_X64_EPS;
  }

}

VOID PrintSmbiosEPS(SMBIOS_TABLE_3_0_ENTRY_POINT *SMBIOS_X64_EPS)
{
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_64_BIT_ENTRY_POINT_SIGN), HiiHandle);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ANCHOR_STR), HiiHandle, SMBIOS_X64_EPS->AnchorString);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_EPS_CHECKSUM), HiiHandle, SMBIOS_X64_EPS->EntryPointStructureChecksum);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ENTRY_POINT_LEN), HiiHandle, SMBIOS_X64_EPS->EntryPointLength);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_MAJOR_VERSION), HiiHandle, SMBIOS_X64_EPS->MajorVersion);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_MINOR_VERSION), HiiHandle, SMBIOS_X64_EPS->MinorVersion);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_DOCREV), HiiHandle, SMBIOS_X64_EPS->DocRev);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_ENTRY_POINT_REVISION), HiiHandle, SMBIOS_X64_EPS->EntryPointRevision);
  //ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_RESERVED_BIT), HiiHandle);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_MAX_STRUCT_TABLE_SIZE), HiiHandle, SMBIOS_X64_EPS->TableMaximumSize);
  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_PRINTINFO_TABLE_ADDR), HiiHandle, SMBIOS_X64_EPS->TableAddress); 
}

UINT64 GetNextTypeAddr(UINT64 TypeAddr)
{
  SMBIOS_STRUCTURE *Header = (SMBIOS_STRUCTURE*)TypeAddr;
  Print (L"\nheader: 0x%x   length:%d   Type:0x%x", Header, Header->Length, Header->Type);
  UINT64  TextAreaAddr = (UINT64)Header + Header->Length;
  UINT8  *Tmp;
  UINT8 Index = 0;
  for (Tmp = Header; Tmp <= Header + Header->Length; Tmp++){
    if(Index % 16 == 0){
      Print (L"\n");
    }
    Print (L"%02x ",*Tmp);
    Index++;
  }
 
  BOOLEAN IsStringEnd = FALSE;
  for (Tmp = TextAreaAddr; Tmp <= TextAreaAddr + Table_Max_Size; Tmp++){
    Index++;
    if(Index % 16 == 0){
      Print (L"\n");
    }
    
    Print (L"%02x ",*Tmp);
    if (*Tmp == 0){
      if (IsStringEnd == TRUE){
        Print (L"\n");
        return ++Tmp;
      }
      IsStringEnd = TRUE;
    }else{
      IsStringEnd = FALSE;
    }
    
  }
  return 0;
}


VOID InitTypeTableAddr(SMBIOS_TABLE_3_0_ENTRY_POINT *SMBIOS_X64_EPS)
{
  SMBIOS_VERSION = SMBIOS_X64_EPS->MajorVersion * 10 + SMBIOS_X64_EPS->MinorVersion;
  SMBIOS_STRUCTURE *TypeHeader = (SMBIOS_STRUCTURE *)SMBIOS_X64_EPS->TableAddress;
  while ((TypeHeader->Type != SMBIOS_TYPE_END_OF_TABLE && (TypeHeader != 0)))
  {
    PrintTypeContent((UINT64)TypeHeader);
    TypeHeader = GetNextTypeAddr(TypeHeader);
    Table_Count++;
  }
  Print (L"end of Table~~~~~~~~   Total Table:  %d (except end of table)\n", Table_Count);
}


EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  mSystemTable = SystemTable;

  EFI_GUID mStrPackageGuid = {0x5a0d8524,0xf6cb,0xfd32,{0x01,0x01,0x17,0x44,0xb5,0xc3,0x37,0x67}};
  HiiHandle = HiiAddPackages(&mStrPackageGuid, ImageHandle, SmbiosReaderStrings, NULL);
  if(HiiHandle == NULL){
    Print (L"HiiHandle is NULL\n");
    return EFI_LOAD_ERROR;
  }
  TmpUnicodeBuffer = AllocatePool(SMBIOS_STRING_MAX_LENGTH);
  SMBIOS_TABLE_3_0_ENTRY_POINT *SMBIOS_X64_EPS;
  SMBIOS_X64_EPS = GetSmbiosX64EPS();
  Table_Max_Size = SMBIOS_X64_EPS->TableMaximumSize;
  Print (L"smbios eps addr: 0x%x\n", SMBIOS_X64_EPS);
  PrintSmbiosEPS(SMBIOS_X64_EPS);
  InitTypeTableAddr(SMBIOS_X64_EPS);
  FreePool(TmpUnicodeBuffer);
  return EFI_SUCCESS;
}
