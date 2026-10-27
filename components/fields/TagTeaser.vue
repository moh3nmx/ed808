<template>
  <!--Tag list Page -->
  <div class="tag">
    <div class="card card-blog" v-if="tagTeaserType != 'suggestedTag'">
      <div class="card-header card-header-image">
        <nuxt-link :to="'/tag/'+ tid + '/' + tag_name">
          <div class="img" :style="`background-image:url(${tag_image != null ? tag_image : '/images/tag.jpg'})`"></div>
          <div class="card-title">
            {{ tag_name }}
          </div>
        </nuxt-link>
        <div class="colored-shadow" :style="`background-image: url(${tag_image != null ? tag_image : '/images/tag.jpg'}); opacity: 1;`"></div></div>
      <div class="card-body">
        <h6 class="card-category text-info"> Related Contents: {{ contents }}</h6>
        <p class="card-description">
          {{ tag_description != null ? tag_description.substring(0,50) + ' ...' : 'No Description' }}
        </p>
        <a
          class="btn btn-info btn-round"
          :class="{ 'active' : flw }"
          @click="follow(tid)"
        >
          {{ flw ? 'Following' : 'Follow' }}
          <span class="follow-counts">{{ count != 0 ? '+'+count : 0 }}</span>
        </a>
      </div>
    </div>

    <!--Section for tag suggestion -->
    <div class="content" v-if="tagTeaserType == 'suggestedTag'">
      <div class="card purple">
        <div class="firstinfo"><img src="/images/hashtag-hex.png" />
          <div class="profileinfo">
            <nuxt-link :to="'/tag/'+ tid + '/' + tag_name">
              <h1>#{{ tag_name }}</h1>
            </nuxt-link>
            <a
              class="btn btn-info btn-round purple"
              :class="{ 'active' : flw }"
              @click="follow(tid)"
            >
              {{ flw ? 'Following' : 'Follow' }}
              <span class="follow-counts">{{ count != 0 ? '+'+count : 0 }}</span>
            </a>
          </div>
        </div>
      </div>
    </div>
  </div>

</template>

<script>
import axios from 'axios'
import {cookie} from '@/components/mixins/cookie.js'
export default {
  name: 'TagTeaser',
  mixins: [cookie],
  props: [
    'tagTeaserType',
    'tag_name',
    'tag_image',
    'tid',
    'user_follow',
    'contents',
    'tag_description',
    'follow_counts',
    'user_tags'
  ],
  data(){
    return {
      flw: this.user_follow,
      count: this.follow_counts
    }
  },
  methods: {
    async follow(tid){
      if(this.$store.getters.getUid){
        if(this.flw){
          this.count--
        }else{
          this.count++
        }
        this.flw = !this.flw
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials  = true;
        await axios.post('https://ed808.com:92/latin/tag/' + tid + '/follow',
          {
            action : !this.flw ? "unfollow": "follow",
          },
          {
            headers: {
              'Content-type': 'application/json',
              'X-CSRF-Token' : this.getCookie("token")
            }
          })
          .then((data) => {

          }).catch((err) => {
            if(!this.flw){
              this.count++
            }else{
              this.count--
            }
            this.flw = !this.flw
          })
      } else{
        this.$store.commit('TOGGLE_LOGIN')
      }
    }
  }
}
</script>

