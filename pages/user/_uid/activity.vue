<template>
  <!-- bookmarked node + clap node + liked comment + author comment +  -->
  <div id="activity">
    <md-tabs :md-swipeable="true">
      <md-tab id="tab-home" md-label="Stories" md-icon="subject">
        <div v-if="!sameUser && count == 0" class="text-center" style="width: 100%">No stories.</div>

        <div class="user-articles md-layout" style="position: relative;margin: 0 -1%;">

          <!--@Todo: Handle Types of Content and Topics and Category-->
          <add-post
            v-if="sameUser"
            :uploadedFiles.sync="uploadedFiles"
            viewMode="minimal"
            @updatePosts="getUserPosts()"
            @deleteUploaded="deleteImage"
            :tags="allTags"
          />

          <div class="posts">
            <div class="spinner-loading" v-if="spinner_loading">
              <md-progress-spinner :md-diameter="100" :md-stroke="5" md-mode="indeterminate"></md-progress-spinner>
            </div>
            <div class="description text-center" v-if="count == 0 && sameUser">
              No stories!
              <br>
              <span>
                Share your experience with your connections by writing stories.
              </span>
            </div>

            <div class="md-layout" v-else>
              <div class="md-layout-item md-size-50" v-for="post in posts" :key="post.nid">
                <div class="md-card md-card-blog md-theme-default">
                  <md-badge class="md-square" md-content="Unpublished" v-if="post.status == 0" style="top: 5px;right: 5px;" />
                  <div class="md-card-header md-card-header-image" v-if="post.picture">
                    <nuxt-link :to="'/node/'+ post.nid">
                      <div class="img" :style="`background-image: url(${post.picture})`"></div>
                    </nuxt-link>
                    <div data-v-c79ac4a2="" class="colored-shadow" :style="'background-image: url('+ post.picture +');opacity: 1;'"></div>
                  </div>
                  <div class="md-card-content">
                    <h6 class="card-category text-info">{{post.topic_name}}</h6>
                    <h4 class="card-title">
                      <nuxt-link :to="'/node/'+ post.nid">{{post.title}}</nuxt-link>
                    </h4>
                    <p v-html="body_minimizer(post.body,post.nid)" class="card-description">
                    </p>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </md-tab>
      <md-tab id="tab-bookmarks" md-label="Bookmark" md-icon="turned_in" v-if="sameUser">
        <div class="bookmarked_node md-layout" style="position: relative;margin: 0 -1%;">
          <div v-if="bookmarked_node.hasOwnProperty('contents') && bookmarked_node.contents.length == 0">No post is bookmarked!</div>
          <NodeTeaser v-for="post in bookmarked_node.contents" :key="post.nid" :title="post.title" :pic="post.picture" :date="post.created" :nid="post.nid" />
        </div>
      </md-tab>
      <md-tab id="tab-pages" md-label="clap" md-icon="stars" v-if="sameUser">
        <div v-if="clap_node.hasOwnProperty('contents') && clap_node.contents.length == 0">You have clapped no post yet!</div>
        <div class="clap_node md-layout" style="position: relative;margin: 0 -1%;">
          <NodeTeaser v-for="post in clap_node.contents" :key="post.nid" :title="post.title" :pic="post.picture" :date="post.created" :nid="post.nid" />
        </div>
      </md-tab>
      <md-tab id="tab-tags" md-label="Tags" md-icon="/images/sharp.svg" v-if="sameUser">
        <div class="md-layout">
          <tag-teaser
            v-for="tag in tags"
            :key="tag.tid"
            :tag_name="tag.name"
            :tag_image="tag.image"
            :tid="tag.tid"
            :tag_description="tag.meta_description"
            :contents="tag.count_contents"
            :user_follow="tag.user_follow"
            :follow_counts="tag.follower_count"
            :user_tags="true"
          />
        </div>

      </md-tab>
      <md-tab id="tab-comments" md-label="Comments" md-icon="mode_comment">
        <div v-if="comments.length == 0">No comments.</div>
        <div class="user-articles" v-else>
          <div class="posts" >
            <div class="md-layout md-gutter user-comments">
              <div class="md-layout-item md-size-25 md-small-size-100 md-medium-size-50" v-for="comment in comments" :key="comment.cid">
                <div class="md-card md-card-blog md-theme-default">
                  <md-badge class="md-square" md-content="Approved" v-if="comment.status == 0" style="top: 5px;right: 5px;" />
                  <div class="md-card-header md-card-header-image" v-if="comment.image">
                    <nuxt-link :to="'/node/'+ comment.nid + '#comments'">
                      <div class="img" :style="`background-image: url(${comment.image})`"></div>
                    </nuxt-link>
                    <div data-v-c79ac4a2="" class="colored-shadow" :style="'background-image: url('+ comment.image +');opacity: 1;'"></div>
                  </div>
                  <div class="md-card-content">
                    <h6 class="card-category text-info">{{comment.created}}</h6>
                    <!--<h4 class="card-title">-->
                      <!--<nuxt-link :to="'/node/'+ comment.nid + '#comments'">{{comment.title}}</nuxt-link>-->
                    <!--</h4>-->
                    <p class="card-description">
                      "{{ body_minimizer(comment.body,null,true) }}"
                    </p>

                    <div>
                      <md-badge :md-content="comment.likes" class="md-accent">
                        <md-button class="md-icon-button" disabled>
                          <md-icon>favorite</md-icon>
                        </md-button>
                      </md-badge>
                      &nbsp;
                      <md-badge :md-content="comment.replies" class="md-accent">
                        <md-button class="md-icon-button" disabled>
                          <md-icon>reply</md-icon>
                        </md-button>
                      </md-badge>
                    </div>

                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </md-tab>
    </md-tabs>

  </div>
