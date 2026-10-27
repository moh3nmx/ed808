<template>
  <div class="with-background cart-page">
    <md-card v-if="payment">

      <md-card-header>
        <div class="md-title">
          Thank You For Your Purchase
        </div>
      </md-card-header>
      <v-card-content>
        <p style="padding:1rem">
          Dear User,
          A receipt of your purchase is sent to your email. Be sure to keep it in a safe place for future reference.
        </p>
      </v-card-content>

    </md-card>
    <md-card v-else>
      <md-card-header>
        <div class="md-title">Cart Checkout</div>
      </md-card-header>
      <md-card-content>

        <md-table>
          <md-table-row>
            <md-table-head>
              Description
            </md-table-head>
            <md-table-head>
              Price
            </md-table-head>
          </md-table-row>

          <md-table-row>
            <md-table-cell>
              {{ product }}
            </md-table-cell>
            <md-table-cell>
              {{ convertPrice(price, '978') }}
            </md-table-cell>
          </md-table-row>

          <md-table-row>
            <md-table-cell>
              Tax
            </md-table-cell>
            <md-table-cell>
              {{ convertPrice(0.09 * price, '978') }}
            </md-table-cell>
          </md-table-row>
          <md-table-row>
            <md-table-cell>
              Total
            </md-table-cell>
            <md-table-cell>
              {{ convertPrice(1.09 * price, '978') }}
            </md-table-cell>
          </md-table-row>
        </md-table>


        <hr>

        <form action="https://temp.ed808.com/api/admin/yekpay" method="post" ref="payForm" @submit.prevent="submitForm">
          <md-field>
            <label for="currency">
              Your Currency
            </label>
            <md-select id="currency" v-model="currency">
              <md-option value="840">
                USD - $
              </md-option>
              <md-option value="978">
                EUR - €
              </md-option>
            </md-select>
          </md-field>

          <md-field
            :class="getValidationClass('firstname')"
          >
            <label for="username">
              Firstname
            </label>
            <md-input
              id="firstname"
              v-model="form.firstname"
              name="firstName"
              autocomplete="given-name"
              
            />
            <span
              v-if="!$v.form.firstname.required"
              class="md-error"
            >
              Firstname is required
            </span>
            <!--<span class="md-error" v-else-if="!$v.form.mobile.taken">The User is already taken</span>-->
          </md-field>
          
          <md-field
            :class="getValidationClass('lastname')"
          >
            <label for="username">
              Lastname
            </label>
            <md-input
              id="lastname"
              v-model="form.lastname"
              name="lastName"
              autocomplete="given-name"
              
            />
            <span
              v-if="!$v.form.lastname.required"
              class="md-error"
            >
              Lastname is required
            </span>
            <!--<span class="md-error" v-else-if="!$v.form.mobile.taken">The User is already taken</span>-->
          </md-field>

          <md-field
            :class="getValidationClass('mobile')"
          >
            <label for="mobile">
              Mobile Number
            </label>
            <md-input
              id="mobile"
              v-model="form.mobile"
              name="mobile"
              autocomplete="given-name"
              type="number"
            />
            <span
              v-if="!$v.form.mobile.required"
              class="md-error"
            >
              Mobile number is required
            </span>
            <!--<span class="md-error" v-else-if="!$v.form.mobile.taken">The User is already taken</span>-->
          </md-field>

          <md-field
            :class="getValidationClass('email')"
          >
            <label for="email">
              Email
            </label>
            <md-input
              id="email"
              v-model="form.email"
              name="email"
              autocomplete="given-name"
              
            />
            <span
              v-if="!$v.form.email.required"
              class="md-error"
            >
              Email is required
            </span>
            <span class="md-error" v-else-if="!$v.form.email.email">Wrong email address</span>
          </md-field>

          <md-field
            :class="getValidationClass('city')"
          >
            <label for="city">
              Country
            </label>
            <md-input
              id="country"
              v-model="form.country"
              name="country"
              autocomplete="given-name"
              
            />
            <span
              v-if="!$v.form.email.required"
              class="md-error"
            >
              Country is required
            </span>
          </md-field>

          <md-field
            :class="getValidationClass('city')"
          >
            <label for="email">
              City
            </label>
            <md-input
              id="city"
              v-model="form.city"
              name="city"
              autocomplete="given-name"
              
            />
            <span
              v-if="!$v.form.city.required"
              class="md-error"
            >
              Country is required
            </span>
          </md-field>

          <md-field
            :class="getValidationClass('postalCode')"
          >
            <label for="postalCode">
              Postal Code
            </label>
            <md-input
              id="postalCode"
              type="number"
              v-model="form.postalCode"
              name="postalCode"
              autocomplete="given-name"
              
            />
            <span
              v-if="!$v.form.postalCode.required"
              class="md-error"
            >
              Country is required
            </span>
          </md-field>

          <md-field
            :class="getValidationClass('address')"
          >
            <label for="address">
              Address
            </label>
            <md-textarea
              id="address"
              v-model="form.address"
              name="address"
              md-autogrow
              
            />
            <span
              v-if="!$v.form.address.required"
              class="md-error"
            >
              Address is required
            </span>
          </md-field>

          <div style="text-align: center">
            <md-button class="md-raised md-primary" type="submit">
              Submit & Pay
            </md-button>
          </div>
        </form>

      </md-card-content>
    </md-card>
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

