<!--Done-->
<template>
  <div>
    <div class="calendar-header">
      <h2>Up Coming Events</h2>
    </div>
    <div class="container-raised">
      <div v-if="viewType == 'list'">
        <!-- progress bar  -->
        <div style="height:5px ">
          <md-progress-bar
            md-mode="query"
            v-if="loadingEvents"
          >
          </md-progress-bar>
        </div>

        <!-- year selector -->
        <div
          class="year-selector"
          :class="{'loading-events' : loadingEvents}"
        >
          <div class="selector-content">
            <md-button
              class="md-icon-button md-raised md-default"
              @click="setYear(-1)"
            >
              <i class="mdi mdi-chevron-left"></i>
            </md-button>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="year"
                class="change-month-2 hide-on-mobile"
                @click="setYear(-2)"
              >
                {{ showNextYear(-2) }}
              </a>
            </transition>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="year"
                class="change-month-1"
                @click="setYear(-1)"
              >
                {{ showNextYear(-1) }}
              </a>
            </transition>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <span :key="year">
                {{ year }}
              </span>
            </transition>

            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="year"
                class="change-month-1"
                @click="setYear(1)"
              >
                {{ showNextYear(1) }}
              </a>
            </transition>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="year"
                class="change-month-2 hide-on-mobile"
                @click="setYear(2)"
              >
                {{ showNextYear(2) }}
              </a>
            </transition>
            <md-button
              class="md-icon-button md-raised md-default"
              @click="setYear(1)"
            >
              <i class="mdi mdi-chevron-right"></i>
            </md-button>
          </div>
        </div>

        <!-- month selector -->
        <div
          class="month-selector"
          :class="{'loading-events' : loadingEvents}"
        >
          <div class="selector-content">
            <md-button
              class="md-icon-button md-raised md-default"
              @click="setMonth(-1)"
            >
              <i class="mdi mdi-chevron-left"></i>
            </md-button>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="month"
                class="change-month-2 hide-on-mobile"
                @click="setMonth(-2)"
              >
                {{ showNextMonth(-2) }}
              </a>
            </transition>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="month"
                class="change-month-1"
                @click="setMonth(-1)"
              >
                {{ showNextMonth(-1) }}
              </a>
            </transition>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <span :key="month">
                {{ monthNames[month] }}
              </span>
            </transition>

            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="month"
                class="change-month-1"
                @click="setMonth(1)"
              >
                {{ showNextMonth(1) }}
              </a>
            </transition>
            <transition
              name="slide-fade"
              mode="out-in"
            >
              <a
                :key="month"
                class="change-month-2 hide-on-mobile"
                @click="setMonth(2)"
              >
                {{ showNextMonth(2) }}
              </a>
            </transition>
            <md-button
              class="md-icon-button md-raised md-default"
              @click="setMonth(1)"
            >
              <i class="mdi mdi-chevron-right"></i>
            </md-button>
          </div>
        </div>


        <!-- upcoming events list -->
        <div class="events-list">
          <div class="day-information" v-for="(value, name, index) in events" :key="index" v-if="convertMDYToTimestamp(month,name,year) > timestamp">
            <div class="event-day">
              <h2>
                {{ monthNames[month] }}
                {{ name }}, {{ year }}
                <br>
                {{  }}
              </h2>
            </div>
            <div class="day-events">
              <div class="event" v-for="event in value" :key="event.nid">
                <nuxt-link :to="'/node/'+event.nid">
                  <h3>{{event.title}}</h3>
                </nuxt-link>
                <p v-html="event.meta_description">

                </p>
                <nuxt-link :to="'/node/'+event.nid">Find out more »</nuxt-link>
              </div>
            </div>
          </div>
        </div>

        <!-- past events list -->
        <div class="events-list">
          <div class="day-information" v-for="(value, name, index) in events" :key="index" v-if="convertMDYToTimestamp(month,name,year) <= timestamp">
            <div class="event-day">
              <h2>
                {{ monthNames[month] }}
                {{ name }}, {{ year }}
              </h2>
            </div>
            <div class="day-events">
              <div class="event" v-for="event in value" :key="event.nid">
                <nuxt-link :to="'/node/'+event.nid">
                  <h3 class="past">{{event.title}} </h3>
                </nuxt-link>
                <p v-html="event.meta_description">

                </p>
                <nuxt-link :to="'/node/'+event.nid">Find out more »</nuxt-link>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div v-if="viewType == 'month'">
        <!-- calendar with months & days comes here -->
      </div>
    </div>
  </div>
