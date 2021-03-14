//
// Created by Jsiyong on 2021-03-14.
//

#include "../../container/controller.h"
#include "../vo/resultvo.h"
#include "../../protocol/http/httpmultipart.h"
#include "../service/fileservice.h"

struct FileController : public Controller<FileController> {

    RequestMapping(upload, "/upload")
    ResponseBody upload(HttpRequest *request, HttpResponse *response) {
        HttpMultiPart multiPart;
        multiPart.fromHttpRequest(request);//解析HttpMultipart
        if (!multiPart.completed()) {
            return ResultVO(1, "invalid multipart formatter");
        }

        auto params = request->getRequestParams();
        std::string path = params["path"];

        //支持多文件上传
        for (const FormDataItem &item:multiPart.getFormData()) {
            auto disposition = item.header;
            auto keyValuePair = HttpMultiPart::parseContentDisposition(disposition["Content-Disposition"]);
            //取出文件名和文件路径
            FileService::saveFile(path + "/" + keyValuePair["filename"], item.body);
        }
        return ResultVO(0, "success");
    }
};