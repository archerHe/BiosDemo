#ifndef _PCI_COMMON_H
#define _PCI_COMMON_H

#pragma pack(1)
typedef enum {
  PciDevice,
  PciP2pBridge,
  PciCardBusBridge,
  PciUndefined
} PCI_HEADER_TYPE;

#define MAX_BUS_NUMBER                255
#define MAX_DEVICE_NUMBER             31
#define MAX_FUNCTION_NUMBER           7

// PCI_3.0 spec
#define MAX_SUBCLASS_COUNT            18


typedef struct {
  UINT16  VendorId;
  UINT16  DeviceId;

  UINT16  Command;
  UINT16  Status;

  UINT8   RevisionId;
  UINT8   ClassCode[3];

  UINT8   CacheLineSize;
  UINT8   PrimaryLatencyTimer;
  UINT8   HeaderType;
  UINT8   BIST;

} PCI_COMMON_HEADER;

typedef struct {
  UINT8       BaseClassCode;
  CHAR16*     Description;
} PCI_BASE_CLASS_CODE;

typedef struct {
  UINT8       SubClassCode;
  CHAR16*     Description;  
} PCI_SUB_CLASS_CODE;

#pragma pack()
#endif