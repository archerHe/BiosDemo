#ifndef _SMBIOS_READER_H
#define _SMBIOS_READER_H




typedef struct {
  UINT64  Type0;
  UINT64  Type1;
  UINT64  Type2;
  UINT64  Type3;
  UINT64  Type4;
  UINT64  Type5;
  UINT64  Type6;
  UINT64  Type7;
  UINT64  Type8;
  UINT64  Type9;
  UINT64  Type10;
  UINT64  Type11;
  UINT64  Type12;
  UINT64  Type13;
  UINT64  Type14;
  UINT64  Type15;
  UINT64  Type16;
  UINT64  Type17;
} Type_Table_Addr;

CHAR16  *
Ascii2Unicode (
  OUT CHAR16         *UnicodeStr,
  IN  CHAR8          *AsciiStr
  );

typedef struct {
  UINT16  CacheLevel          :3;
  UINT16  CacheSocked         :1;
  UINT16  Reserved            :1;
  UINT16  RelativeCpuModule   :2;
  UINT16  BootTimeStatus      :1;
  UINT16  OptionalMode        :2;
  UINT16  Reserved2           :6;
}Type07CacheConf;

typedef struct TABLE_ITEM {
  UINT16  Key;
  CHAR16  *Info;
} TABLE_ITEM;

#endif