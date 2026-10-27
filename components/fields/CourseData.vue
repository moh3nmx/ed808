<template>
  <div class="event_header">
    <!-- <Countdown :deadline="date" v-if="date != null"></Countdown> -->
    <div>
      <a v-if="need_login != '1'" :href="registration_link" target="_blank">
        <md-button v-if="registration_link != null" class="md-raised md-primary">
          <span>Enroll</span>
        </md-button>
      </a>
      <a v-else-if="need_login == '1' && $store.getters.getUid" :href="registration_link" target="_blank">
        <md-button v-if="registration_link != null" class="md-raised md-primary">
          <span>Enroll</span>
        </md-button>
      </a>
      <a v-else-if="need_login == '1' && !$store.getters.getUid" @click="$store.commit('TOGGLE_LOGIN')">
        <md-button v-if="registration_link != null" class="md-raised md-primary">
          <span>Login</span>
        </md-button>
      </a>
    </div>
    <div v-if="!$store.getters.getUid && need_login == '1'">Please login to enroll.</div>
    <div class="event_bottom">
      <div v-if="organizer != null" class="feature">
        <md-icon class="md-size-2x">home</md-icon>
        <span>Organizer</span>
        <span class="event_data">{{organizer}}</span>
      </div>
      <div v-if="author != null" class="feature">
        <md-icon class="md-size-2x">person</md-icon>
        <span>Lecturer</span>
        <span class="event_data">{{author}}</span>
      </div>
    </div>
  </div>
</template>

<script>
import Countdown from 'vuejs-countdown'

export default {
  name: 'CourseData',
  props: ['date','time','organizer','author','webinar_covered','registration_link','nid','need_login'],
  filters:{
    erasetime: function (value) {
      if (!value) return ''
      value = value.toString()
      return value.split(' ')[0]
    }
  },
  components:{
    Countdown
  }
}
</script>

<style lang="scss">
  .vuejs-countdown{
    margin: 0px 0 15px 0 !important;
    li:after {
      top: 7px!important;
      font-size: 20px !important;
      font-weight: bold;
    }
    .digit {
      font-size: 26px !important;
    }
  }
  .event_header{
    color: #a7a7a7 !important;
    width: 100%;
  }
  .md-primary{
    a{
      color: #fff !important;
    }
  }
  .event_bottom{
    position: absolute;
    bottom: -111px;
    left: 0;
    right: 0;
    background-color: #fff;
    box-shadow: 0 3px 1px -2px rgba(0,0,0,.2), -2px 2px 4px 0 rgba(0,0,0,.14), 0 1px 5px 0 rgba(0,0,0,.12);
    height: 110px;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: row-reverse;
    border-bottom: 2px solid #EEEEEE;

    .feature {
      display: flex;
      flex-direction: column;
      padding: 0 20px;
      border-left: 1px solid #eee;
      &:last-child {
        border-left: 0px;
      }
      .md-icon{
        font-size: 30px !important;
        color: #b5b5b5;
        width: 30px !important;
        height: 30px !important;
      }
      .event_data{
        color: black;
        padding: 11px 0 0 0;
        font-size: 15px;
      }
    }
  }
</style>
