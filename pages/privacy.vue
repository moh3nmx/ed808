<!--Done-->
<template>
  <div class="privacy">
    <div class="top-pic">
      <img
        :src="pic"
        width="100%"
        height="241"
      >
      <h1>
        {{ title }}
      </h1>
    </div>
    <div class="inner container">
      <div class="atext" v-html="text"></div>
    </div>
  </div>
</template>

<script>
import axios from "@/node_modules/axios"

export default {
  name: "Privacy",
  scrollToTop: true,
  data() {
    return {
      text: "",
      pic: "",
      title: "Privacy Policies"
    };
  },
  async asyncData({ params, error }) {
    try {
      const { data } = await axios.get(
        "https://temp.ed808.com/api/latin/page/20166"
      );
      if (data) {
        return {
          text: data.body_value,
          pic: data.image,
          title: data.title,
        };
      }
    } catch (e) {
      if (e.response.hasOwnProperty('status')) {
        error({ statusCode: e.response.status, message: e.response.statusText })
      }
    }
  },
  head() {
    return {
      links: [
        { rel: "canonical", href: "https://ed808.com/privacy" },
        { rel: "alternate", href: "https://ed808.com/privacy", hreflang: "en" },
        { rel: "shortlink", href: "https://ed808.com/privacy" }
      ],
      title: "Privacy Policy",
      meta: [
        // OpenGraph data (Most widely used)
        { property: "og:title", content: this.title, hid: "og:title" },
        {
          property: "og:url",
          content: "https://ed808.com/privacy",
          hid: "og:url"
        },
        {
          property: "og:image",
          content: this.pic,
          hid: "og:image"
        },

        // Twitter card
        { name: "twitter:title", content: this.title, hid: "twitter:title" },
        {
          name: "twitter:image:src",
          content: this.pic,
          hid: "twitter:image:src"
        },

        // Google / Schema.org markup:
        { itemprop: "name", content: this.title, hid: "name" },
        { itemprop: "image", content: this.pic, hid: "image" }
      ]
    };
  }
};
</script>

<style scoped lang="scss">
.atext {
  margin: 50px 0;
  text-align: left;
}

.inner {
  padding: 0 36px;
}
.top-pic {
  height: 241px;
  position: relative;
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
  &:before {
    content: "";
    width: 100%;
    position: absolute;
    top: 0;
    right: 0;
    height: 100%;
    background-color: #0000005e;
    z-index: 1;
  }
  img {
    min-width: 100%;
    min-height: 100%;
    max-width: none;
    width: auto;
    position: absolute;
    z-index: 0;
  }
  h1 {
    text-shadow: 0px -1px 5px #000000, 0 -1px 0px #000000;
    color: white;
    position: relative;
    font-size: 35px;
    z-index: 2;
  }
}
.page-enter-active,
.page-leave-active {
  opacity: 1;
  transition: opacity 0.25s;
  animation-duration: 0.3s;
  animation-fill-mode: both;
}

.page-enter-active {
  animation-name: pageFadeInUp;
}

.page-leave-active {
  animation-name: pageFadeOutDown;
}

.page-enter,
.page-leave-to {
  opacity: 0;
}

@-webkit-keyframes pageFadeInUp {
  0% {
    opacity: 0;
    transform: translate3d(0, 1.25%, 0);
  }

  to {
    opacity: 1;
    transform: none;
  }
}

@-webkit-keyframes pageFadeOutDown {
  0% {
    opacity: 1;
  }

  to {
    opacity: 0;
    transform: translate3d(0, 1.25%, 0);
  }
}
.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.5s;
}
.fade-enter, .fade-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
/* Enter and leave animations can use different */
/* durations and timing functions.              */
.slide-fade-enter-active {
  transition: all 0.9s ease;
}
.slide-fade-leave-active {
  transition: all 0.8s cubic-bezier(1, 0.5, 0.8, 1);
}
.slide-fade-enter, .slide-fade-leave-to
/* .slide-fade-leave-active below version 2.1.8 */ {
  transform: translateX(10px);
  opacity: 0;
}
</style>
