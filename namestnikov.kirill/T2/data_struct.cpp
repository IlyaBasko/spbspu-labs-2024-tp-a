#include "data_struct.hpp"
#include "delimeters.hpp"

namestnikov::DataStruct::DataStruct(double key1, unsigned long long key2, std::string key3):
  key1_(key1),
  key2_(key2),
  key3_(key3)
{}

double namestnikov::DataStruct::get_key1() const
{
  return key1_;
}

unsigned long long namestnikov::DataStruct::get_key2() const
{
  return key2_;
}

std::string namestnikov::DataStruct::get_key3() const
{
  return key3_;
}

std::istream & namestnikov::operator>>(std::istream & in, DataStruct & data)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  using delC = DelimeterChar;
  using delS = DelimeterString;
  double key1 = 0;
  unsigned long long key2 = 0;
  std::string key3 = "";
  size_t keyNumber = 0;
  size_t MAX_LEN_KEYS = 3;
  in >> delS{"(:"};
  for (size_t i = 0; i < MAX_LEN_KEYS; ++i)
  {
    in >> delS{"key"} >> keyNumber;
    switch (keyNumber)
    {
      case 1:
        in >> key1 >> delC{'d'};
      case 2:
        in >> delC{'0'} >> key2;
      case 3:
        in >> key3;
    }
  }
  in >> delS{":)"};
  if (in)
  {
    data = DataStruct(key1, key2, key3);
  }
  return in;
}

std::ostream & namestnikov::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry guard(out);
  if (!guard)
  {
    return out;
  }
  out << "(:key1 " << data.get_key1() << "d";
  out << ":key2 " << "0" << data.get_key2();
  out << ":key3 " << '\"' << data.get_key3() << '\"' << ":)";
  return out;
}

std::istream & namestnikov::operator>>(std::istream & in, std::string & s)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  char sym = 0;
  in >> std::noskipws;
  while ((in >> sym) && (sym != '\"'))
  {
    s += sym;
  }
  in >> std::skipws;
  return in;
}