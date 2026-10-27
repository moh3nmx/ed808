<template>
  <header :class="transparent ? 'transparent' : ''" id="header">
    <div class="inner md-layout">
      <nuxt-link to="/" class="logo-808">
        <img
          v-if="transparent"
          src="/images/logo-white.png"
          class="logo"
        >
        <img
          v-else
          src="/images/logo.png"
          class="logo"
        >
      </nuxt-link>
      <div class="md-layout md-gutter">
        <div
          class="header-menu-text md-layout-item md-size-100"
        >
          <md-menu md-direction="bottom-start" class="show-on-large">
            <nuxt-link to="/">
              <md-button>Home</md-button>
            </nuxt-link>
          </md-menu>

          <md-menu md-direction="bottom-start" class="show-on-large">
            <nuxt-link to="/contents">
              <md-button>Contents</md-button>
            </nuxt-link>
          </md-menu>

          <md-menu md-direction="bottom-start" class="show-on-large">
            <nuxt-link to="/calendar">
              <md-button>Calendar</md-button>
            </nuxt-link>
          </md-menu>

          <!-- <md-menu md-direction="bottom-start" class="show-on-large">
            <nuxt-link to="/events">
              <md-button>Events</md-button>
            </nuxt-link>
          </md-menu> -->

          <md-menu md-direction="bottom-start" class="show-on-large">
            <nuxt-link to="/tags">
              <md-button>Tags</md-button>
            </nuxt-link>
          </md-menu>

          <md-menu md-direction="bottom-start" class="show-on-large hide-on-tablets">
            <nuxt-link to="/pricing">
              <md-button>Business Services</md-button>
            </nuxt-link>
          </md-menu>

          <md-menu md-direction="bottom-start" class="show-on-large hide-on-tablets">
            <nuxt-link to="/aboutus">
              <md-button>About Us</md-button>
            </nuxt-link>
          </md-menu>

          <md-menu md-direction="bottom-start" class="show-on-large hide-on-tablets">
            <nuxt-link to="/contactus">
              <md-button>Contact Us</md-button>
            </nuxt-link>
          </md-menu>

          <div
            v-if="!$store.getters.getUid"
            class="md-menu user-links"
          >
            <md-menu md-direction="bottom-start">
              <nuxt-link :to="'/login?callback='+$route.path">
                <md-button>Login</md-button>
              </nuxt-link>
            </md-menu>

            <md-menu md-direction="bottom-start">
              <nuxt-link to="/register">
                <md-button
                  class="md-raised"
                  style="background: #2196F3;color: #fff;"
                >
                  Register
                </md-button>
              </nuxt-link>
            </md-menu>
          </div>

          <div
            v-else
            class="md-menu user-links loggedin">
            <md-menu
              md-size="big"
              md-direction="bottom-end"
              md-align-trigger
              :md-active.sync="menu_flag">
              <div @click="opening_menu">
                <md-button>
                    <span
                      v-if="$store.state.user.full_name"
                      class="md-xsmall-hide">
                      {{$store.state.user.full_name}}
                    </span>
                  <md-avatar>
                    <img
                      v-if="$store.state.user.picture"
                      :src="$store.state.user.picture"
                      alt="user_image"
                    >
                    <img
                      v-else
                      src="/images/avatar.png"
                      alt="user_image"
                    >
                  </md-avatar>
                </md-button>
              </div>

              <md-menu-content class="user-menu">
                <md-menu-item>
                  <nuxt-link
                    :to="'/user/'+ $store.state.user.uid"
                  >
                    My Profile
                  </nuxt-link>
                </md-menu-item>
                <md-menu-item>
                  <nuxt-link
                    :to="'/user/'+ $store.state.user.uid + '/settings'"
                  >
                    Settings
                  </nuxt-link>
                </md-menu-item>
                <md-menu-item >
                  <a @click="logUserOut" style="cursor: pointer;">Logout</a>
                </md-menu-item>
              </md-menu-content>
            </md-menu>
          </div>

          <!-- Toggle Side Menu -->
          <md-menu
            v-if="true"
            class="md-toggle-menu"
            md-size="small"
            :md-active.sync="$store.state.sideMenuActive"
            @click="toggle"
          >
            <md-button>
              <md-icon
                v-if="!$store.state.sideMenuActive"
                class="md-primary"
              >
                menu
              </md-icon>
              <md-icon
                v-else
                class="md-primary"
              >
                close
              </md-icon>
            </md-button>
          </md-menu>
        </div>
      </div>
    </div>
  </header>

</template>

