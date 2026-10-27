<template>
  <div class="event_header">
    <Countdown :deadline="date" v-if="date != null"></Countdown>
    <div v-if="!ended">
      <div>
        <a v-if="need_login != '1'" :href="registration_link" target="_blank">
          <md-button v-if="registration_link != null" class="md-raised md-primary">
            <span>Register Now</span>
          </md-button>
        </a>

        <a
          v-if="need_login == '1' && $store.getters.getUid"
          :href="registration_link"
          target="_blank"
        >
          <md-button v-if="registration_link != null" class="md-raised md-primary">
            <span>Register Now</span>
          </md-button>
        </a>
        <a
          v-else-if="need_login == '1' && !$store.getters.getUid"
          @click="$store.commit('TOGGLE_LOGIN')"
        >
          <md-button v-if="registration_link != null" class="md-raised md-primary">
            <span>Login</span>
          </md-button>
        </a>
      </div>
      <div
        v-if="!$store.getters.getUid && need_login == '1'"
      >You need to login first for registration.</div>
    </div>
    <div v-else>Event has ended.</div>
    <div
      v-if="(date != null) || (time != null) || (organizer != null) || (place != null)"
      class="event_bottom"
    >
      <div class="feature" v-if="nid == 20676">
        <md-icon class="md-size-2x">attach_money</md-icon>
        <span style="color:#a7a7a7 !important">Price</span>
        <span class="event_data">Free</span>
      </div>
      <div class="feature" style="cursor:pointer" @click="addToGoogleCal()">
        <md-icon class="md-size-2x">event_note</md-icon>
        <span style="color:#a7a7a7 !important">Google Calendar</span>
        <span class="event_data">Add To Calendar</span>
      </div>
      <div v-if="date != null" class="feature">
        <md-icon class="md-size-2x">date_range</md-icon>
        <span>Date</span>
        <span class="event_data">{{formatted}}</span>
      </div>
      <div v-if="time != null" class="feature">
        <md-icon class="md-size-2x">alarm</md-icon>
        <span>Time</span>
        <span class="event_data">{{time}}</span>
      </div>
      <div v-if="organizer != null" class="feature">
        <md-icon class="md-size-2x">home</md-icon>
        <span>Organizer</span>
        <span class="event_data">{{organizer}}</span>
      </div>
      <div v-if="place != null" class="feature">
        <md-icon class="md-size-2x">place</md-icon>
        <span>Place</span>
        <span class="event_data">{{place}}</span>
      </div>
      <!--<p v-if="webinar_covered == 1">
        <span>This webinar is covered by 808 company throw go to webinar website</span>
      </p>-->
    </div>
  </div>
</template>

<script>
import Countdown from "vuejs-countdown";

export default {
  name: "EventData",
  props: [
    "title",
    "date",
    "time",
    "organizer",
    "place",
    "webinar_covered",
    "registration_link",
    "nid",
    "need_login",
    "meta"
  ],
  filters: {
    erasetime: function(value) {
      if (!value) return "";
      value = value.toString();
      return value.split(" ")[0];
    }
  },
  data() {
    return {
      formatted: "",
      ended: false
    };
  },
  components: {
    Countdown
  },
  methods: {
    addToGoogleCal() {
      window.open(
        `https://calendar.google.com/calendar/r/eventedit?text=${
          this.title
        }&dates=${new Date(this.date)
          .toISOString()
          .replace(/-|:|\.\d+/g, "")}/${new Date(this.date)
          .toISOString()
          .replace(/-|:|\.\d+/g, "")}&details=${this.meta}&location=${
          this.organizer
        }&sf=true&output=xml`,
        "_blank"
      );
    },
    addToCart() {
      let cart = {
        price: 19.99,
        product: this.title
      };
      this.$store.commit("SET_CART", cart);
      this.$router.push("/cart/checkout");
    }
  },
  mounted() {
    const months = [
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec"
    ];
    let current_datetime = new Date(this.date);
    let formatted_date =
      months[current_datetime.getMonth()] +
      " " +
      current_datetime.getDate() +
      ", " +
      current_datetime.getFullYear();
    this.formatted = formatted_date;
    let event_date = new Date(this.date);
    let now = new Date();
    this.ended = event_date < now;
  }
};
</script>

<style lang="scss">
.vuejs-countdown {
  margin: 0px 0 15px 0 !important;
  li:after {
    top: 7px !important;
    font-size: 20px !important;
    font-weight: bold;
  }
  .digit {
    font-size: 26px !important;
  }
}
.event_header {
  color: #a7a7a7 !important;
  width: 100%;
}
.md-primary {
  a {
    color: #fff !important;
  }
}
.event_bottom {
  position: absolute;
  bottom: -111px;
  left: 0;
  right: 0;
  background-color: #fff;
  box-shadow: 0 3px 1px -2px rgba(0, 0, 0, 0.2),
    -2px 2px 4px 0 rgba(0, 0, 0, 0.14), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
  height: 110px;
  display: flex;
  justify-content: center;
  align-items: center;
  flex-direction: row-reverse;
  border-bottom: 2px solid #eeeeee;

  .feature {
    display: flex;
    flex-direction: column;
    padding: 0 20px;
    border-left: 1px solid #eee;
    &:last-child {
      border-left: 0px;
    }
    .md-icon {
      font-size: 30px !important;
      color: #b5b5b5;
      width: 30px !important;
      height: 30px !important;
    }
    .event_data {
      color: black;
      padding: 11px 0 0 0;
      font-size: 15px;
    }
  }
}
</style>
