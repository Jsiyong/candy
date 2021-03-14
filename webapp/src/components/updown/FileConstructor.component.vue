<template>
    <div class="file-constructor">
        <div class="url-paths" v-if="constructorShow">
            <el-link v-for="(item,index) in pathList" :key="index" @click="clickPathHandler(index)">
                {{item}}/
            </el-link>
        </div>
        <input ref="fileSelector" name="file" type="file" @change="uploadFile(currentClickNode)">
        <el-tree :data="folderList"
                 node-key="name"
                 render-after-expand
                 highlight-current
                 default-expand-all
                 :props="defaultProps"
                 :expand-on-click-node="true"
                 @node-click="clickHandler">
            <template #default="{ node, data }">
        <span class="custom-tree-node">
          <span class="folder-icn" :class="{'expanded':node.expanded,'is-leaf':node.isLeaf}"></span>
          <span>{{data.name }}</span>
          <span class="opt-icn">
<!--            <i class="el-icon-download" v-if="node.isLeaf" title="下载文件"></i>-->
              <!--            <i class="el-icon-upload2" v-else title="上传文件" @click="clickSelectFile(data)"></i>-->
              <i class="el-icon-upload2" title="上传文件" @click.stop="clickSelectFile(data)"></i>
          </span>
        </span>
            </template>
        </el-tree>
        <div class="expand-icn" @click="clickConstructorIcn">
            <i class="el-icon-d-arrow-left" v-if="constructorShow"></i>
            <i class="el-icon-d-arrow-right" v-else></i>
        </div>

    </div>
</template>

<script>

    module.exports = {
        name: 'FileConstructor',
        props: ["changeSelectedNode", "toggleConstructor", "constructorShow"],
        data() {
            return {
                folderList: [],
                defaultProps: {
                    children: 'folderList',
                    label: 'name'
                },
                pathList: [],
                currentClickNode: {}
            }
        },
        methods: {
            uploadFile(data) {
                //上传文件
                const inputFile = this.$refs.fileSelector.files[0];
                var forms = new FormData()
                forms.append('file', inputFile)
                axios.post(`${window.$config.addr}/upload?path=${this.formatPath(data.path + "/" + data.name)}`, forms).then(res => {
                    this.$message(res.data.msg);
                })
            },
            clickSelectFile(data) {
                this.currentClickNode = data;
                this.$refs.fileSelector.dispatchEvent(new MouseEvent('click'));//触发input框的点击事件
            },
            clickPathHandler(index) {
                console.log(index)
                let path = ''
                var pathListTmp = []
                for (var i = 0; i <= index; i++) {
                    path += '/' + this.pathList[i]
                    pathListTmp.push(this.pathList[i])
                }
                this.pathList = pathListTmp;
                this.fetchData(path)
            },
            //点击选中节点
            clickHandler(data, node, nodeSelf) {
                this.changeSelectedNode(node);//改变upDown中的选中节点
                this.fetchData(data.path + '/' + data.name)
            },
            //显示隐藏左边列表
            clickConstructorIcn() {
                this.toggleConstructor();
            },
            fetchData(path) {
                path = this.formatPath(path)//路径格式化一下
                axios.get(`${window.$config.addr}/getFolder?path=${path}`).then((res) => {
                    this.folderList = [res.data];
                    this.pathList = this.genPathList(path);
                })
            },
            //路径列表格式化
            genPathList(path) {
                let list = path.split('/');
                let res = ['']
                for (var i = 0; i < list.length; i++) {
                    if (list[i] !== '') {
                        res.push(list[i])
                    }
                }
                return res;
            },
            //路径格式化
            formatPath(path) {
                let res = '';
                let list = path.split('/');
                for (var i = 0; i < list.length; i++) {
                    if (list[i] !== '') {
                        res += '/' + list[i];
                    }
                }
                if (res.length === 0) {
                    res = '/';
                }
                return res;
            }
        },
        mounted() {
            this.fetchData('/');
        }
    }
</script>
<style scoped>
    input[type=file] {
        display: none;
    }

    .url-paths {
        margin-top: 2%;
        margin-left: 4%;
    }

    .file-constructor {
        pointer-events: all;
        height: 100%;
        background: rgba(44, 113, 15, 0.38);
        border-radius: 0 8px 8px 0;
        position: relative;
        color: #fff;
    }


    .file-constructor .expand-icn {
        background: rgb(98, 133, 95);
        border-radius: 0 4px 4px 0;
        color: white;
        padding: 30px 2px;
        cursor: pointer;
        position: absolute;
        right: 0;
        top: 50%;
        transform: translate(100%, -100%);
    }


    .el-tree {
        overflow: scroll;
        height: 100%;
        background: transparent !important;
        color: inherit;
        margin-left: 2%;
    }

    .el-tree-node:focus > .el-tree-node__content {
        background-color: transparent;
    }

    .el-tree-node__content > .el-tree-node__expand-icon {
        padding: 0;
    }

    .el-icon-caret-right:before {
        content: none;
    }

    .el-tree-node.is-current > .el-tree-node__content {
        background-color: #62855F;
    }

    .el-tree-node .el-tree-node__content {
        position: relative;
    }

    .el-tree-node .el-tree-node__content .el-tree-node__expand-icon {
        color: transparent;
    }

    /*
        .el-tree-node .el-tree-node__content .el-tree-node__expand-icon.is-leaf {
            color: transparent;
        }
    */
    .el-tree-node .el-tree-node__content .folder-icn {
        display: inline-block;
        background-image: url("../../assets/folder-close.png");
        background-size: 100% 100%;
        margin-left: 5px;
        width: 18px;
        height: 14px;
        margin-right: 10px;
    }

    .el-tree-node .el-tree-node__content .folder-icn.expanded {
        background-image: url("../../assets/folder-open.png");
        background-size: 100% 100%;
    }

    /*
        .el-tree-node .el-tree-node__content .folder-icn.is-leaf {
            opacity: 0;
        }
    */

    .el-tree-node .el-tree-node__content .opt-icn {
        position: absolute;
        right: 5px;
        top: 3px;
    }

    .el-tree-node .el-tree-node__content .opt-icn > i {
        border-radius: 50%;
        color: white;
        padding: 2px;
    }

    .el-tree-node .el-tree-node__content .opt-icn > i.el-icon-download {
        background: #2aa7ad;
    }

    .el-tree-node .el-tree-node__content .opt-icn > i.el-icon-upload2 {
        background: #cc698d;
    }

    .el-tree-node .el-tree-node__content .opt-icn:hover {
        filter: drop-shadow(0px 0px 5px #fff);
    }

    .el-tree-node .el-tree-node__content:hover {
        background-color: #62855F;
    }

    .el-tree-node .el-tree-node__content:hover .opt-icn > i {
        filter: brightness(1.2);
    }

    .el-tree-node .el-tree-node__content:hover .opt-icn:hover {
        filter: drop-shadow(0px 0px 5px #fff);
    }
</style>