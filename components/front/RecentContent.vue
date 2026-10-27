<template>
  <div class="container" v-if="contents.length > 0">
    <div class="md-headline">
      <h5>Recent Contents</h5>
      <div class="more-link">
        <nuxt-link tag="md-button" to="/contents">See All</nuxt-link>
      </div>
    </div>
    <div class="md-layout md-gutter">
      <div class="md-layout-item">
        <recent-content-teaser
          :title="contents[0].title"
          :picture="contents[0].picture"
          :createdTimeStamp="contents[0].created_timestamp"
          :nid="contents[0].nid"
          :uid="contents[0].uid"
          :topic="contents[0].topics[0].name"
          :author="contents[0].author_name"
          :metaDesc="contents[0].meta_description"
          :withImg="true"
        />
        <recent-content-teaser
          :title="contents[1].title"
          :createdTimeStamp="contents[1].created_timestamp"
          :nid="contents[1].nid"
          :uid="contents[1].uid"
          :topic="contents[1].topics[0].name"
          :author="contents[1].author_name"
          :metaDesc="contents[1].meta_description"
          :withImg="false"
        />
      </div>
      <div class="md-layout-item">
        <recent-content-teaser
          :title="contents[2].title"
          :createdTimeStamp="contents[2].created_timestamp"
          :nid="contents[2].nid"
          :uid="contents[2].uid"
          :topic="contents[2].topics[0].name"
          :author="contents[2].author_name"
          :metaDesc="contents[2].meta_description"
          :withImg="false"
        />
        <recent-content-teaser
          :title="contents[3].title"
          :picture="contents[3].picture"
          :createdTimeStamp="contents[3].created_timestamp"
          :nid="contents[3].nid"
          :uid="contents[3].uid"
          :topic="contents[3].topics[0].name"
          :author="contents[3].author_name"
          :metaDesc="contents[3].meta_description"
          :withImg="true"
        />
      </div>
      <div class="md-layout-item">
        <recent-content-teaser
          :title="contents[4].title"
          :picture="contents[4].picture"
          :createdTimeStamp="contents[4].created_timestamp"
          :nid="contents[4].nid"
          :uid="contents[4].uid"
          :topic="contents[4].topics[0].name"
          :author="contents[4].author_name"
          :metaDesc="contents[4].meta_description"
          :withImg="true"
        />
        <recent-content-teaser
          :title="contents[5].title"
          :createdTimeStamp="contents[5].created_timestamp"
          :nid="contents[5].nid"
          :uid="contents[5].uid"
          :topic="contents[5].topics[0].name"
          :author="contents[5].author_name"
          :metaDesc="contents[5].meta_description"
          :withImg="false"
        />
      </div>
    </div>
  </div>
</template>

<script>
  import axios from 'axios'
  import RecentContentTeaser from "@/components/fields/RecentContentTeaser";

  export default {
    name: 'RecentContent',
    components: {
      RecentContentTeaser
    },
    data () {
      return {
        contents:[],
        loading: true
      }
    },
    mounted() {
      //get recent posts to display
      axios.get('https://ed808.com:92/latin/contents?parameter[page]=0&parameter[limits]=6&parameter[sort]=newest&parameter[stick]=-1')
        .then((data) => {
          this.contents = data.data.contents;
          this.loading = false;
        })
    }
  }
</script>

<style lang="scss">
  .container {
    padding: 15px;
  }
  .md-layout-item {
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
