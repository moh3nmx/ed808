<template>
  <div class="favourite-teaser">
    <md-card>
      <md-card-media>
        <nuxt-link :to="'/node/' + nid">
          <div class="card-image">
            <div class="img" :style="`background-image:url(${picture != null ? picture : '/images/default.jpg'})`"></div>
          </div>
        </nuxt-link>
      </md-card-media>

      <md-card-header>
        <h6 class="category text-danger"><md-icon style="color: #f44336; font-size: 16px !important;">subject</md-icon>{{ topic }}</h6>
        <div class="md-title"><nuxt-link :to="'/node/' + nid">{{ title }}</nuxt-link></div>
      </md-card-header>

      <md-card-expand>
        <md-card-actions md-alignment="space-between">

          <div class="stats" v-if="claps && comments">
            <span>
              <img src="/images/clap-active.png" alt="" style="padding-bottom: 2px" width="20px">
              <md-tooltip md-direction="bottom">Claps</md-tooltip>
              {{ claps }}
            </span>
            <md-icon class="stat-2">chat_bubble_outline
              <md-tooltip md-direction="bottom">Comments</md-tooltip>
            </md-icon>{{ comments }}
          </div>

          <md-card-expand-trigger>
            <md-button class="md-icon-button" @click="toolTipActive=!toolTipActive">
              <md-icon>keyboard_arrow_down</md-icon>
              <md-tooltip md-direction="bottom">See More</md-tooltip>
              <!--<md-icon v-if="clicked">keyboard_arrow_up</md-icon>-->
              <!--<md-tooltip :md-active.sync="toolTipActive" md-direction="bottom">See Less</md-tooltip>-->
            </md-button>
          </md-card-expand-trigger>

        </md-card-actions>

        <md-card-expand-content>
          <md-card-content>
            {{ metaDesc }}
          </md-card-content>
        </md-card-expand-content>
      </md-card-expand>

    </md-card>
  </div>
</template>

<script>
  export default {
    name: "FavouriteTeaser",
    props: ['title', 'picture', 'metaDesc', 'topic', 'nid', 'claps', 'comments'],
    data () {
      return {
        toolTipActive: false
      }
    }
  }
</script>

<style lang="scss" scoped>
  .card-expansion {
    height: 480px;
  }
  .md-card {
    margin: 4px;
    display: inline-block;
    vertical-align: top;
    border-radius: 6px;
    overflow: hidden;
    flex: 1;
  }
  .md-title {
    font-weight: 700;
    min-height: 72px;
    line-height: 24px;
    color: #3C4858;
    font-family: "Roboto Slab", "Times New Roman", serif;
    font-size: 14px;
  }
  .md-title a {
    color: #3C4858;
  }
  .md-title a:hover {
    color: #3C4858;
  }
  .favourite-teaser {
    text-align: left;
    display: flex;
  }
  .md-card-media {
    padding: 10px;
  }
  .text-danger {
    color: #f44336;
    cursor: default;
    font-size: 10px;
    text-transform: uppercase;
    margin-top: 10px;
    margin-bottom: 10px;
  }
  .img {
    width: 100%;
    padding-bottom: 60%;
    background-size: cover;
    background-position: center;
  }
  .md-card-header {
    padding-top: 0 !important;
    padding-bottom: 0 !important;
  }
  .md-card-actions {
    padding: 2px 8px !important;
    text-align: right;
    display: block;
  }
  .md-button {
    cursor: default;
  }
  .stats {
    display: inline-block;
    padding-left: 16px;
    padding-top: 10px;
    font-size: 12px;
    position: absolute;
    width: 100%;
    left: 0;
    text-align: left;
  }
  .stats .stat-1 {
     font-weight: 400;
     color: #f44336;
     font-size: 18px !important;
   }
  .stats .stat-2 {
    font-weight: 400;
    color: #43A047;
    font-size: 18px !important;
  }
</style>
