//
// detail/signal_blocker.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_SIGNAL_BLOCKER_HPP
#define ASIO_DETAIL_SIGNAL_BLOCKER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/config.hpp"

#if !defined(BOOST_HAS_THREADS) || defined(ASIO_DISABLE_THREADS) \
  || defined(BOOST_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
# include "RCF/external/asio/asio/detail/null_signal_blocker.hpp"
#elif defined(BOOST_HAS_PTHREADS)
# include "RCF/external/asio/asio/detail/posix_signal_blocker.hpp"
#else
# error Only Windows and POSIX are supported!
#endif

namespace asio {
namespace detail {

#if !defined(BOOST_HAS_THREADS) || defined(ASIO_DISABLE_THREADS) \
  || defined(BOOST_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
typedef null_signal_blocker signal_blocker;
#elif defined(BOOST_HAS_PTHREADS)
typedef posix_signal_blocker signal_blocker;
#endif

} // namespace detail
} // namespace asio

#endif // ASIO_DETAIL_SIGNAL_BLOCKER_HPP
