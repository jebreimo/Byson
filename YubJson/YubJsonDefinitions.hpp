//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#ifdef _WIN32
  #ifdef UBJSON_EXPORTS
    #define UBJSON_API __declspec(dllexport)
  #else
    #define UBJSON_API __declspec(dllimport)
    #ifdef _DEBUG
      #pragma comment (lib, "YubJsonD.lib")
    #else
      #pragma comment (lib, "YubJson.lib")
    #endif
  #endif
#else
  #define UBJSON_API
#endif
