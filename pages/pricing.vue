<template>
  <div>
    <div class="calendar-header">
      <h2>ED808 Business Services</h2>
    </div>
    <div class="main main-raised">
      <div class="container">
        <div class="pricing-2">
          <div class="row">
            <div class="col-md-6 col-md-offset-3 text-center">
              <span class="first">
                <p style="font-weight: 700;font-size: 24px; font-family: 'Roboto Slab', Times New Roman, serif">Get closer to the right people</p>
                <p style="font-family: 'Roboto Slab'">If you have a company in AEC industry and you need to promote your services</p>
                <p style="font-family: 'Roboto Slab'">You can start your free 1-month trial, via filling the "Contact Us" form at bottom of this page</p>
                <p style="font-family: 'Roboto Slab'; line-height: 40px">Find leads and close deals with ED808 Sales Navigator. No matter the size of your business, advertising on ED808 will help your business achieve it's goals.</p>
              </span>
              <ul class="nav nav-pills nav-pills-rose" role="tablist">
                <li :class="{'active' : activeTab===1 }"
                    @click="activeTab = 1">
                  <a href="#monthly" role="tab" data-toggle="tab" aria-expanded="false">
                    Monthly
                  </a>
                </li>
                <li :class="{'active' : activeTab===2 }"
                    @click="activeTab = 2">
                  <a href="#annual" role="tab" data-toggle="tab" aria-expanded="true">
                    Annual
                  </a>
                </li>
              </ul>
            </div>
          </div>

          <div class="md-layout md-gutter">
            <div class="md-layout-item">
              <div class="card card-pricing card-plain">
                <div class="card-content">
                  <h6 class="category text-info">Plus</h6>
                  <h1 class="card-title" v-if="activeTab==1"><small>$</small>200<small>/mo</small></h1>
                  <h1 class="card-title" v-else><small>$</small>1920<small>/yr</small></h1>
                  <ul>
                    <li><b>1</b> Brand</li>
                    <li><b>50</b> Hours</li>
                    <li><b>1</b> Administrator</li>
                  </ul>
                </div>
              </div>
            </div>
            <div class="md-layout-item">
              <div class="card card-pricing card-raised">
                <div class="card-content content-rose">
                  <h6 class="category text-info">Business</h6>
                  <h1 class="card-title" v-if="activeTab==1"><small>$</small>300<small>/mo</small></h1>
                  <h1 class="card-title" v-else><small>$</small>2880<small>/yr</small></h1>
                  <ul>
                    <li><b>2</b> Brands</li>
                    <li><b>100</b> Hours + Advertisements</li>
                    <li><b>5</b> Administrators</li>
                  </ul>
                </div>
              </div>
            </div>
          </div>

        </div>
        <hr>
        <div class="features-2">
          <div class="text-center">
            <h3 class="title">Descriptions</h3>
          </div>
          <div class="row">
            <div class="md-layout md-gutter">
              <div class="md-layout-item">
                <div class="info info-horizontal">
                  <div class="icon icon-info">
                    <md-icon>shopping_cart</md-icon>
                  </div>
                  <div class="description">
                    <h4 class="info-title">Sales Solutions</h4>
                    <p>You can completely introduce all your products through a page (ed808.com/example).</p>
                    <p>You are able to show the price and all your contact details (including address, phone number, etc.).</p>
                    <p>You can insert a direct link to your website on your company page.</p>
                  </div>
                </div>
                <div class="info info-horizontal">
                  <div class="icon icon-rose">
                    <md-icon>branding_watermark</md-icon>
                  </div>
                  <div class="description">
                    <h4 class="info-title">Branding</h4>
                    <p>You can share any type of content including videos, articles, e-books, podcasts, etc., directly on your company page. In this case you have the opportunity to attract more customers through gathering useful data along with other solutions.</p>
                  </div>
                </div>
              </div>

              <div class="md-layout-item">
                <div class="info info-horizontal">
                  <div class="icon icon-success">
                    <md-icon>cast</md-icon>
                  </div>
                  <div class="description">
                    <h4 class="info-title">e-Learning Solutions</h4>
                    <p><b>Live Webinars:</b> You can hold live webinars for any goals regarding your company and products, and users can join freely.</p>
                    <p><b>Offline Webinars and Courses:</b> The same as live webinars, you can publish a recorded video of a seminar or a training course.</p>
                    <p><b>Live Online Course:</b> You can hold online training courses to which users can join freely.</p>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
    <contact-form
      :component="true"
      altText="If you have any question or require any further information, please contact us."
    />
  </div>
