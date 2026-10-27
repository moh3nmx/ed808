<!--Done-->
<template>
  <div>
    <div :class="'main-container node_type_' + types[0] + (types.includes('service') ? ' node_type_event' : '')">

      <!--Content Progress Bar Based On Scroll-->
      <scroll-bar v-if="node_content.nid"></scroll-bar>
      <div class="top_header">
        <h1 v-if="node_content.hasOwnProperty('title')">
          {{ node_content.title }}
        </h1>
        <div
          v-if="!types.includes('event')"
          class="date_and_category"
        >
          <span v-if="node_content.hasOwnProperty('created')">
            {{ node_content.created }}
          </span>
          <nuxt-link
            v-if="node_content.hasOwnProperty('topic') && (node_content.topic.length != 0)"
            :to="'/contents?topic='+ node_content.topic[0].tid"
            target="_blank"
            :title="node_content.topic[0].name"
          >
            {{ node_content.topic[0].name }}
          </nuxt-link>
        </div>
        <div>
          <!-- If Contenttype is Service or Recorded Video-->
          <no-ssr>
            <course-data
              v-if="types.includes('service')"
              :date="node_content.event_time"
              :time="node_content.time_and_duration"
              :organizer="node_content.company"
              :place="node_content.event_place"
              :webinar_covered="node_content.ifwebinar"
              :registration_link="node_content.registration_link"
              :nid="nid"
              :author="author.full_name"
              :need_login='node_content.need_login'
            />
            <!-- If Contenttype is event-->
            <eventData
              v-if="types.includes('event')"
              :title="node_content.title"
              :meta="node_content.meta_description"
              :date="node_content.event_time"
              :time="node_content.time_and_duration"
              :organizer="node_content.company"
              :place="node_content.event_place"
              :webinar_covered="node_content.ifwebinar"
              :registration_link="node_content.registration_link"
              :nid="nid"
              :need_login='node_content.need_login'
            />
          </no-ssr>
          
        </div>
      </div>

      <!-- content main tag -->
      <md-content
        id="node_body"
        class="node-body"
      >
        <!-- if content type is podcast this tag will be shown -->
        <div
          v-if="types.includes('podcast') && node_content.hasOwnProperty('files') && (node_content.files.length != 0)"
          id="audio-demos-vuejs"
        >
          <!--audioplayer for podcasts-->
          <audio-player
            :source="node_content.files[0]"
          />
        </div>
        <!--if content has video & its link-->
        <youtube-video
          v-if="types.includes('video') && node_content.hasOwnProperty('video_link') && (node_content.video_link != null)"
          :url="node_content.video_link"
        />
        <!--content image comes here-->
        <img
          v-if="node_content.hasOwnProperty('image') && (node_content.image != null)"
          id="image"
          :src="convertDomain(node_content.image)"
          :alt="node_content.title"
        >
        <!-- Side Fixed Elements On Scroll (Social Share) -->
        <div style="position:relative;">
          <div>
            <affix
              class="sidebar-menu affix-bottom"
              relative-element-selector="#article"
              :offset="{ top: 20, bottom: 20 }"
              style=""
            >
              <div class="side-social">
                <span class="clap">
                  <!--Clap Button-->
                  <md-badge
                    md-position="bottom"
                    :md-content="node_content.clap_point"
                  >
                    <md-button
                      class="md-icon-button clap"
                      :disabled="node_content.user_clap >= 10"
                      @click="clapContent()"
                    >
                      <img
                        v-if="node_content.user_clap < 10"
                        :src="node_content.user_clap != 0 ? '/images/clap-active.png' : '/images/clap.png'"
                        height="23px"
                      >
                      <img
                        v-else
                        :src="!$store.getters.getUid ? '/images/clap.png' : '/images/clap-active.png'"
                        height="23px"
                        style="opacity: 0.6;"
                      >
                      <md-tooltip
                        v-if="$store.getters.getUid && node_content.user_clap < 10"
                        md-direction="bottom"
                      >
                        Clap
                      </md-tooltip>
                    </md-button>
                    <md-tooltip
                      v-if="node_content.user_clap >= 10"
                      md-direction="bottom"
                    >
                      You clapped so much!
                    </md-tooltip>
                    <md-tooltip
                      v-if="!$store.getters.getUid"
                      md-direction="bottom"
                    >
                      Please login to clap.
                    </md-tooltip>
                  </md-badge>
                </span>
                <span class="clap">
                  <!-- Bookmark Button -->
                  <md-button
                    class="md-icon-button"
                    :class="{ 'bookmark-active': node_content.user_bookmark }"
                    @click="bookmarkContent()"
                  >
                    <i
                      class="mdi"
                      :class="node_content.user_bookmark ? 'mdi-bookmark' : 'mdi-bookmark-outline'"
                      :style="'color:' + node_content.user_bookmark ? 'yellow' : ''"
                    />
                    <md-tooltip
                      v-if="!node_content.user_bookmark && $store.getters.getUid"
                      md-direction="bottom"
                    >
                      Bookmark this article
                    </md-tooltip>
                    <md-tooltip
                      v-if="node_content.user_bookmark && $store.getters.getUid"
                      md-direction="bottom"
                    >
                      Remove from bookmarks
                    </md-tooltip>
                  </md-button>
                  <md-tooltip
                    v-if="!$store.getters.getUid"
                    md-direction="bottom"
                  >
                    Please login to bookmark this content.
                  </md-tooltip>
                </span>


                <a href="#comments">
                  <md-button class="md-icon-button">
                    <i class="mdi mdi-comment-multiple-outline" />
                    <md-tooltip md-direction="bottom">
                      Jump to comments
                    </md-tooltip>
                  </md-button>
                </a>

                <md-button class="md-icon-button" @click="socialExpanded = !socialExpanded">
                  <i
                    class="mdi"
                    :class="socialExpanded ? 'mdi-close' : 'mdi-share-variant'"
                  />
                  <md-tooltip md-direction="bottom">
                    Share on social media.
                  </md-tooltip>
                </md-button>
                <transition name="fade">
                  <social-sharing
                    v-if="socialExpanded"
                    :url="'https://ed808.com/node/' + nid"
                    :description="meta_desc"
                    :title="node_content.title"
                    inline-template>
                    <div>
                      <network network="linkedin">
                        <md-button class="md-icon-button md-linkedin-icon">
                          <i class="mdi mdi-linkedin" />
                          <md-tooltip md-direction="bottom">
                            Share on LinkedIn
                          </md-tooltip>
                        </md-button>
                      </network>
                      <network network="facebook">
                        <md-button class="md-icon-button md-facebook-icon">
                          <i class="mdi mdi-facebook" />
                          <md-tooltip md-direction="bottom">
                            Share on Facebook
                          </md-tooltip>
                        </md-button>
                      </network>
                      <network network="twitter">
                        <md-button class="md-icon-button md-twitter-icon">
                          <i class="mdi mdi-twitter" />
                          <md-tooltip md-direction="bottom">
                            Share on Twitter
                          </md-tooltip>
                        </md-button>
                      </network>
                      <network network="whatsapp">
                        <md-button class="md-icon-button md-whatsapp-icon">
                          <i class="mdi mdi-whatsapp" />
                          <md-tooltip md-direction="bottom">
                            Share on WhatsApp
                          </md-tooltip>
                        </md-button>
                      </network>
                    </div>
                  </social-sharing>
                </transition>
              </div>

              <!--Content Headings Navigator-->

              <!--<scrollactive-->
              <!--class="my-nav"-->
              <!--@:itemchanged="onItemChanged"-->
              <!--active-class="active"-->
              <!--:offset="20">-->


              <!--<a :key="head.id" v-for="head in articleHeadings" :href="'#'+ head.id" class="scrollactive-item" v-html="head.text"></a>-->

              <!--</scrollactive>-->
            </affix>
          </div>

          <!--Text of content Comes Here-->
          <article
            v-if="node_content.hasOwnProperty('body_value') && (node_content.body_value != null)"
            id="article"
            v-html="convertDomain(node_content.body_value)"
          />
        </div>

        <!-- Section for Attached Files like Books & Archives or ... -->
        <h2
          v-if="types.includes('ebook') && node_content.files.length > 0"
          class="section-title"
        >
          Attached Files:
        </h2>
        <div
          v-if="types.includes('ebook') && node_content.files.length > 0"
          class="attachment"
        >
          <a
            v-for="file in node_content.files"
            :key="file"
            :href="file"
          >
            <div
              class="file-attached"
            >
              <div class="header">
                <i
                  v-if="file.split('/').pop().split('.').pop() == 'pdf'"
                  class="mdi mdi-file-pdf"
                />
                <i
                  v-if="file.split('/').pop().split('.').pop() == 'rar'
                    || file.split('/').pop().split('.').pop() == 'zip'"
                  class="mdi mdi-folder-multiple"
                />
              </div>
              <div class="footer">
                {{ file.split('/').pop() }}
              </div>
              <md-tooltip md-direction="bottom">
                Click to download this file.
              </md-tooltip>
            </div>
          </a>
        </div>

        <!-- Section For Content Refrences -->
        <div
          v-if="node_content.hasOwnProperty('references') && (node_content.references.length != 0)"
          class="reference"
        >
          <h3>Reference:</h3>
          <a
            v-for="(reference, index) in node_content.references"
            :key="index"
            :href="reference"
            target="_blank"
            rel="nofollow"
          >
            {{ reference }}
          </a>
        </div>

        <!-- Section for Content Tags -->
        <div
          v-if="node_content.hasOwnProperty('tags') && (node_content.tags.length != 0)"
          class="tags"
        >
          <content-tags
            v-for="(tag, index) in node_content.tags"
            :key="index"
            :name="tag.name"
            :tid="tag.tid"
          />
        </div>

        <!--Social Share & Content Actions in Mobile Mode -->
        <div class="share mobile-share">
          <div class="actions">
            <span class="clap">
              <md-button
                class="md-icon-button"
                @click="bookmarkContent()"
              >
                <i
                  class="mdi"
                  :class="node_content.user_bookmark ? 'mdi-bookmark' : 'mdi-bookmark-outline'"
                ></i>
                <md-tooltip
                  v-if="!node_content.user_bookmark && $store.getters.getUid"
                  md-direction="bottom"
                >
                  Bookmark this article
                </md-tooltip>
                <md-tooltip
                  v-if="node_content.user_bookmark && $store.getters.getUid"
                  md-direction="bottom"
                >
                  Remove from bookmarks
                </md-tooltip>
              </md-button>
              <md-tooltip
                v-if="!$store.getters.getUid"
                md-direction="bottom"
              >
                Please login to bookmark this content.
              </md-tooltip>
            </span>

            <span class="clap">
              <md-button
                class="md-icon-button clap"
                @click="clapContent()"
              >
                <img
                  v-if="node_content.user_clap < 10"
                  :src="node_content.user_clap != 0 ? '/images/clap-active.png' : '/images/clap.png'"
                  height="23px"
                >
                <img
                  v-else
                  :src="!$store.getters.getUid ? '/images/clap.png' : '/images/clap-active.png'"
                  height="23px"
                  style="opacity: 0.6;"
                >
                <md-tooltip
                  md-direction="bottom"
                  v-if="$store.getters.getUid && node_content.user_clap < 10"
                >
                  Clap
                </md-tooltip>
              </md-button>
              <md-tooltip
                md-direction="bottom"
                v-if="node_content.user_clap >= 10"
              >
                You clapped so much!
              </md-tooltip>
              <md-tooltip
                md-direction="bottom"
                v-if="!$store.getters.getUid"
              >
                Please login to clap.
              </md-tooltip>
            </span>
            <span
              v-if="node_content.clap_point != 0"
              class="clap-counts"
            >
              {{ node_content.clap_point }} {{ node_content.clap_point == 1 ? 'clap!' : 'claps!' }}
            </span>
          </div>

          <div class="social">
            <!--Content Social Share-->
            <social-sharing
              :url="'https://ed808.com/node/' + nid"
              :description="meta_desc"
              :title="node_content.title"
              inline-template
            >
              <div>

                <network network="linkedin">
                  <md-button class="md-icon-button md-linkedin-icon">
                    <i class="mdi mdi-linkedin" />
                    <md-tooltip md-direction="bottom">
                      Share on LinkedIn
                    </md-tooltip>
                  </md-button>
                </network>


                <network network="facebook">
                  <md-button class="md-icon-button md-facebook-icon">
                    <i class="mdi mdi-facebook" />
                    <md-tooltip md-direction="bottom">
                      Share on Facebook
                    </md-tooltip>
                  </md-button>
                </network>

                <network network="twitter">
                  <md-button class="md-icon-button md-twitter-icon">
                    <i class="mdi mdi-twitter" />
                    <md-tooltip md-direction="bottom">
                      Share on Twitter
                    </md-tooltip>
                  </md-button>
                </network>

                <network network="whatsapp">
                  <md-button class="md-icon-button md-whatsapp-icon">
                    <i class="mdi mdi-whatsapp" />
                    <md-tooltip md-direction="bottom">
                      Share on WhatsApp
                    </md-tooltip>
                  </md-button>
                </network>

              </div>
            </social-sharing>
          </div>
        </div>
      </md-content>

      <!-- Content Author Information -->
      <user-teaser
        teaserType="author"
        :uid="author.uid"
        :name="author.full_name"
        :picture="author.picture"
        :about_me="author.about_me"
        :following="author.user_follow"
      />

      <!-- Section For Related Contents -->
      <h2
        v-if="relatedNodes.length > 0"
        class="section-title"
      >
        Related Contents:
      </h2>
      <div
        v-if="relatedNodes.length > 0"
        class="md-layout node-page"
        style="position: relative;"
      >
        <node-teaser
          v-if="relatedNodes[0]"
          :title="relatedNodes[0].title"
          :pic="relatedNodes[0].picture"
          :date="relatedNodes[0].created"
          :nid="relatedNodes[0].nid"
          :type="relatedNodes[0].types"
        />
        <node-teaser
          v-if="relatedNodes[1]"
          :title="relatedNodes[1].title"
          :pic="relatedNodes[1].picture"
          :date="relatedNodes[1].created"
          :nid="relatedNodes[1].nid"
          :type="relatedNodes[1].types"
        />
        <node-teaser
          v-if="relatedNodes[2]"
          :title="relatedNodes[2].title"
          :pic="relatedNodes[2].picture"
          :date="relatedNodes[2].created"
          :nid="relatedNodes[2].nid"
          :type="relatedNodes[2].types"
        />
      </div>

      <!-- Section For Comments -->
      <div id="comments">
        <comments-block :nid="nid" />
      </div>

      <!-- Showing Errors For Sending Forms -->
      <md-snackbar
        class="error"
        :md-active.sync="showError"
      >
        {{ errors }}
      </md-snackbar>
    </div>
  </div>
