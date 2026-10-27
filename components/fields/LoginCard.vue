<template>
  <!--gihmmsWer2563-->
  <div class="card card-signup">
    <div class="header header-primary text-center">

      <h4 class="card-title">
        Log in with
      </h4>
      <div class="social-line">
        <!--<a href="#pablo" class="btn btn-just-icon btn-simple">-->
        <!--<md-button class="md-icon-button" disabled>-->
        <!--<i class="mdi mdi-facebook-box"></i>-->
        <!--</md-button>-->
        <!--</a>-->
        <a
          class="btn btn-just-icon btn-simple"
          @click="loginWithLinkedin"
        >
          <md-button class="md-icon-button">
            <i class="mdi mdi-linkedin-box"></i>
          </md-button>
          <md-tooltip>LinkedIn</md-tooltip>
        </a>
        <!--<a href="#pablo" class="btn btn-just-icon btn-simple">-->
        <!--<md-button class="md-icon-button" disabled>-->
        <!--<i class="mdi mdi-google-plus"></i>-->
        <!--</md-button>-->
        <!--</a>-->
        <md-progress-bar
          v-if="loading.sending"
          md-mode="indeterminate"
        />
      </div>

    </div>
    <p>Or login with your username</p>
    <div class="card-content">
      <form
        novalidate
        @submit.prevent="formSubmit"
        id="loginForm"
      >
        <md-field
          class="input-with-icon"
          :class="getValidationClass('emailOrUsename')"
        >
          <label>Username</label>
          <span class="icon-prefix mdi mdi-account-circle"></span>
          <span
            v-if="!$v.form.username_email.required"
            class="md-error"
          ></span>
          <md-input
            id="username_email"
            v-model="form.username_email"
            name="username_email"
            autocomplete="email"
            :disabled="loading.sending"
            required
          ></md-input>
        </md-field>
        <md-field
          class="input-with-icon"
          :class="getValidationClass('password')"
        >
          <label>Password</label>
          <span class="icon-prefix mdi mdi-lock"></span>
          <span
            v-if="!$v.form.password.required"
            class="md-error"
          ></span>
          <md-input
            id="password"
            v-model="form.password"
            type="password"
            name="password"
            :disabled="loading.sending"
            autocomplete="current-password"
            required
          ></md-input>
        </md-field>
        <vue-recaptcha
          ref="recaptcha"
          @verify="logUserIn"
          @expired="onCaptchaExpired"
          size="invisible"
          sitekey="6Ldp3XMUAAAAACZav47_l9to_2uESNGLa1RvQOU6">
        </vue-recaptcha>

        <md-snackbar :md-active.sync="userSaved">
          {{ lastUser }}, you have logged in successfully!
        </md-snackbar>
        <md-snackbar
          class="error"
          :md-active.sync="showError"
        >
          {{ errors }}
        </md-snackbar>
      </form>

    </div>
    <div>
      Not registered yet?
      <nuxt-link to="/register">
        Join Now
      </nuxt-link>
    </div>
    <div>
      Forgot your password?
      <nuxt-link to="/login/reset_password">
        Reset Password
      </nuxt-link>
    </div>
    <div class="footer text-center">
      <md-button
        v-if="!pageComponent"
        @click="hideLogin()"
        class="md-primary"
        :disabled="loading.sending"
      >
        Not Now
      </md-button>
      <md-button
        form="loginForm"
        type="submit"
        class="md-primary"
        :disabled="loading.sending"
      >
        Log In
      </md-button>
    </div>
  </div>
</template>

