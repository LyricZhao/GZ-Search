#include <iconv.h> //for gbk/big5/utf8
#include <string.h>
#include <string>
#include <cstdlib>

int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    iconv_t cd;
    int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);
    if (cd==0)
        return -1;
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen) == -1)
        return -1;
    iconv_close(cd);
    return 0;
}

std::string any2utf8(std::string in,std::string fromEncode,std::string toEncode) {
    char* inbuf=(char*) in.c_str();
    int inlen=strlen(inbuf);
    int outlen=inlen*4;//in case unicode 3 times than ascii
    char *outbuf = (char *) malloc(outlen);
    memset(outbuf, 0, outlen);
    int rst=code_convert((char*)fromEncode.c_str(),(char*)toEncode.c_str(),inbuf,inlen,outbuf,outlen);
    if(rst==0){
        std:: string v = std::string(outbuf);
        free(outbuf);
        return v;
    }else{
        free(outbuf);
        return in;
    }
}

std::string gbk2utf8(const char* in) {
    return any2utf8(std::string(in), std::string("GB18030"), std::string("utf-8"));
}

std:: string utf82gbk(std:: string in) {
    return any2utf8(in, std:: string("utf-8"), std:: string("GB18030"));
}