#include "cmds.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>

void altun::createDictionary(std::istream& in,
    std::map< std::string, std::map< std::string, std::vector< std::string > > >& dicts)
{
  std::string name = "";
  in >> name;

  if (dicts.find(name) != dicts.end())
  {
    throw std::logic_error("<ALREADY EXISTS>");
  }

  std::map< std::string, std::vector< std::string > > newDict = {};
  dicts[name] = newDict;
}

void altun::deleteDictionary(std::istream& in,
    std::map< std::string, std::map< std::string, std::vector< std::string > > >& dicts)
{
  std::string name = "";
  in >> name;
  if (dicts.find(name) == dicts.end())
  {
    throw std::logic_error("<DICTIONARY NOT FOUND>");
  }
  dicts.erase(name);
}

void altun::addWord(std::istream& in,
    std::map< std::string, std::map< std::string, std::vector< std::string > > >& dicts)
{
  std::string name = "";
  in >> name;
  if (dicts.find(name) == dicts.end())
  {
    throw std::logic_error("<DICTIONARY NOT FOUND>");
  }
  std::map< std::string, std::vector< std::string > >& wantedDict = dicts[name];
  std::string word = "";
  std::string translation = "";
  in >> word >> translation;
  wantedDict[word].push_back(translation);
}

void altun::removeWord(std::istream& in,
    std::map< std::string, std::map< std::string, std::vector< std::string > > >& dicts)
{
  std::string name = "";
  in >> name;
  if (dicts.find(name) == dicts.end())
  {
    throw std::logic_error("<DICTIONARY NOT FOUND>");
  }
  std::map< std::string, std::vector< std::string > >& wantedDict = dicts[name];
  std::string word = "";
  in >> word;
  if (wantedDict.find(word) == wantedDict.end())
  {
    throw std::logic_error("<WORD NOT FOUND>");
  }
  wantedDict.erase(word);
}

void altun::translateWord(std::ostream& out,
    std::istream& in,
    const std::map< std::string, std::map< std::string, std::vector< std::string > > >& dicts)
{
  std::string name = "";
  std::string engWord = "";
  in >> name >> engWord;
  if (dicts.find(name) == dicts.end())
  {
    throw std::logic_error("<DICTIONARY NOT FOUND>");
  }
  const std::map< std::string, std::vector< std::string > >& wantedDict = dicts.at(name);

  if (wantedDict.find(engWord) == wantedDict.end())
  {
    throw std::logic_error("<WORD NOT FOUND>");
  }
  const std::vector< std::string > translations = wantedDict.at(engWord);
  out << engWord << " ";
  std::copy(translations.cbegin(), translations.cend(), std::ostream_iterator< std::string >(out, " "));
  out << "\n";
  return;
}

void mergeEntry(std::map< std::string, std::vector< std::string > >& first,
    const std::pair< const std::string, std::vector< std::string > >& entry)
{
  const std::string& key = entry.first;
  const std::vector< std::string >& values = entry.second;

  if (first.find(key) != first.end())
  {
    first[key].insert(first[key].end(), values.begin(), values.end());
  }
  else
  {
    first[key] = values;
  }
}

void altun::mergeDictionaries(std::istream& in,
    std::map< std::string, std::map< std::string, std::vector< std::string > > >& dicts)
{
  std::string first = "";
  std::string second = "";
  std::map< std::string, std::vector< std::string > > result = {};
  in >> first >> second;
  if (dicts.find(first) == dicts.end() || dicts.find(second) == dicts.end())
  {
    throw std::logic_error("<DICTIONARY NOT FOUND>");
  }
  std::map< std::string, std::vector< std::string > >& firstDict = dicts[first];
  const std::map< std::string, std::vector< std::string > >& secondDict = dicts[second];
  std::for_each(secondDict.cbegin(), secondDict.cend(), std::bind(mergeEntry, std::ref(firstDict), std::placeholders::_1));
}

bool isCommonTranslation(const std::string& firstTranslation, const std::vector< std::string >& secondTranslations)
{
  return std::find(secondTranslations.begin(), secondTranslations.end(), firstTranslation) != secondTranslations.end();
}

void insertCommonTranslations(std::map< std::string, std::vector< std::string > >& result,
    const std::pair< const std::string, std::vector< std::string > >& entry,
    const std::map< std::string, std::vector< std::string > >& secondDict)
{
  const std::string& word = entry.first;
  const std::vector< std::string >& firstTranslations = entry.second;

  auto it2 = secondDict.find(word);
  if (it2 != secondDict.end())
  {
    const std::vector< std::string >& secondTranslations = it2->second;
    std::vector< std::string > common;
    std::copy_if(firstTranslations.begin(), firstTranslations.end(), std::back_inserter(common),
        std::bind(isCommonTranslation, std::placeholders::_1, std::cref(secondTranslations)));
    if (!common.empty())
    {
      result[word] = common;
    }
  }
}

void altun::intersection(std::istream& in,
    std::map< std::string, std::map< std::string, std::vector< std::string > > >& dictionaries)
{
  std::string firstName, secondName;
  in >> firstName >> secondName;

  if (dictionaries.find(firstName) == dictionaries.end() || dictionaries.find(secondName) == dictionaries.end())
  {
    throw std::logic_error("<DICTIONARY NOT FOUND>");
  }

  const std::map< std::string, std::vector< std::string > >& first_dict = dictionaries[firstName];
  const std::map< std::string, std::vector< std::string > >& secondDict = dictionaries[secondName];
  std::map< std::string, std::vector< std::string > > result = {};
  using namespace std::placeholders;
  std::for_each(first_dict.begin(), first_dict.end(),
      std::bind(insertCommonTranslations, std::ref(result), _1, std::cref(secondDict)));

  if (result.empty()) {
    std::cout << "<NO INTERSECTION>\n";
  } else {
    std::for_each(result.begin(), result.end(), [](const std::pair<const std::string, std::vector<std::string>>& pair) {
        const auto& translations = pair.second;
        for (const auto& translation : translations) {
            std::cout << translation << std::endl;
        }
    });
  }
}