</template>

<script>

  import axios from 'axios'
  export default {
    name: "Calendar",
    data(){
      return {
        title: 'Calendar',
        month : '',
        year : '',
        loadingEvents: false,
        monthNames: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'],
        monthNamesShort: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
        events : [],
        viewType: 'list'
      }
    },
    async asyncData({}){
      let month = new Date().getMonth();
      let year = new Date().getFullYear();
      try {
        let { data } = await axios.get(`https://temp.ed808.com/api/latin/contents/all/event?parameter[year]=${year}&parameter[month]=${month+1}`)
        return {
          month: month,
          year: year,
          timestamp : new Date().getTime()/1000,
          events: data
        }
      }
      catch (e) {

      }
    },
    methods:{
      // action => number of change of month (negative or positive)
      // change month in slider
      setMonth(action){
        let month = this.month + action
        if(month < 0)
          month += 12
        if(month > 11)
          month -= 12
        this.month = month
        this.loadingEvents = true
        this.events = []
        axios.get(`https://temp.ed808.com/api/latin/contents/all/event?parameter[year]=${this.year}&parameter[month]=${this.month+1}`)
          .then((data)=>{
            this.events = data.data
            this.loadingEvents = false
          })
      },
      // action => next or prev month that you want its name
      // show next month in slider
      showNextMonth(action) {
        let month = this.month
        month += action
        if(month < 0)
          month += 12
        if(month > 11)
          month -= 12
        let name = this.monthNamesShort
        return name[month]
      },
      // action => number of years (negative or positive)
      // change year in slider
      setYear(action){
        let year = this.year + action
        if(year <= 2000)
          year += 25
        if(year > 2025)
          year -= 25
        this.year = year
        this.loadingEvents = true
        this.events = []
        axios.get(`https://temp.ed808.com/api/latin/contents/all/event?parameter[year]=${this.year}&parameter[month]=${this.month+1}`)
          .then((data)=>{
            this.events = data.data
            this.loadingEvents = false
          })
      },
      // action => next or prev month that you want its name
      // show next year in slider
      showNextYear(action) {
        let year = this.year + action
        if(year <= 2000)
          year += 25
        if(year > 2025)
          year -= 25
        return year
      },
      convertMDYToTimestamp(month,day,year){
        return new Date(`${month+1}/${day}/${year}`).getTime()/1000
      }
    },
    head() {
    return {
      links: [
        { rel: "canonical", href: "https://ed808.com/calendar" },
        { rel: "alternate", href: "https://ed808.com/calendar", hreflang: "en" },
        { rel: "shortlink", href: "https://ed808.com/calendar" }
      ],
      title: "Calendar",
      meta: [
        // OpenGraph data (Most widely used)
        { property: "og:title", content: this.title, hid: "og:title" },
        {
          property: "og:url",
          content: "https://ed808.com/calendar",
          hid: "og:url"
        },
        {
          property: "og:image",
          content: '/images/calendar.jpg',
          hid: "og:image"
        },

        // Twitter card
        { name: "twitter:title", content: this.title, hid: "twitter:title" },
        {
          name: "twitter:image:src",
          content: '/images/calendar.jpg',
          hid: "twitter:image:src"
        },

        // Google / Schema.org markup:
        { itemprop: "name", content: this.title, hid: "name" },
        { itemprop: "image", content: '/images/calendar.jpg', hid: "image" }
      ]
    };
  }

  }
