import Vue from 'vue'
import VueAxios from 'vue-axios'
import VueAuthenticate from 'vue-authenticate'
import axios from 'axios'

Vue.use(VueAxios, axios)
Vue.use(VueAuthenticate, {
  baseUrl: 'https://ed808.com:2', // Your API domain

  providers: {
    linkedin: {
      name: 'linkedin',
      url: '/login/linkedin',
      responseType: 'code',
      authorizationEndpoint: 'https://www.linkedin.com/oauth/v2/authorization',
      clientId: '77gasbcb9ngw6d',
      clientSecret: 'eLNyPD8CUAuMBf0d',
      redirectUri: 'https://ed808.com:2/login/linkedin',
      requiredUrlParams: ['scope'],
      scope: ['r_basicprofile', 'r_emailaddress'],
      scopeDelimiter: ' ',
      oauthType: '2.0',
      popupOptions: { width: 527, height: 582 }
    },
  }
})
