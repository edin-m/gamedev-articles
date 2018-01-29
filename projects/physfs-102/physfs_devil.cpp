#include "physfs_devil.h"

#include "physfs.h"

namespace physfs_devil {

void phys_IL_close(ILHANDLE handle) {
  PHYSFS_close((PHYSFS_File*) handle);
}

ILboolean phys_IL_eof(ILHANDLE handle) {
  return PHYSFS_eof((PHYSFS_File*) handle) == 0 ? IL_FALSE : IL_TRUE;
}

ILint phys_IL_getc(ILHANDLE handle) {
  char c;
  int x = PHYSFS_readBytes((PHYSFS_File*) handle, &c, 1);
  if (x < 1) {
    return IL_EOF;
  }
  return c;
}

ILHANDLE phys_IL_open(ILconst_string file) {
  PHYSFS_File* fp;
  fp = PHYSFS_openRead(file);
  return (ILHANDLE) fp;
}

ILint phys_IL_read(void* Buffer, ILuint Size, ILuint Number, ILHANDLE Handle) {
  return ILint(PHYSFS_readBytes((PHYSFS_File*) Handle, Buffer, Size * Number));
}

ILint phys_IL_seek(ILHANDLE Handle, ILint Offset, ILint Mode) {
  if (Mode == IL_SEEK_END) {
    PHYSFS_sint64 size = PHYSFS_fileLength((PHYSFS_File*) Handle);
    return (ILint) PHYSFS_seek((PHYSFS_File*) Handle, (PHYSFS_uint64) size);
  } else if (Mode == IL_SEEK_SET) {
    return (ILint) PHYSFS_seek((PHYSFS_File*) Handle, (PHYSFS_uint64) Offset);
  } else if (Mode == IL_SEEK_CUR) {
    PHYSFS_sint64 curr = PHYSFS_tell((PHYSFS_File*) Handle);
    return (ILint) PHYSFS_seek((PHYSFS_File*) Handle, curr + ((PHYSFS_uint64) Offset));
  }
  return -1;
}

ILint phys_IL_tell(ILHANDLE handle) {
  return (ILint) PHYSFS_tell((PHYSFS_File*) handle);
}

void rebindILToPhysFS() {
  ilSetRead((fOpenRProc) &physfs_devil::phys_IL_open, (fCloseRProc) &physfs_devil::phys_IL_close,
            (fEofProc) &physfs_devil::phys_IL_eof, (fGetcProc) &physfs_devil::phys_IL_getc,
            (fReadProc) &physfs_devil::phys_IL_read, (fSeekRProc) &physfs_devil::phys_IL_seek,
            (fTellRProc) &physfs_devil::phys_IL_tell);
}

void resetIL() {
  ilResetRead();
}

}

