<!-- This Page is temporary & must be changed -->
<template>
  <div class="content-container">
    <h2>Up Coming Events</h2>
    <p v-if="events.length == 0">Please Wait</p>
    <div v-if="eventNid != 0">
      <event-description
        :nid="eventNid"
        @clearNid="eventNid = 0"
      />
    </div>
    <div class="event-container">
      <event-teaser
        v-for="event in events"
        :id="event.sticky == '1' ? 'special-event' : ''"
        :key="event.nid"
        :newstitle="event.title"
        :newscompany="event.company"
        :newsdate="event.eventtime | erasetime"
        :newsnid="event.nid"
        :sticky='event.sticky'
        @setNid="show_event(event.nid)"
      />
    </div>
  </div>
</template>

<script>
  import axios from 'axios'
  import EventTeaser from "@/components/front/EventTeaser";
  import EventDescription from "@/components/front/EventDescription";
  export default {
    name: "Events",
    components: {EventDescription, EventTeaser},
    filters: {
      erasetime: function (value) {
        if (!value) return ''
        value = value.toString()
        return value.split(' ')[0]
      }
    },
    data () {
      return {
        events:[],
        specialEvent: {},
        loading: true,
        eventNid: 0,
      }
    },
    mounted() {
      axios.get('https://ed808.com:92/latin/contents/list/event')
        .then((data) => {
          let events = []
          let spec = []
          data.data.forEach((el)=>{
            if(el.sticky != '1') {
              events.push(el)
            }else {
              spec.push(el)
            }
          })
          if(spec.length > 0){
            events = spec.concat(events);
          }
          this.events = events
          this.specialEvent = spec
          this.loading = false
        })
    },
    methods: {
      show_event(nid){
        this.eventNid = nid
      }
    },
    head(){
      return{
        title: 'Upcoming Events',
        meta: [
          // OpenGraph data (Most widely used)
          {property: 'og:url', content: 'https://ed808.com/events', hid: 'og:url'},
          {
            property: 'og:title',
            content: 'Upcoming Events',
            hid: 'og:title'
          },
          {
            name: 'twitter:title',
            content: 'Upcoming Events',
            hid: 'twitter:title'
          },
          // Google / Schema.org markup:
          {
            itemprop: 'name',
            content: 'Upcoming Events',
            hid: 'name'
          },
        ],
        links: [
          {rel: 'canonical', href: 'https://ed808.com/events'}
        ]
      }
    }
  }
</script>

<style lang="scss" scoped>
  @import "../assets/scss/vars.scss";
  .content-container {
    width: 80%;
    margin: 30px auto;
    padding: 30px 0;
    min-height: 500px;
    @media #{$x960} {
      width: 95%;
    }

    .event-container {
      display: flex;
      flex-wrap: wrap;

      .news-teaser {
        overflow: hidden;
        width: calc(25% - 30px);
        margin: 15px;
        .md-card-header {
          overflow: hidden;
        }
        @media #{$x1280} {
          width: calc(33% - 20px);
          margin: 10px;
        }
        @media #{$x960} {
          width: calc(50% - 20px);
          margin: 10px;
        }
        @media #{$x720} {
          width: 100%;
          margin: 10px;
        }
      }
    }
  }

</style>
