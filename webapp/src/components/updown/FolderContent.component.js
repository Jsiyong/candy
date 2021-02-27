export default {
  name: 'FolderContent',
  template:`
    <div class="folder-content">
      <div class="folder-bg">
        <div class="folder-table">
          <div class="line header">
            <div class="min">名称</div>
            <div class="min">修改日期</div>
            <div class="min">类型</div>
            <div class="min">大小</div>
          </div>
          <div class="container">
            <div class="line content" v-for="item in list">
              <div class="name min">{{item.fileName}}</div>
              <div class="min">{{item.updateTime}}</div>
              <div class="min">{{item.fileType}}</div>
              <div class="min">{{item.fileSize}}</div>
            </div>
          </div>
        </div>
      </div>
  
    </div>`,
  props: [],
  data() {
    return {
      list: [
        {
          id: 4,
          fileName: "哈哈哈",
          fileType: "文件夹",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
        {
          id: 4,
          fileName: "test",
          fileType: "文本文档",
          updateTime: "2020-02-12 12:21:21",
          fileSize: "12KB"
        },
      ]
    }
  },
  methods: {

  },
  created(){
  }
}