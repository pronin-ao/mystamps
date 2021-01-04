#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace db {

  using Image = std::string;
  using Spec = std::string;
  using Price = std::string;
  struct Stamp{
    Image image = "";
    Spec spec = "";
    Price price = "";
  };
  using Series = std::vector<Stamp>;
  using SeriesName = std::string;
  using Year = std::unordered_map<SeriesName, Series>;
  using YearVal = short;
  using Country = std::unordered_map<YearVal, Year>;
  using CountryName = std::string;
  using Catalogue = std::unordered_map<CountryName, Country>;

}