</template>

<script>
import axios from "@/node_modules/axios";
import EventData from "@/components/fields/EventData";
import CourseData from "@/components/fields/CourseData";
import ScrollBar from "@/components/elements/ScrollBar";
import ContentTags from "@/components/fields/ContentTags";
import CommentsBlock from "@/components/fields/CommentsBlock";
import NodeTeaser from "@/components/allContents/NodeTeaser";
import AudioPlayer from "@/components/fields/AudioPlayer"
import { cookie } from "@/components/mixins/cookie.js";
import UserTeaser from "@/components/fields/UserTeaser";
// import AddToCalendar from 'vue-add-to-calendar'

export default {
  name: "Node",
  components: {
    UserTeaser,
    NodeTeaser,
    EventData,
    CourseData,
    ContentTags,
    CommentsBlock,
    ScrollBar,
    AudioPlayer,
    // AddToCalendar,
    YoutubeVideo: () => import("@/components/fields/YoutubeVideo")
  },
  mixins: [cookie],
  data() {
    return {
      nid: this.$route.params.nid,
      node_content: {},
      author: {},
      loading: true,
      meta_desc: '',
      types: [],
      errors: "",
      showError: false,
      relatedNodes: [],
      relatedLoading: false,
      socialExpanded: false,
      articleHeadings: [],
      source: '',
      addToCal:false
    };
  },

  // Validate URL Params
  validate ({ params }) {
    // Must be a number
    return /^\d+$/.test(params.nid)
  },
  // Fetch Content Data
  async asyncData({ params, query, req, error }) {
    try {
      axios.defaults.withCredentials = true
      axios.defaults.crossDomain = true
      let cookie =''
      if (req && req.headers && req.headers.cookie) {
        cookie = req.headers.cookie
      }
      const { data } = await axios({
        url: "https://ed808.com:92/latin/contents/" + params.nid,
        method: 'get',
        headers:  {
          'Cookie' :  cookie,
        }
      })

      if (data) {
        let contentTypes = [];
        let meta_desc= '';
        if (
          data.content.hasOwnProperty("type") &&
          data.content.type.length != 0
        ) {
          data.content.type.forEach(element => {
            if (element.tid == 4058) contentTypes.push("event");
            if (element.tid == 3938) contentTypes.push("podcast");
            if (element.tid == 3941) contentTypes.push("video");
            if (element.tid == 3940) contentTypes.push("ebook");
            if (element.tid == 4599) contentTypes.push("service");
          });
        }
        if(data.content.meta_description == '') {
          meta_desc = this.bodySummarizer(data.content.body_value)
        } else {
          meta_desc = data.content.meta_description
        }
        return {
          // Fill The Vars With Fetched Data
          node_content: data.content,
          meta_desc: meta_desc,
          types: contentTypes,
          author: data.author,
          loading: false
        };
      }
    }
    catch (e) {
      console.log(e);
      if (e.response.hasOwnProperty('status')) {
        error({ statusCode: e.response.status, message: e.response.statusText })
      }
    }
  },

  mounted() {
    this.getRelatedNodes();
    console.log(this.types)
    // this.getHeadings()
  },
  methods: {
    // Get Headings in Content Text For Navigating with links
    getHeadings() {
      let headings = document
        .getElementById("article")
        .getElementsByTagName("h2");
      let headObject = [];
      Array.from(headings).forEach(function(el, i) {
        let obj = {
          id: "head" + i,
          text: el.innerHTML
        };
        headObject.push(obj);
        el.setAttribute("id", "head" + i);
      });
      this.articleHeadings = headObject;
    },
    // To Clap for a Content
    clapContent() {
      if(!this.$store.getters.getUid){
        this.$store.commit('TOGGLE_LOGIN')
      }else{
        this.node_content.user_clap++;
        this.node_content.clap_point++;
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        axios
          .post(
            "https://ed808.com:92/latin/contents/" + this.nid + "/clap",
            {},
            {
              headers: {
                "Content-Type": "application/json",
                "X-CSRF-Token": this.getCookie("token")
              }
            }
          )
          .then(() => {

          })
          .catch(err => {
            console.log(err);
            this.node_content.user_clap--;
            this.node_content.clap_point--;
          });
      }
    },
    // Summarize Text to Limited Characters
    bodySummarizer(str){
      if(str) return str.replace(/<(?:.|\n)*?>/gm, '').slice(0,300).concat('<a :href="\'/node/\'+ post.nid">... Read More</a>')
      else return ''
    },
    // To Bookmark or Unbookmark a Content
    bookmarkContent() {
      if(!this.$store.getters.getUid){
        this.$store.commit('TOGGLE_LOGIN')
      }else {
        this.node_content.user_bookmark = !this.node_content.user_bookmark;
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        axios
          .post(
            "https://ed808.com:92/latin/contents/" + this.nid + "/bookmark",
            {
              action: !this.node_content.user_bookmark ? "unbookmark" : "bookmark"
            },
            {
              headers: {
                "Content-Type": "application/json",
                "X-CSRF-Token": this.getCookie("token")
              }
            }
          )
          .then(() => {
          })
          .catch(err => {
            console.log(err);
            this.node_content.user_bookmark = !this.node_content.user_bookmark;
          });
      }
    },
    convertDomain(value) {
      //this works but its performance is slow
      /* return value.split('="/sites').join('="http://api.ed808.com/sites') */

      String.prototype.replaceAll = function(search, replacement) {
        let target = this;
        return target.replace(new RegExp(search, "g"), replacement);
      };
      return value != null
        ? value
            .replaceAll('href="http://api.ed808.com', 'href="https://ed808.com')
            .replaceAll('href="https://ed808.com:92', 'href="https://ed808.com')
            .replaceAll('href="http://ed808.com:92', 'href="https://ed808.com')
            .replaceAll('="/sites', '="https://ed808.com:92/sites')
            .replaceAll('="/node', '="https://ed808.com/node')
        : "";
    },
    // To Get Related Contents and Show Them
    getRelatedNodes() {
      this.relatedLoading = true
      axios
        .get(
          "https://ed808.com:92/latin/contents/" +
            this.nid +
            "/relative?parameter[page]=1"
        )
        .then(response => response.data)
        .then(response => {
          // console.log( response )
          this.relatedNodes = response;
          this.relatedLoading = false
        });
    }
  },

  head() {
    return {
      links: [
        //these three line doesn't work
        {
          rel: "canonical",
          href: "https://ed808.com/node/" + this.$route.params.nid
        },
        {
          rel: "alternate",
          href: "https://ed808.com/node/" + this.$route.params.nid,
          hreflang: "en"
        },
        {
          rel: "shortlink",
          href: "https://ed808.com/node/" + this.$route.params.nid
        }
      ],
      title: this.node_content.title,
      meta: [
        {
          name: "description",
          content: this.node_content.meta_description,
          hid: "description"
        },

        // OpenGraph data (Most widely used)
        { property: "og:type", content: "article", hid: "og:type" },
        {
          property: "og:title",
          content: this.node_content.title,
          hid: "og:title"
        },
        {
          property: "og:url",
          content: "https://ed808.com/node/" + this.$route.params.nid,
          hid: "og:url"
        },
        {
          property: "og:image",
          content: this.node_content.image,
          hid: "og:image"
        },
        {
          property: "og:description",
          content: this.node_content.meta_description,
          hid: "og:description"
        },

        // Twitter card
        {
          name: "twitter:title",
          content: this.node_content.title,
          hid: "twitter:title"
        },
        {
          name: "twitter:image:src",
          content: this.node_content.image,
          hid: "twitter:image:src"
        },
        {
          name: "twitter:description",
          content: this.node_content.meta_description,
          hid: "twitter:description"
        },

        // Google / Schema.org markup:
        { itemprop: "name", content: this.node_content.title, hid: "name" },
        {
          itemprop: "image",
          content: this.node_content.image,
          hid: "image"
        },
        {
          itemprop: "description",
          content: this.node_content.meta_description,
          hid: "description"
        }
      ]
    };
  }
};
</script>


