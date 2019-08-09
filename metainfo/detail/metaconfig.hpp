//
// Created by Dmytro Gyrba on 10/4/18.
// Copyright (c) 2018 PacificInteractive. All rights reserved.
//

#pragma once

#define CUT_m_FROM_NAMES 1

#if defined(OS) && OS == OS_WP8
#define NO_CONSTEXPR
#define NO_NOEXCEPT
#endif //OS == OS_WP8

#ifdef NO_CONSTEXPR
#define CONSTEXPR
#define NOEXCEPT
#else
#define CONSTEXPR constexpr
#define NOEXCEPT noexcept
#endif //OS == OS_WP8
