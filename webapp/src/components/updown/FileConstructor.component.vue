<template>
    <div class="file-constructor">
        <el-tree :data="folderList"
                 node-key="name"
                 render-after-expand
                 highlight-current
                 :props="defaultProps"
                 :expand-on-click-node="true"
                 @node-click="clickHandler"
                 @node-expand="clickHandler"
                 @node-collapse="clickHandler">
            <template #default="{ node, data }">
        <span class="custom-tree-node">
          <span class="folder-icn" :class="{'expanded':node.expanded,'is-leaf':node.isLeaf}"></span>
          <span>{{data.name }}</span>
          <span class="opt-icn">
            <i class="el-icon-download" v-if="node.isLeaf" title="下载文件"></i>
            <i class="el-icon-upload2" v-else title="上传文件"></i>
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
                }
            }
        },
        methods: {
            //点击选中节点
            clickHandler(data, node, nodeSelf) {
                this.changeSelectedNode(node);//改变upDown中的选中节点
            },
            //显示隐藏左边列表
            clickConstructorIcn() {
                this.toggleConstructor();
            },
            fetchData() {
                axios.get(window.$config.addr + '/getFolder?path=/tmp').then((res) => {
                    console.log(res.data);
                    this.folderList.push(res.data);
                })
            }
        },
        mounted() {
            this.fetchData();
        }
    }
</script>
<style scoped>
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
        right: 0px;
        top: 50%;
        transform: translate(100%, -100%);
    }


    .el-tree {
        overflow: scroll;
        height: 100%;
        background: transparent !important;
        color: inherit;
        margin: 50px 10px;
    }

    .el-tree-node:focus > .el-tree-node__content {
        background-color: transparent;
    }

    .el-tree-node.is-current > .el-tree-node__content {
        background-color: #62855F;
    }

    .el-tree-node .el-tree-node__content {
        position: relative;
    }

    .el-tree-node .el-tree-node__content .el-tree-node__expand-icon {
        color: #fff;
    }

    .el-tree-node .el-tree-node__content .el-tree-node__expand-icon.is-leaf {
        color: transparent;
    }

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