<template>
  <div>
    <!--author profile card-->
    <div class="author" v-if="teaserType == 'author'">
      <md-content class="md-elevation-2">
        <div class="author-details">
          <div class="userimage">
            <nuxt-link :to="'/user/'+uid" target="_blank">
              <md-avatar class="md-large">
                <img v-if="picture" :src="picture" alt="user_image" />
                <img v-else src="/images/avatar.png" alt="user_image" />
              </md-avatar>
            </nuxt-link>
          </div>
          <md-content class="userdetails">
            <p>Written By:</p>
            <nuxt-link :to="'/user/'+uid">
              {{ name }}
            </nuxt-link>
            <span v-if="$store.getters.getUid != uid">
              <button
                class="md-follow"
                :class="flw ? 'active' : ''"
                @click="follow(flw, uid)"
              >
                {{ flw ? 'Following' : 'Follow' }}
              </button>
            </span>

          </md-content>
        </div>
        <div class="about-author">
          <p>{{ about_me }}</p>
        </div>
      </md-content>
    </div>

    <!--list item type-->
    <div class="user-teaser" v-if="teaserType == 'list-item'">
      <div class="user-card">
        <nuxt-link :to="`/user/${uid}`">
          <img :src="picture != null ? picture : '/images/avatar.png'" alt="" width="64px">
          <p>
            <span class="name">{{name}}</span>
            <br>
            <span class="desc"> {{ about_me != null ? about_me.substring(0,30) : ' ' }} </span>
          </p>
        </nuxt-link>
        <span v-if="$store.getters.getUid != uid">
          <button
            class="md-follow"
            :class="flw ? 'active' : ''"
            @click="follow(flw, uid)"
          >
            {{ flw ? 'Following' : 'Follow' }}
          </button>
        </span>
      </div>
    </div>

    <!--tag follower type-->
    <div class="tag-follower" v-if="teaserType == 'tag-follower'">
      <div class="user_image">
        <nuxt-link :to="`/user/${uid}`" target="_blank">
          <md-avatar class="md-large-tag">
            <img :src="picture != null ? picture : '/images/avatar.png'" alt="user_image" />
          </md-avatar>
          <div class="colored-shadow" :style="{ backgroundImage: `url(${picture})`, opacity: 1}"></div>
        </nuxt-link>
      </div>
      <div class="user_details">
        <nuxt-link v-if="name" :to="`/user/${uid}`">
          {{ name }}
        </nuxt-link>
        <nuxt-link v-else :to="`/user/${uid}`">
          {{ user_id }}
        </nuxt-link>
      </div>
      <div>
        <span v-if="$store.getters.getUid != uid">
          <button
            class="mdfollow"
            :class="flw ? 'active' : ''"
            @click="follow(flw, uid)"
          >
            {{ flw ? 'Following' : 'Follow' }}
          </button>
        </span>
      </div>
    </div>
  </div>
</template>

<script>
  import axios from "@/node_modules/axios";
  import { cookie } from "@/components/mixins/cookie.js";

  export default {
    name: "UserTeaser",
    mixins: [cookie],
    props: [
      'teaserType',
      'uid',
      'name',
      'picture',
      'user_id',
      'about_me',
      'following'
    ],
    data() {
      return {
        flw: this.following
      }
    },
    methods: {
      // To Follow or Unfollow a User
      follow(following, uid) {
        if(!this.$store.getters.getUid){
          this.$store.commit('TOGGLE_LOGIN')
        }else {
          this.flw = !this.flw;
          axios.defaults.crossDomain = true;
          axios.defaults.withCredentials = true;
          axios
            .post(
              "https://ed808.com:92/latin/user/" + uid + "/follow",
              {
                action: following ? "unfollow" : "follow"
              },
              {
                headers: {
                  "Content-Type": "application/json",
                  "X-CSRF-Token": this.getCookie("token"),
                  'Cache-Control': 'no-cache',
                }
              }
            )
            .then(() => {

            })
            .catch(err => {
              this.flw = !this.flw;
              console.log(err);
            });
        }
      }
    }
  }
