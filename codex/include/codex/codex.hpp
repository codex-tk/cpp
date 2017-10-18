#ifndef __codex_codex_h__
#define __codex_codex_h__

#if defined( _WIN32 )

#if !defined(NOMINMAX)
#define NOMINMAX
#endif

#include <WinSock2.h>
#include <MSWSock.h>z
#include <ws2tcpip.h>
#include <Windows.h>

#else

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/tcp.h>

#endif

#include <errno.h>
#include <iostream>
#include <mutex>
#include <atomic>
#include <system_error>
#include <memory>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

#if defined( _WIN32 )

/// @brief win32 predef
/// @detail
/// _WIN32 always
/// _WIN64 : win64 application
/// _M_AMD64 or _M_X64 : x64
/// _M_IX86 : x86
/// _M_ARM : ARM 
/// _M_ARMT : ARM Thumb Mode
#define __codex_win32__

#elif defined( __linux__ )

/// @brief linux predef
/// @detail
/// __x86_64__ : x64
/// __arm__ : ARM
/// __thumb__ : ARM Thumb Mode
/// __i386__ : x86
/// __ANDROID__ : android
#define __codex_linux__

#elif defined( __APPLE__ )

/// @brief apple predef
/// @detail
/// __x86_64__ : x64
/// __arm__ : ARM
/// __thumb__ : ARM Thumb Mode
/// __i386__ : x86
/// TARGET_IPHONE_SIMULATOR
/// TARGET_OS_IPHONE
/// TARGET_OS_MAC
#define __codex_apple__

#endif

namespace codex{
  int universe(void);
}

#ifndef CODEX_ASSERT
#define CODEX_ASSERT( expr , message ) assert( expr && message )
#endif

#ifndef DELETE_MOVE
#define DELETE_MOVE( clazz ) clazz( clazz&& ) = delete; clazz& operator=( clazz&& ) = delete
#endif

#ifndef DELETE_COPY
#define DELETE_COPY( clazz ) clazz( clazz& ) = delete; clazz& operator=( clazz& ) = delete
#endif

#ifndef DELETE_MOVE_AND_COPY
#define DELETE_MOVE_AND_COPY( clazz ) DELETE_MOVE( clazz ); DELETE_COPY( clazz )
#endif

namespace {
  class defaulted_and_deleted_function_order_sample {
  public:
    // 소멸자 명시 선언
    // 이동 연산자는 delete 됨
    ~defaulted_and_deleted_function_order_sample(void) = default;

    // 이동 연산자 명시 선언
    // 이동 연산자 명시 선언시 복사 연산자는 delete 
    defaulted_and_deleted_function_order_sample(
      defaulted_and_deleted_function_order_sample&& 
    ) = default;
    defaulted_and_deleted_function_order_sample& operator=(
      defaulted_and_deleted_function_order_sample&& 
    ) = default;

    // 복사 연산자 명시 선언
    defaulted_and_deleted_function_order_sample(
      defaulted_and_deleted_function_order_sample&
    ) = default;
    defaulted_and_deleted_function_order_sample& operator=(
      defaulted_and_deleted_function_order_sample& 
    ) = default;
    
  };
}

#endif
