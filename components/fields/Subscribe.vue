<template>
  <div>
    <transition name="fade">
      <div
        v-if="modalActive"
        class="modal-mask"
        @click="hideSubscribe($event)"
      >
        <div
          class="modal"
          tabindex="-1"
          role="dialog"
        >
          <div
            class="modal-dialog"
            role="document"
          >
            <div class="modal-content">
              <div class="modal-header">
                <h5 class="modal-title">
                  Subscribe to Newsletter
                </h5>
                <md-button
                  class="md-icon-button"
                  @click="modalActive=false"
                >
                  <md-icon>close</md-icon>
                </md-button>
              </div>
              <div class="modal-body">
                <p>Join our newsletter and get news in your inbox every week! We hate spam too, so no worries about this.</p>
              </div>
              <div>
                <md-field
                  class="md-field"
                  :class="getValidationClass('fullName')"
                >
                  <md-icon>person</md-icon>
                  <label>Full Name</label>
                  <md-input
                    v-model="form.fullName"
                    :disabled="submiting"
                  />
                  <span
                    v-if="!$v.form.fullName.required"
                    class="md-error"
                  >
                    Full name is required
                  </span>
                </md-field>
                <md-field
                  class="md-field"
                  :class="getValidationClass('email')"
                >
                  <md-icon>email</md-icon>
                  <label>Your Email</label>
                  <md-input
                    v-model="form.email"
                    :disabled="submiting"
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
                <vue-recaptcha
                  ref="recaptcha"
                  size="invisible"
                  sitekey="6Ldp3XMUAAAAACZav47_l9to_2uESNGLa1RvQOU6"
                  @verify="subscribeUser"
                  @expired="onCaptchaExpired"
                />
              </div>
              <div class="modal-footer">
                <md-button
                  class="md-raised"
                  @click="modalActive=false"
                >
                  CLOSE
                </md-button>
                <md-button
                  class="md-raised md-primary"
                  :disabled="submiting"
                  @click="validateForm"
                >
                  SUBSCRIBE
                </md-button>
              </div>
            </div>
          </div>
        </div>
      </div>
    </transition>
  </div>
</template>

<script>
  import { cookie } from '@/components/mixins/cookie.js'
  import axios from '@/node_modules/axios'
  import VueRecaptcha from 'vue-recaptcha'
  import { validationMixin } from 'vuelidate'
  import {required, email } from 'vuelidate/lib/validators'

    export default {
      name: "Subscribe",
      components: {
        'vue-recaptcha': VueRecaptcha
      },
      mixins: [validationMixin,cookie],
      data() {
        return {
          modalActive: false,
          aftersubmit:false,
          inputBox:'',
          submiting: false,
          form: {
            email: '',
            fullName: ''
          }
        }
      },
      validations: {
        form: {
          email: {
            required,
            email
          },
          fullName: {
            required,
          },
        }
      },
      mounted() {
        if(!this.$store.getters.getUid) {
          if (this.getCookie("subscription") == null){
            this.modalActive = true;
            this.setCookie("subscription", 1, 30);
          }
        }
      },
      methods:{
        hideSubscribe(event){
          if(event.target.getAttribute('class') === 'modal-mask'){
            this.modalActive = false;
          }
        },
        validateForm(){
          console.log(this.$v)
          //run recaptcha and let it to call 'saveUser' function with google token
          if (!this.$v.$invalid) {
            this.$refs.recaptcha.execute();
          }
        },
        onCaptchaExpired: function () {
          // reset recaptcha when expired
          this.$refs.recaptcha.reset();
        },
        getValidationClass (fieldName) {
          // set invalid class for invalid fields
          const field = this.$v.form[fieldName];
          if (field) {
            return {
              'md-invalid': field.$invalid && field.$dirty
            }
          }
        },
        subscribeUser(recaptchaToken) {
          // Register User in DataBase
          this.submiting = true;
          this.$refs.recaptcha.reset();
          console.log(recaptchaToken)
          axios.post('https://temp.ed808.com/api/latin/web_form',
            {
              "nid" : 20162,
              "reCaptchaToken" : recaptchaToken,
              "name" : this.form.fullName,
              "email" : this.form.email
            },
            {
              headers: {
                'Content-type': 'application/json'
              }
            })
            .then(() => {
              this.aftersubmit = true;
              this.inputBox = '';
              this.submiting = false;
              this.modalActive = false
            })
            .catch(e => {
              this.submiting = false;
            });
        },
      }
    }
</script>

