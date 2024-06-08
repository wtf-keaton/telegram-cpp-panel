/* there's only work part of file before include of localization */
#include <fstream>
constexpr auto OUTPUT_FILENAME = "localization_test.txt";
constexpr auto ENABLE_EXCEPTION_RESULT = true;

#define LOC_TEST

#ifdef LOC_TEST
#define MAIN main
#else
#define MAIN localization_test_main
#endif

#include "localization.h"

void init_russian_localization() {
  using enum sh::localization_t;
  auto& localization = sh::localization::instance();
  localization[ru].add("welcome_text", "Привет мир!");
  localization[ru].add("bye_text", "Пока мир!");
  localization[ru].add("random", "Рандомный текст, Например, 1234.");
  if constexpr (ENABLE_EXCEPTION_RESULT) localization[ru].add("random", "rand");
}

void init_english_localization() {
  using enum sh::localization_t;
  auto& localization = sh::localization::instance();
  localization[en].add("welcome_text", "Hello world!");
  localization[en].add("bye_text", "Bye world!");
  localization[en].add("random", "Here's nothing for you.");
}

int MAIN() {
  init_english_localization();
  init_russian_localization();
  std::ofstream file(OUTPUT_FILENAME);
  using enum sh::localization_t;
  for (auto&& loc : {ru, en}) {
    auto& local = sh::localization::instance()[loc];
    for (auto&& key : {"welcome_text", "bye_text", "random"}) {
      if (auto&& str = local[key]; !str.empty())
        file << str << std::endl;
      else
        file << "key = '" << key << "' does not have value" << std::endl;
    }
  }
  file.close();
  return 0;
}
