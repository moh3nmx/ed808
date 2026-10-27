<template>
  <div>
    <md-button
      class="on-small md-raised filter-button"
      :class="scrolled > 100 ? 'slide-in' : ''"
      @click="showDialog = true"
    >
      <md-icon>filter_list</md-icon>
      Filters
    </md-button>
    <div class="filters on-large" style="position: relative;">
      <div class="loading" v-if="filtersLoading">
        <md-progress-spinner
          :md-diameter="50"
          :md-stroke="2"
          md-mode="indeterminate"
          style="position: fixed;top: calc(50% - 50px); z-index: 3;">
        </md-progress-spinner>
      </div>
      <div class="md-card" style="margin-bottom: 15px;" v-for="item in filters" :key="item.name" v-if="item.name">
        <md-toolbar style="min-height: 50px;" :md-elevation="0">
          <span class="md-subheading">{{item.name | formatName}}</span>
        </md-toolbar>
        <md-list class="md-scrollbar" style="max-height: 250px;overflow: overlay; overflow-x: hidden;">
          <md-checkbox
            v-for="sub_item in item.children" :key="sub_item.tid"
            v-model="$store.state.selected[item.name]"
            :value="sub_item.tid">
            {{sub_item.name | formatName}}
          </md-checkbox>
        </md-list>
      </div>
      <div v-if="!filtersLoading" style="margin-bottom: 15px;">
        <md-toolbar style="min-height: 50px;" :md-elevation="0">
          <span class="md-subheading">Tags</span>
        </md-toolbar>
        <div class="md-tags">
        <span v-for="(tag,index) in tags" :key="index" >
          <nuxt-link :to="`/tag/${tag.tid}/${tag.name}`">
            <md-button class="md-raised">{{ tag.name }}</md-button>
          </nuxt-link>
        </span>
        </div>
      </div>
    </div>
    <md-dialog :md-active.sync="showDialog">
      <md-dialog-title>
        Preferences
        <md-button class="md-icon-button right" @click="showDialog=false">
          <md-icon>close</md-icon>
        </md-button>
      </md-dialog-title>
      <div class="filters" style="position: relative;padding: 10px">
        <div class="loading" v-if="filtersLoading">
          <md-progress-spinner
            :md-diameter="50"
            :md-stroke="2"
            md-mode="indeterminate"
            style="position: fixed;top: calc(50% - 50px); z-index: 3;">
          </md-progress-spinner>
        </div>
        <div class="md-card" style="margin-bottom: 15px;" v-for="item in filters" :key="item.name" v-if="item.name">
          <md-toolbar style="min-height: 50px;" :md-elevation="0">
            <span class="md-subheading">{{item.name | formatName}}</span>
          </md-toolbar>
          <md-list class="md-scrollbar" style="max-height: 250px;overflow: overlay; overflow-x: hidden;">
            <md-checkbox
              v-for="sub_item in item.children" :key="sub_item.tid"
              v-model="$store.state.selected[item.name]"
              :value="sub_item.tid">
              {{sub_item.name | formatName}}
            </md-checkbox>
          </md-list>
        </div>
      </div>

      <md-dialog-actions>
        <md-button class="md-primary" @click="showDialog = false">Ok</md-button>
      </md-dialog-actions>
    </md-dialog>
  </div>
</template>

<script>


export default {
  name: 'ContentFilters',
  data () {
    return {
      filters:[],
      filtersLoading: true,
      tags:[],
      showDialog: false,
      scrolled: 0,
    }
  },
  mounted() {
    let vm = this
    window.addEventListener('scroll', function(e){
      vm.scrolled = window.scrollY
    })
    fetch('https://temp.ed808.com/api/latin/contents/all/filters')
      .then(response => response.json())
      .then((data) => {
        this.filters = data

        //making a dictionary for filters
        let filters = {}
        data.forEach(category => {
          category.children.forEach(child => {filters[child.tid] = child.name})
        })
        this.$store.commit('SET_FILTERS', filters)

        // setTimeout(() => {this.filtersLoading = false}, 300)
    })
    fetch('https://temp.ed808.com/api/latin/tag/home/list')
      .then(response => response.json())
      .then((data) => {
        this.tags = data
        console.log(data)
        setTimeout(() => {this.filtersLoading = false}, 300)
    })

    /*@todo: we use v-model="$store.state.selected[item.name]" and this is wrong with 'store strict mode : on'
    * instead use setters and getters
    * https://vuex.vuejs.org/guide/forms.html
    */
  },
  filters: {
    formatName: function(str) {
      let words = str.split(' ').filter(str => str !== '')
      // remove accents / diacritics
      words = words.map(str => str.normalize('NFD').replace(/[\u0300-\u036f]/g, ''))
      // capitalize
      words = words.map(str => str[0].toUpperCase() + str.slice(1))
      return words.join(' ')
    }
  }
}
</script>

<style lang="scss">
  @import '@/assets/scss/vars.scss';
  div.md-checkbox {
    margin: 5px 0px 6px 16px;
  }
  label.md-checkbox-label {
    white-space: nowrap;
  }
  .filters.md-layout-item{

    @media (max-width: 1280px) {
      min-width: 25%;
      max-width: 25%;
      flex: 0 1 25%;
    }
  }
  .md-tags {
    text-align: left;
    span{
      display: inline-block;
      .md-button {
        min-width: 60px;
      }
      .md-ripple {
        background-color: #EEEEEE;

        .md-button-content {
          font-size: 12px
        }
      }
      /*a{*/
        /*display: inline-block;*/
        /*line-height: 24px;*/
        /*padding: 5px;*/
        /*background-color: #EEEEEE;*/
        /*transition-duration: .5s;*/
        /*border-radius: 3px;*/
        /*color: #212121 !important;*/
        /*&:hover {*/
          /*background-color: #E0E0E0;*/

        /*}*/
      /*}*/
    }
  }
  .on-large {
    @media #{$x960} {
      display: none;
    }
  }
  .on-small {
    @media #{$m960} {
      display: none;
    }
  }
  .right {
    float: right;
  }
  .filters {
    .md-title {
      line-height: 36px !important;
    }
  }
  .filter-button {
    border-radius: 36px;
    position: fixed;
    bottom: -80px;
    left: calc(50% - 53px);
    z-index: 4;
    height: auto;
    opacity: 0.8;
    transition-duration: 0.5s;
    .md-ripple {
      padding: 10px 20px !important;
    }
    &:hover {
      opacity: 1;
    }
    &.slide-in {
      bottom: 30px;
    }
  }

  .md-dialog {
    overflow-y: scroll;
  }
</style>


