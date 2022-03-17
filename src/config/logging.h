#ifndef OLYMPUS_LOGGING_CONFIG_H
#define OLYMPUS_LOGGING_CONFIG_H

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
namespace src = boost::log::sources;
namespace logging = boost::log;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_NONE, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_READFILE, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_INTERNAL, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_TARGETING, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_RAGNAROK, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_IOUI, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_YGGDRASIL, src::logger)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(DBG_NETWORK, src::logger)

#define gdebug(x) src::logger& lg = x::get(); BOOST_LOG(lg)

#define OPEN_LOG_AS(x, strm) src::logger& lg4242 = x::get(); logging::record rec = lg4242.open_record(); logging::record_ostream strm(rec)
#define CLOSE_LOG(strm) strm.flush(); lg4242.push_record(boost::move(rec))

#endif //OLYMPUS_LOGGING_CONFIG_H
