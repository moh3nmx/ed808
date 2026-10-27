<!-- Done -->
<template>
  <div class="md-layout container tags">

    <md-card class="title md-layout-item md-size-100">

      <breadcrumb :links="[{url:'/tag/'+ tid +'/'+ name, title: 'Tag: '+ name}]"></breadcrumb>

      <md-card-content>
        <div class="top_header md-layout-item md-size-100">
          <h1>{{name}}</h1>

          <md-button class="follow" :class="bookmarked? 'bookmarked': ''" @click="follow()">
            <md-progress-spinner
              :md-diameter="15"
              :md-stroke="2"
              v-if="loading.bookmark"
              class="md-primary"
              md-mode="indeterminate">
            </md-progress-spinner>
            {{bookmarked? 'Following': 'Follow'}}
          </md-button>
        </div>
        <p v-if="image != ''" class="seperator"></p>
        <div>
          <img :src="image" v-if="image != ''" style="display: block;margin: auto;padding: 15px;">
          <article class="md-layout-item md-size-100"
                   v-if="description != ''"
                   v-html="convertDomain(description)"
                   :class="(windowWidth < 600 && collapse) ? 'mobile':''"
          ></article>
          <md-button
            v-if="windowWidth < 600 && description != ''"
            @click="collapse = !collapse"
            v-html="collapse? 'More':'Less'"
            class="show-more"
            :class="collapse? 'More':'Less'"></md-button>
        </div>
      </md-card-content>
    </md-card>

    <div class="md-headline" v-if="followers.length > 0">
      <h5>Users Following this Tag</h5>
    </div>
    <md-card class="title md-layout-item md-size-100" v-if="followers.length > 0">
      <carousel
        :per-page="6"
        :navigationEnabled="true"
        navigationNextLabel="<i class='mdi mdi-chevron-right'></i>"
        navigationPrevLabel="<i class='mdi mdi-chevron-left'></i>"
        :paginationEnabled="false"
      >
        <slide
          v-for="follower in followers"
          :key="follower.uid"
        >
          <user-teaser
            teaserType = "tag-follower"
            :uid="follower.uid"
            :name="follower.full_name"
            :user_id="follower.name"
            :picture="follower.picture"
            :following="follower.user_follow"
          >
          </user-teaser>
        </slide>
      </carousel>
    </md-card>

    <div class="md-size-100 md-layout-item">
      <NodeList
        :filterEnabled="false"
        :router="'allContents'"
        :tag="this.$route.params.tid"
        :component="true"
        class="md-card md-theme-default md-layout-item md-size-80 md-medium-size-100"
      />
    </div>

  </div>
</template>