</template>

<script>
  import axios from "@/node_modules/axios"
  import addPost from "@/components/user/addPost";
  import NodeTeaser from "@/components/allContents/NodeTeaser";
  import { cookie } from "@/components/mixins/cookie";
  import TagTeaser from "@/components/fields/TagTeaser"

  export default {
    name: "Activities",
    components: {
      NodeTeaser,
      addPost,
      TagTeaser
    },
    mixins: [cookie],
    layout: 'userpanel',
    scrollToTop: true,
    head(){
      return {
        links: [
          { rel: "canonical", href: "https://ed808.com/user/"+ this.$route.params.uid + '/activity' },
          { rel: "alternate", href: "https://ed808.com/user/"+ this.$route.params.uid + '/activity', hreflang: "en" },
          { rel: "shortlink", href: "https://ed808.com/user/"+ this.$route.params.uid + '/activity' }
        ],
        title:'User Activities',
        meta: [
          // OpenGraph data (Most widely used)
          { property: "og:title", content: 'User Activities', hid: "og:title" },
          {
            property: "og:url",
            content: "https://ed808.com/user/"+ this.$route.params.uid + '/activity',
            hid: "og:url"
          },

          // Twitter card
          { name: "twitter:title", content: 'User Activities', hid: "twitter:title" },


          // Google / Schema.org markup:
          { itemprop: "name", content: 'User Activities', hid: "name" },
        ]
      };
    },
    data(){
      return {
        uid: this.$route.params.uid,
        bookmarked_node:[],
        clap_node:[],
        tags: [],
        allTags: [],
        spinner_loading:true,
        comments: [],
        page:'',
        posts:[],
        uploadedFiles: [],
        brief:{},
        count:0,
        posts_copy:{},
        hasError: false,
        errorText: null,
        sameUser: this.$store.getters.getUid == this.$route.params.uid
      }
    },
    beforeRouteLeave: function(from, to, next){
      if (this.uploadedFiles.length > 0)
        this.deleteImage(this.uploadedFiles)
      next()
    },
    created() {

    },
    mounted(){
      this.bookmarkedNode()
      this.clappedNode()
      this.getUserPosts()
      this.getUserComments()
      this.getAllTags()
    },
    methods:{
      syncUploadedFiles(obj){

      },
      deleteImage(files){
        if (files.length > 0) {
          axios.defaults.withCredentials = true
          axios({
            url: 'https://ed808.com:92/latin/file/delete_image', // server url. If the url is empty then the base64 returns
            method: 'POST',
            data : {
              names: files
            },
            headers: {
              "Content-Type": "multipart/form-data",
              "X-CSRF-Token": this.getCookie("token")
            }
          }).then((data)=>{
            console.log(data)
            let uploaded = this.uploadedFiles
            Object.keys(data.data).forEach(function (el) {
                let index = uploaded.indexOf(el);
                if (index > -1) {
                  uploaded.splice(index, 1);
                }
              })
            this.uploadedFiles = uploaded
            console.log(uploaded)
          })
        }
      },
      body_minimizer(str,nid,meta=false){
        if(str && !meta) return str.replace(/<(?:.|\n)*?>/gm, ' ').slice(0,300).concat('<a href="/node/'+nid+'" style="cursor: pointer;">... Read More</a>')
        else if(str && meta) return str.replace(/<(?:.|\n)*?>/gm, ' ')
        else return ''
      },
      // Get bookmarked Contents
      bookmarkedNode(){
        if(this.sameUser) {
          axios.defaults.crossDomain = true
          axios.defaults.withCredentials  = true
          axios.get('https://ed808.com:92/latin/user/'+ this.uid + '/bookmark?parameter[page]='+ this.page +'&parameter[limits]=' + 10 ,
            {
              headers: {
                'Content-type': 'application/json'
              }
            })
            .then((data) => {
              this.bookmarked_node = data.data
              this.spinner_loading = false
            })
            .catch(e => {
              this.errors = e.response.data
            })
        }
      },
      // Get Clapped Nodes
      clappedNode(){
        if(this.sameUser) {
          axios.defaults.crossDomain = true
          axios.defaults.withCredentials  = true
          axios.get('https://ed808.com:92/latin/user/'+ this.uid + '/favorite_content?parameter[page]='+ this.page +'&parameter[limits]=' + 10 ,
            {
              headers: {
                'Content-type': 'application/json'
              }
            })
            .then((data) => {
              this.clap_node = data.data
              this.spinner_loading = false
            })
            .catch(e => {
              if(e.hasOwnProperty('response')){
                if(e.response.hasOwnProperty('data'))
                  this.errorText = e.response.data
                else{
                  this.errorText = e.response
                }
              }
              else{
                this.errorText = e
              }
              this.hasError = true
            })
        }
      },
      getUserPosts(){
        axios.defaults.crossDomain = true
        axios.defaults.withCredentials  = true
        axios.get('https://ed808.com:92/latin/user/'+ this.uid + '/posts?parameter[page]='+ this.page +'&parameter[limits]=' + 10 + '&parameter[publish]=0',
          {
            headers: {
              'Content-type': 'application/json'
            }
          })
          .then((data) => {
            this.posts = data.data.posts
            this.count = data.data.count
            this.brief = data.data.brief
            this.spinner_loading = false
            this.posts_copy = Object.assign({}, this.posts)
          })
          .catch(e => {
            if(e.hasOwnProperty('response')){
              if(e.response.hasOwnProperty('data'))
                this.errorText = e.response.data
              else{
                this.errorText = e.response
              }
            }
            else{
              this.errorText = e
            }
            this.hasError = true
          })
      },
      getUserComments(){
        axios.defaults.crossDomain = true
        axios.defaults.withCredentials  = true
        axios.get('https://ed808.com:92/latin/user/'+ this.uid + '/wrote_comment?parameter[page]=0&parameter[limits]=10',
          {
            headers: {
              'Content-type': 'application/json'
            }
          })
          .then((data) => {
            this.comments = data.data.comments
            // this.count = data.data.count
            // this.brief = data.data.brief
            this.spinner_loading = false
            this.comments = Object.assign({}, this.comments)
          })
          .catch(e => {
            if(e.hasOwnProperty('response')){
              if(e.response.hasOwnProperty('data'))
                this.errorText = e.response.data
              else{
                this.errorText = e.response
              }
            }
            else{
              this.errorText = e
            }
            this.hasError = true
          })
      },
      getAllTags(){
        axios.defaults.crossDomain = true
        axios.defaults.withCredentials  = true
        axios.get('https://ed808.com:92/latin/tag?parameter[page]=0&parameter[limits]=10000')
          .then(data=>{
            this.allTags= data.data.tags
            let tags = []
            data.data.tags.forEach(function (tag) {
              if(tag.user_follow == true) {
                tags.push(tag)
              }
            })
            this.tags = tags
          })
      }
    },

  }
