<template>
  <div class="main-container">
    <section>
      <div>
        <h1>Contact Us</h1>
        <div class="text">
          <p v-if="!component">
            We are ready to lead you into the future of Civil Engineering and Construction Industry. Get in touch with experts and share your experience.
            Use the form below to drop us an email.
          </p>
          <p v-else>{{ altText }}</p>
          <div class="row" v-if="!component">
            <md-icon>place</md-icon>
            <span>
              <b>Address:</b>
              4, Stapleford Close, Kingston Upon Thames, United Kingdom, KT1 3HL
            </span>
          </div>
          <div class="row" v-if="!component">
            <md-icon>email</md-icon>
            <span>
              <b>Email:</b>
              Info[at]ed808.com
            </span>
          </div>
        </div>
      </div>
      <form novalidate class="md-layout" @submit.prevent="validateUser">
        <md-card class="md-layout-item md-size-100 md-small-size-100">
          <md-card-header>
            <div class="md-title">Get in touch</div>
          </md-card-header>

          <md-card-content>
            <div class="md-layout md-gutter">
              <div class="md-layout-item md-small-size-100">
                <md-field :class="getValidationClass('fullName')">
                  <label for="first-name">Full Name</label>
                  <md-input
                    id="first-name"
                    v-model="form.fullName"
                    name="full-name"
                    autocomplete="given-name"
                    :disabled="sending"
                  />
                  <span v-if="!$v.form.fullName.required" class="md-error">The full name is required</span>
                  <span v-else-if="!$v.form.fullName.minlength" class="md-error">Invalid full name</span>
                </md-field>
              </div>
            </div>

            <div class="md-layout md-gutter">
              <div class="md-layout-item md-small-size-100">
                <md-field :class="getValidationClass('email')">
                  <label for="email">Email</label>
                  <md-input
                    id="email"
                    v-model="form.email"
                    type="email"
                    name="email"
                    autocomplete="email"
                    :disabled="sending"
                  />
                  <span v-if="!$v.form.email.required" class="md-error">The email is required</span>
                  <span v-else-if="!$v.form.email.email" class="md-error">Invalid email</span>
                </md-field>
              </div>
            </div>

            <div class="md-layout md-gutter">
              <div class="md-layout-item md-small-size-100">
                <md-field :class="getValidationClass('phone')">
                  <label for="phone">Phone</label>
                  <md-input id="phone" v-model="form.phone" name="phone" :disabled="sending" />
                  <span
                    v-if="!$v.form.phone.numeric"
                    class="md-error"
                  >The phone number must be numeric</span>
                  <span v-else-if="!$v.form.phone.minlength" class="md-error">Invalid phone number</span>
                </md-field>
              </div>
            </div>

            <div class="md-layout md-gutter">
              <div class="md-layout-item md-small-size-100">
                <md-field :class="getValidationClass('company')">
                  <label for="company">Company</label>
                  <md-input
                    id="company"
                    v-model="form.company"
                    name="company"
                    autocomplete="given-name"
                    :disabled="sending"
                  />
                  <span v-if="!$v.form.company.minlength" class="md-error">Invalid company name</span>
                </md-field>
              </div>
            </div>

            <div class="md-layout md-gutter">
              <div class="md-layout-item md-small-size-100">
                <md-field :class="getValidationClass('description')">
                  <label for="description">Description</label>
                  <md-textarea
                    id="description"
                    v-model="form.description"
                    name="description"
                    :disabled="sending"
                  />
                  <span v-if="!$v.form.description.required" class="md-error">is required</span>
                  <span
                    v-else-if="!$v.form.description.minlength"
                    class="md-error"
                  >Invalid description</span>
                </md-field>
              </div>
            </div>
          </md-card-content>

          <md-progress-bar v-if="sending" md-mode="indeterminate" />

          <md-card-actions>
            <md-button type="submit" class="md-primary" :disabled="sending">Send</md-button>
          </md-card-actions>
        </md-card>

        <md-snackbar
          :md-active.sync="userSaved"
        >Dear {{ lastUser }}, your information has been submited successfully!</md-snackbar>
      </form>
    </section>
  </div>
</template>