<script>
import axios from '@/node_modules/axios'
import { cookie } from '@/components/mixins/cookie.js'

export default {
  name: "MainNav",
  mixins: [cookie],
  props:['transparent'],
  data() {
    return {
      toggleCard: false,
      loginActive: false,
      IsLogin:false,
      IsLogOut:false,
      menu_flag:false,
    }
  },
  methods: {
    opening_menu(){
      this.menu_flag = !this.menu_flag
    },
    toggle () {
      this.$store.commit('TOGGLE_SIDE_MENU')
    },
    menu_classes() {
      return {'show-on-large': !this.$store.getters.getUid };
    },
    logUserOut(){
      axios.defaults.crossDomain = true
      axios.defaults.withCredentials  = true
      axios.post('https://temp.ed808.com/api/latin/user/logout',
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
        })
        .catch(e => {
          console.log('errors for logout : ' + e)
        });
    },

  },
  watch:{
    $route (to, from){
      if(this.$store.state.sideMenuActive)
        this.$store.commit('TOGGLE_SIDE_MENU')
    }
  }
}
</script>

<style lang="scss" scoped>
  @import "@/assets/scss/vars.scss";
  @mixin icon-size($size) {
    width: $size;
    height: $size;
    min-width: $size;
    font-size: $size !important;
  }
  .mobile-menu {
    width: 100%;
    max-width: 100%;
    top:90px !important;
    right:0px !important;
    left:0px !important;
    .md-menu:not(.login-in-menu){
      width: 100%;
      display: block;
      a{
        display: block;
      }
    }
    .md-button{
      width:100%;
    }
  }

  .md-menu{
    button {
      margin: 0;
      vertical-align: middle;
    }
    &.user-links {
      padding-right: 10px;
      margin-right: 10px;
      position: relative;
      @media #{$m720} {
          margin-left: 15px;
          margin-right: 0;
          padding-right: 0;
        }
      &:before {
        content: "";
        height: 20px;
        right: 0;
        width: 1px;
        background: #ccc;
        position: absolute;
        top: 10px;
        @media #{$m720} {
          left: 0;
          right: auto;
        }
      }
      &.loggedin{
        padding-right: 10px;
        @media #{$m720} {
          padding-left: 15px;
          padding-right: 0;
        }
        button{
          height: 40px;

          @media #{$x600}{
            width: 40px;
            min-width: 40px;
            margin: 0 6px;
            border-radius: 50%;
          }
          span{
            margin: 0 10px;
            font-weight: 300 !important;
          }
          @media #{$m600}{.md-ripple{padding-right: 0;}}
        }
      }
    }
    &.login-in-menu{
      display: flex;
      justify-content: space-around;
      width: 320px;
      margin: auto;
    }
  }

  .user-menu ul.md-list {
    min-width: 160px;
    padding: 5px 0;
    margin: 2px 0 0;
    font-size: 14px;
    text-align: left;
    list-style: none;
    background-color: #fff;
    border-radius: 3px!important;
    li {
      position: relative;
      .md-list-item-content {
        padding: 0;
        height: 39px;
      }
      a {
        font-size: 14px;
        padding: 10px 1.5rem;
        margin: 0 5px;
        text-transform: none;
        color: #333!important;
        border-radius: 2px;
        display: -webkit-box;
        display: -ms-flexbox;
        display: flex;
        font-weight: 400;
        line-height: 1.428571;
        -webkit-box-align: center;
        -ms-flex-align: center;
        align-items: center;
        white-space: nowrap;
        -webkit-transition: all .15s linear;
        transition: all .15s linear;
        width: 100%;
        &:hover {
          background: #eee;
        }
      }
    }
  }


  .transparent .md-menu.md-toggle-menu .md-button {
    i {
      color: #fff;
    }
  }

  .md-menu.md-toggle-menu .md-button{
    width: 36px;
    min-width: 36px;
    display: none;
    @media #{$x720} {
      display: inline-block;
    }
  }
   @media all and (max-width:720px) {
    .show-on-large{
      display:none !important;
    }
    header#header .inner{
      justify-content: space-between;
    }
  }
  .md-layout.md-gutter {
    .header-menu-text {
      text-align: right;
      @media #{$x720} {
        text-align: left;
      }
    }
  }
  .hide-on-tablets {
    @media #{$x1080} {
      display: none !important;
    }
  }

  .small-screen{
    display: none;
  }

  .logo-808 {
    @media #{$x720} {
      position: absolute;
      left: calc(50% - 35px)
    }
  }
  .md-menu.user-links {
    @media #{$x720} {
      display: none;
    }
  }
</style>