<script>

  import axios from '@/node_modules/axios'
  import NodeList from '@/components/allContents/NodeList'
  import Breadcrumb from '@/components/fields/Breadcrumb'
  import UserTeaser from "@/components/fields/UserTeaser"
  import { cookie } from '@/components/mixins/cookie.js'

  export default {
    name: 'Tags',
    scrollToTop: true,
    components: {
      NodeList,
      Breadcrumb,
      UserTeaser
    },
    mixins:[cookie],
    data(){
      return {
        tid: this.$route.params.tid,
        name: this.$route.params.tname,
        image: '',
        description: '',
        loading: {page: true, bookmark: false},
        errors:'',
        showError:false,
        bookmarked: false,
        windowWidth: 1366,
        collapse: true,
        metaDescription:'',
        followers: [],
        window: {
          width: 0,
          height: 0
        }
      }
    },
    async asyncData({req, res, query, params, error}) {
      try {
        const {data} = await axios.get('https://temp.ed808.com/api/latin/tag/' + params.tid ,{
          withCredentials:true,
          headers: {
            'Cookie': req && req.headers && req.headers.cookie ? req.headers.cookie : '',
            'Content-type': 'application/json',
          }
        })
        if (data.tid) {
          return {
            name: data.name,
            image: data.image != null ? data.image : '',
            description: data.description != null ? data.description : '',
            loading: {page: false, bookmark: false},
            bookmarked: data.user_follow,
            collapse: true,
            metaDescription: data.meta_description != null ? data.meta_description : ''
          }
        }
      } catch (e) {
        if (e.response.hasOwnProperty('status')) {
          error({ statusCode: e.response.status, message: e.response.statusText })
        }
      }
    },
    computed: {
      perPage: function(){
        let perPage = 1
        if(this.window.width < 600) {
          perPage = 1
        }else if(this.window.width >= 600 && this.window.width < 1200) {
          perPage = 2
        }
        return perPage
      }
    },
    mounted(){
      if(this.name != this.replaceUrlSpace(this.$route.params.tname, true)){
            this.$router.replace({path:`/tag/${this.$route.params.tid}/${this.replaceUrlSpace(this.name)}`})
          }
      this.$nextTick(() => {
        window.addEventListener('resize', () => {
          this.windowWidth = window.innerWidth
        });
      })
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials  = true;
      axios.get('https://temp.ed808.com/api/latin/tag/' + this.tid + '/follower?parameter[page]=0&parameter[limits]=20')
        .then((data) => {
          this.followers = data.data.followers;
          console.log(data.data)
        })
    },
    methods:{
      // replace domains with diffrent port
      convertDomain(value){
        String.prototype.replaceAll = function(search, replacement) {
          let target = this;
          return target.replace(new RegExp(search, 'g'), replacement);
        };
        return value != null ? value.replaceAll('href="http://api.ed808.com', 'href="https://ed808.com')
          .replaceAll('href="https://temp.ed808.com/api', 'href="https://ed808.com')
          .replaceAll('href="http://ed808.com:92', 'href="https://ed808.com')
          .replaceAll('="/sites', '="https://temp.ed808.com/api/sites')
          .replaceAll('="/node', '="https://ed808.com/node') : ''
      },
      replaceUrlSpace(value, reverse){
        if(reverse == undefined) reverse = false
        String.prototype.replaceAll = function(search, replacement) {
          return this.replace(new RegExp(search, 'g'), replacement)
        };
        return value != null ? (reverse==false ? value.replaceAll(' ', '-'): value.replaceAll('-', ' ')) : ''
      },
      // Follow Tag contents
      async follow(){
        this.loading.bookmark = true
        if(this.getCookie("token") != null){
          axios.defaults.crossDomain = true;
          axios.defaults.withCredentials  = true;
          await axios.post('https://temp.ed808.com/api/latin/tag/' + this.tid + '/follow',
            {
              action : this.bookmarked? "unfollow": "follow",
            },
            {
              headers: {
                'Content-type': 'application/json',
                'X-CSRF-Token' : this.getCookie("token")
              }
            })
            .then((data) => {
              this.loading.bookmark = false
              this.bookmarked = !this.bookmarked
            })
        } else{
          this.loading.bookmark = false
          this.$router.push(`/login?callback=/tag/${this.tid}/${this.replaceUrlSpace(this.name)}`)
        }
      },
    },
    head(){
      return{
        links: [
          //these three line doesn't work
          { rel: 'canonical', href: 'https://ed808.com/tag/' + this.$route.params.tid + '/' + this.$route.params.tname},
          { rel: 'alternate', href: 'https://ed808.com/tag/' + this.$route.params.tid + '/' + this.$route.params.tname, hreflang:'en'},
          { rel: 'shortlink', href: 'https://ed808.com/tag/' + this.$route.params.tid + '/' + this.$route.params.tname}
        ],
        title: this.name,
        meta: [
          { name: 'description', content: this.metaDescription, hid:'description'},

          // OpenGraph data (Most widely used)
          {property: 'og:type', content: 'article', hid: 'og:type'},
          {property: 'og:title', content: this.name, hid: 'og:title'},
          {property: 'og:url', content: 'https://ed808.com/tag/' + this.$route.params.tid + '/' + this.$route.params.tname , hid: 'og:url'},
          {property: 'og:image', content: this.image, hid: 'og:image'},
          {property: 'og:description', content: this.metaDescription, hid:'og:description'},

          // Twitter card
          {name: 'twitter:title', content: this.name, hid: 'twitter:title'},
          {name: 'twitter:image:src', content: this.image, hid: 'twitter:image:src'},
          {name: 'twitter:description', content: this.metaDescription, hid:'twitter:description'},

          // Google / Schema.org markup:
          {itemprop: 'name', content: this.name, hid: 'name'},
          {itemprop: 'image', content: this.image, hid: 'image'},
          {itemprop: 'description', content: this.metaDescription, hid:'description'}
        ]
      }
    }
  }
</script>

