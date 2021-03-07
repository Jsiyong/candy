<template>
    <div class="up-down" :class="{'constructor-hidden':!constructorShow}">
        <file-constructor class="left"
                          :change-selected-node="changeSelectedNode"
                          :toggle-constructor="toggleConstructor"
                          :constructor-show="constructorShow"></file-constructor>
        <folder-content class="right"
                        :file-list="fileList"></folder-content>
    </div>
</template>

<script>

    module.exports = {
        name: 'UpDown',
        components: {
            'file-constructor': httpVueLoader('../components/updown/FileConstructor.component.vue'),
            'folder-content': httpVueLoader('../components/updown/FolderContent.component.vue')
        },
        data() {
            return {
                fileList: [],//选中节点
                constructorShow: true,//是否隐藏左边列表
            }
        },
        methods: {
            //点击选中节点
            changeSelectedNode(node) {
                this.fileList = node.data.fileList;
            },
            //显示隐藏左边列表
            toggleConstructor() {
                this.constructorShow = !this.constructorShow;
            }
        }
    }
</script>

<style>
    .up-down {
        height: calc(100% - 61px);
        display: flex;
    }

    .up-down .left {
        width: 50%;
        transition: all 0.5s;
    }

    .up-down .right {
        width: 50%;
        transition: all 0.5s;
    }

    .up-down.constructor-hidden .left {
        width: 0;
    }

    .up-down.constructor-hidden .right {
        width: 100%;
    }
    ::-webkit-scrollbar {
        width: 6px !important;
        background: transparent !important;
    }

    ::-webkit-scrollbar-thumb {
        border-radius: 10px !important;
        background: #6d986a !important;
    }

    ::-webkit-scrollbar-track {
        background: transparent !important;
    }
</style>