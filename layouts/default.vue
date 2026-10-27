<template>
  <div
    id="app"
    :class="[queryClasses]"
  >

    <!--login with overlay-->
    <md-dialog class="login-dialog" :md-active.sync="$store.state.loginActive">
      <div @click="hideLoginCard($event)">
        <div class="login-card active">
          <login-card :pageComponent="false" />
        </div>
      </div>
    </md-dialog>


    <main-nav :transparent="false"></main-nav>

    <nuxt />

    <md-snackbar :md-active.sync="IsLogOut">
      You've logged out successfully!
    </md-snackbar>

    <footer class="footer footer-white footer-big">
      <div class="container">
        <div class="content">
          <div class="md-layout">
            <div class="md-layout-item md-medium-size-25 md-large-size-25 md-small-size-100">
              <h5>About</h5>
              <ul class="links-vertical">
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
              </ul>
            </div>
            <div class="md-layout-item md-medium-size-25 md-large-size-25 md-small-size-100">
              <h5>Legal</h5>
              <ul class="links-vertical">
                <li>
                  <nuxt-link to="/privacy">
                    Privacy Policy
                  </nuxt-link>
                </li>
              </ul>
            </div>
            <div class="md-layout-item md-medium-size-45 md-large-size-45 md-small-size-100">
              <nuxt-link to="/">
                <h5>ED 808 .ltd</h5>
              </nuxt-link>
              <p>
                808 is trying to connect engineering knowledge and industry through combining technology and the newest global experiences in all fields of civil and architectural engineering.
              </p>
            </div>
            <div
              style="display: none;"
              class="md-layout-item md-medium-size-40 md-large-size-40 md-small-size-100"
            >
              <h5>Subscribe to Newsletter</h5>
              <p>
                Join our mailing list to receive the latest updates and personalized content right in your inbox.
              </p>
              <form
                action=""
                method=""
                class="form-newsletter"
              >
                <div class="md-layout">
                  <div class="md-layout-item md-large-size-66 medium-size-50">
                    <div class="md-field md-theme-default md-has-placeholder">
                      <input
                        id="md-input-zyl6y53yh"
                        type="text"
                        placeholder="Your Email.."
                        class="md-input"
                      >
                    </div>
                  </div>
                  <div class="md-layout-item md-large-size-33 medium-size-50">
                    <button
                      type="button"
                      class="md-button md-primary md-just-icon md-block md-theme-default"
                    >
                      <div class="md-ripple">
                        <div class="md-button-content">
                          <i class="md-icon md-icon-font md-theme-default">
                            mail
                          </i>
                        </div>
                      </div>
                    </button>
                  </div>
                </div>
              </form>
            </div>
          </div>
        </div>
        <hr>
        <ul class="social-buttons">
          <li>
            <a
              href="http://telegram.me/ed808"
              class="md-telegram"
              target="_blank"
            >
              <md-button class="md-icon-button" />
            </a>
          </li>
          <li>
            <a
              href="https://www.facebook.com/ED808.ltd"
              class="md-facebook"
              target="_blank"
            >
              <md-button class="md-icon-button" />
            </a>
          </li>
          <li>
            <a
              href="https://www.instagram.com/ed808.ltd/"
              class="md-instagram"
              target="_blank"
            >
              <md-button class="md-icon-button" />
            </a>
          </li>
          <li>
            <a
              href="https://www.linkedin.com/company/ed808"
              class="md-linkedin"
              target="_blank"
            >
              <md-button class="md-icon-button" />
            </a>
          </li>
          <li>
            <a
              href="https://www.youtube.com/channel/UCa-R4IdZ-bCbsVTpKzt4r4A"
              class="md-youtube"
              target="_blank"
            >
              <md-button class="md-icon-button" />
            </a>
          </li>
        </ul>
        <div class="copyright">
          Copyright © 2019 ED 808 .ltd All Rights Reserved.
        </div>
      </div>
    </footer>
    <side-menu></side-menu>
  </div>
</template>

<script>

  import LoginCard from '@/components/fields/LoginCard'
  import axios from '@/node_modules/axios'
  import SideMenu from "@/components/elements/SideMenu"
  import { cookie } from '@/components/mixins/cookie.js'
  import MainNav from "@/components/elements/MainNav"

  export default {
    name: 'Default',
    components: {MainNav, LoginCard,SideMenu},
    mixins:[cookie],
    data(){
      return{
        IsLogin:false,
        IsLogOut:false,
        menu_flag:false,
        queryClasses: '',
        inputBox:'',
        aftersubmit:false,
        loginActive: false,
        submiting: false
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
    methods:{
      hideLoginCard(event){
        if(event.target.getAttribute('class') == 'login-card-with-overlay active') {
          if(this.$store.state.loginActive)
            this.$store.commit('TOGGLE_LOGIN')
        }
      },
    }
  }
</script>

<style lang="scss">
  @import "@/assets/scss/vars.scss";

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


  section#subscribe{
    background: #ddd;
    padding: 10px 0 30px 0;
    color: #3c4858;
    margin-top: auto;
    h2 {font-family: roboto slab;}
    .input-wrapper{
      position: relative;
      width: 400px;
      margin: auto;
      @media all and (max-width: 700px){
        width: 300px;
      }
    }
    input{
      width: 400px;
      box-sizing: border-box;
      -webkit-transition: width 0.4s ease-in-out;
      transition: width 0.4s ease-in-out;
      border-radius: 30px;
      height: 50px;
      -webkit-appearance: button-bevel;
      padding: 0 7%;
      font-size: 15px;
      background: rgba(255, 255, 255, 0.7);
      outline: none;
      color: #555;
      @media all and (max-width: 700px){
        width: 300px;
      }
    }
    button{
      position: absolute;
      height: 44px;
      right: -1px;
      top: -2px;
      width: 45px;
      border-radius: 50%;
      -webkit-appearance: button-bevel;
      margin: 5px;
      cursor: pointer;
      background-color: #BA68C8;
      &:before {
        content: '+';
        font-size: 28px;
        color: #ffffff;
      }
    }
  }



  footer {
    padding: .9375rem 0;
    text-align: center;
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
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

  .footer-white {
    background-color: #fff
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
