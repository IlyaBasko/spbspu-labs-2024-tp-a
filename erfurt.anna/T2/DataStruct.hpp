#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iosfwd>

namespace erfurt
{
  struct DataStruct
  {
    long long key1;
    unsigned long long key2;
    std::string key3;

    bool operator<(const DataStruct& value) const;
  };

  std::istream& operator>>(std::istream &in, DataStruct &value);
  std::ostream& operator<<(std::ostream &out, const DataStruct &value);
}

#endif

