<template>
  <!--Popular posts if not login, user intrests if login -->
  <div class="container"  v-if="contents">
    <div class="md-headline" v-if="$store.getters.getUid">
      <h5>User Interests</h5>
    </div>
    <div class="md-headline" v-else>
      <h5>Popular Contents</h5>
    </div>
    <div class="md-layout md-gutter">
      <div
        v-for="content in contents"
        :key="content.nid"
        class="md-layout-item"
      >
        <favourite-test
          :title="content.title"
          :picture="content.picture"
          :nid="content.nid"
          :topic="content.topics.hasOwnProperty('name') ? content.topics.name : content.topics[0].name"
          :metaDesc="content.meta_description"
          :claps="content.clap"
          :comments="content.comment_count"
        />
      </div>
    </div>
  </div>
</template>

<script>
  import axios from 'axios'
  import FavouriteTeaser from '@/components/fields/FavouriteTeaser'
  import FavouriteTest from '@/components/fields/FavouriteTest'
  export default {
    name: 'FavouriteContent',
    components: {
      FavouriteTest
    },
    data () {
      return {
        loginActive: false,
        contents: false
      }
    },
    mounted() {
      //get user intrests
      if (this.$store.getters.getUid) {
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials  = true;
        axios.get('https://temp.ed808.com/api/latin/contents/user/interest?parameter[page]=0&parameter[limits]=4')
          .then((data) => {
            this.contents = data.data.contents;
          })
        }
      else {
        //get popular contents
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials  = true;
        axios.get('https://temp.ed808.com/api/latin/test?parameter[page]=0&parameter[limits]=4&parameter[query]=picture,meta_description,topics,clap,comment&parameter[sort]=popular')
          .then((data) => {
            this.contents = data.data.contents;
          })
      }
    }
  }
</script>

<style scoped lang="scss">
  .md-layout-item {
    padding: 15px !important;
    @media (max-width: 960px){
      width: 50%;
      -ms-flex: 0 1 50%;
      flex: 0 1 50%;
    }
    @media (max-width: 600px){
      width: 100%;
      -ms-flex: 0 1 100%;
      flex: 0 1 100%;
    }
  }
</style>
