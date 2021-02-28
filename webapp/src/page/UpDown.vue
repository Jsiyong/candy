<template>
  <div class="up-down" :class="{'constructor-hidden':!constructorShow}">
    <file-constructor class="left"
                      :change-selected-node="changeSelectedNode"
                      :selected-id="selectedId"
                      :toggle-constructor="toggleConstructor"
                      :constructor-show="constructorShow"></file-constructor>
    <folder-content class="right"
                    :selected-id="selectedId"></folder-content>
  </div>
</template>

<script>
// import FileConstructor from '/@/components/updown/FileConstructor.component.vue'
// import FolderContent from "../components/updown/FolderContent.component.vue";

module.exports = {
  name: 'UpDown',
  components: {
    'file-constructor': httpVueLoader('../components/updown/FileConstructor.component.vue'),
    'folder-content': httpVueLoader('../components/updown/FolderContent.component.vue')
  },
  data(){
    return{
      selectedId:'',//选中节点id
      constructorShow:true,//是否隐藏左边列表
    }
  },
  methods: {
    //点击选中节点
    changeSelectedNode(id){
      this.selectedId = id;
    },
    //显示隐藏左边列表
    toggleConstructor(){
      this.constructorShow=!this.constructorShow;
    }
  },
  // components: {
  //   FolderContent,
  //   FileConstructor
  // }
}
</script>

<style>
  .up-down{
    height: calc(100% - 61px);
    display: flex;
  }
  .up-down .left{
    width:20%;
    transition: all 0.5s;
  }
  .up-down .right{
    width:80%;
    transition: all 0.5s;
  }
  .up-down.constructor-hidden .left{
    width:0;
  }
  .up-down.constructor-hidden .right{
    width:100%;
  }
</style>