export default {
  name: 'Checkout',
  mixins: [validationMixin],
  layout: 'static',
  data() {
    return {
      currency: '840',
      price: 19.99,
      product: '',
      payment: false,
      form : {
        firstname: null,
        lastname: null,
        mobile: null,
        email: null,
        address: null,
        country: null,
        city: null,
        postalCode: null
      }
    }
  },
  asyncData({isDev, route, store, env, params, query, req, res, redirect, error}) {
    if(store.state.cart){
      return {
        price: store.state.cart.price,
        product: store.state.cart.product
      }
    }else{
      if(query.hasOwnProperty('payment')) {
        if(query.payment = 'true') {
          return {
            payment: true
          }
        }
      }else{
        redirect('/')
      }
    }
  },
  created(){
    this.$store.state.backGround = '/images/login-bg.jpg'
  },
  validations: {
    form: {
      firstname: {
        required
      },
      lastname: {
        required
      },
      mobile: {
        required
      },
      email: {
        required,
        email
      },
      address: {
        required
      },
      country: {
        required
      },
      city: {
        required
      },
      postalCode: {
        required
      }
    }
  },
  methods: {
    getValidationClass (fieldName) {
      const field = this.$v.form[fieldName]

      if (field) {
        return {
          'md-invalid': field.$invalid && field.$dirty
        }
      }
    },
    convertPrice(dollar,currency) {
      switch(currency) {
        case '840':
          return dollar.toFixed(2) + '$'
          break
        case '978':
          return (.91 * dollar).toFixed(2) + '€'
          break
      }
    },
    submitForm(e){
      let form = this.$v.form
      form.$touch()
      if(!form.$invalid) {
        let product = document.createElement("input");
        let currency = document.createElement("input");
        product.value = parseFloat(this.convertPrice(1.09 * this.price, '978'));
        product.name = "amount";
        product.type = "hidden";
        currency.value = this.currency
        currency.name = "currency";
        currency.type = "hidden";

        e.target.appendChild(product);
        e.target.appendChild(currency);
        e.target.submit()
        
      }
    }
  },
  head(){
    return {
      title: 'Cart'
    }
  },
}
</script>

<style lang="scss">

  .cart-page {
    .md-card {
      margin: auto;
      margin-top: 100px;
      width: 50%;
      min-width: 300px;
      max-width: 400px;
    }
    .md-table-cell {
      text-align: left
    }
  }
  
</style>