///
#ifndef BAULK_HPP
#define BAULK_HPP
#include <bela/base.hpp>
#include <bela/stdwriter.hpp>
#include "compiler.hpp"

namespace baulk {
extern bool IsDebugMode;
extern bool IsForceMode;
constexpr size_t UerAgentMaximumLength = 64;
extern wchar_t UserAgent[UerAgentMaximumLength];
// DbgPrint added newline
template <typename... Args>
bela::ssize_t DbgPrint(const wchar_t *fmt, Args... args) {
  if (!IsDebugMode) {
    return 0;
  }
  const bela::format_internal::FormatArg arg_array[] = {args...};
  std::wstring str;
  str.append(L"\x1b[33m* ");
  bela::format_internal::StrAppendFormatInternal(&str, fmt, arg_array,
                                                 sizeof...(args));
  if (str.back() == '\n') {
    str.pop_back();
  }
  str.append(L"\x1b[0m\n");
  return bela::FileWrite(stderr, str);
}
inline bela::ssize_t DbgPrint(const wchar_t *fmt) {
  if (!IsDebugMode) {
    return 0;
  }
  std::wstring_view msg(fmt);
  if (!msg.empty() && msg.back() == '\n') {
    msg.remove_suffix(1);
  }
  return bela::FileWrite(stderr,
                         bela::StringCat(L"\x1b[33m", msg, L"\x1b[0m\n"));
}

template <typename... Args>
bela::ssize_t DbgPrintEx(char32_t prefix, const wchar_t *fmt, Args... args) {
  if (!IsDebugMode) {
    return 0;
  }
  const bela::format_internal::FormatArg arg_array[] = {args...};
  auto str = bela::StringCat(L"\x1b[32m", prefix, L" ");
  bela::format_internal::StrAppendFormatInternal(&str, fmt, arg_array,
                                                 sizeof...(args));
  if (str.back() == '\n') {
    str.pop_back();
  }
  str.append(L"\x1b[0m\n");
  return bela::FileWrite(stderr, str);
}
inline bela::ssize_t DbgPrintEx(char32_t prefix, const wchar_t *fmt) {
  if (!IsDebugMode) {
    return 0;
  }
  std::wstring_view msg(fmt);
  if (!msg.empty() && msg.back() == '\n') {
    msg.remove_suffix(1);
  }
  return bela::FileWrite(
      stderr, bela::StringCat(L"\x1b[32m", prefix, L" ", msg, L"\x1b[0m\n"));
}

/// defines

struct Bucket {
  Bucket() = default;
  Bucket(std::wstring_view desc, std::wstring_view n, std::wstring_view u) {
    description = desc;
    name = n;
    url = u;
  }
  std::wstring description;
  std::wstring name;
  std::wstring url;
  std::wstring latest;
};
using Buckets = std::vector<Bucket>;

// Env functions
bool InitializeBaulkEnv(int argc, wchar_t *const *argv,
                        std::wstring_view profile);
std::wstring_view BaulkProfile();
bool InitializeBaulkBuckets();
bool BaulkIsFrozenPkg(std::wstring_view pkg);
std::wstring_view BaulkRoot();
Buckets &BaulkBuckets();
std::wstring_view BaulkGit();
baulk::compiler::Executor &BaulkExecutor();
bool BaulkInitializeExecutor(bela::error_code &ec);
// package base
struct Package {
  std::wstring name;
  std::wstring description;
  std::wstring url;
  std::wstring version;
  std::vector<std::wstring> links;
  std::vector<std::wstring> launchers;
};
} // namespace baulk

#endif