</script>

<style lang="scss" scoped>
  @import "../assets/scss/vars.scss";
  .md-progress-bar {
    max-width: 520px;
    margin: auto;
    opacity: 0.5;
  }
  .calendar-header {
    min-height: 50vh;
    position: relative;
    background-size: cover;
    background-position: center;
    overflow: hidden;
    background-image: url(/images/calendar.jpg);

    &:after {
      content: '';
      position: absolute;
      left: 0;
      top: 0;
      bottom: 0;
      right: 0;
      background: rgba(0,0,0,0.5);
    }

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

  .container-raised {
    background: #fff;
    position: relative;
    z-index: 1;
    margin: -60px 30px 0;
    border-radius: 6px;
    -webkit-box-shadow: 0 16px 24px 2px rgba(0, 0, 0, 0.14),
    0 6px 30px 5px rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
    box-shadow: 0 16px 24px 2px rgba(0, 0, 0, 0.14),
    0 6px 30px 5px rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
    padding: 130px 8%;
    @media #{$x600} {
      margin: -60px 15PX 0;
      padding: 130px 15px;
    }

    a {
      color: #92278f;
    }

    .month-selector {
      transition: 0.5s;
    }
    .selector-content {
      font-size: 1rem;
      font-weight: bold;
      line-height: 40px;
      margin: 15px;
      a {
        cursor: pointer;
        margin: 0 20px;
        display: inline-block;
        text-align: center;
        width: 35px;
        @media #{$x960} {
          margin: 0 5px;
        }
      }
      span {
        display: inline-block;
        width: 100px;
        text-align: center;
        @media #{$x960} {
          width: auto;
        }
      }
      .change-month-2 {
        color: #E0E0E0;
      }
      .change-month-1 {
        color: #9E9E9E;
      }
      .md-button {
        margin: 0 20px;
        @media #{$x960} {
          margin: 0 5px;
        }
      }
    }
    .loading-events {
      cursor: wait;
      pointer-events: none;
      transition: 0.5s;
      opacity: 0.5;
    }
    .events-list {
      margin-top: 100px;
      .event-day {
        position: relative;
        h2 {
          &.past {
            color: #999;
          }
        }
        h2:before {
          content: '';
          display: block;
          position: absolute;
          top: 50%;
          left: 0;
          width: calc(50% - 100px);
          border-bottom: 1px solid #BDBDBD;
        }
        h2:after {
          content: '';
          display: block;
          position: absolute;
          top: 50%;
          right: 0;
          width: calc(50% - 100px);
          border-bottom: 1px solid #BDBDBD;
        }
      }
      .day-events {
        .event {
          margin-bottom: 50px;

          &:not(:last-child) {
            border-bottom: 1px solid #EEEEEE;
          }
          padding: 10px 0;
          p {
            text-align: left;
          }
          h3 {
            text-align: left;
            &.past {
              color: #757575;
            }
            &.past:after {
              content: 'Expired';
              font-family: 'roboto slab';
              font-weight: 400;
              font-size: 12px;
              line-height: 29px;
              padding: 2px;
              border-radius: 3px;
              color: #fff;
              background: #757575;
            }
          }
        }
      }
      .day-information {
        margin-bottom: 30px;
      }
    }
  }



  /*transitions*/
  .slide-fade-enter-active {
    transition: all .3s ease;
  }
  .slide-fade-leave-active {
    transition: all .3s ease;
  }
  .slide-fade-enter, .slide-fade-leave-to
    /* .slide-fade-leave-active for <2.1.8 */ {
    /*transform: translateX(10px);*/
    opacity: 0;
  }
  .hide-on-mobile {
    @media #{$x960} {
      display: none !important;
    }
  }
</style>