<script>
import axios from "@/node_modules/axios";
import { validationMixin } from "vuelidate";
import { required, email, minLength, numeric } from "vuelidate/lib/validators";
import { cookie } from "@/components/mixins/cookie.js";
export default {
  name: "ContactForm",
  mixins: [validationMixin, cookie],
  props: ["altText", "component"],
  data: () => ({
    form: {
      fullName: null,
      email: null,
      phone: null,
      company: null,
      description: null
    },
    userSaved: false,
    sending: false, // Data is Sending or not
    lastUser: null
  }),
  // Validation Configs for Vuelidate
  validations: {
    form: {
      fullName: {
        required,
        minLength: minLength(3)
      },
      phone: {
        numeric,
        minLength: minLength(3)
      },
      company: {
        minLength: minLength(2)
      },
      email: {
        required,
        email
      },
      description: {
        required,
        minLength: minLength(3)
      }
    }
  },
  mounted() {},
  methods: {
    // Set Invalid Class For Invalid Fields
    getValidationClass(fieldName) {
      const field = this.$v.form[fieldName];
      if (field) {
        return {
          "md-invalid": field.$invalid && field.$dirty
        };
      }
    },

    // Refresh Form Data
    clearForm() {
      this.$v.$reset();
      this.form.fullName = null;
      this.form.email = null;
    },

    // Sends Contact Form & Save In DB
    saveUser() {
      this.sending = true;
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      let headers = {};
      if (this.$store.getters.getUid) {
        headers = {
          "Content-Type": "application/json",
          "X-CSRF-Token": this.getCookie("token")
        };
      } else {
        headers = {
          "Content-Type": "application/json"
        };
      }
      axios
        .post(
          "https://temp.ed808.com/api/latin/web_form",
          {
            nid: 19768,
            reCaptchaToken: "admin@ed808",
            name: this.form.fullName,
            email: this.form.email,
            phone: this.form.phone,
            company: this.form.company,
            description: this.form.description
          },
          {
            headers: headers
          }
        )
        .then(data => {
          this.lastUser = `${this.form.fullName}`;
          this.userSaved = true;
          this.sending = false;
          this.clearForm();
        })
        .catch(e => {
          console.log("FAILURE!!" + e);
        });
    },

    // Validating Data Inputs
    validateUser() {
      this.$v.$touch();
      if (!this.$v.$invalid) {
        this.saveUser();
      }
    }
  }
};
</script>

<style lang="scss" scoped>
@import "@/assets/scss/vars.scss";
.md-progress-bar {
  @include absolute("", "", 0, 0, "", 0);
}
.main-container {
  min-height: 100vh;
  /*background-image: url(/images/295.jpg);*/
  /*background-position: center;*/
  /*background-size: cover;*/
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 30px 4vw;
  @media #{$x840} {
    flex-direction: column;
    padding-top: 50px;
    padding-bottom: 50px;
  }
  @media #{$x600} {
    padding: 0 0 50px 0;
  }
  form {
    width: 50%;
    margin-right: -4vw;
    margin-top: -5vw;
    max-width: 450px;
    min-width: 350px;
    @media #{$x840} {
      margin: -44px 0 0 0;
      width: 450px;
    }
    @media #{$x600} {
      width: 100%;
      min-width: 300px;
      margin: 0;
      margin-top: 30px;
    }
    > div {
      margin: auto;
    }
    .md-card-content {
      padding: 0 3vw;
    }
    .md-card-actions {
      padding-top: 0;
      margin: -8px 0 6px 0;
      button {
        @include blue-btn();
        margin: 0 auto !important;
        width: 137px;
        border-radius: 7px;
      }
    }
  }
  section {
    flex-wrap: wrap;
    background-color: rgba(0, 0, 0, 0.8);
    width: 75%;
    height: 505px;
    position: absolute;
    text-align: left;
    padding: 4vw 5vw;
    color: white;
    max-width: 1000px;
    display: flex;
    > div {
      width: 50%;
    }
    @media #{$x840} {
      position: relative;
      width: 100%;
      height: auto;
      padding-bottom: 70px;
      > div {
        width: 100%;
      }
    }
    @media #{$x600} {
      padding-top: 30px;
    }
    h1 {
      font-size: 25px;
    }
    .text {
      width: calc(100% - 30px);
      @media #{$x840} {
        width: 100%;
      }
      .row {
        padding: 7px 3px;
        span {
          vertical-align: middle;
          padding-left: 5px;
        }
        i {
          color: white;
        }
      }
    }
  }
}
</style>