<style lang="scss" scoped>
  @import '@/assets/scss/vars.scss';
  .md-headline{
    padding: 35px 0 0 0;
    margin: 0 !important;
    position: relative;
    text-align: left;
    h5{
      margin: 0;
      display: inline-block;
      background: #fafafa;
      z-index: 1;
      position: relative;
      padding-left: 15px;
      font-family: roboto slab;
      color: #3c4858;
    }
  }
  .tags {
    text-align: left;
    > .md-size-100{
      margin: 15px auto !important;
      max-width: calc(100% - 30px);
      min-width: inherit;
      &.md-card{padding: 15px !important;}
      @media #{$x840}{
        padding: 0px;
        margin: 0 !important;
        max-width: 100% !important;
        box-shadow: none;
      }
    }
    .title .md-card-content{
      border-top: solid #eee;
      border-width: 1px 0;
      @media #{$x840}{padding: 0;}
      .seperator{
        border-top: 1px solid rgb(238, 238, 238);
        margin-top: 0;
        @media #{$m840}{
          width: calc(100% + 30px);
          margin-left: -15px;
          margin-top: 15px;
        }
      }
    }
  }

  .VueCarousel {
    width: 100%;
  }
  .VueCarousel-inner{
    @media screen and (max-width: 600px) {
      flex-basis: auto !important;

    }
  }
  .VueCarousel-slide{
    padding: 10px;
    min-width: 200px;
    text-align: center;
    flex-shrink: unset;
    transition: 0.5s;
    @media screen and (max-width: 992px){
      width: 50% !important;
    }
    @media screen and (max-width: 600px) {
      width: 100% !important;
    }
    &:hover {
      transform: scale(1.1);
    }
  }
  button.VueCarousel-navigation-button{
    box-shadow: 0 3px 1px -2px rgba(0,0,0,.2), 0 2px 2px 0 rgba(0,0,0,.14), 0 1px 5px 0 rgba(0,0,0,.12);
    border-radius: 50%;
    width: 40px;
    height: 40px;
    background-color:white !important;
    font-family: Material Icons!important;
    font-size: 30px;
    text-align: center;
    line-height: 30px;
    padding: 0 !important;
    color:#92278f !important;
  }
  article {
    padding: 15px;
    font-size: 1.1em;
    line-height: 1.75em;
    transition: all .5s ease;
    max-height: 1000vh;
    overflow: hidden;
    &.mobile {
      max-height: 50vh;
    }
  }
  .show-more {
    margin: -20px 0 0 0;
    background: linear-gradient(rgba(255, 255, 255, 0.5), white 50%);
    height: 70px;
    @include absolute(100%,'','',0,0);
    &.Less{
      margin-top: 0;
      height: 50px;
      position: relative;
    }
  }
  .top_header {
    @include flex('',center,space-between);
    min-height: 100px;
    .tags &{
      h1{
        line-height: 1.75em;
        margin: 0;
        &:before {
          margin: 0 10px 0 0;
          font-weight: normal;
        }
        @media #{$x600}{font-size: 1.5em;}
      }
    }
    .follow {
      width: 110px;
      border: 1px solid currentColor;
      border-radius: 30px;
      color: #2196f3 !important;
      height: 44px;
      cursor: pointer;
      transition: all 0.2s;
      @media #{$x600}{
        margin: 0;
        width: 80px;
        height: 38px;
      }
      &.bookmarked {
        color: #fff !important;
        background: #2196f3;
      }
      .md-progress-spinner{
        vertical-align: middle;
        position: absolute;
        left: -25px;
        circle {stroke: #2196F3 !important;}
      }
    }
  }
  .tags .md-layout-item.contents{
    margin: 0px auto 50px !important;
    @media #{$x1024}{
      min-width: 100%;
      max-width: 100%;
      -webkit-box-flex: 1;
      flex: 1 1 100%;
    }
    @media #{$x840}{
      margin-bottom: 0 !important;
      background: #fcfcfc;
      padding: 0 !important;
      > .md-layout{margin: 0 !important;}
      .control-box {
        padding: 25px;
        background: #f5f5f5;
        hr {display: none;}
      }
      .content-teaser{
        width: 94%;
        flex: 0 1 92%;
        margin: 4% auto;
        position: relative;
        .md-card {
          box-shadow: none !important;
          background: transparent !important;
        }
        &:not(:last-child):after {
          background: #ccc;
          @include absolute(45%, 1px, '','',-4vw);
        }
        .content-subheader {
          font-size: 16px;
          opacity: 0.7;
          @media #{$x400}{
            font-size: 12px;
            margin: 0;
          }
        }
      }
      .md-ripple {display: flex;}
      .md-card-media {
        width: 26%;
        &:before {padding-top: 67%;}
        img {width: 100% !important;}
      }
      .md-card-header {
        width: 74%;
        padding: 0 1% 0 5% !important;
        margin: 0 !important;
        @include flex(column-reverse, '', space-between);
        @media #{$x400}{
          .md-title.content-title {
            font-size: 15px !important;
            line-height: 1.75em;
            max-height: 3.5em !important;
            margin-bottom: 5px;
          }
        }
      }
    }
  }
  .md-layout.container.tags{
    width: 100%;
  }

  .md-card-content img, article {
    /*padding: 0 !important;*/
    padding: 10px;
    @media screen and (max-width: 992px){
      width: 50% !important;
    }
    @media screen and (max-width: 600px) {
      width: 100% !important;
    }
  }

</style>