<style lang="scss">
@import '@/assets/scss/vars.scss';


@mixin main-center-content() {
  max-width: 800px;
  text-align: left;
  direction: ltr;
  margin: auto;
  padding: 0 15px;
  @media screen and (max-width: 992px){
    width: 80%;
    margin: auto;
  }
  @media screen and (max-width: 600px){
    width: 95%;
    margin: auto;
  }
}
body {
  background-color: white !important;
}
.main-container {
  margin-bottom: 50px;
  padding: 0px 0 15px 0;
  #audio-demos-vuejs {
    max-width: 800px;
    margin: auto;
    padding: 15px;
  }
  .md-layout {
    @include main-center-content();
  }
  h2.section-title {
    @include main-center-content();
    text-align: left;
    font-size: 22px;
    margin: 35px auto 11px auto !important;
    line-height: 40px;
  }
  .loading {
    position: absolute;
    width: 100%;
    height: 5px;
    left: 0;
    top: 90px;
    background: #fff;
    z-index: 3;
  }
  .edit_link {
    position: absolute;
    top: 78px;
    left: 25px;
  }
  .top_header {
    min-height: 220px;
    display: flex;
    justify-content: center;
    align-items: center;
    background-image: linear-gradient(to right, #eee, #f9f9f9);
    flex-direction: column;
    position: relative;
    h1 {
      font-family: sans-serif;
      color: black;
      margin-bottom: 30px !important;
      line-height: 43px;
      padding: 0 10px;
      @media all and (max-width: 600px) {
        font-size: 24px;
        line-height: 30px;
      }
    }
    > .date_and_category {
      height: 40px;
      display: flex;
      justify-content: space-between;
      align-items: center;
      width: 80%;
      position: absolute;
      bottom: 13px;
      @media all and (max-width: 768px) {
        flex-direction: column;
        margin: 10px;
        height: 49px;
        position: relative;
      }
      a {
        color: #0000008a !important;
        font-size: 17px;
        @media all and (max-width: 768px) {
          padding-top: 10px;
        }
      }
      span {
        //color: #ccc !important;
        color: #0000008a !important;
        font-size: 15px;
      }
    }
  }
  .node-body {
    > img {
      margin: 0px 0px 0px 0px;
      max-width: 100%;
      box-shadow: 0 3px 1px -2px rgba(0, 0, 0, 0.2),
        0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 1px 5px 0 rgba(0, 0, 0, 0.12);
      /* border: 1px solid #eee; */
    }
    article {
      @include main-center-content();
      font-size: 18px;
      line-height: 33px;
      letter-spacing: 0.3px;
      margin-top: 30px !important;
      .mejs-container .mejs-controls .mejs-time span {
        font-family: 'Roboto' !important;
        font-size: 13px !important;
        width: 78px;
        line-height: 13px !important;
      }
      p {
        margin: 0 0 10px 0;
      }
      h2 {
        font-size: 22px;
        margin: 35px 0 11px 0;
        line-height: 40px;
      }
      h3 {
        font-size: 20px;
        margin: 24px 0 12px 0;
        line-height: 38px;
      }
      h4 {
        font-size: 19px;
        margin: 18px 0 12px 0;
        line-height: 36px;
      }
      h5,
      h6 {
        font-size: 18px;
        margin: 18px 0 12px 0;
        line-height: 33px;
      }
      img {
        display: block;
        margin: 40px auto;
      }
      .picture_info {
        text-align: center;
        margin: -29px 0 40px 0;
        font-size: 15px;
      }
      .boldCenter_line {
        display: inline-block;
        width: 100%;
        text-align: center !important;
        padding: 20px 0;
        background: rgb(252, 252, 252);
        border: 1px solid #f0f1f2;
        margin: 15px 0 !important;
        a {
          display: inline-block;
          width: 100%;
          text-align: center;
        }
      }
    }
    &:before {
      content: "";
      display: table;
      width: 0;
      height: 0;
    }
  }
  .tags {
    @include main-center-content();
    margin: 20px auto 20px auto !important;
    a {
      display: inline-block;
    }
    button {
      border: none;
      color: rgba(0, 0, 0, 0.68) !important;
      background: rgba(0, 0, 0, 0.05) !important;
      box-shadow: none !important;
      text-transform: unset !important;
      margin: 6px 8px 6px 0px !important;
    }
  }
  .reference {
    @include main-center-content();
    overflow: hidden;
    h3 {
      display: inline;
    }
  }
}
.md-snackbar.error {
  background-color: #f44336;
}
.node_type_event {
  .top_header {
    height: 300px !important;
    justify-content: center;
    margin-bottom: 110px;
  }
}
.node_type_video {
  .node-body > img {
    display: none;
  }
}
#audio-demos-vuejs {
  /*margin: 45px auto 30px auto;*/
  /*padding: 0 10%;*/
  > div {
    /*box-shadow: 0 3px 1px -2px rgba(0,0,0,.2), 0 2px 2px 0 rgba(0,0,0,.14), 0 1px 5px 0 rgba(0,0,0,.12);*/
    padding: 20px 15px;
    border: 1px solid #eee;
    border-radius: 20px;
  }
}
.vue-affix {
  /*padding-top: 20px;*/
  position: absolute !important;
  width: calc(((100% - 800px) / 2) - 15px);
  left: 0;
  @media screen and (max-width: 992px) {
    width: 10%;
  }
  @media screen and (max-width: 600px) {
    display: none;
  }
  a.scrollactive-item {
    display: block;
  }
  a.active {
    color: #fff;
    background: #92278f;
    background: #92278f;
    background: var(--md-theme-default-primary-on-background, #92278f);
  }
  &.affix {
    position: fixed !important;
  }
}
.affix-bottom {
  position:absolute;
}
.side-social {
  width: 50px;
  margin: auto;
  .md-ripple {
    .md-button-content {
      .mdi {
        color: rgba(0, 0, 0, 0.54);
        color: rgba(0, 0, 0, 0.54);
        color: var(--md-theme-default-icon-on-background, rgba(0, 0, 0, 0.54));
      }
      .mdi-facebook {
        color: #3b5998;
      }
      .mdi-linkedin {
        color: #0976b4;
      }
      .mdi-twitter {
        color: #55acee;
      }
      .mdi-whatsapp {
        color: #25D366;
      }
    }
  }
}
.attachment {
  @include main-center-content();
  display: flex;
  flex-wrap: wrap;
  margin-bottom: 30px !important;
  height: auto;
  .file-attached {
    width: 150px;
    float: left;
    margin-right: 15px;
    text-align: center;
    box-shadow: 0 3px 1px -2px rgba(0,0,0,.2),
    0 2px 2px 0 rgba(0,0,0,.14),
    0 1px 5px 0 rgba(0,0,0,.12);
    border-radius: 5px;
    padding: 10px;
    transition-duration: 0.5s;
    .header{
      width: 100%;
      padding: 20px 0;
    }
    .footer {
      width: 100%;
      overflow: hidden;
      color: #424242;
      font-weight: 500;
      font-size: 12px;
      transition-duration: 0.5s;
    }
    .mdi {
      transition-duration: 0.5s;
      font-size: 40px !important;
      &.mdi-folder-multiple{
        color: #5E35B171 !important;
      }
      &.mdi-file-pdf {
        color: #e5393571 !important;

      }
    }
    &:hover {
      .footer {
        width: 100%;
        overflow: hidden;
        color: #92278f;
      }
      .mdi {
        font-size: 40px !important;
        &.mdi-folder-multiple{
          color: #5E35B1 !important;
        }
        &.mdi-file-pdf {
          color: #e53935 !important;
        }
      }
      background: #eeeeee;
      box-shadow: 0 3px 1px -2px rgba(0,0,0,.3),
      0 2px 2px 0 rgba(0,0,0,.24),
      0 1px 5px 0 rgba(0,0,0,.22);
    }
  }
}
.bookmark-active {
  .mdi {
    color: #fbc02d !important;
  }
}
.share {
  &.mobile-share{
    display: none;
    @media #{$x600} {
      display: block;
    }
  }
  @include main-center-content();
  text-align: right;
  margin: 20px auto;
  min-height: 40px;
  .social {
    float: right;
    @media #{$x600} {
      float: none;
      text-align: center;
      margin-bottom: 10px;
    }
  }
  .actions {
    float: left;
    @media #{$x600}  {
      float: none;
      text-align: center;
      margin-bottom: 10px;
    }
    .md-button.clap {
      &:hover {
        animation: clapAnim infinite 2s;
      }
    }
    .bookmark-active {
      .mdi {
        color: #fbc02d !important;
      }
    }
    span.clap-counts {
      display: inline-block;
      line-height: 40px;
      margin-left: 5px;
    }
    span.clap {
      position: relative;
      top: -2px;
      padding-top: 7px;
      padding-bottom: 13px;
      min-height: 40px;
    }
  }
  .md-ripple {
    .md-button-content {
      .mdi {
        color: rgba(0, 0, 0, 0.54);
        color: rgba(0, 0, 0, 0.54);
        color: var(--md-theme-default-icon-on-background, rgba(0, 0, 0, 0.54));
      }
      .mdi-facebook {
        color: #3b5998;
      }
      .mdi-linkedin {
        color: #0976b4;
      }
      .mdi-twitter {
        color: #55acee;
      }
      .mdi-whatsapp {
        color: #25D366;
      }
    }
  }
}

