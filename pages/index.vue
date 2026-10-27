<!--Done-->
<template>
  <div class="home-page">
    <nuxt-link to="/node/20760">
      <section id="banner">
        <div>
          <!-- <h1>International Social Network of Civil Engineers and Architects</h1> -->
          <!-- <span style="font-size: 11px">
          Enhance your working life with new contacts, inspiring industry news, and exciting specialized contents
          <span>ED808.COM</span>
          </span>-->
        </div>
      </section>
    </nuxt-link>

    <!--Up Comming Events SlideShow -->
    <event-carousel />

    <!-- Section for Services and Products -->
    <products />

    <!--Section for Suggested Tags -->
    <suggested-tag />

    <!--Section For Editor Picked Contents -->
    <!--<editor-picked />-->

    <!-- Section for Recent Contents -->
    <recent-content />

    <!-- Section for Favourite Contents -->
    <favourite-content />

    <!-- Section For All Contents -->
    <!--<node-list-->
    <!--class="md-card md-theme-default md-layout-item md-size-80"-->
    <!--:filterEnabled="false"-->
    <!--/>-->

    <!-- Subscription Popup -->
    <subscribe />
  </div>
</template>

<script>
import EditorPicked from "@/components/front/EditorPicked";
import EventCarousel from "@/components/front/EventCarousel";
import NodeList from "@/components/allContents/NodeList";
import Subscribe from "@/components/fields/Subscribe";
import RecentContent from "@/components/front/RecentContent";
import Products from "@/components/front/Products";
import FavouriteContent from "@/components/front/FavouriteContent";
import SuggestedTag from "@/components/front/SuggestedTag";

export default {
  name: "Home",
  components: {
    FavouriteContent,
    Subscribe,
    EventCarousel,
    RecentContent,
    Products,
    SuggestedTag
  },
  scrollToTop: true,
  data() {
    return {
      title: "Home",
      pic: "https://ed808.com/images/front-page.jpg"
    };
  },
  head() {
    return {
      links: [
        { rel: "canonical", href: "https://ed808.com" },
        { rel: "alternate", href: "https://ed808.com", hreflang: "en" },
        { rel: "shortlink", href: "https://ed808.com" }
      ],
      title: this.title,
      meta: [
        // OpenGraph data (Most widely used)
        { property: "og:title", content: this.title, hid: "og:title" },
        { property: "og:url", content: "https://ed808.com", hid: "og:url" },
        { property: "og:image", content: this.pic, hid: "og:image" },

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

<style lang="scss">
@import "@/assets/scss/vars.scss";
.home-page {
  section#banner {
    background-image: url(/images/corona.jpg);
    background-size: contain;
    background-position: center;
    background-repeat: no-repeat;
    height: 60vh;
    display: flex;
    align-items: center;
    position: relative;
    justify-content: flex-end;
    @media all and (max-width: 700px) {
      justify-content: center;
    }
    > div {
      // position: absolute;
      // bottom: 40px;
      // left: 0;
      // right: 0;
      margin: 0 11% 0 0;
      @media all and (max-width: 700px) {
        margin: 0px;
      }
      h1 {
        font-size: 24px;
        padding: 26px 26px;
        font-weight: bold;
        background-color: #ffffff;
        margin-bottom: 7px;
        @media all and (max-width: 600px) {
          font-size: 17px;
          padding: 26px 10px;
        }
      }
      > span {
        color: white;
        font-size: 17px;
        border: 1px solid white;
        padding: 24px 20px 30px 20px;
        display: inline-block;
        text-shadow: 2px 2px 6px #000;
        border-width: 1px 1px 0px 1px;
        position: relative;
        @media all and (max-width: 600px) {
          font-size: 16px;
          padding: 24px 10px 30px 10px;
        }
        > span {
          font-size: 10px;
          position: absolute;
          bottom: -7px;
          right: calc(50% - 28px);
        }
        &:before,
        &:after {
          content: "";
          position: absolute;
          background-color: white;
          width: 100px;
          height: 1px;
          bottom: 0;
        }
        &:before {
          left: 0;
        }
        &:after {
          right: 0;
        }
      }
    }
  }
  .md-layout-item.contents {
    @media #{$x480} {
      max-width: 100%;
      flex: 0 1 100%;
      margin: 0 15px 50px !important;
    }
  }
  .page-iframe .mejs-container {
    padding-bottom: 53% !important;
  }
  .admin-error {
    display: none;
  }
  .bg-elements-3-0 {
    fill: #ffc107;
  }
  .bg-elements-3-1 {
    fill: #00dffc;
  }
  .bg-elements-3-2 {
    fill: #00c9a7;
  }
  .bg-elements-3-3 {
    fill: #de4437;
  }
  .bg-elements-3-4 {
    fill: #377dff;
  }
  .bg-elements-4-0 {
    fill: #f3f3f3;
  }
}
</style>
