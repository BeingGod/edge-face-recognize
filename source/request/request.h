#include <cjson/cJSON.h>

#include <string>
#include <vector>

namespace request {

bool get(const std::string& url, std::string& params, const cJSON* response);

bool post(const std::string& url, const cJSON* params, const cJSON* response);

}   // namespace request