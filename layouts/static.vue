<template>
  <div
    id="app"
    :class="[ queryClasses ] + bgImage != '' ? 'static-bg' : '' "
    :style="{ backgroundImage : `url(${bgImage})` }"
  >

    <!-- Man Nav With User Informations - Transparent -->
    <main-nav :transparent="true" />

    <!-- Main Contents Goes Here -->
    <nuxt />

    <!-- Logout Success Alert -->
    <md-snackbar :md-active.sync="IsLogOut">
      You've logged out successfully!
    </md-snackbar>

    <!-- Transparent Mini Footer -->
    <footer class="footer footer-transparent footer-big">
      <div class="container">
        <div class="content">
          <div class="md-layout">
            <div class="md-layout-item md-medium-size-50 md-large-size-50 md-small-size-100">
              <ul class="links-horizontal">
                <li>
                  <nuxt-link to="/aboutus">
                    About Us
                  </nuxt-link>
                </li>
                <li>
                  <nuxt-link to="/contactus">
                    Contact Us
                  </nuxt-link>
                </li>
                <li>
                  <nuxt-link to="/faq">
                    FAQ
                  </nuxt-link>
                </li>
                <li>
                  <nuxt-link to="/privacy">
                    Privacy Policy
                  </nuxt-link>
                </li>
              </ul>
            </div>
            <div class="copyright md-layout-item md-medium-size-50 md-large-size-50 md-small-size-100">
              Copyright © 2019 ED 808 .ltd All Rights Reserved.
            </div>
          </div>
        </div>
      </div>
    </footer>
    <side-menu></side-menu>
  </div>
</template>

<script>

  import axios from '@/node_modules/axios'
  import { cookie } from '@/components/mixins/cookie.js'
  import MainNav from "../components/elements/MainNav";
  import SideMenu from "../components/elements/SideMenu";

  export default {
    name: 'Static',
    components: {MainNav, SideMenu},
    mixins:[cookie],
    data(){
      return{
        IsLogin:false,
        IsLogOut:false,
        menu_flag:false,
        queryClasses: '',
        inputBox:'',
        aftersubmit:false,
        loginActive: false
      }
    },
    computed: {
      bgImage() {
        return this.$store.state.backGround
      }
    },
    created(){
      var query = this.$router.currentRoute.path.split('/')
      query.forEach(element => {
        if(element != '')
          this.queryClasses += ' page-' + element
      });
    },
    mounted(){
      if(this.$store.getters.getUid)
        this.IsLogin = true
    },
  }
</script>

