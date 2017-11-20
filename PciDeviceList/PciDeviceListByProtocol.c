#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciRootBridgeIo.h>
#include <IndustryStandard/Pci30.h>
#include "PciCommon.h"

#define CALC_EFI_PCI_ADDRESS(Bus, Dev, Func, Reg) \
    ((UINT64) ((((UINTN) Bus) << 24) + (((UINTN) Dev) << 16) + (((UINTN) Func) << 8) + ((UINTN) Reg)))


PCI_BASE_CLASS_CODE PciBaseClassCodeList[] = {
  {
    0x00,
    L"Device was built before class code definition"
  },
  {
    0x01,
      L"Mass storge controller"
  },
      {
    0x02,
      L"Network controller"
  },
      {
    0x03,
      L"Display controller"
  },
      {
    0x04,
      L"Multimedia device"
  },
      {
    0x05,
      L"Memory controller"
  },
      {
    0x06,
      L"Bridge controller"
  },
      {
    0x07,
      L"Simple comunication controllers"
  },
      {
    0x08,
      L"Base system peripherals"
  },
      {
    0x09,
      L"input devices"
  },
      {
    0x0A,
      L"Docking stations"
  },
      {
    0x0B,
      L"Processors"
  },
      {
    0x0C,
      L"Serial bus controller"
  },
      {
    0x0D,
      L"Wireless controller"
  },
      {
      0x0E,
      L"inteligent I/O controllers"
  },
        {
      0x0F,
        L"Satellite comunication controllers"
  },
        {
      0x10,
        L"Encryption/Decryption controllers"
  },
        {
      0x11,
        L"Data acquisition and signal processing controllers"
  },
        {
      0xFF,
        L"not fit any classes"
  }
    
};

// PCI_3.0 spec
UINT8 PciSubclassCount[MAX_SUBCLASS_COUNT] = {2, 9, 8, 4, 4, 3, 12, 7, 7, 6,    2, 8, 11, 8, 1, 5, 3, 5};


