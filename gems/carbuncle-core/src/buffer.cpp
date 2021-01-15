#include <map>

namespace {
  std::map<void *, void *> carbuncle_ptr_buffer;
}

extern "C" void
mrb_carbuncle_add_buffer(void *key, void *value)
{
  carbuncle_ptr_buffer.insert(std::pair<void *, void *>(key, value));
}

extern "C" void *
mrb_carbuncle_get_buffer(void *key)
{
  std::map<void *, void *>::iterator it;
  it = carbuncle_ptr_buffer.find(key);
  if (it != carbuncle_ptr_buffer.end())
  {
    return it->second;
  }
  return 0;
}

extern "C" void
mrb_carbuncle_remove_buffer(void *key)
{
  std::map<void *, void *>::iterator it;
  it = carbuncle_ptr_buffer.find(key);
  if (it != carbuncle_ptr_buffer.end())
  {
    carbuncle_ptr_buffer.erase(it);
  }
}