<style lang="scss" scoped>
  @import "@/assets/scss/vars.scss";
  #app.static-bg {
    background: fixed center;
    background-size: cover;
    /*padding-bottom: 117px;*/
    &:before {
      content: '';
      position: fixed;
      left: 0;
      top: 0;
      height: calc(100% + 62px);
      /*bottom: 0;*/
      right: 0;
      background: rgba(0,0,0,0.3);
    }
  }
  .login-card-with-overlay{
    z-index: 10;
    padding-top: calc(90px + 10vh);
    opacity: 0;
    transition: ease opacity .5s;
    width: 100vw;
    height: 100vh;
    position: fixed;
    pointer-events: none;
    top: 0;
    background-color: rgba(0,0,0,0.3);
    &.active {
      z-index: 10;
      display: block;
      opacity: 1;
      width: 100vw;
      height: 100vh;
      position: fixed;
      top: 0;
      left: 0;
      transition: ease opacity .5s;
      pointer-events: all;

    }
    .login-card {
      width: 30%;
      margin: auto;
      margin-top: -100px;
      opacity: 0;
      transition-duration: 0.5s;
      &.active {
        opacity: 1;
        margin-top: 0;
        transition-duration: 0.5s;
      }
      @media screen and (max-width: 992px) {
        width: 60%;
      }
      @media screen and (max-width: 600px) {
        width: 90%;
      }

    }

  }

  @mixin icon-size($size) {
    width: $size;
    height: $size;
    min-width: $size;
    font-size: $size !important;
  }
  @media all and (max-width:960px) {
    .large-screen{
      display:none !important;
    }
    .small-screen{
      display: block !important;
      .md-icon{
        @include icon-size(30px);
      }
    }
    header#header .inner{
      justify-content: space-between;
    }
  }
  .small-screen{
    display: none;
  }


  footer {
    padding: .9375rem 0;
    text-align: center;
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
    position: static;
    bottom: 0;
    left: 0;
    width: 100%;
    &.footer-transparent {
      .links-horizontal {
        @media screen and (max-width: 992px ){
          text-align: center;
        }
        li {
          margin-right: .9375rem;
          color: #fff;
        }
        &:first-child a {padding-left: 0}
        &:last-child a {padding-right: 0}
      }
      .copyright {
        color: #fff;
        font-size: 12px;
        text-align: right;
        @media screen and (max-width: 992px ){
          text-align: center;
        }
      }
    }
    ul {
      margin: 0;
      padding: 0;
      list-style: none;

      li {
        display: inline-block;

        a {
          color: inherit !important;
          padding: .9375rem;
          font-weight: 500;
          font-size: 12px;
          text-transform: uppercase;
          border-radius: 3px;
          position: relative;
          display: block;
          text-decoration: none;

          &:hover {
            text-decoration: none;
          }
        }

        .btn {
          margin: 0
        }
      }

      &.links-horizontal {
        li a {
          padding: .9375rem;
        }
        &:first-child a {padding-left: 0}
        &:last-child a {padding-right: 0}
      }
      &.links-vertical{
        li {
          display: block;
          margin-left: -5px;
          margin-right: -5px;
          a { padding: 5px}
        }
      }
    }
    a:not(.md-button):hover {
      text-decoration: none !important;
      color: #9C27B0 !important;
    }
    .social-buttons{
      .btn, a{
        margin-top: 5px;
        margin-bottom: 5px;
        padding: 0
      }
    }
    .footer-brand {
      padding: 15px;
      font-size: 18px;
      margin-left: -15px;
      color: #3c4858 !important;
      font-weight: bold;
      &:focus,
      &:hover {
        color: #3c4858!important;
      }
    }
    .copyright {
      padding: 15px 0;
      .md-icon {
        font-size: 18px!important;
        color: inherit!important
      }
    }
    nav ul li a {
      margin-left: -15px
    }
    .container, .container-fluid {
      padding-right: 15px;
      padding-left: 15px;
      display: -webkit-box;
      display: -ms-flexbox;
      display: flex;
      -webkit-box-pack: justify;
      -ms-flex-pack: justify;
      justify-content: space-between;
      width: 100%;
      margin-left: auto;
      margin-right: auto;
      position: relative;
    }
    .md-layout {
      margin-right: -15px;
      margin-left: -15px;
    }
    .md-layout-item {
      padding-right: 15px;
      padding-left: 15px;
    }
    .md-field {
      padding-top: 13px;
      margin: -5px 0 10px;
      min-height: auto;
      .md-input{
        height: 36px;
        padding: 7px 0;
        font-size: 14px!important;
        -webkit-text-fill-color: #3c4858!important;
        -webkit-appearance: none;
        -moz-appearance: none;
        appearance: none;
      }
    }
  }

  .footer-transparent {
    background-color: transparent;
  }

  .footer-black {
    background-color: #232323;
    background: radial-gradient(ellipse at center,#585858 0,#232323 100%);
    background-size: 550% 450%;
    hr {border-color: hsla(0,0%,100%,.2)}
    a {
      opacity: .86;
      &:focus,&:hover {opacity: 1;}
    }
    .copyright,
    .footer-brand,
    .footer-brand:focus,
    .footer-brand:hover,
    a,h4,h5,i {color: #fff!important;}
  }

  .footer-big {
    padding: 1.875rem 0;
    .gallery-feed img {
      width: 20%;
      margin-right: 5%;
      margin-bottom: 5%;
      float: left
    }
    .content {
      text-align: left
    }
    .social-feed i {
      font-size: 20px;
      padding-right: 10px
    }
    .feed-line {
      display: -webkit-box;
      display: -ms-flexbox;
      display: flex;
      p {
        padding: 0 20px 0 5px
      }
    }
    h4 {
      font-weight: bold;
    }
    h5, .h5{
      color: #3c4858!important;
      text-decoration: none;
      font-size: 1.0625rem;
      line-height: 1.55em;
      margin-bottom: 15px;
      font-weight: bold;
      font-family: roboto slab;
    }
    p {
      color: #999;
    }
  }

  hr {
    margin-top: 1rem;
    margin-bottom: 1rem;
    border: 0;
    width: 100%;
    border-top: 1px solid rgba(0,0,0,.1);
  }

  .footer-big, .page-header, .section{
    .container, .container-fluid {
      -webkit-box-orient: vertical;
      -webkit-box-direction: normal;
      -ms-flex-direction: column;
      flex-direction: column;
    }
  }

  .social-buttons a{
    &.md-telegram .md-button-content{
      @include before(F501, '', 24px, #0188cc);
    }
    &.md-facebook .md-button-content{
      @include before(f20c, '', 24px, #4a66a0);
    }
    &.md-instagram .md-button-content{
      @include before(f2fe, '', 24px, #E91E63);
    }
    &.md-linkedin .md-button-content{
      @include before(f33b, '', 24px, #0976b4);
    }
    &.md-youtube .md-button-content{
      @include before(f5c3, '', 24px, #e52d27);
    }

  }
</style>
