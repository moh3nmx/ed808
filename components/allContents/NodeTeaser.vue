<template>
  <md-card md-with-hover
           class="content-teaser"
           :class="[('content-'+ nid), DisplayMode]">
    <nuxt-link
      v-if="DisplayMode != 'medium'"
      :to="'/node/' + nid">
      <md-ripple>
        <md-card-media md-ratio="16:9">
          <img v-bind:src="pic | converturl" />
        </md-card-media>

        <md-card-header>
          <div class="md-subhead content-subheader">
            <div v-for="item in type" :key="item.tid" style="display: inline-block;">
              <md-icon style="font-size: 19px !important;">{{item.name | TypesToIcon}}</md-icon>
              {{item.name}}
            </div>
            <span style="float: right;">{{date}}</span>
          </div>

          <h3 class="md-title content-title">{{title}}</h3>

          <div class="body-val" v-if="DisplayMode == 'medium'">
            {{body_minimizer(body)}}
          </div>
        </md-card-header>

      </md-ripple>
    </nuxt-link>

    <!--medium DisplayMode-->
    <div v-if="DisplayMode == 'medium'" style="display: flex;align-items: center;">
      <md-card-media md-ratio="4:3">
        <nuxt-link :to="'/node/' + nid">
          <md-ripple>
            <img v-bind:src="pic | converturl" />
          </md-ripple>
        </nuxt-link>
      </md-card-media>

      <md-card-header>
        <nuxt-link :to="'/node/' + nid"><h3 class="md-title content-title">{{title}}</h3></nuxt-link>

        <p class="body-val">{{body_minimizer(body)}}</p>

        <p class="author">
          by <nuxt-link :to="'/user/' + uid">{{userName}}</nuxt-link>,
          <client-only>
            <time-ago :refresh="60" :datetime="createdTimestamp * 1000" long>{{createdTimestamp}}</time-ago>
          </client-only>
        </p>
      </md-card-header>
    </div>

  </md-card>
  

</template>

<script>
// import TimeAgo from 'vue2-timeago'

export default {
    name: 'NodeTeaser',
    props: ['title','pic','date','createdTimestamp','nid','type','body','uid','userName','DisplayMode'],
    // components: {
    //   TimeAgo
    // },
    filters: {
      year_detecter: function (value) {
          if (!value) return ''
          value = value.toString()
          return value.split('-')[0]
      },
      month_detecter: function (value) {
          if (!value) return ''
          value = value.toString()
          if(value.split('-')[1] == '01') return 'Jan'
          else if(value.split('-')[1] == '02') return 'Feb'
          else if(value.split('-')[1] == '03') return 'Mar'
          else if(value.split('-')[1] == '04') return 'Apr'
          else if(value.split('-')[1] == '05') return 'May'
          else if(value.split('-')[1] == '06') return 'Jun'
          else if(value.split('-')[1] == '07') return 'Jul'
          else if(value.split('-')[1] == '08') return 'Aug'
          else if(value.split('-')[1] == '09') return 'Sep'
          else if(value.split('-')[1] == '10') return 'Oct'
          else if(value.split('-')[1] == '11') return 'Nov'
          else if(value.split('-')[1] == '12') return 'Dec'
      },
      day_detecter: function (value) {
          if (!value) return ''
          value = value.toString()
          return value.split('-')[2]
      },
      converturl: function(value){
          if (!value || value == "") return '/images/default.jpg'
          return value
      },
      priceFormat: function(value){
          return parseFloat(value, 10).toFixed(0).replace(/(\d)(?=(\d{3})+(?!\d))/g, "$1,").toString()//Math.round()
      },
      TypesToIcon: str => {
        switch(str){
          case 'video':
              return 'play_circle_outline'
          break;
          case 'podcast':
              return 'music_note'
          break;
          case 'picture':
              return 'image'
          break;
          case 'ebook':
              return 'book'
          break;
          case 'article':
              return 'description'
          break;
          case 'news':
            return 'web'
            break;
          case 'case study':
            return 'all_inclusive'
          break;
          case 'Products and Services':
            return 'shopping_basket'
          break;
          default:
          return str
        }
      },
    },
    methods:{
      body_minimizer(str){
        if(str)
          return str.replace(/<(?:.|\n)*?>/gm, '').slice(0,100)//.concat('<a :href="\'/node/\'+ this.nid">... Read More</a>')
        else
          return ''
      },
    }
}

</script>

<style lang="scss">
.content-teaser{
  width: calc(25% - 10px);
	margin: 15px 5px;
	-webkit-box-flex: 0;
	-ms-flex: 0 1 calc(25% - 10px);
	flex: 0 1 calc(25% - 10px);
    max-height: 370px;
    text-decoration: none !important;
    @media (max-width: 1280px){
    	width: calc(33.33% - 10px);
	    -ms-flex: 0 1 calc(33.33% - 10px);
    	flex: 0 1 calc(33.33% - 10px);
    }
    @media (max-width: 992px){
      width: calc(50% - 10px);
      -ms-flex: 0 1 calc(50% - 10px);
      flex: 0 1 calc(50% - 10px);
    }
    @media (max-width: 600px){
      width: calc(100% - 10px);
      -ms-flex: 0 1 calc(100% - 10px);
      flex: 0 1 calc(100% - 10px);
    }
    .content-subheader{
        text-align: left;
        margin: -5px 0 10px;
        font-size: 13px;
        overflow: hidden;
    }
  a {
    color: #3c4858 !important;
  }
}
.md-card .md-title.content-title {
  text-align: left;
	font-size: 16px;
  max-height: 64px;
  overflow: hidden;
  line-height: 32px;
}
.md-card-media {
    border-top-right-radius: 6px;
    border-top-left-radius: 6px;

    img {
    max-width: 100%;
    margin: auto;
    display: block;
    max-height: 300px;
    width: inherit;
  }
}


.medium{
  width: 98%;
  flex: 0 1 98%;
  text-align: left;
  box-shadow: none !important;
  background: transparent !important;
  cursor: default !important;

  .md-card-header {
    width: 70%;
    padding: 0px 15px 0px 30px;

    .md-title.content-title {
      font-size: 1.125rem;
      line-height: 1.5em;
      max-height: 3em;
      font-weight: 700;
      margin: 10px 0;
      font-family: roboto slab;
      color: #3c4858;
    }
  }
  .md-card-media {
    width: 30%;

    a {
      width: 100%;
      height: 100%;
      position: absolute;
      top: 0;
    }
    img {
      width: 100%;
      box-shadow: 0 5px 15px -8px rgba(0,0,0,.24), 0 8px 10px -5px rgba(0,0,0,.2);
      border-radius: 5px;
    }
  }
  p {
    font-size: 14px;
    margin: 0 0 10px;
    color: #555;
  }
  .author{
    margin: 0;
    a:not(.v_time_ago_a){
      font-weight: bold;
    }
  }
}

</style>

