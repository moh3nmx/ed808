<template>
  <div
    v-if="loading || events.length > 0"
    class="eventspane container"
  >
    <div class="md-headline">
      <h5>Upcoming AEC Industry Events</h5>
      <div class="more-link">
        <!--<router-link tag="md-button" to="/contents?type=4058">See All</router-link>-->
        <!-- this link is temporary. it will change after events page create -->
        <nuxt-link
          tag="md-button"
          to="/events"
        >
          See All
        </nuxt-link>
      </div>
    </div>
    <section id="events">
      <div
        v-if="loading"
        class="events-pane loading"
      >
        <md-progress-spinner
          :md-diameter="100"
          :md-stroke="5"
          md-mode="indeterminate"
        />
      </div>
      <carousel
        :per-page="perPage"
        :navigationEnabled="true"
        navigationNextLabel="navigate_next"
        navigationPrevLabel="navigate_before"
        :paginationEnabled="false"
      >
        <slide
          v-for="event in events"
          :key="event.nid"
        >
          <event-teaser
            :id="event.sticky == '1' ? 'special-event' : ''"
            :newstitle="event.title"
            :newscompany="event.company"
            :newsdate="event.eventtime | erasetime"
            :newsnid="event.nid"
            :sticky='event.sticky'
            @setNid="show_event(event.nid)"
          />
        </slide>
      </carousel>
    </section>
    <div v-if="eventNid != 0">
      <event-description
        :nid="eventNid"
        @clearNid="eventNid = 0"
      />
    </div>
  </div>
</template>

<script>
import EventTeaser from '@/components/front/EventTeaser'
import EventDescription from '@/components/front/EventDescription'
import axios from '@/node_modules/axios'

export default {
  name: 'EventCarousel',
  components: {
    EventTeaser,
    EventDescription
  },
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
      specialEvent: [],
      loading: true,
      eventNid: 0,
      window: {
        width: 0,
        height: 0
      }
    }
  },
  computed: {
    perPage: function(){
      let perPage = 4
      if(this.window.width < 600) {
        perPage = 1
      }else if(this.window.width >= 600 && this.window.width < 1200) {
        perPage = 2
      }
      return perPage
    }
  },
  mounted() {
    this.handleResize()
    window.removeEventListener('resize', this.handleResize())
    axios.get('https://temp.ed808.com/api/latin/contents/list/event')
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
      .catch(() => {
        this.loading = false
      })
  },
  methods:{
    show_event(nid){
      this.eventNid = nid
    },
    handleResize() {
      this.window.width = window.innerWidth;
      this.window.height = window.innerHeight;
    }
  },

}
</script>

<style lang="scss">

.VueCarousel {
  width: 100%;
}
.VueCarousel-inner{
  @media screen and (max-width: 600px) {
    flex-basis: auto !important;

  }
}

.VueCarousel-slide{
  padding: 10px;
  @media screen and (max-width: 992px){
    width: 50% !important;
  }
  @media screen and (max-width: 600px) {
    width: 100% !important;
  }
}
button.VueCarousel-navigation-button{
  box-shadow: 0 3px 1px -2px rgba(0,0,0,.2), 0 2px 2px 0 rgba(0,0,0,.14), 0 1px 5px 0 rgba(0,0,0,.12);
  border-radius: 50%;
  width: 40px;
  height: 40px;
  background-color:white !important;
  font-family: Material Icons!important;
  font-size: 30px;
  text-align: center;
  line-height: 30px;
  padding: 0 !important;
  color:#92278f !important;
}
.eventspane{
	padding: 0 15px 36px;
  position: relative;
  z-index: 1;
  .events-pane.loading {
    position: absolute;
    z-index: 3;
    width: 100%;
    right: 0;
    top: 0;
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
  }
}
section#events{
  width: 100%;
	margin: 0px auto 20px auto;
	padding: 0 36px;
  min-height: 300px;
}
.md-headline{
	padding: 35px 0;
	margin: 0 !important;
	position: relative;
  text-align: left;
  &:before {
    content: "";
    width: 100%;
    height: 1px;
    background: #ddd;
    position: absolute;
    top: 53px;
  }
  h5{
    margin: 0;
    display: inline-block;
    background: #fafafa;
    z-index: 1;
    position: relative;
    padding-right: 15px;
    font-family: roboto slab;
    color: #3c4858;
  }
}
.more-link {
	background: #fafafa;
	z-index: 1;
	position: relative;
  float: right;
  button {
    margin: 0px 10px 0 10px;
    border: 1px solid #ddd;
    border-radius: 30px;
  }
}

</style>