</script>

<style lang="scss">
  @import '@/assets/scss/vars.scss';
  #activity{
    .md-tabs.md-theme-default .md-tabs-navigation .md-button {
      width: 100px;
      height: 104px;
      border-radius: 5px;
      margin: 5px;
      &.md-active {
        background-color: #4caf50 !important;
        -webkit-box-shadow: 0 12px 20px -10px rgba(76, 175, 80, .28), 0 4px 20px 0 rgba(0, 0, 0, .12), 0 7px 8px -5px rgba(76, 175, 80, .2);
        box-shadow: 0 12px 20px -10px rgba(76, 175, 80, .28), 0 4px 20px 0 rgba(0, 0, 0, .12), 0 7px 8px -5px rgba(76, 175, 80, .2);

        * {
          fill: #fff;
          color: white !important;
        }
      }
    }
    .md-tabs.md-theme-default .md-tabs-navigation .md-button:not(.md-active):hover {
      background-color: hsla(0, 0%, 78%, .2) !important;
    }
    .md-tabs-navigation .md-icon-label .md-tab-icon+.md-tab-label {
      margin-top: 15px;
    }
    .md-tabs-indicator{
      display: none;
    }

    .user-articles {
      .posts{
        min-height: 100px;
        justify-content: center;
        align-items: center;
        flex-direction: column;
        width: 100%;
        .md-layout-item {
          padding: 10px;
          &.md-size-50 {
            @media #{$x840} {
              min-width: 100%;
              max-width: 100%;
            }
          }
        }

        .md-card.md-card-blog.md-theme-default {
          width: 100% ;
        }

        .card-title, .card-title a, .card-title a:focus, .card-title a:hover, .comment .comment-heading, .comment .comment-heading a, .comment .comment-heading a:focus, .comment .comment-heading a:hover, .footer-big h4, .footer-big h4 a, .footer-big h4 a:focus, .footer-big h4 a:hover, .footer-big h5, .footer-big h5 a, .footer-big h5 a:focus, .footer-big h5 a:hover, .footer-brand, .footer-brand a, .footer-brand a:focus, .footer-brand a:hover, .info-title, .info-title a, .info-title a:focus, .info-title a:hover, .title, .title a, .title a:focus, .title a:hover {
          color: #3c4858!important;
          text-decoration: none;
        }
        .card-title, .comment .comment-heading, .footer-big h4, .footer-big h5, .footer-brand, .info-title, .title {
          font-family: Roboto Slab,Times New Roman,serif;
        }
        h4{
          font-size: 18px;
          margin:10px 0;
          font-weight: normal;
          text-align: left;
        }
        h6 {
          /*font-size: .75rem;*/
          font-size: 12px;
          text-transform: uppercase;
          font-weight: 500;
          margin:10px 0;
        }
        .card-description, .description, .footer-big p {
          color: #999;
          font-size: 14px;
          margin: 0 0 10px;
          text-align: left;
          &.text-center {
            text-align: center;
          }
        }
        .md-card.row-space .header {
          padding: 15px 20px 0
        }

        .md-card .card-title {
          margin-top: .625rem
        }

        .md-card .card-category-social .fa,.md-card .card-category-social .fab,.md-card .card-category-social .far,.md-card .card-category-social .fas,.md-card .card-category-social .md-icon {
          font-size: 24px;
          position: relative;
          margin-top: -4px;
          top: 2px;
          margin-right: 5px
        }

        .md-card .card-category-social .md-icon {
          margin-top: -7px;
          color: inherit
        }

        .md-card .card-title:last-child {
          margin-bottom: 0
        }

        .md-card[class*=bg-] .author a,.md-card[class*=bg-] .card-category:not([class*=text-]),.md-card[class*=bg-] .card-description,.md-card[class*=bg-] .card-title small,.md-card[class*=bg-] .stats,.md-card[class*=bg-] .stats .md-icon:not(.md-icon-button),.md-card [class*=md-card-header-] .author a,.md-card [class*=md-card-header-] .card-category,.md-card [class*=md-card-header-] .card-description,.md-card [class*=md-card-header-] .stats {
          color: hsla(0,0%,100%,.8)!important
        }

        .md-card[class*=bg-] .card-title,.md-card[class*=bg-] .card-title a,.md-card[class*=bg-] .icon:not([class*=icon-]) i {
          color: #fff!important;
          fill: #fff;
        }

        .md-card[class*=bg-] .icon:not([class*=icon-]) i,.md-card [class*=md-card-header-] .icon i {
          border-color: hsla(0,0%,100%,.25)!important
        }

        .md-card.md-card-form-horizontal .md-field {
          margin: 0;
          padding-top: 0
        }

        .md-card.md-card-form-horizontal .md-card-content {
          padding-left: 15px;
          padding-right: 15px
        }

        .md-card.md-card-form-horizontal .md-button {
          margin: 0
        }

        .md-card .title {
          margin-top: 0;
          margin-bottom: 5px
        }

        .md-card .card-image {
          height: 60%;
          position: relative;
          overflow: hidden;
          margin-left: 15px;
          margin-right: 15px;
          margin-top: -30px;
          border-radius: 6px
        }

        .md-card .card-image img {
          width: 100%;
          height: 200px;
          border-radius: 6px;
          pointer-events: none
        }

        .md-card .card-image .card-title {
          position: absolute;
          bottom: 15px;
          left: 15px;
          color: #fff;
          font-size: 1.125rem;
          text-shadow: 0 2px 5px rgba(33,33,33,.5)
        }

        .md-card .category:not([class*=text-]) {
          color: #999
        }

        .md-card .md-card-content {
          padding: .9375rem 1.875rem;
          line-height: inherit
        }

        .md-card .md-card-content .category {
          margin: 0
        }

        .md-card .md-card-header {
          margin: -40px 20px 0;
          border-radius: 3px;
          padding: 0;
          background-color: #999
        }

        .md-card .md-card-header .card-title,.md-card .md-card-header .title {
          color: #fff!important
        }

        .md-card .md-card-header .category {
          margin: 0;
          color: hsla(0,0%,100%,.62)
        }

        .md-card .md-card-header a {
          color: #fff!important
        }

        .md-card .md-card-header.card-chart {
          padding: 0;
          min-height: 160px
        }

        .md-card .md-card-header.card-chart+.content h4 {
          margin-top: 0
        }

        .md-card .md-card-header .ct-label {
          color: hsla(0,0%,100%,.7)
        }

        .md-card .md-card-header .ct-grid {
          stroke: hsla(0,0%,100%,.2)
        }

        .md-card .md-card-header .ct-series-a .ct-bar,.md-card .md-card-header .ct-series-a .ct-line,.md-card .md-card-header .ct-series-a .ct-point,.md-card .md-card-header .ct-series-a .ct-slice-donut {
          stroke: hsla(0,0%,100%,.8)
        }

        .md-card .md-card-header .ct-series-a .ct-area,.md-card .md-card-header .ct-series-a .ct-slice-pie {
          fill: hsla(0,0%,100%,.4)
        }

        .md-card .md-card-header-image {
          position: relative;
          padding: 0;
          z-index: 1;
          margin-left: 15px;
          margin-right: 15px;
          margin-top: -30px;
          border-radius: 6px
        }

        .md-card .md-card-header-image a {
          display: block
        }

        .md-card .md-card-header-image .img {
          width: 100%;
          background-position: center;
          background-size: cover;
          padding-bottom: 60%;
          border-radius: 6px;
          pointer-events: none
        }

        .md-card .md-card-header-image:not([shadow-off=true]) img {
          -webkit-box-shadow: 0 5px 15px -8px rgba(0,0,0,.24),0 8px 10px -5px rgba(0,0,0,.2);
          box-shadow: 0 5px 15px -8px rgba(0,0,0,.24),0 8px 10px -5px rgba(0,0,0,.2)
        }

        .md-card .md-card-header-image .card-title {
          position: absolute;
          bottom: 15px;
          left: 15px;
          color: #fff!important;
          font-size: 1.125rem;
          text-shadow: 0 2px 5px rgba(33,33,33,.5)
        }

        .md-card .md-card-header-image .colored-shadow {
          -webkit-transform: scale(.94);
          transform: scale(.94);
          top: 12px;
          -webkit-filter: blur(12px);
          filter: blur(12px);
          position: absolute;
          width: 100%;
          height: 100%;
          background-size: cover;
          z-index: -1;
          -webkit-transition: opacity .45s;
          transition: opacity .45s;
          opacity: 0
        }

        .md-card .md-card-header-image[no-colored-shadow=true] {
          -webkit-box-shadow: none;
          box-shadow: none
        }

        .md-card .md-card-header-image[no-colored-shadow=true] .colored-shadow {
          display: none!important
        }

        .md-card .md-card-header-image:not([shadow-off=true])[shadow-normal=true] {
          -webkit-box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2);
          box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2)
        }

        .md-card .md-card-header-image:not([shadow-off=true])[shadow-normal=true] .colored-shadow {
          display: none!important
        }

        .md-card .chart-title {
          position: absolute;
          top: 25px;
          width: 100%;
          text-align: center
        }

        .md-card .chart-title h3 {
          margin: 0;
          color: #fff
        }

        .md-card .chart-title h6 {
          margin: 0;
          color: hsla(0,0%,100%,.4)
        }

        .md-card .md-card-actions {
          padding: 0 1.875rem .9375rem
        }

        .md-card .md-card-actions .content {
          display: block
        }

        .md-card .md-card-actions .author {
          color: #999
        }

        .md-card .md-card-actions .stats {
          display: -webkit-box;
          display: -ms-flexbox;
          display: flex;
          -webkit-box-align: center;
          -ms-flex-align: center;
          align-items: center;
          color: #999;
          margin-right: 3px;
          margin-left: 3px
        }

        .md-card .md-card-actions .stats .md-icon.md-theme-default.md-icon-font {
          position: relative;
          top: -1px;
          font-size: 18px!important;
          color: #999
        }

        .md-card .md-card-actions h6 {
          color: #999
        }

        .md-card img {
          width: 100%;
          height: auto
        }

        .md-card .category .md-icon {
          position: relative;
          top: 6px;
          line-height: 0
        }

        .md-card .category-social .fa {
          font-size: 24px;
          position: relative;
          margin-top: -4px;
          top: 2px;
          margin-right: 5px
        }

        .md-card .author .avatar {
          width: 30px;
          height: 30px;
          overflow: hidden;
          border-radius: 50%;
          margin-right: 5px
        }

        .md-card .author a {
          color: #3c4858;
          text-decoration: none
        }

        .md-card .author a .ripple-container {
          display: none
        }

        .md-card .table {
          margin-bottom: 0
        }

        .md-card .table tr:first-child td {
          border-top: none
        }

        .md-card.bg-primary,.md-card .md-card-header-primary,.md-card.md-card-rotate.bg-primary .back,.md-card.md-card-rotate.bg-primary .front,.md-card .md-tabs.md-primary .md-tabs-navigation,.md-card [data-background-color=purple] {
          background: linear-gradient(60deg,#ab47bc,#7b1fa2);
          -webkit-box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2);
          box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2);
          background-repeat: no-repeat
        }
        .md-card.bg-info,.md-card .md-card-header-info,.md-card.md-card-rotate.bg-info .back,.md-card.md-card-rotate.bg-info .front,.md-card .md-tabs.md-info .md-tabs-navigation,.md-card [data-background-color=blue] {
          background: linear-gradient(60deg,#26c6da,#0097a7);
          -webkit-box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2);
          box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2);
          background-repeat: no-repeat
        }

        .md-card.bg-success,.md-card .md-card-header-green,.md-card.md-card-rotate.bg-success .back,.md-card.md-card-rotate.bg-success .front,.md-card .md-tabs.md-success .md-tabs-navigation,.md-card [data-background-color=green] {
          background: linear-gradient(60deg,#66bb6a,#388e3c);
          -webkit-box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2);
          box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2);
          background-repeat: no-repeat
        }

        .md-card.bg-warning,.md-card .md-card-header-orange,.md-card.md-card-rotate.bg-warning .back,.md-card.md-card-rotate.bg-warning .front,.md-card .md-tabs.md-warning .md-tabs-navigation,.md-card [data-background-color=orange] {
          background: linear-gradient(60deg,#ffa726,#f57c00);
          -webkit-box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2);
          box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2);
          background-repeat: no-repeat
        }

        .md-card.bg-danger,.md-card .md-card-header-red,.md-card.md-card-rotate.bg-danger .back,.md-card.md-card-rotate.bg-danger .front,.md-card .md-tabs.md-danger .md-tabs-navigation,.md-card [data-background-color=red] {
          background: linear-gradient(60deg,#ef5350,#d32f2f);
          -webkit-box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2);
          box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2);
          background-repeat: no-repeat
        }

        .md-card.bg-rose,.md-card .md-card-header-rose,.md-card.md-card-rotate.bg-rose .back,.md-card.md-card-rotate.bg-rose .front,.md-card .md-tabs.md-rose .md-tabs-navigation,.md-card [data-background-color=rose] {
          background: linear-gradient(60deg,#ec407a,#c2185b);
          -webkit-box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2);
          box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2);
          background-repeat: no-repeat
        }

        .md-card [data-background-color],.md-card [data-background-color] a {
          color: #fff
        }

        .md-card-stats .title {
          margin: 0
        }

        .md-card-stats .title small {
          color: #999;
          font-size: 65%;
          line-height: 1;
          font-weight: 400
        }

        .md-card-stats .md-card-header {
          float: left;
          text-align: center
        }

        .md-card-stats .md-card-header i {
          font-size: 36px!important;
          line-height: 56px;
          width: 56px;
          height: 56px;
          color: #fff!important
        }

        .md-card-stats .md-card-content {
          text-align: right;
          padding-top: 10px
        }

        .md-card-blog {
          margin-top: 45px;
        }

        .card-nav-tabs .header-raised {
          margin-top: -30px;
        }

        .card-nav-tabs .nav-tabs {
          background: transparent;
          padding: 0
        }

        .card-nav-tabs .nav-tabs-title {
          float: left;
          padding: 10px 10px 10px 0;
          line-height: 24px
        }

        .md-card-plain {
          background: transparent!important;
          -webkit-box-shadow: none!important;
          box-shadow: none!important
        }

        .md-card-plain .md-card-header {
          background-color: transparent!important;
          margin-left: 0;
          margin-right: 0
        }

        .md-card-plain .md-card-actions,.md-card-plain .md-card-content {
          padding-left: 5px;
          padding-right: 5px
        }

        .md-card-plain .card-image {
          margin: 0;
          border-radius: 3px
        }

        .md-card-plain .card-image img {
          border-radius: 3px
        }

        .md-card-horizontal .md-card-header-image {
          margin-top: 0
        }

        .md-card-profile,.md-card-testimonial {
          margin-top: 30px
        }

        .md-card-profile:not(.md-card-horizontal),.md-card-testimonial:not(.md-card-horizontal) {
          text-align: center
        }

        .md-card-profile .btn-just-icon.btn-raised,.md-card-testimonial .btn-just-icon.btn-raised {
          margin-left: 6px;
          margin-right: 6px
        }

        .md-card-profile .md-card-avatar,.md-card-testimonial .md-card-avatar {
          max-width: 130px;
          max-height: 130px;
          margin: -50px auto 0;
          border-radius: 50%;
          overflow: hidden;
          -webkit-box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2);
          box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2)
        }

        .md-card-profile .md-card-avatar+.md-card-content,.md-card-testimonial .md-card-avatar+.md-card-content {
          margin-top: 15px
        }

        .md-card-profile.flex-direction-row .md-card-avatar,.md-card-profile.flex-direction-row .md-card-content,.md-card-profile.md-card-plain .card-avatar,.md-card-testimonial.flex-direction-row .md-card-avatar,.md-card-testimonial.flex-direction-row .md-card-content,.md-card-testimonial.md-card-plain .card-avatar {
          margin-top: 0
        }

        .md-card-profile.flex-direction-row .md-card-content,.md-card-testimonial.flex-direction-row .md-card-content {
          text-align: left;
          margin-left: 15px
        }

        .md-card-testimonial .md-card-header {
          background-color: #fff;
          margin-top: 0
        }

        .md-card-testimonial .card-description {
          font-style: italic;
          margin-bottom: 35px
        }

        .md-card-testimonial .md-card-actions {
          display: block
        }

        .md-card-testimonial .md-card-actions .md-card-avatar {
          margin-top: 10px;
          margin-bottom: -60px
        }

        .md-card-testimonial .md-card-avatar {
          max-width: 100px;
          max-height: 100px
        }

        .md-card-testimonial .icon {
          margin-top: 30px
        }

        .md-card-testimonial .icon.icon-black i {
          color: rgba(0,0,0,.87)
        }

        .md-card-testimonial .icon .md-icon {
          font-size: 40px!important
        }

        .md-card-contact .md-card-header {
          padding: 1rem 15px
        }

        .md-card-tabs {
          margin-top: 0
        }

        .md-card-tabs .md-list {
          -webkit-box-orient: vertical;
          -webkit-box-direction: normal;
          -ms-flex-flow: initial;
          flex-flow: initial;
          padding: 0 15px
        }

        .md-card-tabs.page-subcategories>.md-card-header .title {
          margin-top: 30px!important;
          margin-bottom: 25px!important;
          min-height: 32px
        }

        .md-card-tabs .tab-content {
          padding: 20px 0 9px
        }

        .md-card-tabs .md-list-item+.md-list-item {
          margin-left: 5px
        }

        .md-card-tabs .md-list .md-list-item .md-list-item-button {
          line-height: 24px;
          text-transform: uppercase;
          font-size: 12px;
          font-weight: 500;
          min-width: 100px;
          color: #3c4858!important;
          -webkit-transition: all .3s;
          transition: all .3s;
          border-radius: 30px;
          padding: 0
        }

        .md-card-tabs .md-list .md-list-item .md-list-item-button .md-icon {
          display: -webkit-box;
          display: -ms-flexbox;
          display: flex;
          width: auto;
          height: auto;
          margin: 0;
          font-size: 30px!important;
          padding: 15px 0;
          color: #555
        }

        .md-card-tabs .md-list .md-list-item .md-list-item-button .md-list-item-content {
          min-height: auto;
          padding: 10px 15px;
          -webkit-box-pack: center;
          -ms-flex-pack: center;
          justify-content: center;
          display: -webkit-box;
          display: -ms-flexbox;
          display: flex
        }

        .md-card-tabs .md-list .md-list-item .md-list-item-button:focus,.md-card-tabs .md-list .md-list-item .md-list-item-button:hover {
          background-color: hsla(0,0%,78%,.2)!important
        }

        .md-card-tabs .md-list .md-list-item:not(.active) .md-list-item-button .md-list-item-content {
          color: #555
        }

        .md-card-tabs .active.md-list-item .md-list-item-button {
          color: #fff!important
        }

        .md-card-tabs .active.md-list-item .md-list-item-button,.md-card-tabs .active.md-list-item .md-list-item-button:focus,.md-card-tabs .active.md-list-item .md-list-item-button:hover {
          background-color: #999!important;
          -webkit-box-shadow: 0 12px 20px -10px hsla(0,0%,60%,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px hsla(0,0%,60%,.2);
          box-shadow: 0 12px 20px -10px hsla(0,0%,60%,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px hsla(0,0%,60%,.2)
        }

        .md-card-tabs .active.md-list-item .md-list-item-button,.md-card-tabs .active.md-list-item .md-list-item-button .md-list-item-content .md-icon {
          color: #fff!important
        }

        .md-card-tabs .active.md-list-item.md-rose .md-list-item-button,.md-card-tabs .active.md-list-item.md-rose .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-rose .md-list-item-button:hover {
          background-color: #e91e63!important;
          -webkit-box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2);
          box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2)
        }

        .md-card-tabs .active.md-list-item.md-primary .md-list-item-button,.md-card-tabs .active.md-list-item.md-primary .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-primary .md-list-item-button:hover {
          background-color: #9c27b0!important;
          -webkit-box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2);
          box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2)
        }

        .md-card-tabs .active.md-list-item.md-warning .md-list-item-button,.md-card-tabs .active.md-list-item.md-warning .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-warning .md-list-item-button:hover {
          background-color: #ff9800!important;
          -webkit-box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2);
          box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2)
        }

        .md-card-tabs .active.md-list-item.md-info .md-list-item-button,.md-card-tabs .active.md-list-item.md-info .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-info .md-list-item-button:hover {
          background-color: #00bcd4!important;
          -webkit-box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2);
          box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2)
        }

        .md-card-tabs .active.md-list-item.md-success .md-list-item-button,.md-card-tabs .active.md-list-item.md-success .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-success .md-list-item-button:hover {
          background-color: #4caf50!important;
          -webkit-box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2);
          box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2)
        }

        .md-card-tabs .active.md-list-item.md-danger .md-list-item-button,.md-card-tabs .active.md-list-item.md-danger .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-danger .md-list-item-button:hover {
          background-color: #f44336!important;
          -webkit-box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2);
          box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2)
        }

        .md-card-tabs.md-card-plain .md-card-header,.md-card-tabs.md-card-plain .md-list {
          background-color: transparent!important
        }

        .md-card-tabs.md-card-plain .title {
          -webkit-box-pack: center;
          -ms-flex-pack: center;
          justify-content: center
        }

      }
    }
  }

  .md-tabs.md-theme-default .md-tabs-navigation {
    @media #{$x600} {
      display: flex;
      justify-content: center;
      flex-wrap: wrap;
      align-content: center;
      text-align: center;
      .md-button {
        width: 70px !important;
        height: 70px !important;
        border-radius: 5px !important;
        margin: 5px !important;
      }
    }
  }

</style>
