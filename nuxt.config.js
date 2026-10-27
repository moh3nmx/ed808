const pkg = require('./package')

module.exports = {
  mode: 'universal',

  /*
  ** Headers of the page
  */
  head: {
    link: [
      { rel: 'icon', type: 'image/png', href: '/favlogo.png' },
      { rel: "stylesheet", href : "//cdn.materialdesignicons.com/3.4.93/css/materialdesignicons.min.css"},
      { rel: "stylesheet", type: "text/css", href: "https://fonts.googleapis.com/css?family=Roboto:300,400,500,700|Roboto+Slab:400,700"}
    ],
    script: [
      //{ src: 'https://www.google.com/recaptcha/api.js?onload=vueRecaptchaApiLoaded&render=explicit', async , defer}
      { src: 'https://www.google.com/recaptcha/api.js?onload=vueRecaptchaApiLoaded&render=explicit'}
    ],
    title: '808 808 Social Network of Engineers',
    titleTemplate: '%s | 808 Social Network of Engineers',
    meta: [
      { charset: 'utf-8' },
      { 'http-equiv': 'Content-Type', content: 'text/html; charset=utf-8' },
      { name: 'viewport', content: 'width=device-width, initial-scale=1' },
      { name: 'mobile-web-app-capable', content: 'yes' },
      { name: 'description', content: 'A civil engineering social media, providing online training courses in all different fields of civil engineering and architecture focusing on Performance-based design of Structures (PBD) and Building Information Modeling (BIM) and other new trendes in the form of articles, videos, podcasts, ebooks and so on.', hid: 'description' },

      // OpenGraph data (Most widely used) - Dynamic
      { property: 'og:title', content: this.title, template: '%s - 808 Social Network of Engineers', hid: 'og:title' },
      { property: 'og:url', content: 'https://ed808.com', hid: 'og:url' },
      { property: 'og:image', content: "https://ed808.com/images/front-page.jpg", hid: 'og:image' },
      { property: 'og:description', content: 'A civil engineering social media, providing online training courses in all different fields of civil engineering and architecture focusing on Performance-based design of Structures (PBD) and Building Information Modeling (BIM) and other new trendes in the form of articles, videos, podcasts, ebooks and so on.', hid: 'og:description' },
      { property: 'og:type', content: 'website' },
      // OpenGraph data (Most widely used) - Static
      { property: 'og:site_name', content: '808 Social Network of Engineers' },

      // Twitter card - Dynamic
      { name: 'twitter:title', content: this.title, template: '%s - 808 Social Network of Engineers', hid: 'twitter:title' },
      { name: 'twitter:description', content: 'A civil engineering social media, providing online training courses in all different fields of civil engineering and architecture focusing on Performance-based design of Structures (PBD) and Building Information Modeling (BIM) and other new trendes in the form of articles, videos, podcasts, ebooks and so on.', hid: 'twitter:description' },
      { name: 'twitter:image:src', content: 'https://ed808.com/images/front-page.jpg', hid: 'twitter:image:src' },
      // Twitter card - Static
      { name: 'twitter:site', content: 'https://ed808.com' },
      { name: 'twitter:card', content: 'summary' },
      // Your twitter handle, if you have one.{name: 'twitter:creator', content: ''},

      // Google / Schema.org markup: - Dynamic
      { itemprop: 'name', content: this.title, template: '%s - 808 Social Network of Engineers', hid: 'name' },
      { itemprop: 'description', content: 'A civil engineering social media, providing online training courses in all different fields of civil engineering and architecture focusing on Performance-based design of Structures (PBD) and Building Information Modeling (BIM) and other new trendes in the form of articles, videos, podcasts, ebooks and so on.', hid: 'description' },
      { itemprop: 'image', content: 'https://ed808.com/images/front-page.jpg', hid: 'image' },
    ],
    htmlAttrs: {
      class: 'md-scrollbar'
    }
  },

  /*
  ** Customize the progress-bar color
  */
  loading: {
    color: '#673AB7',
    height: '3px'
  },

  /*
  ** Global CSS
  */
  css: [
    '@/assets/scss/app.scss',
    '@/assets/scss/vars.scss',
  ],

  /*
  ** Plugins to load before mounting the App
  */

  plugins: [
    { src: '~plugins/vue-carousel', ssr: false },
    { src: '~plugins/affix', ssr: false },
    { src: '~plugins/ga.js', ssr: false },
    { src: '~plugins/vue-sharing.js', ssr: false },
    { src: '~plugins/atc.js', ssr: false },
    { src: '~plugins/VueEditor.js', ssr: false },
    { src: '~plugins/vue-authenticate.js', ssr: false },
    { src: '~plugins/jspdf.js', ssr: false },
    { src: '~plugins/multiselect.js', ssr: false },
    { src: '~plugins/hotjar.js', ssr: false },
  ],

  /*
  ** Nuxt.js modules
  */

  modules: [
    ['@nuxtjs/axios'],
    'nuxt-vue-material',
  ],

  // Dev Access middleware
  // router: {
  //   middleware: ['dev']
  // },
  /*
  ** Build configuration
  */
  build: {
    /*
    ** You can extend webpack config here
    */
    extractCSS: true
  },
  server: {
    port:3000
  }
}
