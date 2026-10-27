<!--Done-->
<template>
  <div id="linkedin_form">

    <div v-if="!register">
      <h2>Please Wait ...</h2>
    </div>

    <div class="login-card-page password-set" v-if="register == 1">
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
              <label for="password">Password</label>
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
            <div :class="getValidationClass('rules')">
              <md-checkbox v-model="form.rules" name="rules" required @change="$v.form.rules.$touch()">
                I accept the
                Terms of Use
                and
                <nuxt-link to="/privacy">
                  Privacy Policy
                </nuxt-link>
                .
              </md-checkbox>
              <span class="checkbox-error" v-if="rulesErr">
                You should accept the rules for registration.
              </span>
            </div>
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



    <interested-fields v-if="register == 2"></interested-fields>

    <md-snackbar :md-active.sync="loggedIn">
      Dear {{ user.username }}, You have successfully logged in.
    </md-snackbar>
    <md-snackbar
      class="error"
      :md-active.sync="showError"
    >
      {{ errors }}
    </md-snackbar>


  </div>
</template>

<script>
  import axios from '@/node_modules/axios'
  import { cookie } from '@/components/mixins/cookie.js'
  import InterestedFields from "@/components/fields/InterestedFields";
  import { validationMixin } from 'vuelidate'
  import {
    required,
    minLength,
    sameAs
  } from 'vuelidate/lib/validators'
  axios.defaults.crossDomain = true;
  axios.defaults.withCredentials  = true;

  export default {
    name: 'Linkedin',
    components: {InterestedFields},
    mixins: [cookie,validationMixin],
    layout: 'static',
    middleware: 'isauth',
    data() {
      return {
        code: '',
        user: {},
        register: false,
        sending: false,
        rulesErr: false,
        showError: false,
        errors: false,
        loggedIn:false,
        form: {
          password: null,
          rePassword: null,
          rules: false
        },
      }
    },
    validations: {
      form: {
        password: {
          required,
          minLength: minLength(6)
        },
        rePassword: {
          required,
          sameAsPassword: sameAs('password')
        },
        rules: {
          required
        }
      }
    },
    mounted() {
      // set page background
      this.$store.state.backGround = '/images/login-bg.jpg'
      // Checking If Login is successful or not
      if(this.$route.query.hasOwnProperty('uid')) {

        // Set Token in Cookies
        this.setCookie('token', this.$route.query.token , 23)

        // Get User Data For Updating Navbar
        axios.get('https://ed808.com:92/latin/user/login/nav_bar_info', {
          headers: {
            'Content-Type': 'application/json',
          }
        }).then((data)=>{
          this.loggedIn = true;
          //Set User Data in Store
          this.$store.commit('SET_USER_DATA', data.data)
          this.user = data.data
        }).catch((e)=>{
          // handling errors if request interrupted
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

        // Check If User Is Registered Or Not => if true => User Must Set Password and Interested Fields
        if(this.$route.query.action === 'register'){
          this.register = 1
        }else {
          // if false => call login and push user to desired url
          // Check Callback URL For Handling
          if (localStorage.getItem('callback')){
            setTimeout(()=>{
              this.$router.push(localStorage.getItem('callback'))
            },500)
          }else{
            setTimeout(()=>{
              this.$router.push('/')
            },500)
          }
        }
      }
    },
    methods: {

      // Validating Password Form
      getValidationClass (fieldName) {
        const field = this.$v.form[fieldName]
        if (field) {
          return {
            'md-invalid': field.$invalid && field.$dirty
          }
        }
      },

      // Set Password By Registered User
      setPassword(){
        this.$v.$touch()
        if(!this.form.rules){
          this.rulesErr = true
        }

        if (!this.$v.$invalid && this.form.rules) {
          this.sending = true
          this.rulesErr = false
          axios.put('https://ed808.com:92/latin/user/'+this.$store.state.user.uid,
            {
              "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "from_linkedin" : true,
              "password" : this.form.password,
              "laws" : 1 // user accepted rules & privacy policies
            },
            {
              headers: {
                'Content-type': 'application/json',
                'X-CSRF-Token': this.getCookie("token"),

              }
            }
          )
            .then((data)=>{
              console.log(data)
              // Go To Interested Fields
              this.register = 2
            })
            .catch(e =>{
              console.log(e)
            })
        }
      }
    }

  }
</script>

<style lang="scss" scoped>
  #linkedin_form {
    padding: 100px 0;
    h2 {
      color: white;
      margin-top: calc(50vh - 100px);
    }
    span.checkbox-error {
      color: #ff1744;
      font-size: 12px;
      display: block;
    }
  }
</style>