PCI_SUB_CLASS_CODE PCISubClass_00[] = {
  {
    0x00,
    L"All devices other than VGA"
  },
  {
    0x01,
    L"VGA-compatible devices"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_01[] = {
  {
    0x00,
    L"SCSI controller"
  },
  {
    0x01,
    L"IDE controller"
  },
  {
    0x02,
    L"Floppy disk controller"
  },
  {
    0x03,
    L"IPI controller"
  },
  {
    0x04,
    L"RAID controller"
  },
  {
    0X05,
      L"ATA controller with ADMA interface"
  },
  {
    0x06,
      L"Serial ATA controller"
  },
  {
    0x07,
      L"Serial Attached SCSI (SAS) controller"
  },
  {
    0x80,
    L"Other mass storage controller"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_02[] = {
  {
    0x00,
    L"Ethernet controller"
  },
  {
    0x01,
    L"Token ring controller"
  },
  {
    0x02,
    L"FDDI controller"
  },
  {
    0x03,
    L"ATM controller"
  },
  {
    0x04,
    L"ISDN controller"
  },
  {
    0x05,
    L"WorldFip controller"
  },
  {
    0x06,
    L"PICMG 2.14 Multi Computing"
  },
  {
    0x80,
    L"Other network controller"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_03[] = {
  {
    0x00,
    L"VGA/8514 controller"
  },
  {
    0x01,
    L"XGA controller"
  },
  {
    0x02,
    L"3D controller"
  },
  {
    0x80,
    L"Other display controller"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_04[] = {
  {
    0x00,
    L"Video device"
  },
  {
    0x01,
    L"Audio device"
  },
  {
    0x02,
    L"Computer Telephony device"
  },
  {
    0x80,
    L"Other multimedia device"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_05[] = {
  {
    0x00,
    L"RAM memory controller"
  },
  {
    0x01,
    L"Flash memory controller"
  },
  {
    0x80,
    L"Other memory controller"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_06[] = {
  {
    0x00,
    L"Host/PCI bridge"
  },
  {
    0x01,
    L"PCI/ISA bridge"
  },
  {
    0x02,
    L"PCI/EISA bridge"
  },
  {
    0x03,
    L"PCI/Micro Channel bridge"
  },
  {
    0x04,
    L"PCI/PCI bridge"
  },
  {
    0x05,
    L"PCI/PCMCIA bridge"
  },
  {
    0x06,
    L"NuBus bridge"
  },
  {
    0x07,
    L"CardBus bridge"
  },
  {
    0x08,
    L"RACEway bridge"
  },
  {
    0x09,
    L"Semi-transparent PCI-to-PCI bridge"
  },
  {
    0x0A,
    L"InfiniBand-to-PCI host bridge"
  },
  {
    0x80,
    L"Other bridge type"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_07[] = {
  {
    0x00,
    L"Serial controller"
  },
  {
    0x01,
    L"Parallel port"
  },
  {
    0x02,
    L"Multiport serial controller"
  },
  {
    0x03,
    L"Modem"
  },
  {
    0x04,
    L"GPIB (IEEE 488.1/2) controller"
  },
  {
    0x05,
    L"Smart Card"
  },
  {
    0x80,
    L"Other communication device"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_08[] = {
  {
    0x00,
    L"PIC"
  },
  {
    0x01,
    L"DMA controller"
  },
  {
    0x02,
    L"System timer"
  },
  {
    0x03,
    L"RTC controller"
  },
  {
    0x04,
    L"Generic PCI Hot-Plug controller"
  },
  {
    0x05,
    L"SD Host controller"
  },
  {
    0x80,
    L"Other system peripheral"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_09[] = {
  {
    0x00,
    L"Keyboard controller"
  },
  {
    0x01,
    L"Digitizer (pen)"
  },
  {
    0x02,
    L"Mouse controller"
  },
  {
    0x03,
    L"Scanner controller"
  },
  {
    0x04,
    L"Gameport controller"
  },
  {
    0x80,
    L"Other input controller"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_0a[] = {
  {
    0x00,
    L"Generic docking station"
  },
  {
    0x80,
    L"Other type of docking station"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_0b[] = {
  {
    0x00,
    L"386"
  },
  {
    0x01,
    L"486"
  },
  {
    0x02,
    L"Pentium"
  },
  {
    0x10,
    L"Alpha"
  },
  {
    0x20,
    L"PowerPC"
  },
  {
    0x30,
    L"MIPS"
  },
  {
    0x40,
    L"Co-processor"
  },
  {
    0x80,
    L"Other processor"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_0c[] = {
  {
    0x00,
    L"Firewire(IEEE 1394)"
  },
  {
    0x01,
    L"ACCESS.bus"
  },
  {
    0x02,
    L"SSA"
  },
  {
    0x03,
    L"USB"
  },
  {
    0x04,
    L"Fibre Channel"
  },
  {
    0x05,
    L"System Management Bus"
  },
  {
    0x06,
    L"InfiniBand"
  },
  {
    0x07,
    L"IPMI Interface"
  },
  {
    0x08,
    L"SERCOS Interface Standard"
  },
  {
    0x09,
    L"CANbus"
  },
  {
    0x80,
    L"Other bus type"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_0d[] = {
  {
    0x00,
    L"iRDA compatible controller"
  },
  {
    0x01,
    L"Consumer IR controller"
  },
  {
    0x10,
    L"RF controller"
  },
  {
    0x11,
    L"Bluetooth"
  },
  {
    0x12,
    L"Broadband"
  },
  {
    0x20,
    L"Ethernet (802.11a – 5 GHz)"
  },
  {
    0x21,
    L"Ethernet (802.11b – 2.4 GHz)"
  },
  {
    0x80,
    L"Other type of wireless controller"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_0e[] = {
  {
    0x00,
    L"I2O Architecture"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_0f[] = {
  {
    0x01,
    L"TV"
  },
  {
    0x02,
    L"Audio"
  },
  {
    0x03,
    L"Voice"
  },
  {
    0x04,
    L"Data"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_10[] = {
  {
    0x00,
    L"Network & computing Encrypt/Decrypt"
  },
  {
    0x01,
    L"Entertainment Encrypt/Decrypt"
  },
  {
    0x80,
    L"Other Encrypt/Decrypt"
  }
};

PCI_SUB_CLASS_CODE PCISubClass_11[] = {
  {
    0x00,
    L"DPIO modules"
  },
  {
    0x01,
    L"Performance counters"
  },
  {
    0x10,
    L"Communications synchronization plus time and frequency test/measurement"
  },
  {
    0x20,
    L"Management card"
  },
  {
    0x80,
    L"Other data acquisition/signal processing controllers"
  }
};

PCI_SUB_CLASS_CODE* PciSubClassOrderList[] = {
    PCISubClass_00,
    PCISubClass_01,
    PCISubClass_02,
    PCISubClass_03,
    PCISubClass_04,
    PCISubClass_05,
    PCISubClass_06,
    PCISubClass_07,
    PCISubClass_08,
    PCISubClass_09,
    PCISubClass_0a,
    PCISubClass_0b,
    PCISubClass_0c,
    PCISubClass_0d,
    PCISubClass_0e,
    PCISubClass_0f,
    PCISubClass_10,
    PCISubClass_11
};

CHAR16 *GetStringFromBaseClassCode(UINT8 BaseClassCode)
{
  UINT8 Count = sizeof(PciBaseClassCodeList) / sizeof(PCI_BASE_CLASS_CODE);
  UINT8 Index;
  for (Index = 0; Index < Count; Index++){
    if(BaseClassCode == PciBaseClassCodeList[Index].BaseClassCode){
      return PciBaseClassCodeList[Index].Description;
    }
  }
  return L"no match Base class code!!!";
}

CHAR16 *GetStringFromSubClassArrary(UINT8 BaseClassCode,           UINT8 SubclassCode)
{
  UINTN Index;
  for (Index = 0; Index < PciSubclassCount[BaseClassCode]; Index++){
    if(PciSubClassOrderList[BaseClassCode][Index].SubClassCode == SubclassCode){
      return PciSubClassOrderList[BaseClassCode][Index].Description;
    }
  }
  return L"no match sub class";
}


VOID *PrintPciComonHeader(UINT8 Bus, UINT8 Dev, UINT8 Func, PCI_COMMON_HEADER *PciHeader)
{
  UINT8 BaseClassCode = PciHeader->ClassCode[2];
  UINT8 SubClassCode = PciHeader->ClassCode[1];
  Print (L"Bus:0x%x   Dev:0x%x    Func:0x%x     %s - %s\n", Bus, Dev, Func, GetStringFromBaseClassCode(BaseClassCode), GetStringFromSubClassArrary(BaseClassCode, SubClassCode));
  Print (L"                                 Vendor:0x%x  Device:0x%x   Prog Interface:0x%x\n\n",PciHeader->VendorId, PciHeader->DeviceId, PciHeader->ClassCode[0]);
}


EFI_STATUS ListPciDeviceByProtocol()
{
  EFI_STATUS Status;
  UINT16 Bus;
  UINT16 Device;
  UINT16 Func;
  UINT64 Address;
  PCI_COMMON_HEADER PciHeader;
  EFI_HANDLE *ControllerHandle = NULL;
  UINTN HandleNum, HandleIndex;
  Status = gBS->LocateHandleBuffer(ByProtocol, 
          &gEfiPciRootBridgeIoProtocolGuid,
          NULL, 
          &HandleNum, 
          &ControllerHandle);
  if(Status != EFI_SUCCESS){
    Print (L"error!!! Status: %d\n", Status);
    return Status;
  }

  //Print (L"num: %d\n", HandleNum);
  for(HandleIndex = 0; HandleIndex < HandleNum; HandleIndex++){
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *IoDev;
    Status = gBS->HandleProtocol(ControllerHandle[HandleIndex], 
            &gEfiPciRootBridgeIoProtocolGuid,
            (VOID **) &IoDev);
    if(Status != EFI_SUCCESS){
      //Print (L"HandleProtocol gEfiPciRootBridgeIoProtocolGuid fail!! index:%d\n", HandleIndex);
      continue;
    }else{
      
      for (Bus = 0; Bus <= PCI_MAX_BUS; Bus++){
        for (Device = 0; Device <= PCI_MAX_DEVICE; Device++){
          for (Func = 0; Func <= PCI_MAX_FUNC; Func++){
            Address = CALC_EFI_PCI_ADDRESS(Bus, Device, Func, 0);
            Status = IoDev->Pci.Read(IoDev, 
                                      EfiPciWidthFifoUint16, 
                                      Address, 
                                      1, 
                                      &PciHeader.VendorId);
          if(Status != EFI_SUCCESS){
            Print (L"pci->read fail!  status:%d\n", Status);
            continue;
          }
          if(PciHeader.VendorId != 0xFFFF){
           // Print (L"vendorId: %x\n", PciHeader.VendorId);
            IoDev->Pci.Read (IoDev,
                            EfiPciWidthUint32,
                            Address,
                            sizeof (PciHeader) / sizeof (UINT32),
                            &PciHeader);
           PrintPciComonHeader(Bus, Device, Func, &PciHeader);
           
           /*
            Print (L"segmentNumber:%x Bus:%x  Device:%x Func:%x  vendor:%x    deviceId:%x\n", 
                    IoDev->SegmentNumber, 
                    Bus,
                    Device,
                    Func,
                    PciHeader.VendorId,
                    PciHeader.DeviceId
                    );
            */
            //Print (L"base code: %x        sub code: %x   interface: %x\n\n", PciHeader.ClassCode[2], PciHeader.ClassCode[1],PciHeader.ClassCode[0]);
          }
              
          }
        }
      }
    }
    
  }
}


