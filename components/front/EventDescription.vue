<template>
  <md-content class="news md-elevation-2">
    
    <div class="event loading" v-if="loading">
      <md-progress-bar md-mode="indeterminate"></md-progress-bar>
    </div>
    
    <md-button @click="clear_news()" class="md-icon-button myclose">
        <md-icon>close</md-icon>
      </md-button>
    <div class="md-title">{{news.title}}</div>

    <!--
    ------ if you wanna print image uncomment below code:
    -->
    <!--<div class="md-layout md-gutter" v-if="'image' in news">
      <div class="md-layout-item">
        <img v-bind:src="news.image" />
      </div>
      <div class="md-body-2 md-layout-item md-size-100" v-html="news.body_value"></div>
    </div>
    <div class="md-layout md-gutter" v-else>
      <div class="md-body-2 md-layout-item md-size-100 " v-html="news.body_value"></div>
    </div>-->

    <!--
    ------ we just print body without main image of content
    -->
    <div class="md-layout md-gutter front-page-node-teaser">
      <div class="md-body-2 md-layout-item md-size-100 " v-html="news.body_value"></div>
    </div>
    
    <nuxt-link :to="'/node/' + nid">
      <md-button class="md-raised" style="margin: 0 0 0 auto;display: block;">read more</md-button>
    </nuxt-link>
  </md-content>
</template>

<script>

export default {
  name: 'EventDescription',
  props: ['nid'],
  data(){
    return{
      news:[],
      loading: true
    }
  },
  mounted(){
    this.getNews()
  },
  watch: {
    nid: function(){
      this.getNews()
    }
  },
  methods:{
    getNews(){
      if(this.nid != 0){
        this.loading = true
        fetch("https://ed808.com:92/latin/contents/" + this.nid)
          .then(response => response.json())
          .then((data) => {
            this.news = data.content
            this.loading = false
        })
      }
    },
    clear_news(){
      this.$emit('clearNid');
    }
  }
}
</script>

<style lang="scss">
.event.loading {
	position: absolute;
	width: 100%;
	height: 100%;
	left: 0;
	top: 0;
  background: rgba(255, 255, 255, 0.5);
}
:root{
    --md-theme-default-primary: #673AB7;
}
.md-content.news{
  width: calc(100% - 72px);
	margin: 0 36px;
	padding: 25px 25px;
	text-align: left;
	position: relative;
  .md-title{
    border-bottom: 1px solid #eee;
    padding-bottom: 20px;
    margin-bottom: 9px;
  }
  .myclose{
    position: absolute;
    top:10px;
    right:5px;
    .md-icon{
      color:#92278f;
    }
  }
}
.md-layout.front-page-node-teaser {
  img {
    display: none!important;
  }
}
</style>

