<template>
    <div class="folder-content">
        <div class="folder-bg">
            <div class="folder-table">
                <div class="line header">
                    <div class="min">路径</div>
                    <div class="min">名称</div>
                    <div class="min">修改日期</div>
                    <div class="min">大小</div>
                </div>
                <div class="container">
                    <div class="line content" v-for="item in fileList">
                        <div class="min">{{item.path}}</div>
                        <div class="name min">{{item.name}}</div>
                        <div class="min">{{item.lastModifyTime}}</div>
                        <div class="min">{{item.size}}</div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
    module.exports = {
        name: 'FolderContent',
        props: ["fileList"],
        data() {
            return {}
        },
        methods: {},
        created() {
        }
    }
</script>
<style>
    .folder-content {
        margin: 30px;
        background: linear-gradient(to left, #89ba86, #89ba86) left top no-repeat,
        linear-gradient(to bottom, #89ba86, #89ba86) left top no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) right top no-repeat,
        linear-gradient(to bottom, #89ba86, #89ba86) right top no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) left bottom no-repeat,
        linear-gradient(to bottom, #89ba86, #89ba86) left bottom no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) right bottom no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) right bottom no-repeat;
        background-size: 3px 50px, 50px 3px, 3px 50px, 50px 3px;

    }

    .folder-content .folder-bg {
        background: #525d4e0f;
        width: calc(100% - 20px);
        height: calc(100% - 20px);
        margin: 10px;
        display: flex;
    }

    .folder-content .folder-table {
        width: 100%;
        height: 100%;
        padding: 30px;
        text-align: center;
    }

    .folder-content .folder-table .line {
        display: flex;
    }

    .folder-content .folder-table .line .min {
        width: 50%;
        display: flex;
        align-items: center;
        justify-content: center;
    }

    .folder-content .folder-table .header {
        height: 40px;
        font-weight: bold;
        color: #62855F;
        font-size: 18px;
        pointer-events: all;
    }

    .folder-content .folder-table .header .min:not(:last-of-type) {
        position: relative;
    }

    .folder-content .folder-table .header .min:not(:last-of-type):after {
        content: " ";
        width: 1px;
        height: 100%;
        display: inline-block;
        background: #ccc;
        position: absolute;
        right: 0;
    }

    .folder-content .folder-table .container {
        height: calc(100% - 100px);
        overflow-y: scroll;
        pointer-events: all;
    }

    .folder-content .folder-table .container .content {
        height: 25px;
        font-size: 14px;
        cursor: pointer;
        color: #747474;
    }

    .folder-content .folder-table .container .content:hover {
        background: rgba(26, 139, 40, 0.18)
    }

    .folder-content .folder-table .container .content .name {
        color: black;
    }
</style>