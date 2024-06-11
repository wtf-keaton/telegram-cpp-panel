/*
* there's only work part of file before include of localization
                before_test:
        mkdir test_build; mkdir test_build/localization;
* compile script:
g++ .\tests\localization\localization_test.cpp -O3 --std=c++20
 -I.\telegram-service\ -o .\test_build\localization\a.exe -Werror -Wall
-Wpedantic;
* compile & run script:
g++ .\tests\localization\localization_test.cpp -O3 --std=c++20
 -I.\telegram-service\ -o .\test_build\localization\a.exe -Werror -Wall
-Wpedantic;
  .\test_build\localization\a.exe;
*/
#include <fstream>
#include <iostream>
constexpr auto OUTPUT_FILENAME =
    "test_build/localization/localization_test.txt";
constexpr auto JSON_FILENAME = "test_build/localization/localization.json";
constexpr auto ENABLE_EXCEPTION_RESULT = false;

#define LOC_TEST

#ifdef LOC_TEST
#define MAIN main
#else
#define MAIN localization_test_main
#endif

#ifndef GLOBAL_TEST
#include "localization/localization.h"
#else
#include "../../telegram-service/localization/localization.h"
#endif
void init_russian_localization() {
  using enum sh::e_localization;
  auto& localization = sh::c_localization::instance();
  localization[_ru].add("welcome_text", "Привет мир!");
  localization[_ru].add("bye_text", "Пока мир!");
  localization[_ru].add("random", "Рандомный текст, Например, 1234.");
  if constexpr (ENABLE_EXCEPTION_RESULT)
    localization[_ru].add("random"_h, "rand");
}

void init_english_localization() {
  using enum sh::e_localization;
  auto& localization = sh::c_localization::instance();
  localization[_en].add("welcome_text", "Hello world!");
  localization[_en].add("bye_text", "Bye world!");
  localization[_en].add("random", "Here's nothing for you.");
}

int MAIN() {
  init_russian_localization();
  std::ofstream file(OUTPUT_FILENAME);
  using enum sh::e_localization;
  {
    std::ifstream jfile(JSON_FILENAME);
    if (jfile.is_open()) {
      nlohmann::json json = nlohmann::json::parse(jfile);
      sh::c_localization::instance()[_en].from_json(json);
    } else {
      init_english_localization();
    }
  }
  for (auto&& loc : {_ru, _en}) {
    auto& local = sh::c_localization::instance()[loc];
    for (auto&& key : {"welcome_text"_h, "bye_text"_h, "random"_h}) {
      if (auto&& str = local[key]; !str.empty())
        file << str << std::endl;
      else
        file << "key = '" << key << "' does not have value" << std::endl;
    }
  }
  file.close();
  try {
    std::ofstream jfile(JSON_FILENAME);

    nlohmann::json json;

    sh::c_localization::instance()[_en].to_json(json);

    jfile << json;

    jfile.close();
  } catch (std::exception& e) {
    std::cout << "caught exception while json working with value = " << e.what()
              << std::endl;
  }

  return 0;
}
