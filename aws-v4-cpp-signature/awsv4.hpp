#ifndef AWSV4_HPP
#define AWSV4_HPP
#include <iomanip>
#include <curl/curl.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <ctime>
#include <iostream>
#include <sstream>

#include "Poco/URI.h"
#include "Poco/StringTokenizer.h"

#include "openssl/sha.h"
#include "openssl/hmac.h"

using namespace std;

namespace AWSV4 {
    string url_encode(const string &value);

    std::string base64_encode(unsigned char const* , unsigned int len);
    const std::string sign(const std::string key, const std::string msg);    
    const std::string ISO8601_date(const std::time_t& t) noexcept;
    
    const std::string utc_yyyymmdd(const std::time_t& t) noexcept;

}

#endif
