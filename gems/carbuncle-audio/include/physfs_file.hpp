#ifndef CARBUNCLE_PHYSFS_FILE_H
#define CARBUNCLE_PHYSFS_FILE_H

#include "physfs.h"
#include "soloud_file.h"

#include <mruby.h>

class PhysFsFile : public SoLoud::File
{
  private:
    PHYSFS_File *m_file;
    mrb_state   *m_mrb;
  public:
    void init(mrb_state *mrb, const char *filename);
    virtual ~PhysFsFile();
    virtual int eof();
    virtual unsigned int read(unsigned char *aDst, unsigned int aBytes);
    virtual unsigned int length();
    virtual void seek(int aOffset);
    virtual unsigned int pos();
};

#endif
