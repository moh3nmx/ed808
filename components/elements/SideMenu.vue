<template>
  <div class="md-side-menu">
    <md-drawer class=" md-scrollbar" :md-active.sync="$store.state.sideMenuActive" :md-swipeable="true">

      <div class="side-menu-header">
        <div class="background" :style="`background-image: url(/images/city-profile.jpg)`"></div>
        <div class="user-avatar">
          <nuxt-link :to="$store.getters.getUid ? `/user/${ $store.state.user.uid }` : `/login`">
            <img :src="$store.state.user.picture ? $store.state.user.picture : '/images/avatar.png'" width="64px" height="64px" alt="">
          </nuxt-link>
        </div>
        <div class="user-details" @click="userActions = !userActions" v-if="$store.getters.getUid">
          <span class="name">{{ $store.state.user.full_name }}</span>
          <span class="username">@{{ $store.state.user.username }}</span>
          <md-icon>{{ userActions ? 'expand_less' : 'expand_more' }}</md-icon>
        </div>
        <nuxt-link to="/login" v-else>
          <div class="user-details">
            <span class="name">Guest</span>
            <span class="username">Please login or Register</span>
          </div>
        </nuxt-link>

      </div>
      <div
        class="user-actions active"
        v-if="!$store.getters.getUid"
      >
        <md-list>
          <nuxt-link :to="'/login?callback='+$route.path">
            <md-list-item>
              <!-- <md-icon>mdi-login-variant</md-icon> -->
              <i class="md-icon md-theme-default md-icon-font mdi mdi-login-variant"></i>

              <span class="md-list-item-text">
                  Login
              </span>
            </md-list-item>
          </nuxt-link>

          <nuxt-link to="/register">
            <md-list-item>
              <!-- <md-icon>mdi-clipboard-text</md-icon> -->
              <i class="md-icon md-theme-default md-icon-font mdi mdi-clipboard-text"></i>

              <span class="md-list-item-text">
                  Register
              </span>
            </md-list-item>
          </nuxt-link>
        </md-list>
      </div>
      <div
        class="user-actions"
        :class="{'active' : userActions}"
        v-else
      >
        <md-list>
          <nuxt-link :to="'/user/'+$store.state.user.uid">
            <md-list-item>
              <!-- <md-icon>account_circle</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-account-circle"></i>

              <span class="md-list-item-text">
                  Profile
              </span>
            </md-list-item>
          </nuxt-link>

          <nuxt-link :to="'/user/'+$store.state.user.uid + '/settings'">
            <md-list-item>
              <!-- <md-icon>settings</md-icon> -->
              <i class="md-icon md-theme-default md-icon-font mdi mdi-settings"></i>

              <span class="md-list-item-text">
                  Settings
              </span>
            </md-list-item>
          </nuxt-link>

          <a @click="logUserOut()">
            <md-list-item>
              <!-- mdi-power -->
              <!-- <md-icon>power_settings_new</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-power"></i>

              <span class="md-list-item-text">
                  Logout
              </span>
            </md-list-item>
          </a>
        </md-list>
      </div>
      <md-list>
        <nuxt-link to="/">
          <md-list-item>
            <!-- <md-icon>home</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-home"></i>

            <span class="md-list-item-text">
                Home
            </span>
          </md-list-item>
        </nuxt-link>

        <nuxt-link to="/contents">
          <md-list-item>
            <i class="md-icon md-theme-default md-icon-font mdi mdi-view-module"></i>
            <!-- <md-icon>view_module</md-icon> -->
            <span class="md-list-item-text">
                Contents
            </span>
          </md-list-item>
        </nuxt-link>

        <nuxt-link to="/calendar">
          <md-list-item>
            <i class="md-icon md-theme-default md-icon-font mdi mdi-calendar-text"></i>
            <!-- <md-icon>event_note</md-icon> -->
            <span class="md-list-item-text">
                Calendar
            </span>
          </md-list-item>
        </nuxt-link>

        <!-- <nuxt-link to="/events">
          <md-list-item>
            <md-icon>event_note </md-icon>
            <span class="md-list-item-text">
                Events
            </span>
          </md-list-item>
        </nuxt-link> -->

        <nuxt-link to="/tags">
          <md-list-item>
            <!-- <md-icon>local_offer</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-tag"></i>
            <span class="md-list-item-text">
                Tags
            </span>
          </md-list-item>
        </nuxt-link>

        <nuxt-link to="/pricing">
          <md-list-item>
            <!-- <md-icon>local_offer</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-credit-card"></i>
            <span class="md-list-item-text">
                Business Services
            </span>
          </md-list-item>
        </nuxt-link>

        <nuxt-link to="/aboutus">
          <md-list-item>
            <!-- <md-icon>info</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-information-outline"></i>

            <span class="md-list-item-text">
                About Us
            </span>
          </md-list-item>
        </nuxt-link>

        <nuxt-link to="/contactus">
          <md-list-item>
            <!-- <md-icon>contacts</md-icon> -->
            <i class="md-icon md-theme-default md-icon-font mdi mdi-contact-mail"></i>

            <span class="md-list-item-text">
                Contact Us
            </span>
          </md-list-item>
        </nuxt-link>
      </md-list>
    </md-drawer>
  </div>
