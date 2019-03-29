#include "awsv4.hpp"

namespace AWSV4{

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}
const std::string ISO8601_date(const std::time_t& t) noexcept {
        char buf[sizeof "2011-10-08T07:07:09.000Z"];
        std::strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&t));
        return std::string{buf};
    }

// time_t -> 20131222
const std::string utc_yyyymmdd(const std::time_t& t) noexcept {
        char buf[sizeof "20111008"];
        std::strftime(buf, sizeof buf, "%Y%m%d", std::gmtime(&t));
        return std::string{buf};
    }
const std::string sign(const std::string key, const std::string msg){
        unsigned char *c_key = new unsigned char[key.length() + 1];
        memcpy(c_key, (unsigned char *)key.data(), key.length());

        unsigned char *c_msg = new unsigned char[msg.length() + 1];
        memcpy(c_msg, (unsigned char *)msg.data(), msg.length());

         unsigned char * digest = HMAC(EVP_sha256(),c_key, key.length(),c_msg, msg.length(), NULL, NULL);


        delete[] c_key;
        delete[] c_msg;

        std::string signed_str = std::string((char *)digest, 32);

        return signed_str;
    }
string url_encode(const string &value) {
 ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}
}


