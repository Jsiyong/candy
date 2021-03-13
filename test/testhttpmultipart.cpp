//
// Created by Jsiyong on 2021-03-13.
//
#include "../src/protocol/http/httpmultipart.h"

/*
POST / HTTP/1.1\r\n
Host: 192.168.66.66:8888\r\n
Connection: keep-alive\r\n
Content-Length: 591\r\n
Cache-Control: no-cache\r\n
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36\r\n
Content-Type: multipart/form-data; boundary=----WebKitFormBoundarySAJsB3ZsTmgE6job\r\n
Accept: **\r\n
Origin: chrome-extension://coohjcphdfgbiolnekdpbcijmhambjff\r\n
Accept-Encoding: gzip, deflate\r\n
Accept-Language: zh-CN,zh;q=0.9\r\n
\r\n
------WebKitFormBoundarySAJsB3ZsTmgE6job\r\n
Content-Disposition: form-data; name=\"cs1\"; filename=\"tp2.png\"\r\n
Content-Type: image/png\r\n
\r\n
\211PNG\r\n\032\n\000\000\000\rIHDR\000\000\000%\000\000\000\027\b\002\000\000\000\003N\021\362\000\000\000\001sRGB\000\256\316\034\351\000\000\000\004gAMA\000\000\261\217\v\374a\005\000\000\000\tpHYs\000\000\022t\000\000\022t\001\336f\037x\000\000\000\245IDATHK\275\224A\016\300 \b\004\245\377\377\263%\321\030\003\270\202$̥=\310NA+\365\336[!\337|VQ\355\213͓\210\346\233\302\231\343\362\001\215\006\a\"_H\243\061\223\217\373\227\224\061f\202\355\313\313\006:ǘ'\220\275\r\177\257\362\376\017\\\003d\314u\301@\366\247?ӓ\"\000!\227\376\036d\f\250\252\276_.>p\n\000\240J\372\364(\242J\274\276\372\177\260\357\263hO\000\221o\357\037\350#\204\316\071\236\227\274Ҟ\234'74^\034\350\362-ގ\322N̗\247\366~i\355\an\315e\372\070\305u\300\000\000\000\000IEND\256B`\202\r\n
------WebKitFormBoundarySAJsB3ZsTmgE6job\r\n
Content-Disposition: form-data; name=\"cs1\"; filename=\"wj1.txt\"\r\n
Content-Type: text/plain\r\n
\r\n1234\r\n
------WebKitFormBoundarySAJsB3ZsTmgE6job--\r\n
 */

int main() {

    std::string src;
    src.append("------WebKitFormBoundarySAJsB3ZsTmgE6job\r\n"
               "Content-Disposition: form-data; name=\"cs1\"; filename=\"tp2.png\"\r\n"
               "Content-Type: image/png\r\n"
               "\r\n"
               "\211PNG\r\n\123dd\r\n---WebKitFormBoud2\r\n"
               "------WebKitFormBoundarySAJsB3ZsTmgE6job\r\n"
               "Content-Disposition: form-data; name=\"cs1\"; filename=\"wj1.txt\"\r\n"
               "Content-Type: text/plain\r\n"
               "\r\n1234\r\n------WebKitFormBoundarySAJsB3ZsTmgE6job-\r\n"
               "------WebKitFormBoundarySAJsB3ZsTmgE6job--\r\n");

    HttpMultiPart multiPart;
    multiPart.setBoundary("----WebKitFormBoundarySAJsB3ZsTmgE6job");
    multiPart.tryDecode(src);

    return 0;
}