</template>

<script>
import { cookie } from '@/components/mixins/cookie.js'
import axios from 'axios'
export default {
  mixins:[cookie],
  data(){
    return {
      userActions: false,
      IsLogin:false,
      IsLogOut:false,
    }
  },
  methods: {
    logUserOut(){
      axios.defaults.crossDomain = true
      axios.defaults.withCredentials  = true
      axios.post('https://ed808.com:92/latin/user/logout',
        true,{
          headers:{
            'Content-type': 'application/json',
            'X-CSRF-Token': this.getCookie("token")
          }
        })
        .then((data) => {
          this.eraseCookie('token')
          this.$store.commit('LOGOUT')
          this.IsLogin = false
          //This line show the log out snackbar
          this.IsLogOut = true
          this.$store.commit('SET_USER_DATA',{uid:false})
          if(this.$store.state.sideMenuActive)
            this.$store.commit('TOGGLE_SIDE_MENU')
        })
        .catch(e => {
          console.log('errors for logout : ' + e)
        });
    },
  }
}
</script>


<style lang="scss" scoped>
  .md-drawer {
    height: 100%;
    position: fixed;
    z-index: 100;
    width: 300px;
    overflow-y: scroll;
    .side-menu-header {
      position: relative;
      padding: 16px 16px 0;
      margin-bottom: 8px;
      overflow: hidden;
      .user-avatar {
        text-align: left;
        margin-bottom: 10px;
        img {
          border-radius: 50%;
        }
      }
      .user-details {
        cursor: pointer;
        position: relative;
        .md-icon {
          color: #fff;
          position: absolute;
          right: 0;
          top:6px;
        }
        .name {
          text-align: left;
          display: block;
          line-height: 20px;
          color: #fff;
          font-weight: 500;
          font-size: 14px;
        }
        .username {
          text-align: left;
          display: block;
          margin-bottom: 20px;
          line-height: 16px;
          color: #fff;
          font-size: 14px;
        }
      }

      .background {
        background-position: center;
        background-size: cover;
        overflow: hidden;
        position: absolute;
        top: 0;
        right: 0;
        bottom: 0;
        left: 0;
        z-index: -1;
        &:after {
          content: '';
          background: rgba(0,0,0,.3);
          position: absolute;
          top: 0;
          right: 0;
          bottom: 0;
          left: 0;
        }
      }
    }
    .user-actions {
      transition: 0.5s ease-out;
      overflow: hidden;
      max-height: 0;
      border-bottom: 1px solid transparent;
      &.active {
        max-height: 100%;
        border-bottom: 1px solid #E0E0E0;
      }
      .md-list-item {
        cursor: pointer;
      }
    }
    .md-list {
      .md-list-item {
        line-height: 40px;
        transition-duration: 0.5s;
        &:hover {
          background-color: #EEEEEE;
        }
      }
    }
  }
  .md-overlay {
    position:fixed;
  }
</style>
