<!--Done-->
<template>
  <div class="with-background">
    <transition name="fade">
      <interest-fields v-if="registered"></interest-fields>
    </transition>
    <transition name="fade">
      <div class="login-card-page"  v-if="!registered">
        <div class="card card-signup">
          <div class="header header-primary text-center">

            <h4 class="card-title">Register in with</h4>
            <div class="social-line">
              <a class="btn btn-just-icon btn-simple" @click="loginWithLinkedin()">
                <md-button class="md-icon-button">
                  <i class="mdi mdi-linkedin-box"></i>
                </md-button>
                <md-tooltip>Linkedin</md-tooltip>
              </a>
              <md-progress-bar
                v-if="sending"
                md-mode="indeterminate"
              />
            </div>
          </div>
          <p>Or register with your personal data</p>
          <div class="card-content">
            <!--prevent Submit and run formSubmit function on Submit-->
            <form
              novalidate
              @submit.prevent="formSubmit"
              id="registerForm"
            >
              <!-- each field hast validation class that is set by vuelidate -->
              <md-field
                :class="getValidationClass('fullName')"
                class="input-with-icon"
              >
                <label for="fullName">
                  Full Name
                </label>
                <span class="icon-prefix mdi mdi-account-card-details"></span>
                <md-input
                  name="fullName"
                  id="fullName"
                  autocomplete="given-name"
                  v-model="form.fullName"
                  :disabled="sending"
                  required
                />
                <span
                  v-if="!$v.form.fullName.required"
                  class="md-error"
                >
                  The full name is required
                </span>
                <span
                  v-else-if="!$v.form.fullName.minLength"
                  class="md-error"
                >
                  Should be at least 3 character
                </span>
              </md-field>
              <!-- each field hast validation class that is set by vuelidate -->
              <md-field
                :class="getValidationClass('email')"
                class="input-with-icon"
              >
                <label for="email">
                  Email
                </label>
                <span class="icon-prefix mdi mdi-email"></span>
                <md-input
                  id="email"
                  v-model="form.email"
                  type="email"
                  name="email"
                  autocomplete="email"
                  :disabled="sending"
                  required
                />
                <span
                  v-if="!$v.form.email.required"
                  class="md-error"
                >
                  The email is required
                </span>
                <span
                  v-else-if="!$v.form.email.email"
                  class="md-error"
                >
                  Invalid email
                </span>
              </md-field>
              <!-- each field hast validation class that is set by vuelidate -->
              <md-field
                :class="getValidationClass('username')"
                class="input-with-icon"
              >
                <label for="username">
                  Username
                </label>
                <span class="icon-prefix mdi mdi-account-circle"></span>
                <md-input
                  id="username"
                  v-model="form.username"
                  name="username"
                  autocomplete="given-name"
                  :disabled="sending"
                  required
                />
                <span
                  v-if="!$v.form.username.required"
                  class="md-error"
                >
                  The Username is required
                </span>
                <!--<span class="md-error" v-else-if="!$v.form.mobile.taken">The User is already taken</span>-->
              </md-field>
              <!-- each field hast validation class that is set by vuelidate -->
              <md-field
                :class="getValidationClass('password')"
                class="input-with-icon"
              >
                <span class="icon-prefix mdi mdi-lock"></span>
                <label for="password">
                  Password
                </label>
                <md-input
                  id="password"
                  v-model="form.password"
                  type="password"
                  name="password"
                  autocomplete="new-password"
                  :disabled="sending"
                  required
                />
                <span
                  v-if="!$v.form.password.required"
                  class="md-error"
                >
                  The Password is required
                </span>
                <span
                  v-else-if="!$v.form.password.minLength"
                  class="md-error"
                >
                  Should be at least 6 character
                </span>
              </md-field>
              <!-- each field hast validation class that is set by vuelidate -->
              <md-field
                :class="getValidationClass('rePassword')"
                class="input-with-icon"
              >
                <span class="icon-prefix mdi mdi-lock"></span>
                <label for="rePassword">
                  Password Confirmation
                </label>
                <md-input
                  id="rePassword"
                  v-model="form.rePassword"
                  type="password"
                  name="rePassword"
                  autocomplete="new-password"
                  :disabled="sending"
                  required
                />
                <span
                  v-if="!$v.form.rePassword.required"
                  class="md-error"
                >
                  The Password Confirmation is required
                </span>
                <span
                  v-else-if="!$v.form.rePassword.sameAsPassword"
                  class="md-error"
                >
                  Password is not the same
                </span>
              </md-field>

              <!--On Recaptcha verification => SaveUser Will Be Ran              -->
              <!--On Recaptcha expiration => onCaptchaExpired Will Be Ran              -->
              <vue-recaptcha
                ref="recaptcha"
                size="invisible"
                sitekey="6Ldp3XMUAAAAACZav47_l9to_2uESNGLa1RvQOU6"
                @verify="saveUser"
                @expired="onCaptchaExpired"
              />

              <!-- Snackbar for showing success or error in registration -->
              <md-snackbar
                :md-active.sync="userSaved"
              >
                {{ lastUser }} you are registered successfully!
              </md-snackbar>
              <md-snackbar
                class="error"
                :md-active.sync="showError"
              >
                {{ errors }}
              </md-snackbar>

            </form>
          </div>
          <div class="footer text-center">
            <md-button
              form="registerForm"
              type="submit"
              class="md-primary"
              :disabled="sending"
            >
              Join
            </md-button>
          </div>
        </div>
      </div>
    </transition>
  </div>
