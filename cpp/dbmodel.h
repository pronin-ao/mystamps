#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace db {

struct UserParams {
  bool checked = false;
};

struct CustomParams {
  std::optional<bool> forced_wishlist = std::nullopt;
  std::optional<std::string> comments = std::nullopt;

  inline bool has_value() { return forced_wishlist || comments; }
};

using Image = std::string;
using Spec = std::string;
using Price = std::string;
using Number = std::string;
using Code = std::string;
using Capture = std::string;
using Color = std::string;
using Conditions = std::vector<std::string>;
using Link = std::string;
using ListNote = std::optional<std::string>;
struct Stamp {

  Image image = "";
  Spec spec = "";
  Price price = "";
  Code code = "";
  Capture capture = "";
  Color color = "";
  bool owned = false;
  Conditions condition{};
  Link sw_link = "";
  bool has_list = false;
  ListNote list_note;
  std::optional<bool> list_owned;
  std::optional<Conditions> list_conditions;

  UserParams user{};
  std::optional<CustomParams> custom{};
};
using Series = std::unordered_map<Number, Stamp>;
using SeriesName = std::string;
using Year = std::unordered_map<SeriesName, Series>;
using YearVal = std::string;
using Country = std::unordered_map<YearVal, Year>;
using CountryName = std::string;
using Catalogue = std::unordered_map<CountryName, Country>;
} // namespace db