blockquote {
  font-style: italic;
  padding: 10px 20px;
  margin: 0 0 20px;
  font-size: 17.5px;
  border-left: 5px solid #eee;

  p {
    font-size: 1.35em;
    line-height: 1.5em;
    color: #555555;
    margin-bottom: 30px;
    &:before {
      content: '\f27e';
      font-family: mat;
      font-size: 18px;
      display: inline-block;
      transform: rotate(180deg);
      position: relative;
      bottom: 5px;

    }
    &:after {
      content: '\f27e';
      font-family: mat;
      font-size: 18px;
      position: relative;
      bottom: 5px;
    }
  }
}

.md-content  {
  position: relative;
}

.mdi {
  width: 24px;
  min-width: 24px;
  height: 24px;
  font-size: 24px !important;
  margin: auto;
  display: inline-flex;
  -webkit-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
  align-items: center;
  justify-content: center;
  vertical-align: middle;
}

@media (min-width: 1440px) {
  .node-page .content-teaser {
    width: 23%;
    flex: 0 1 23%;
  }
}

@media (min-width: 1200px) {
  .node-page .content-teaser {
    width: 31.3%;
    flex: 0 1 31.3%;
  }
}
@media (max-width: 600px) {
  .node-page .content-teaser {
    width: 100%;
    flex: 0 1 100%;
  }
}
@keyframes clapAnim {
  0% {
    -webkit-box-shadow: 0 0;
    box-shadow: 0 0;
  }
  70% {
    -webkit-box-shadow: 0 0 5px 10px rgba(255, 255, 255, 0);
    box-shadow: 0 0 5px 10px rgba(255, 255, 255, 0);
  }

  100% {
    -webkit-box-shadow: 0 0 0 0 rgba(255, 255, 255, 0);
    box-shadow: 0 0 0 0 rgba(255, 255, 255, 0);
  }
}

table {
  border-collapse: collapse;
  border-spacing: 0;
  width: 100%;
  border: 1px solid #EEEEEE;
  border-radius: 5px;
}

th, td {
  text-align: center;
  padding: 8px;
}
table {
  tr:nth-child(even) {
    background-color: #F5F5F5;
  }
  tr:first-child {
    font-weight: 500;
  }
}

.fade-enter-active, .fade-leave-active {
  transition: opacity .5s;
}
.fade-enter, .fade-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
</style>