<style lang="scss" scoped>
  @import 'https://fonts.googleapis.com/css?family=Open+Sans:300,400';
  @import "@/assets/scss/vars.scss";
  .tag {
    margin: 10px;
    width: calc( 25% - 20px );
    @media #{$x1280} {
      width: calc(33% - 20px)
    }
    @media #{$x960} {
      width: calc(50% - 20px)
    }
    @media #{$x600} {
      width: calc(100% - 20px)
    }

    .card {
      position: relative;
      display: flex;
      flex-direction: column;
      min-width: 0;
      word-wrap: break-word;
      background-color: #fff;
      background-clip: border-box;
      border: 1px solid rgba(0,0,0,.12);
      border-radius: .25rem;
    }
    .card {
      border: 0;
      margin-bottom: 30px;
      margin-top: 30px;
      border-radius: 6px;
      color: rgba(0,0,0,.87);
      background: #fff;
      width: 100%;
      box-shadow: 0 2px 2px 0 rgba(0,0,0,.14), 0 3px 1px -2px rgba(0,0,0,.2), 0 1px 5px 0 rgba(0,0,0,.12);
      .card-header.card-header-image .img {
        height: 120px;
        width: 100%;
        background-position: center;
        background-size: cover;
        border-radius: 6px;
        pointer-events: none;
        box-shadow: 0 5px 15px -8px rgba(0,0,0,.24), 0 8px 10px -5px rgba(0,0,0,.2);
      }
      .card-header.card-header-image a:before {
        content: '';
        display: block;
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background: rgba(49,27,146,0.3);
        border-radius: 6px;
        transition: 0.5s;
      }
      .card-header.card-header-image a:hover:before {
        background: rgba(49,27,146,0.6)
      }
      .card-header.card-header-image .card-title {
        position: absolute;
        bottom: 15px;
        font-family: 'roboto slab';
        left: 15px;
        right: 15px;
        text-align: left;
        color: #fff;
        font-size: 1.125rem;
        text-shadow: 0 2px 5px rgba(33,33,33,.5);
      }
      .card-body {
        flex: 1 1 auto;
        padding: 1.25rem;
        h6 {
          font-size: .75rem;
          text-transform: uppercase;
          font-weight: 500;
          margin: 10px 0;
       }
        p.card-description {
          color: #999;
          text-align: left;
          font-size: 14px;
          margin: 0 0 10px;
          min-height: 54px;
        }
      }
      .card-category {
        margin-top: 10px;
        text-align: left;
      }
      .card-blog {
        margin-top: 60px;
      }
      .card-header.card-header-image .colored-shadow {
        transform: scale(.94);
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
      .card-header.card-header-image a {
        display: block;
      }
      .card-header.card-header-image {
        position: relative;
        padding: 0;
        z-index: 1;
        margin-left: 15px;
        margin-right: 15px;
        margin-top: -30px;
        border-radius: 6px;
      }
    }
  }
  .btn {
    display: inline-block;
    text-align: center;
    white-space: nowrap;
    vertical-align: middle;
    user-select: none;
    border: 1px solid transparent;
    padding: .46875rem 1rem;
    font-size: 1rem;
    line-height: 1.5;
    border-radius: .25rem;
    transition: color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out;
  }
  .btn {
    position: relative;
    padding: 12px 30px;
    margin: .3125rem 1px;
    font-size: .75rem;
    font-weight: 400;
    line-height: 1.428571;
    text-decoration: none;
    text-transform: uppercase;
    letter-spacing: 0;
    cursor: pointer;
    background-color: transparent;
    border-radius: .2rem;
    outline: 0;
    transition: box-shadow .2s cubic-bezier(.4,0,1,1),background-color .2s cubic-bezier(.4,0,.2,1);
    will-change: box-shadow,transform;
  }
  .btn.btn-round {
    border-radius: 30px;
    .follow-counts {
      display: inline-block;
      margin-left: 10px;
    }
  }
  .btn:not(:disabled):not(.disabled) {
    cursor: pointer;
  }
  .btn-info {
    color: #2196F3 !important;
    background-color: #fff !important;
    border-color: #2196F3 !important;
    box-shadow: none;
    transition-duration: 0.5s;
  }
  .btn-info.active {
    color: #fff !important;
    background-color: #2196F3 !important;
    border-color: #2196F3 !important;
    box-shadow: none;
  }


  .btn.btn-info:active, .btn.btn-info:focus, .btn.btn-info:hover {
    box-shadow: 0 14px 26px -12px rgba(33,150,243,.42), 0 4px 23px 0 rgba(0,0,0,.12), 0 8px 10px -5px rgba(33,150,243,.2);
  }

  .btn.btn-info.focus, .btn.btn-info:focus, .btn.btn-info:hover {
    color: #fff !important;
    background-color: #2196F3 !important;
    border-color: #2196F3 !important;
  }

  %reset { margin: 0; padding: 0; }
  %flex { display: flex; justify-content: center;	align-items: center; }

  @mixin animated($timein: null, $delay: null) {
    transition: all $timein ease-in-out;
    transition-delay: $delay;
  }

  @mixin dimensions($width: null, $height: null) { width: $width; min-height: $height; }

  .content{
    position: relative;
    animation: animatop 0.9s  cubic-bezier(0.425, 1.140, 0.470, 1.125) forwards ;
    .btn {
      padding: 5px 15px;
    }
  }
  .card.purple:hover {
    transition: box-shadow .5s;
    box-shadow: 0 0 11px rgba(69,39,160,.5);
  }

  .card.purple{
    @include dimensions( 320px,  100px);
    padding: 10px;
    border-radius: 6px;
    background-color: white;
    transition: box-shadow .5s;
    box-shadow: 0 2px 2px 0 rgba(0,0,0,.14), 0 3px 1px -2px rgba(0,0,0,.2), 0 1px 5px 0 rgba(0,0,0,.12);
    position: relative;
    overflow: hidden;
    &:after {
      content: '';
      display: block;
      width: 190px;
      height: 300px;
      background: #673AB7;

      position: absolute;
      animation: rotatemagic 0.75s cubic-bezier(0.425, 1.040, 0.470, 1.105) 1s both;
    }
  }

  .firstinfo{
    @extend %flex;
    flex-direction: row;
    z-index:2;
    position: relative;
    img{
      border-radius: 50%;
      width: 80px;
      height: 80px;
    }
    .profileinfo{
      width: 100%;
      padding: 0px 10px;
      h1{
        color: #47494e;
        font-size: 14px;
        margin-top: 0;
        text-align: center;
        font-family: "Roboto Slab";
        :visited {
          color: #47494e;}
        }
      p.bio{
        padding: 0;
        color: #5A5A5A;
        line-height: 1.2;
        font-style: initial;
        font-size: 14px;
        text-align: left;
        margin: 0;
      }
    }
  }
  .btn-info.purple {
    color: #673AB7 !important;
    background-color: #fff !important;
    border-color: #673AB7 !important;
    box-shadow: none;
    transition-duration: 0.5s;
  }
  .btn-info.purple.active {
    color: #fff !important;
    background-color: #673AB7 !important;
    border-color: #673AB7 !important;
    box-shadow: none;
  }


  .btn.btn-info.purple:active, .btn.btn-info.purple:focus, .btn.btn-info.purple:hover {
    box-shadow: 0 14px 26px -12px rgba(70,20,140,.42), 0 4px 23px 0 rgba(0,0,0,.12), 0 8px 10px -5px rgba(94,53,177,.2);
  }

  .btn.btn-info.purple.focus, .btn.btn-info.purple:focus, .btn.btn-info.purple:hover {
    color: #fff !important;
    background-color: #673AB7 !important;
    border-color: #673AB7 !important;
  }

  @keyframes animatop{
    0%{
      opacity: 0;
      bottom: -500px;}
    100%{
      opacity: 1;
      bottom: 0px;}
  }

  @keyframes animainfos{
    0%{
      bottom:  10px;}
    100%{
      bottom: -42px;}
  }

  @keyframes rotatemagic{
    0%{
      opacity: 0;
      transform: rotate(-0deg);
      top: -24px;
      left: -253px;
    }
    100%{

      transform: rotate(-30deg);
      top: -24px;
      left: -78px;
    }
  }
</style>