</script>

<style lang="scss" scoped>
  @import "@/assets/scss/vars.scss";
  @mixin main-center-content(){
    max-width: 800px;
    text-align: left;
    direction: ltr;
    margin: auto;
    padding: 0 15px;
    @media screen and (max-width: 992px){
      width: 80%;
      margin: auto;
    }
    @media screen and (max-width: 600px){
      width: 95%;
      margin: auto;
    }
  }
  button {
    transition: 0.5s;
    &.md-follow {
      background-color: transparent;
      color: #2196F3;
      border: 1px solid #2196F3;
      padding: 10px 20px;
      border-radius: 30px;
      margin: 0;
      position: absolute;
      min-width: 100px;
      @media #{$x600} {
        min-width: 80px;
        padding: 8px 10px;
        border-radius: 20px;
        font-size: 12px;
      }
      &.active {
        background-color: #2196F3;
        color: white;
      }
    }
    &:hover {
      opacity: 0.7;
      cursor: pointer;
    }
  }
  button {
    transition: 0.5s;
    &.mdfollow {
      background-color: transparent;
      color: #2196F3;
      border: 1px solid #2196F3;
      padding: 8px 20px;
      border-radius: 30px;
      margin: 5px 0 0 0;
      /*position: absolute;*/
      min-width: 80px;
      &.active {
        background-color: #2196F3;
        color: white;
      }
    }
    &:hover {
      opacity: 0.7;
      cursor: pointer;
    }
  }
  .author {
    @include main-center-content();
    .author-details {
      align-items: center;
      padding: 20px !important;
      display: flex;
      border-bottom: 1px solid #f5f5f5;
      @media screen and (max-width: 600px){
        padding-bottom: 60px !important;
      }
    }
    .userimage {
      text-align: center;
      width: 64px;
    }
    .userdetails {
      width: calc(100% - 64px);
      padding-left: 20px;
      position: relative;
      a {
        font-size: 18px;
        font-weight: bold;
      }
      p {
        margin: 0;
      }
    }
    .about-author {
      padding: 20px;
      img {

      }
      p {
        margin: 0;
      }
    }
    button {
      right: 0;
      top: 9px;
      @media screen and (max-width: 600px){
        top: 70px;
        right: calc(50% - 27px)
      }
    }
  }
  .user_image {
    position: relative !important;
    width: 100px !important;
    margin: auto !important;
  }
  .md-avatar.md-large-tag {
    min-width: 100px;
    min-height: 100px;
    border-radius: 64px;
    font-size: 32px;
  }
  .user_details {
    margin: 15px 0 7px;
    a {
      color: #47494e !important;
      font-size: 14px !important;
      font-weight: 700 !important;
      font-family: "Roboto Slab";
    }
    a:visited {
      color: #47494e !important;
    }
  }
  .colored-shadow{
    min-width: 100px;
    min-height: 100px;
    border-radius: 64px;
    font-size: 32px;
    transform: scale(0.94);
    top: 12px;
    filter: blur(12px);
    position: absolute;
    background-size: cover;
    z-index: -1;
    transition: opacity .45s;
    opacity: 0;
  }
  .user-teaser {
    position: relative;
    padding: 5px;
    min-height: 77px;
    .user-card {
      p {
        text-align: left;
        margin-left: 70px;
        margin-right: 70px;
        margin-top: 12px;
        line-height: 18px;
        color: #212121;
        @media #{$x600} {
          margin-left: 50px;
          margin-right: 85px;
          .name {
            font-size: 14px !important;
          }
          .desc {
            font-size: 11px !important;
          }
        }
        .name {
          font-weight: 500;
        }
        .desc {
          color: #626262;
          font-weight: 300;
          font-size: 12px;
        }
      }
      img {
        position: absolute;
        left: 0;
        top: 3.5px;
        border-radius: 50%;
        @media #{$x600} {
          width: 48px;
          top: 13px;
        }
      }
      button {
        right: 0;
        top: 21px;
      }
    }
  }
</style>
