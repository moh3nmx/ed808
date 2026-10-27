<template>
  <div class="video_player">
    <iframe id="ytplayer" type="text/html" width="100%" height="auto"
            :src="'https://www.youtube.com/embed/'+videoId"
            frameborder="0"></iframe>
  </div>
</template>

<script>
  export default {
    name:'YoutubeVideo',
    props:['url'],
    data(){
      return{
        videoId: this.url.split('v=')[1].split('&')[0]
      }
    },
    beforeCreate(){
      // Vue.use(VueYouTubeEmbed)
    },
    mounted(){
      // this.url = this.url.split('v=')[1].split('&')[0]
    },
    methods: {
      method(url) {
        // this.videoId = this.$youtube.getIdFromURL(url)
      }
    }
  }
</script>

<style lang="scss">
  .video_player{
    margin:50px 0 50px 0;
    iframe {
      height:475px !important;
      max-width: 800px;
    }
    .video_inside{
      background-color: #f6f6f6;
      min-height: 200px;
      max-width: 840px;
      display: block;
      margin: 0 auto;
      border: 1px solid #eee;
    }
  }
</style>
