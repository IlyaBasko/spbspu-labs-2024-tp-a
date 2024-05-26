#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <fstream>
#include "additionalcommands.hpp"

void kuznetsov::command_add_word(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }
  std::string dictionary_name;
  std::string word;
  in >> dictionary_name;
  in >> word;
  if (data.find(dictionary_name) != data.end())
  {

    frequency_dictionary data_word = data[dictionary_name];
    if (data_word.find(word) == data_word.end())
    {
      data_word[word] = 1;
      data[dictionary_name] = data_word;
    }
    else
    {
      size_t& volume = data[dictionary_name].at(word);
      ++volume;
    }
  }
  else
  {
    out << "A dictionary with the same name was not found\n";
  }
}

void kuznetsov::command_delete_word(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }
  std::string dictionary_name;
  std::string word;
  in >> dictionary_name;
  in >> word;
  if (data.find(dictionary_name) != data.end())
  {
    frequency_dictionary& data_word = data[dictionary_name];
    auto it = data_word.find(word);
    if (it != data_word.end())
    {
      data_word.erase(it);
    }
    else
    {
      out << "Word in this dictionary not found\n";
    }
  }
  else
  {
    out << "A dictionary with the same name was not found\n";
  }
}

void kuznetsov::command_output(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }

  std::string dictionary_name;
  in >> dictionary_name;

  if (data.find(dictionary_name) != data.end())
  {
    frequency_dictionary data_word = data[dictionary_name];
    if (data_word.empty())
    {
      out << "Dictioanry empty\n";
    }
    else
    {
      frequency_dictionary::iterator it;
      for (it = data_word.begin(); it != data_word.end(); ++it)
      {
        std::pair< std::string, size_t > word = *it;
        out << word.first << "-" << word.second << '\n';
      }
    }
  }
  else
  {
    out << "A dictionary with the same name was not found\n";
  }
}

void kuznetsov::command_create_dictionary(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }

  std::string dictionary_name;
  in >> dictionary_name;

  if (data.find(dictionary_name) == data.end())
  {
    data[dictionary_name] = {};
  }
  else
  {
    out << "A dictionary with this name has already been created\n";
  }
}

void kuznetsov::command_delete_dictionary(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }
  std::string dictionary_name;
  in >> dictionary_name;
  if (data.find(dictionary_name) != data.end())
  {
    frequency_dictionary& data_word = data[dictionary_name];
    data_word.clear();
    data.erase(dictionary_name);
  }
  else
  {
    out << "A dictionary with the same name was not found\n";
  }
}

void kuznetsov::command_search_in_dictionary(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }
  std::string dictionary_name;
  in >> dictionary_name;
  std::string word;
  in >> word;
  if (data.find(dictionary_name) != data.end())
  {
    frequency_dictionary& data_word = data[dictionary_name];
    if (data_word.find(word) != data_word.cend())
    {
      out << "The word is present in this dictionary\n";
    }
    else
    {
      out << "The word isn't present in this dictionary\n";
    }
  }
  else
  {
    out << "A dictionary with the same name was not found\n";
  }
}

void kuznetsov::command_search_in_all_dictionary(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }
  std::string word;
  in >> word;
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
    {
      if ((*it2).first == word)
      {
        out << (*it).first << " ";
        continue;
      }
    }
  }
  out << '\n';
}

void kuznetsov::command_show_dictionary(std::map< std::string, frequency_dictionary >& data, std::ostream& out)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    out << (*it).first << " ";
  }
  out << '\n';
}

void kuznetsov::command_add_words_from_file(std::map< std::string, frequency_dictionary >& data, std::istream& in, std::ostream& out)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return;
  }
  std::string dictionary_name;
  in >> dictionary_name;
  std::string name_file;
  in >> name_file;
  if (data.find(dictionary_name) != data.end())
  {
    frequency_dictionary& data_word = data[dictionary_name];
    std::ifstream file(name_file);
    if (!file.is_open())
    {
      out << "File not open\n";
    }
    else
    {
      std::string word;
      while (!file.eof())
      {
        file >> word;
        if (data_word.find(word) == data_word.end())
        {
          data_word[word] = 1;
        }
        else
        {
          size_t& volume = data[dictionary_name].at(word);
          ++volume;
        }
      }
    }
  }
  else
  {
    out << "A dictionary with the same name was not found\n";
    return;
  }
}
