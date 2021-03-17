//
// Created by Jsiyong on 2021-03-14.
//

#include "../../container/controller.h"
#include "../vo/resultvo.h"
#include "../../protocol/http/httpmultipart.h"
#include "../service/fileservice.h"
#include "../../util/urlutil.h"
#include "../vo/params.h"

struct FileController : public Controller<FileController> {

    /**
     * 上传文件
     */
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

    /**
     * 下载文件
     */
    RequestMapping(download, "/download")
    void download(HttpRequest *request, HttpResponse *response) {
        auto params = request->getRequestParams();
        std::string filePath = UrlUtil::decode(params["path"]);
        response->setHeader("Content-Disposition",
                            "attachment;filename=" + params["path"].substr(params["path"].find_last_of('/') + 1));
        response->setHeader("Content-Type", "application/octet-stream");
        FileService::readFile(filePath, response->getBody());
    }

    /**
     * 创建文件夹
     */
    RequestMapping(createFolder, "/createFolder")
    ResponseBody createFolder(HttpRequest *request, HttpResponse *response) {

        //解析json
        JsonValue jsonValue = JsonParser::toJsonValue(request->getBody());
        FolderCreateParam param = Serializable<FolderCreateParam>::deserialize(jsonValue.toObject());

        if (!FileService::createFolder(param.path == "/" ? param.path + param.name : param.path + "/" + param.name)) {
            return ResultVO(1, "fail");
        }
        return ResultVO(0, "success");
    }
};