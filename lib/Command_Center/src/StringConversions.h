#ifndef STRINGCONVERSIONS_H
#define STRINGCONVERSIONS_H

#include <WString.h>
#include <string>

template<typename T>
T from_string(const String s);
template<>
inline unsigned long long from_string<unsigned long long>(const String s) { return std::stoull(std::string(s.c_str())); }
template<>
inline unsigned long from_string<unsigned long>(const String s) { return std::stoul(std::string(s.c_str())); }
template<>
inline unsigned int from_string<unsigned int>(const String s) { return std::stoul(std::string(s.c_str())); }
template<>
inline unsigned char from_string<unsigned char>(const String s) { return static_cast<unsigned char>(std::stoul(std::string(s.c_str()))); }
template<>
inline long long from_string<long long>(const String s) { return std::stoll(std::string(s.c_str())); }
template<>
inline long from_string<long>(const String s) { return std::stol(std::string(s.c_str())); }
template<>
inline int from_string<int>(const String s) { return std::stoi(std::string(s.c_str())); }
template<>
inline char from_string<char>(const String s) { if (s.isEmpty()) return ' '; return s[0]; }
template<>
inline long double from_string<long double>(const String s) { return std::stold(std::string(s.c_str())); }
template<>
inline double from_string<double>(const String s) { return std::stod(std::string(s.c_str())); }
template<>
inline float from_string<float>(const String s) { return std::stof(std::string(s.c_str())); }
template<>
inline bool from_string<bool>(const String s) { return s == "1" || s == "true" || s == "True" || s=="TRUE"; }
template<>
inline String from_string<String>(const String s) { return s; }


template<typename T>
String to_string_result(T&& value) {
    if constexpr (std::is_same_v<std::decay_t<T>, String>)
        return std::forward<T>(value);
    else if constexpr (std::is_same_v<std::decay_t<T>, void>)
        return "";
    else if constexpr (std::is_same_v<std::decay_t<T>, bool>)
        return value ? "true" : "false";
    else
        return String(value);
}

#endif /* STRINGCONVERSIONS_H */
