#include "awsv4.hpp"
using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
   ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int publishMessage(string access_key,string secret_key,string message,string subject,string topicArn) {

    // current time
    struct std::tm *t;
    time_t rawtime;
    time( &rawtime );
    t = localtime( &rawtime );
    const std::time_t request_date = std::mktime(t);
    cout<<"request date : "<<request_date;
    
    const std::string base_uri{"sns.us-east-1.amazonaws.com"};
  
    //parameteres in param should be sorted in byte order
    string params="AWSAccessKeyId="+access_key+
                  "&Action=Publish"
                  "&Message="+AWSV4::url_encode(message)+
                  "&SignatureMethod=HmacSHA256"
                  "&SignatureVersion=2"
                  "&Subject="+AWSV4::url_encode(subject)+
                  "&Timestamp="+AWSV4::url_encode(AWSV4::ISO8601_date(request_date))+
                  "&TopicArn="+AWSV4::url_encode(topicArn);
   
    string string_to_sign = "GET\n"
                            +base_uri+"\n"
                            "/"+"\n"
                            +params;

    auto s = AWSV4::sign(secret_key,string_to_sign);
    
    std::string signature_str =AWSV4::url_encode(AWSV4::base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length()));
   
    string request_url = "http://"+base_uri + "/?"+params+"&Signature=" + signature_str;
    cout<<request_url<<endl;
    
    CURL *curl;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL,request_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        std::cout << readBuffer << std::endl;
        }
    return 0;
}
/*
int main()
{
  publishMessage( "AKIAsdsJCR7ZQB4S4XOQPNQ","lYxTssdpeSj5vvCXYRvT4uXJvQfbflr8Um3YgH6mgSZ","a message from the main method","final hello 2","arn:aws:sns:us-east-1:650395316324:notifyme");
}*/
