#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace db {

  struct AddParams{
    bool checked = false;
  };

  using Image = std::string;
  using Spec = std::string;
  using Price = std::string;
  using Number = std::string;
  using Code = std::string;
  using Capture = std::string;
  using Color = std::string;
  struct Stamp{
    Image image = "";
    Spec spec = "";
    Price price = "";
    Code code = "";
    Capture capture = "";
    Color color = "";

    AddParams add{};
  };
  using Series = std::unordered_map<Number, Stamp>;
  using SeriesName = std::string;
  using Year = std::unordered_map<SeriesName, Series>;
  using YearVal = std::string;
  using Country = std::unordered_map<YearVal, Year>;
  using CountryName = std::string;
  using Catalogue = std::unordered_map<CountryName, Country>;
}
