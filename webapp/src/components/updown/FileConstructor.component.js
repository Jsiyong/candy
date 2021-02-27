export default {
  name: 'FileConstructor',
  template:`
    <div class="file-constructor">
      <el-tree :data="data"
               node-key="id"
               :current-node-key="selectedId"
               default-expand-all
               :expand-on-click-node="false" @node-click="clickHandler">
        <template #default="{ node, data }">
            <span class="custom-tree-node">
            <span class="folder-icn" :class="{'expanded':node.expanded,'is-leaf':node.isLeaf}"></span>
            <span>{{ node.label }}</span>
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
    `,
  components: {},
  props: ["changeSelectedNode","selectedId","toggleConstructor","constructorShow"],
  data() {
    return {
      data: [
        {
          id: 1,
          label: '一级 1',
          children: [{
            id: 4,
            label: '二级 1-1',
            children: [{
              id: 9,
              label: '三级 1-1-1'
            }, {
              id: 10,
              label: '三级 1-1-2'
            }]
          }]
        }, {
          id: 2,
          label: '一级 2',
          children: [{
            id: 5,
            label: '二级 2-1'
          }, {
            id: 6,
            label: '二级 2-2'
          }]
        }, {
          id: 3,
          label: '一级 3',
          children: [{
            id: 7,
            label: '二级 3-1'
          }, {
            id: 8,
            label: '二级 3-2'
          }]
        }]
    }
  },
  methods: {
    //点击选中节点
    clickHandler(data,node,nodeSelf){
      this.changeSelectedNode(node.id);//改变upDown中的选中节点
    },
    //显示隐藏左边列表
    clickConstructorIcn(){
      this.toggleConstructor();
    }
  }
}