<script>
  import VueRecaptcha from 'vue-recaptcha'
  import { validationMixin } from 'vuelidate'
  import { required } from 'vuelidate/lib/validators'
  import { cookie } from '@/components/mixins/cookie.js'
  import axios from "@/node_modules/axios"

  export default {
    name: 'LoginCard',
    components: {
      'vue-recaptcha': VueRecaptcha
    },
    mixins: [validationMixin,cookie],
    scrollToTop: true,
    middleware: 'isauth',
    props:[
      'pageComponent',
      'loginActive'
    ],
    data(){
      return{
        errors:'',
        showError:false,
        sessid:'',
        session_name:'',
        token:'',
        form:{
          username_email: null,
          password: null,
        },
        userSaved: false,
        loading: {sending: false},
        lastUser: null
      }
    },
    validations: {
      form: {
        username_email: {
          required
        },
        password: {
          required
        }
      }
    },
    mounted(){
      // middleware implemented
      // if(this.$store.getters.getUid){
      //   this.$router.push('/user/'+ this.$store.getters.getUid)
      // }
    },
    methods:{
      authenticate: function (provider) {
        this.$auth.authenticate(provider).then(function () {
          // Execute application logic after successful social authentication
        })
      },
      loginWithLinkedin(){
        if(!this.pageComponent) {
          localStorage.setItem('callback', this.$route.path)
        }
        window.location.replace(
          'https://www.linkedin.com/oauth/v2/authorization?response_type=code&client_id=77gasbcb9ngw6d&redirect_uri=https://temp.ed808.com/api/linkedin/join&scope=r_emailaddress r_liteprofile'
        )
      },
      hideLogin(){
        this.$store.commit('TOGGLE_LOGIN')
      },
      getValidationClass (fieldName) {
        const field = this.$v.form[fieldName]
        if (field) {
          return {
            'md-invalid': field.$invalid && field.$dirty
          }
        }
      },
      clearForm(){
        this.$v.$reset()
        this.form.username_email = null
        this.form.password = null
        this.showError = false
        this.errors = ''
      },
      async logUserIn(recaptchaToken){
        this.loading.sending = true
        this.$refs.recaptcha.reset();

        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        await axios.post('https://temp.ed808.com/api/latin/user/login',
          {
            hash : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            username_email : this.form.username_email,
            password : this.form.password,
            reCaptchaToken : recaptchaToken
          },
          {
            headers: {
              'Content-type': 'application/json'
            }
          })
          .then((data) => {
            //set uid in store
            this.$store.commit('SET_USER',data.data.uid)
            this.$store.commit('SET_USER_DATA', data.data)
            //set token in cookie
            this.setCookie('token', data.data.token , 23)
            this.lastUser = `${this.form.username_email}`
            this.userSaved = true
            this.loading.sending = false

            if(!this.pageComponent) {
              this.$store.commit('TOGGLE_LOGIN')
            }else {
              //redirect after successful login
              if(this.$route.query.hasOwnProperty('callback')) {
                if(this.$route.query.callback.search('http') != -1){
                  window.location.assign(this.$route.query.callback)
                }else{
                  this.$router.push(this.$route.query.callback)
                }
                window.location.assign
              }else{
                this.$router.push('/')
              }
            }

          })
          .catch(e => {
            console.log(e.response)
            if(e && e.response){

              if(e.response.data){
                this.errors = e.response.data[0]
                this.showError = true
              }else if(e.response.statusText){
                console.log('not undefined')
                this.errors = e.response.statusText
                this.showError = true
              }
              else{
                this.errors = "Error While Logging In!"
              }
            }
            else{
              this.errors = e
            }
            this.showError = true
        })
        this.loading.sending = false
        // this.clearForm()

      },
      formSubmit(){
        //validating form
        this.$v.$touch()
        // run recaptcha and let it to fire up 'logUserIn' function with google token
        if (!this.$v.$invalid){
          this.$refs.recaptcha.execute();
        }
      },
      onCaptchaExpired: function () {
        this.$refs.recaptcha.reset();
      }
    },
  }
</script>

<style lang="scss" scoped>
  .card.card-signup {
    display: inline-block;
    position: relative;
    width: 100%;
    margin-bottom: 30px;
    border-radius: 6px;
    color: rgba(0,0,0, 0.87);
    background: #fff;
    box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
    .footer {
      margin-top: 15px;
      padding-bottom: 15px;
      a {
        background-color: transparent;
        color: #9c27b0;
        padding: 18px 5px;
        display: inline-block;
      }
    }
    .card-content {
      padding: 0px 25px 0px 60px;
      .input-with-icon {
        position: relative;
        .icon-prefix {
          position: absolute;
          left: -35px;
          top: 21px;
          font-size: 24px;
          color: #757575;
        }
        .md-input {

        }
      }
    }
    .header {
      box-shadow: 0 16px 38px -12px rgba(0, 0, 0, 0.56), 0 4px 25px 0px rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
      margin-left: 20px;
      margin-right: 20px;
      margin-top: -40px;
      padding: 20px 0;
      border-radius: 3px;
      position: relative;
      .md-progress-bar{
        position: absolute;
        bottom: 0;
        left: 0;
        right: 0;
        border-bottom-right-radius:3px ;
        border-bottom-left-radius:3px ;
        overflow: hidden;
        background-color: rgba(255, 255, 255, 0.50);

      }
      .md-progress-bar.md-theme-default.md-indeterminate .md-progress-bar-track:after, .md-progress-bar.md-theme-default.md-indeterminate .md-progress-bar-fill:after, .md-progress-bar.md-theme-default.md-query .md-progress-bar-track:after, .md-progress-bar.md-theme-default.md-query .md-progress-bar-fill:after {
        background-color: #92278f66 !important;
      }
    }
    .header-primary, .content-primary {
      background: linear-gradient(60deg, #AB47BC, #7b1fa2);
      .card-title {
        color: #fff;
        font-weight: 700;
        font-family: "Roboto Slab";
      }
    }
    .social-line {
      margin-top: 15px;
      text-align: center;
      padding: 0;
    }
    .btn-just-icon {
      .mdi {
        color: #fff;
        font-size: 20px;
      }
    }

  }

</style>
