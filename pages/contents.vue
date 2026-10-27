<!--Done-->
<template>
  <div
    class="content-container"
    style="padding-top: 15px;"
  >
    <div
      class="md-layout md-gutter"
      style="align-items: flex-start; padding: 15px"
    >
      <!--Filter Settings For Contents-->
      <content-filters
        class="md-layout-item md-size-20"
      />

      <!--All Contents Section-->
      <node-list
        :filterEnabled="true"
        :component="false"
        :router="'contents'"
        class="md-theme-default md-layout-item md-size-80"
      />
    </div>
  </div>
</template>

<script>

import NodeList from '../components/allContents/NodeList'
import ContentFilters from "../components/allContents/ContentFilters";

export default {
  name: 'Contents',
  scrollToTop: true,
  components: {
    ContentFilters,
    NodeList,
  },
  head(){
    return{
      title: 'All Contents',
      meta: [
        // OpenGraph data (Most widely used)
        {
          'property': 'og:title',
          'content': this.title,
          'template': '%s - ed808',
          'vmid': 'og:title'
        },
        {property: 'og:url', content: 'https://ed808.com/contents', vmid: 'og:url'},
        {
          'name': 'twitter:title',
          'content': this.title,
          'template': '%s - ed808',
          'vmid': 'twitter:title'
        },
        // Google / Schema.org markup:
        {
          'itemprop': 'name',
          'content': this.title,
          'template': '%s - ed808',
          'vmid': 'name'
        },
      ],
      links: [
        {rel: 'canonical', href: 'https://ed808.com/contents'}
      ]
    }
  }
}
</script>

<style lang="scss">
  .content-container {
    min-height: 500px;
  }
  body {
    background: #F5F5F5;
  }
</style>

