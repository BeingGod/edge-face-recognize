#include "utils/utils.h"

#include <HTTPRequest.hpp>
#include <cjson/cJSON.h>
#include <glog/logging.h>

#include <string>
#include <vector>

#include "request.h"

namespace request {

bool get(const std::string& url, std::string& params, cJSON* response)
{
  return false;
}

bool post(const std::string& url, const cJSON* params, cJSON* response)
{
  return false;
}

}   // namespace request