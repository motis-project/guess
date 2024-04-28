#pragma once

#include <iostream>
#include <string>

#include "utf8proc.h"

namespace guess {

template <typename Function>
inline void for_each_token(char* s, std::size_t length, Function f) {
  auto base = 0;
  auto i = 0u;
  while (i < length) {
    if (s[i] == ' ') {
      char tmp = s[i];
      s[i] = '\0';
      bool exit = f(s + base);
      s[i] = tmp;

      if (exit) {
        return;
      }

      base = i + 1;
    }
    ++i;
  }
  f(s + base);
}

template <typename Function>
inline void for_each_token(std::string const& in, Function f) {
  for_each_token(const_cast<char*>(in.c_str()), in.size(), f);
}

inline void replace_all(std::string& s, std::string const& from,
                        std::string const& to) {
  std::string::size_type pos;
  while ((pos = s.find(from)) != std::string::npos) {
    s.replace(pos, from.size(), to);
  }
}

inline void normalize(std::string& s) {
  replace_all(s, "è", "e");
  replace_all(s, "é", "e");
  replace_all(s, "ß", "ss");
  replace_all(s, "-", " ");
  replace_all(s, "/", " ");
  replace_all(s, ".", " ");
  replace_all(s, ",", " ");
  replace_all(s, "(", " ");
  replace_all(s, ")", " ");

  auto const options =
      static_cast<utf8proc_option_t>(utf8proc_option_t::UTF8PROC_DECOMPOSE |
                                     utf8proc_option_t::UTF8PROC_STRIPMARK |
                                     utf8proc_option_t::UTF8PROC_CASEFOLD);

  utf8proc_uint8_t* utf8_out = nullptr;

  // This is only safe when the representation of char matches unsigned char.
  // Let's hope that's the case on all architectures we care about
  auto* utf8_in = reinterpret_cast<const utf8proc_uint8_t*>(s.c_str());

  auto const size = utf8proc_map(utf8_in, s.length(), &utf8_out, options);
  if (size > 0) {
    s = std::string(reinterpret_cast<const char*>(utf8_out), size);
  } else {
    std::clog << "guess: Failed to normalize unicode string: "
              << utf8proc_errmsg(size);
  }
  free(utf8_out);

  replace_all(s, "  ", " ");
}

}  // namespace guess
