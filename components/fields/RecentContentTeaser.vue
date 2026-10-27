<template>
  <div class="recent-teaser" md-with-hover>
    <!--card with img-->
    <div
      v-if="withImg"
      class="card card-blog"
      md-with-hover
    >
      <nuxt-link :to="'/node/' + nid">
        <div class="card-image">
          <div class="img" :style="`background-image:url(${picture != null ? picture : '/images/default.jpg'})`"></div>
          <div class="card-title">
            {{ title }}
          </div>
          <div class="colored-shadow" :style="{ backgroundImage: `url(${picture})`, opacity: 1}"></div>
        </div>
      </nuxt-link>
      <div class="card-content">
        <h6 class="category text-danger"><md-icon style="color: #f44336; font-size: 18px !important;">subject</md-icon>{{ topic }}</h6>
        <p class="card-description">
          {{ body_minimizer(metaDesc) }}...
        </p>
        <div class="footer">
          <div class="author">
            by <nuxt-link :to="'/user/' + uid" class="">{{ author }}</nuxt-link>,
            <no-ssr>
              <time-ago :refresh="60" :datetime="createdTimeStamp * 1000" long>{{ createdTimeStamp }}</time-ago>
            </no-ssr>
          </div>
        </div>
      </div>
    </div>

    <!--card without img-->
    <div
      v-else
      class="card"
      md-with-hover
    >
      <div class="card-content">
        <h6 class="category text-danger">
          <md-icon style="color: #f44336; font-size: 18px !important;">subject</md-icon>{{ topic }}
        </h6>
        <h4 class="card-title">
          <nuxt-link :to="'/node/' + nid">{{ title }}</nuxt-link>
        </h4>
        <div class="footer">
          <div class="author">
            by <nuxt-link :to="'/user/' + uid" class="">{{ author }}</nuxt-link>,
            <no-ssr>
              <time-ago :refresh="60" :datetime="createdTimeStamp * 1000" long>{{ createdTimeStamp }}</time-ago>
            </no-ssr>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  // import TimeAgo from 'vue2-timeago'
  export default {
    name: 'RecentContentTeaser',
    props: ['title', 'picture', 'createdTimeStamp', 'nid', 'topic', 'uid', 'author', 'metaDesc', 'withImg'],
    // components: {
    //   TimeAgo,
    // },
    methods: {
      //to display first 100 chars of body
      body_minimizer(str){
        if(str)
          return str.replace(/<(?:.|\n)*?>/gm, '').slice(0,100);
        else
          return ''
      }
    }
  }
</script>

<style scoped lang="scss">
  .md-headline {
    padding: 35px 0;
    margin: 0 !important;
    position: relative;
    text-align: left;
    &:before {
      content: "";
      width: 100%;
      height: 1px;
      background: #ddd;
      position: absolute;
      top: 53px;
    }
    h5{
      margin: 0;
      display: inline-block;
      background: #fafafa;
      z-index: 1;
      position: relative;
      padding-right: 15px;
    }
  }
  .author a:not(.v_time_ago_a) {
    font-weight: bold;
    color: #3c4858 !important;
  }
  .author {
    margin-left: 2px !important;
    font-size: 14px !important;
    float: none !important;
  }
  .v_time_ago_a:hover {
    color: #3c4858;
    cursor: default;
  }
  h3 {
    font-size: 1.825em;
    line-height: 1.4em;
    margin: 20px 0 10px;
  }
  .card-blog {
    margin-top: 30px;
  }
  .card-blog .card-title {
    margin-top: 5px;
  }
  .card {
    display: inline-block;
    position: relative;
    width: 100%;
    margin-bottom: 30px;
    border-radius: 6px;
    color: rgba(0,0,0, 0.87);
    background: #fff;
    box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
    text-align: left;
    transition: .4s;
    &:hover {
      box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
    }
  }
  .card .card-image {
    height: 60%;
    position: relative;
    z-index: 1;
    margin-left: 15px;
    margin-right: 15px;
    margin-top: -30px;
    border-radius: 6px;
    &:hover .img:before {
      background: rgba(49,27,146,0.6)
    }
  }
  .card .card-image .img {
    width: 100%;
    border-radius: 6px;
    box-shadow: 0 5px 15px -8px rgba(0, 0, 0, 0.24), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
    padding-bottom: 60%;
    background-size: cover;
    background-position: center;
    &:before {
      content: '';
      position: absolute;
      left: 0;
      right: 0;
      top: 0;
      bottom: 0;
      border-radius: 6px;
      display: block;
      background: rgba(49,27,146,0.3);
      transition: 0.5s;
    }
  }
  .card .card-image .card-title {
    position: absolute;
    bottom: 15px;
    left: 15px;
    right: 15px;
    color: #fff;
    font-size: 1.1em;
    text-shadow: 0 2px 5px rgba(33, 33, 33, 1);
  }
  .card .card-image .colored-shadow {
    transform: scale(0.94);
    top: 12px;
    filter: blur(12px);
    position: absolute;
    width: 100%;
    height: 100%;
    background-size: cover;
    z-index: -1;
    transition: opacity .45s;
    opacity: 0;
  }
  .card-title {
    font-weight: 700;
    font-family: "Roboto Slab", "Times New Roman", serif;
  }
  .card-title a:hover {
    color: #3C4858;
  }
  .card-title a {
    color: #3C4858;
    font-weight: 700;
    text-decoration: none;
  }
  .card .card-content {
    padding: 15px 30px;
  }
  .text-danger {
    color: #f44336;
    cursor: default;
    font-size: 12px;
  }
  h6, .h6 {
    font-size: 0.9em;
    text-transform: uppercase;
    text-align: left;
  }
  .h4, .h5, .h6, h4, h5, h6 {
    margin-top: 10px;
    margin-bottom: 10px;
  }
</style>
