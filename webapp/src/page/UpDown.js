import FileConstructor from '../components/updown/FileConstructor.component.js'
import FolderContent from "../components/updown/FolderContent.component.js";

export default {
  name: 'UpDown',
  template:`
     <div class="up-down" :class="{'constructor-hidden':!constructorShow}">
      <file-constructor class="left"
                        :changeSelectedNode="changeSelectedNode"
                        :selectedId="selectedId"
                        :toggleConstructor="toggleConstructor"
                        :constructorShow="constructorShow"></file-constructor>
      <folder-content class="right"
                      :selectedId="selectedId"></folder-content>
    </div>`,
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
  components: {
    FolderContent,
    FileConstructor
  }
}