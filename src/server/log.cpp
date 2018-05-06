#include <config.h>

#include "log.hpp"

#include <string>

#include <boost/log/common.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;

std::string log_filename = "sample_%N.log";

BOOST_LOG_GLOBAL_LOGGER_INIT(
    logger, src::severity_logger_mt<logging::trivial::severity_level>) {

  src::severity_logger_mt<logging::trivial::severity_level> logger;

  auto format = "[%TimeStamp%]: %Message%";

  boost::log::add_common_attributes();

  logging::add_console_log(std::cout, boost::log::keywords::format = format);

  logging::add_file_log(
      keywords::file_name = log_filename, keywords::auto_flush = true,
      keywords::rotation_size = 10 * 1024 * 1024, keywords::format = format);

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);

  return logger;
}