<style lang="scss" scoped>
  .modal {
    position: relative;
    width: 35%;
    margin: auto;
    z-index: 9997;
    outline: 0;
    margin-top: 130px;
    @media screen and (max-width: 900px){
      width: 60%;
      margin-top: 50px;
    }
    @media screen and (max-width: 600px) {
      width: 90%;
      margin-top: 30px;
    }
  }
  .modal, .modal-open {
    overflow: hidden;
  }
  .modal-mask {
    position: fixed;
    z-index: 9998;
    top: 0;
    left: 0;
    width: 100vw;
    height: 100vh;
    background-color: rgba(0, 0, 0, .5);
    display: table;
    transition: opacity .3s ease;
  }
  .modal-dialog .modal-content {
    box-shadow: 0 27px 24px 0 rgba(0,0,0,.2), 0 40px 77px 0 rgba(0,0,0,.22);
    border-radius: 6px;
    border: none;
  }
  .fade-enter-active,
  .fade-leave-active {
    transition: opacity .5s
  }
  .fade-enter,
  .fade-leave-active {
    opacity: 0
  }
  .modal-content {
    position: relative;
    margin: auto;
    display: flex;
    flex-direction: column;
    width: 100%;
    pointer-events: auto;
    background-color: #fff;
    background-clip: padding-box;
    border: 1px solid rgba(0,0,0,.2);
    border-radius: .3rem;
    box-shadow: 0 0.25rem 0.5rem rgba(0,0,0,.5);
    outline: 0;
  }
  .md-field {
    width: 88%;
    min-height: 48px;
    margin: 4px 0 24px 22px;
    padding-top: 16px;
    display: flex;
    position: relative;
    font-family: inherit;
  }
  .modal-dialog .modal-header {
    border-bottom: none;
    padding: 24px 24px 0;
  }
  .modal-header {
    display: flex;
    align-items: flex-start;
    justify-content: space-between;
    padding: 1rem;
    border-bottom: 1px solid #e9ecef;
    border-top-left-radius: .3rem;
    border-top-right-radius: .3rem;
  }
  .modal-dialog .modal-body {
    padding: 24px 24px 16px;
  }
  .modal-body {
    position: relative;
    flex: 1 1 auto;
    padding: 1rem;
  }
  .modal-dialog .modal-body+.modal-footer {
    padding-top: 0;
  }
  .modal-dialog .modal-footer {
    border-top: none;
    padding: 24px;
  }
  .modal-footer {
    display: flex;
    align-items: center;
    justify-content: flex-end;
    padding: 1rem;
    border-top: 1px solid #e9ecef;
  }
  .form-newsletter .input-group, .form-newsletter .form-group {
    float: left;
    width: 78%;
    margin-right: 2%;
    margin-top: 9px;
  }
  .form-group {
    padding-bottom: 7px;
    margin: 27px 0 0 0;
  }
  .form-group {
    position: relative;
  }
  .form-group {
    margin-bottom: 15px;
  }
  .form-group .form-control {
    margin-bottom: 7px;
  }
  .form-control, .form-group .form-control {
    border: 0;
    background-image: linear-gradient(#9c27b0, #9c27b0), linear-gradient(#D2D2D2, #D2D2D2);
    background-size: 0 2px, 100% 1px;
    background-repeat: no-repeat;
    background-position: center bottom, center calc(100% - 1px);
    background-color: transparent;
    transition: background 0s ease-out;
    float: none;
    box-shadow: none;
    border-radius: 0;
    font-weight: 400;
  }
  .form-control {
    margin-bottom: 7px;
  }
  .form-control {
    height: 36px;
    padding: 7px 0;
    font-size: 14px;
    line-height: 1.42857;
  }
  .form-control {
    display: block;
    width: 100%;
    height: 34px;
    padding: 6px 12px;
    font-size: 14px;
    line-height: 1.42857143;
    color: #555;
    background-color: #fff;
    background-image: none;
    border: 1px solid #ccc;
    border-radius: 4px;
    -webkit-box-shadow: inset 0 1px 1px rgba(0,0,0,.075);
    box-shadow: inset 0 1px 1px rgba(0,0,0,.075);
    -webkit-transition: border-color ease-in-out .15s,-webkit-box-shadow ease-in-out .15s;
    -o-transition: border-color ease-in-out .15s,box-shadow ease-in-out .15s;
    transition: border-color ease-in-out .15s,box-shadow ease-in-out .15s;
  }
  .bd-example-modal .modal-dialog {
    left: auto;
    margin-right: auto;
    margin-left: auto;
  }
    .modal-dialog {
      position: relative;
      width: auto;
      margin: .5rem;
      pointer-events: none;
    }
  .modal-footer button {
    margin: 0 11px;
    padding-left: 16px;
    padding-right: 16px;
    width: auto;
  }
  .md-icon-button{
    margin-top: 3px;
  }
  .modal-title{
    margin-left: 12px;
    margin-top: 10px;
    font-size: medium;
  }
  .md-primary {
    color: #fff;
    background-color: #9c27b0;
    border-color: #9c27b0;
    box-shadow: 0 2px 2px 0 rgba(156,39,176,.14), 0 3px 1px -2px rgba(156,39,176,.2), 0 1px 5px 0 rgba(156,39,176,.12);
  }
</style>