</template>

<script>
  import ContactForm from '@/components/fields/ContactForm'
  export default {
    name: "ED808 Business Services",
    layout: "static",
    components: {ContactForm},
    data() {
      return {
        activeTab: 1
      }
    },
    head(){
      return{
        title: 'ED808 Business Services',
        meta: [
          // OpenGraph data (Most widely used)
          {
            'property': 'og:title',
            'content': 'ED808 Business Services',
            'template': '%s - ed808',
            'vmid': 'og:title'
          },
          {property: 'og:url', content: 'https://ed808.com/pricing', vmid: 'og:url'},
          {
            'name': 'twitter:title',
            'content': 'ED808 Business Services',
            'template': '%s - ed808',
            'vmid': 'twitter:title'
          },
          // Google / Schema.org markup:
          {
            'itemprop': 'name',
            'content': 'ED808 Business Services',
            'template': '%s - ed808',
            'vmid': 'name'
          },
        ],
        links: [
          {rel: 'canonical', href: 'https://ed808.com/pricing'}
        ]
      }
    },
    mounted(){
      this.$store.state.backGround = '/images/business.jpg'
    }
  }
</script>

<style scoped lang="scss">
  .calendar-header {
    min-height: 40vh;
    position: relative;
    background-size: cover;
    background-position: center;
    overflow: hidden;

  h2 {
    font-family: "roboto slab";
    z-index: 1;
    color: #fff;
    margin: 0;
    position: absolute;
    top: calc(50% - 43.5px);
    width: 100%;
  }
  }
  .first {
    color: #3C4858;
    font-size: 17px;
    padding: 0 20px 10px 20px;
    display: inline-block;
    position: relative;
    @media all and (max-width: 600px) {
      font-size: 16px;
      padding: 24px 10px 30px 10px;
    }
  }

    .main-raised {
      margin: -60px 30px 0px;
      border-radius: 6px;
      padding: 8%;
      box-shadow: 0 16px 24px 2px rgba(0, 0, 0, 0.14), 0 6px 30px 5px rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
    }

    .main {
      background: #FFFFFF;
      position: relative;
      z-index: 3;
      /*margin-top: 115px !important;*/
    }

    .container {
      padding-right: 15px;
      padding-left: 15px;
      margin-right: auto;
      margin-left: auto;
    }
    .col-md-6 {
      position: relative;
      min-height: 1px;
      padding-right: 15px;
      padding-left: 15px;
    }

    .pricing-2 .nav-pills {
      display: inline-block;
      margin-bottom: 50px;
      margin-top: 50px;
    }

    .nav {
      padding-left: 0;
      margin-bottom: 0;
      list-style: none;
    }

    ol, ul {
      margin-top: 0;
      margin-bottom: 10px;
    }

    .nav-pills > li {
      float: left;
    }

    .nav > li {
      position: relative;
      display: block;
    }

    .nav-pills > li + li {
      margin-left: 2px;
    }

    .nav-pills > li {
      float: left;
    }

    .nav-pills:not(.nav-pills-icons) > li > a {
      border-radius: 30px;
    }

    .nav-pills > li > a {
      line-height: 24px;
      text-transform: uppercase;
      font-size: 12px;
      font-weight: 500;
      min-width: 100px;
      text-align: center;
      color: #555555;
      transition: all .3s;
    }

    .nav-pills > li > a {
      border-radius: 4px;
    }

    .nav > li > a {
      position: relative;
      display: block;
      padding: 10px 15px;
    }

    .nav-pills.nav-pills-rose > li.active > a, .nav-pills.nav-pills-rose > li.active > a:focus, .nav-pills.nav-pills-rose > li.active > a:hover {
      background: linear-gradient(#3c1053, #ad5389);
      box-shadow: 0 5px 20px 0px rgba(0, 0, 0, 0.2), 0 13px 24px -11px rgba(156, 39, 176, 0.6);
    }

    .nav-pills > li.active > a, .nav-pills > li.active > a:focus, .nav-pills > li.active > a:hover {
      background-color: #9c27b0;
      color: #FFFFFF;
      box-shadow: 0 5px 20px 0px rgba(0, 0, 0, 0.2), 0 13px 24px -11px rgba(156, 39, 176, 0.6);
    }

    .nav-pills:not(.nav-pills-icons) > li > a {
      border-radius: 30px;
    }

    .card-pricing {
      text-align: center;
    }

    .card-plain {
      background: transparent;
      box-shadow: none;
    }

    .card {
      display: inline-block;
      position: relative;
      width: 100%;
      margin-bottom: 30px;
      border-radius: 6px !important;
      color: rgba(0, 0, 0, 0.87);
      background: #fff;
      box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
    }

    .card-pricing .card-content {
      padding: 15px !important;
    }

    .card .content-rose {
      background: linear-gradient(#3c1053, #ad5389);
      border-radius: 6px;
    }

    .card-pricing [class*="content-"] ul li {
      color: #FFFFFF;
      border-color: rgba(255, 255, 255, 0.3);
    }

    .card-pricing ul li:last-child {
      border: 0;
    }

    .text-info {
      color: #00bcd4;
      font-weight: bold;
    }

    .info-title {
      font-weight: 700;
      font-family: "Roboto Slab", "Times New Roman", serif;
    }

    .info p {
      color: #757575;
      font-size: 14px;
      font-family: "Roboto Slab", "Times New Roman", serif;
    }

    .md-layout.md-gutter {
      text-align: left;
      padding-bottom: 20px;
    }

    .info-horizontal .icon.icon-info > i {
      font-size: 1.8em !important;
      color: #00bcd4
    }

    .info-horizontal .icon.icon-success > i {
      font-size: 1.8em !important;
      color: #4caf50;
    }

    .info-horizontal .icon.icon-rose > i {
      font-size: 1.8em !important;
      color: #e91e63;
    }

    .info-horizontal .icon {
      float: left;
      margin-top: -3px;
      margin-right: 10px;
    }

    h6 {
      font-size: 0.9em;
      text-transform: uppercase;
      font-weight: 500;
    }

    body, h1, h2, h3, h4, h5, h6 {
      font-family: "Roboto", "Helvetica", "Arial", sans-serif;
      font-weight: 300;
      line-height: 1.5em;
    }

    h6 {
      font-size: 14px;
    }

    h4, h5, h6 {
      margin-top: 10px;
      margin-bottom: 10px;
    }

    .card-pricing .card-title {
      margin-top: 30px;
    }

    .card-title {
      color: #3C4858;
      text-decoration: none;
      font-weight: 700;
      font-family: "Roboto Slab", "Times New Roman", serif;
    }

    .card [class*="content-"] .card-title {
      color: #FFFFFF;
    }

    .card-pricing ul li b {
      color: #3C4858;
    }

    .card-pricing [class*="content-"] ul li b {
      color: #FFFFFF;
    }

    h1 {
      font-size: 3.8em !important;
      line-height: 1.15em;
    }

    .card-pricing h1 small:first-child {
      position: relative;
      top: -17px;
      font-size: 26px;
    }

    .card-pricing h1 small {
      font-size: 18px;
    }

    h1 small {
      font-weight: 400;
      line-height: 1;
    }

    .card-pricing.card-background [class*="text-"], .card-pricing [class*="content-"] [class*="text-"] {
      color: #FFFFFF;
      font-weight: bold;
    }

    .card-pricing ul {
      list-style: none;
      padding: 0;
      max-width: 240px;
      margin: 10px auto;
    }

    .card-pricing ul li {
      color: #999999;
      text-align: center;
      padding: 12px 0;
      border-bottom: 1px solid rgba(153, 153, 153, 0.3);
    }

    .title {
      color: #3C4858;
      text-decoration: none;
      font-weight: 700;
      font-family: "Roboto Slab", "Times New Roman", serif;
    }

    h3, .h3 {
      font-size: 1.8em;
      line-height: 1.4em;
      margin: 20px 0 20px;
    }

    h2 {
      font-family: "Roboto Slab", "Times New Roman", serif;
      font-weight: 600;
    }

    .container:after, .container:before, .dl-horizontal dd:after, .dl-horizontal dd:before, .form-horizontal .form-group:after, .form-horizontal .form-group:before, .nav:after, .nav:before, .row:after, .row:before {
      display: table;
      content: " ";
    }

    .md-layout-item {
      @media (max-width: 960px) {
        width: 50%;
        -ms-flex: 0 1 50%;
        flex: 0 1 50%;
      }
      @media (max-width: 600px) {
        width: 100%;
        -ms-flex: 0 1 100%;
        flex: 0 1 100%;
      }
    }
</style>
