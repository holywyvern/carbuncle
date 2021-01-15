#include "carbuncle/core.h"

#include "physfs_file.hpp"

void
PhysFsFile::init(mrb_state *mrb, const char *filename)
{
  m_mrb = mrb;
  m_file = PHYSFS_openRead(filename);
  if (!m_file)
  {
    mrb_carbuncle_file_error(mrb, "open");
  }
}

PhysFsFile::~PhysFsFile()
{
  if (!PHYSFS_close(m_file))
  {
    mrb_carbuncle_file_error(m_mrb, "close");
  }
}

int
PhysFsFile::eof()
{
  return PHYSFS_eof(m_file);
}

unsigned int
PhysFsFile::read(unsigned char *aDst, unsigned int aBytes)
{
  PHYSFS_sint64 result = PHYSFS_readBytes(m_file, aDst, aBytes);
  if (result == -1)
  {
    mrb_carbuncle_file_error(m_mrb, "read");
  }
  return result;
}

unsigned int
PhysFsFile::length()
{
  PHYSFS_sint64 bytes = PHYSFS_fileLength(m_file);
  if (bytes == -1)
  {
    mrb_carbuncle_file_error(m_mrb, "read");
  }
  return bytes;
}

void
PhysFsFile::seek(int aOffset)
{
  if (!PHYSFS_seek(m_file, aOffset))
  {
    mrb_carbuncle_file_error(m_mrb, "read");
  }
}

unsigned int
PhysFsFile::pos()
{
  PHYSFS_sint64 offset = PHYSFS_tell(m_file);
  if (offset == -1)
  {
    mrb_carbuncle_file_error(m_mrb, "read");
  }  
  return offset;
}