</template>

<script>



import axios from '@/node_modules/axios'
import { validationMixin } from 'vuelidate'
import {
  required,
  email,
  minLength,
  sameAs
} from 'vuelidate/lib/validators'
import VueRecaptcha from 'vue-recaptcha'
import { cookie } from '@/components/mixins/cookie.js'
import InterestFields from "../components/fields/InterestedFields";

export default {
  name: 'Register',
  scrollToTop: true,
  layout: 'static',
  middleware: ['isauth'], // if user is logged in => cant see this page
  components: {
    InterestFields,
    'vue-recaptcha': VueRecaptcha
  },
  mixins: [validationMixin,cookie],
  data: () => ({
    errors:'',
    showError:false,
    registered: false,

    form: {
      fullName: null,
      email: null,
      username: null,
      password: null,
      rePassword: null,
    },
    userSaved: false,
    sending: false,
    lastUser: null
  }),
  //Vuelidate Validation Options & Configs
  validations: {
    form: {
      fullName: {
        required,
        minLength: minLength(3)
      },
      email: {
        required,
        email
      },
      username: {
        required,
        //taken
      },
      password: {
        required,
        minLength: minLength(6)
      },
      rePassword: {
        required,
        sameAsPassword: sameAs('password')
      }
    }
  },
  mounted(){
    // Set Page Background
    this.$store.state.backGround = '/images/login-bg.jpg'
  },
  methods: {
    // Go To LinkedIn Login URL
    loginWithLinkedin(){
      if(!this.pageComponent) {
        localStorage.setItem('callback', this.$route.path)
      }
      window.location.replace(
        'https://www.linkedin.com/oauth/v2/authorization?response_type=code&client_id=77gasbcb9ngw6d&redirect_uri=https://ed808.com:92/linkedin/join&scope=r_emailaddress r_liteprofile'
      )
    },
    // Set Invalid Class For Invalid Inputs
    getValidationClass (fieldName) {
      const field = this.$v.form[fieldName]

      if (field) {
        return {
          'md-invalid': field.$invalid && field.$dirty
        }
      }
    },
    // Refresh Register Form
    clearForm () {
      this.$v.$reset()
      this.form.fullName = null
      this.form.email = null
      this.form.username = null
      this.form.password = null
      this.form.rePassword = null
      this.showError = false
      this.errors = ''
    },
    // Main Function For Registration
    async saveUser(recaptchaToken) {
      this.sending = true
      this.$refs.recaptcha.reset();
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      await axios.post('https://ed808.com:92/latin/user/register',
        {
          hash : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
          username : this.form.username,
          password : this.form.password,
          email : this.form.email,
          full_name : this.form.fullName,
          reCaptchaToken : recaptchaToken
        },
        {
          headers: {
            'Content-type': 'application/json',
          }
        })
        .then((data) => {
          //set uid in store
          this.$store.commit('SET_USER',data.data.uid)
          this.$store.commit('SET_USER_DATA', data.data)
          //set token in cookie
          this.setCookie('token', data.data.token , 23)
          this.lastUser = `${this.form.fullName}`
          this.userSaved = true
          this.sending = false
          // redirect after successfull login
          this.registered = true
          // window.location.reload()
        })
        .catch(e => {
          if(e.hasOwnProperty('response')){
            if(e.response.hasOwnProperty('data'))
              this.errors = e.response.data
            else{
              this.errors = e.response
            }
          }
          else{
            this.errors = e
          }
          this.showError = true
        })
      this.clearForm()
    },
    // This Functions Runs Some Preprocessing Func Before Submit
    formSubmit() {
      // Validating Form
      this.$v.$touch()
      // run recaptcha and let it to fire up 'saveUser' function with google token
      if (!this.$v.$invalid) {
        this.$refs.recaptcha.execute();
      }
    },
    // Refresh Expired Recaptcha
    onCaptchaExpired: function () {
      this.$refs.recaptcha.reset();
    },
  }
}

</script>


