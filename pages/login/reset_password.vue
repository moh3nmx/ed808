<template>
  <div>
    <div class="login-card-page password-set" v-if="resetRequest" style="margin-top: 100px">
      <div class="card card-signup card-topics">
        <p>Please enter your email address:</p>
        <div class="card-content">
          <form
            id="submitEmail"
            novalidate
            @submit.prevent="formSubmit"
          >
            <md-field :class="getValidationClass('email')" class="input-with-icon">
              <span class="icon-prefix mdi mdi-email"></span>
              <label for="email">Email</label>
              <md-input type="email" id="email" name="email" v-model="form.email" :disabled="sending" required/>
              <span class="md-error" v-if="!$v.form.email.required">Email is required</span>
              <span class="md-error" v-else-if="!$v.form.email.email">Incorrect Email Address</span>
            </md-field>
            <!--On Recaptcha verification => SaveUser Will Be Ran              -->
            <!--On Recaptcha expiration => onCaptchaExpired Will Be Ran              -->
            <vue-recaptcha
              ref="recaptcha"
              size="invisible"
              sitekey="6Ldp3XMUAAAAACZav47_l9to_2uESNGLa1RvQOU6"
              @verify="requestResetPassword"
              @expired="onCaptchaExpired"
            />
          </form>
        </div>
        <div class="footer text-center" style="float: left; width: 100%">
          <md-button
            form="submitEmail"
            type="submit"
            class="md-primary"
            :disabled="sending || $v.form.email.$invalid"
          >
            Submit
          </md-button>
        </div>
      </div>
    </div>

    <div class="login-card-page password-set" v-else-if="isValid" style="margin-top: 100px">
      <div class="card card-signup card-topics">
        <p>Please choose a password:</p>
        <div class="card-content">
          <form
            id="registerForm"
            novalidate
            @submit.prevent="setPassword()"
          >
            <md-field :class="getValidationClass('password')" class="input-with-icon">
              <span class="icon-prefix mdi mdi-lock"></span>
              <label for="password">New Password</label>
              <md-input type="password" id="password" name="password" v-model="form.password" autocomplete="new-password" :disabled="sending" required/>
              <span class="md-error" v-if="!$v.form.password.required">The Password is required</span>
              <span class="md-error" v-else-if="!$v.form.password.minLength">Should be at least 6 character</span>
            </md-field>

            <md-field :class="getValidationClass('rePassword')" class="input-with-icon">
              <span class="icon-prefix mdi mdi-lock"></span>
              <label for="rePassword">Password Confirmation</label>
              <md-input type="password" id="rePassword" name="rePassword" v-model="form.rePassword" autocomplete="new-password" :disabled="sending" required/>
              <span class="md-error" v-if="!$v.form.rePassword.required">The Password Confirmation is required</span>
              <span class="md-error" v-else-if="!$v.form.rePassword.sameAsPassword">Password is not the same</span>
            </md-field>
          </form>
        </div>
        <div class="footer text-center" style="float: left; width: 100%">
          <md-button
            form="registerForm"
            type="submit"
            class="md-primary"
            :disabled="sending"
          >
            Submit
          </md-button>
        </div>
      </div>
    </div>

    <md-snackbar
      class="error"
      :md-active.sync="showError"
    >
      Error While Processing!
    </md-snackbar>

    <md-snackbar

      :md-active.sync="isSent"
    >
      Your request submitted successfully. Please check your email.
    </md-snackbar>
  </div>
</template>

<script>
  import { validationMixin } from 'vuelidate'
  import {
    required,
    email,
    minLength,
    sameAs
  } from 'vuelidate/lib/validators'
  import VueRecaptcha from 'vue-recaptcha'
  import axios from 'axios'
  import { cookie } from '@/components/mixins/cookie.js'

  export default {
    name: "ResetPassword",
    layout: 'static',
    components: {
      'vue-recaptcha': VueRecaptcha
    },
    mixins: [validationMixin,cookie],
    data(){
      return {
        form : {
          email: null,
          password: null,
          rePassword: null,
        },
        resetRequest: true,
        isValid: false,
        sending : false,
        isSent: false,
        showError: false,
        data: null
      }
    },
    validations: {
      form: {
        email: {
          required,
          email
        },
        password: {
          required,
          minLength: minLength(6)
        },
        rePassword: {
          required,
          sameAsPassword: sameAs('password')
        },
      }
    },
    async asyncData({res, req, query, store, error}) {
      store.state.backGround = '/images/login-bg.jpg'
      if (query.hasOwnProperty('token')) {
        try {
          const {data} = await axios.post('https://temp.ed808.com/api/latin/user/login_with_link', {
              "token": query.token
            }, {
              headers: {
                'Content-type': 'application/json',
              }
            }
          )
          if (data) {
            store.commit('SET_USER_DATA',data)
            return {
              data: data,
              token: query.token,
              resetRequest: false,
              isValid: data
            }
          }
          // else {
          //   error({ statusCode: 403, message: 'Forbidden' })
          // }

        }
        catch (e) {
          console.log(e);
          // if (e.response.hasOwnProperty('status')) {
          //   error({ statusCode: e.response.status, message: e.response.statusText })
          // }
        }
      }
    },
    mounted(){
      if(this.isValid) {
        this.setCookie('token', this.data.token , 23)
      }
      // let query = this.$route.query
      // axios.post('https://temp.ed808.com/api/latin/user/login_with_link', {
      //         "token": query.token
      //       }, {
      //         headers: {
      //           'Content-type': 'application/json',
      //         }
      //       }
      //     )
      //     .then((data) => {
      //       console.log(data);
      //     })
    },
    methods :{
      // Validating Password Form
      getValidationClass (fieldName) {
        const field = this.$v.form[fieldName]
        if (field) {
          return {
            'md-invalid': field.$invalid && field.$dirty
          }
        }
      },
      // Refresh Expired Recaptcha
      onCaptchaExpired: function () {
        this.$refs.recaptcha.reset();
      },
      // Request to server for resetting password
      requestResetPassword(recaptchaToken){
        this.showError= false
        this.$v.$touch()
        if (!this.$v.form.email.$invalid) {
          axios.post('https://temp.ed808.com/api/latin/user/password_reset', {
              "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "reCaptchaToken" : recaptchaToken,
              "email" : this.form.email
            },{
              headers: {
                'Content-type': 'application/json',
              }
            }
          ).then((data)=>{
            this.isSent = true

          }).catch((err) => {
            this.showError= true
          })
        }
      },
      // Set Password By Registered User
      setPassword(){
        this.showError= false
        this.$v.$touch()
        if (!this.$v.form.password.$invalid && !this.$v.form.rePassword.$invalid) {
          this.sending = true
          this.rulesErr = false
          axios.put('https://temp.ed808.com/api/latin/user/'+this.$store.state.user.uid,
            {
              "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "request_pass" : true,
              "password" : this.form.password,
            }, {
              headers: {
                'Content-type': 'application/json',
                'X-CSRF-Token': this.$store.state.user.token
              }
            }
          )
            .then((data)=>{
              // Go To Interested Fields
              this.register = 2
            })
            .catch(e =>{
              this.showError= true
            })
        }
      },
      formSubmit(){
        //validating form
        this.$v.$touch()
        // run recaptcha and let it to fire up 'logUserIn' function with google token
        if (!this.$v.form.email.$invalid){
          this.$refs.recaptcha.execute();
        }
      },
    }
  }
</script>

<style scoped>

</style>
