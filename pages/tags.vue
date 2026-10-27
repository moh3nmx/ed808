<template>
  <div>
    <div class="calendar-header">
      <h2>Tags</h2>
    </div>
    <div class="container-raised">
      <div class="tag-filters md-layout">
        <div class="md-layout-item md-size-70">
          <md-field>
            <md-icon>search</md-icon>
            <label>Search in tags</label>
            <md-input v-model="searchText" @keyup.native.enter="searchInTags()"></md-input>
            <span class="md-helper-text">Type and press enter</span>
          </md-field>
        </div>
        <div class="md-layout-item md-size-30 sort-select">
          <md-field>
            <label for="sort">Sort By:</label>
            <md-select
              v-model="sortBy"
              name="sort"
              id="sort"
              @change="getTagList()"
            >
              <md-option value="newest">Recent</md-option>
              <md-option value="first">Early</md-option>
              <md-option value="most_content">Most Contents</md-option>
              <md-option value="most_user">Most Followers</md-option>
            </md-select>
          </md-field>
        </div>
      </div>
      <p v-if="searchText != ''">Results for '{{ searchText }}'</p>
      <transition name="fade">
        <div
          class="tag-holder"
          v-if="tags.length > 0"
        >
          <tag-teaser
            v-for="tag in tags"
            :key="tag.tid"
            :tag_name="tag.name"
            :tag_image="tag.image"
            :tid="tag.tid"
            :tag_description="tag.meta_description"
            :contents="tag.count_contents"
            :user_follow="tag.user_follow"
            :follow_counts="tag.follower_count"
          />
        </div>
      </transition>
      <!-- Pagination -->
      <div class="pager-container">
        <md-button
          class="md-icon-button"
          @click="set_pager('first')"
          v-if="pager.length > 1 && page > 1"
        >
          <md-icon>first_page</md-icon>
        </md-button>
        <md-button
          class="md-icon-button"
          @click="set_pager('prev')"
          v-if="pager.length > 1 && page > 1"
        >
          <md-icon>chevron_left</md-icon>
        </md-button>
        <md-button
          class="md-icon-button"
          v-for="i in pager"
          :key="i"
          v-if="(page > 5 && i < page + 5 && i > page - 5) || (page < 5 && i < 9)"
          :class="pager_classes(i)"
          @click="set_pager(i)"
        >{{i}}</md-button>
        <md-button
          class="md-icon-button"
          @click="set_pager('next')"
          v-if="pager.length > 1 && page < pager.length"
        >
          <md-icon>chevron_right</md-icon>
        </md-button>
        <md-button
          class="md-icon-button"
          @click="set_pager('last')"
          v-if="pager.length > 1 && page < pager.length"
        >
          <md-icon>last_page</md-icon>
        </md-button>
      </div>
    </div>
  </div>
</template>

<script>
import TagTeaser from '@/components/fields/TagTeaser'
import axios from 'axios'
export default {
  name: 'Tags',
  components: {
    TagTeaser
  },
  data() {
    return {
      isLoading: false,
      searchText: '',
      sortBy: 'most_content',
      tags: [],
      count: 0,
      page: 1,
      pager: [],
    }
  },
  async asyncData({params, query, req, error}){
    try {
      axios.defaults.withCredentials = true
      axios.defaults.crossDomain = true
      let cookie =''
      if (req && req.headers && req.headers.cookie) {
        cookie = req.headers.cookie
      }
      let { data } = await axios({
        url: "https://ed808.com:92/latin/tag",
        method: 'get',
        headers:  {
          'Cookie' :  cookie,
        }
      })
      return {
        tags: data.tags,
        count: data.count,
        page: data.page + 1
      }
    }
    catch (e) {
      console.log(e)
      if (e.hasOwnProperty('response') && e.response.hasOwnProperty('status')) {
        error({ statusCode: e.response.status, message: e.response.statusText })
      }
    }
  },
  methods: {
    searchInTags(){
      this.page = 1
      this.getTagList()
    },
    // Get List Of Tags
    getTagList(){
      this.isLoading = true
      this.tags = []
      axios.defaults.withCredentials = true
      axios.defaults.crossDomain = true
      axios.get(`https://ed808.com:92/latin/tag?parameter[page]=${this.page-1}&parameter[limits]=30&parameter[search]=${this.searchText}&parameter[sort]=${this.sortBy}`)
      .then((data)=>{
        console.log(data.data)
        this.tags = data.data.tags,
        this.count = data.data.count,
        this.page = parseInt(data.data.page) + 1
      })
    },
    // Update Pagination
    update_pager() {
      var i = 1;
      this.pager = [];
      if (this.count > 30) {
        this.pager.push(i);
        while (i * 30 < this.count) {
          this.pager.push(i + 1);
          i++;
        }
      }
    },
    // Set Active Page
    pager_classes(i) {
      return { active: i == this.page };
    },
    set_pager(i) {
      switch (i) {
        case "first":
          this.page = 1;
          break;
        case "prev":
          this.page--;
          break;
        case "next":
          this.page++;
          break;
        case "last":
          this.page = Math.ceil(this.count / 30);
          break;
        default:
          this.page = i;
          break;
      }
      this.getTagList();
    },
  },
  mounted(){
    console.log(this.tags)
    this.update_pager()
    // Todo: Handle Pagination
  },
  watch: {
    sortBy: function(event) {
      console.log(this.sortBy)
      this.getTagList()
    }
  },
  head(){
      return{
        title: 'Tags',
        meta: [
          // OpenGraph data (Most widely used)
          {
            'property': 'og:title',
            'content': 'Tags',
            'template': '%s - ed808',
            'vmid': 'og:title'
          },
          {property: 'og:url', content: 'https://ed808.com/tags', vmid: 'og:url'},
          {
            'name': 'twitter:title',
            'content': 'Tags',
            'template': '%s - ed808',
            'vmid': 'twitter:title'
          },
          // Google / Schema.org markup:
          {
            'itemprop': 'name',
            'content': 'Tags',
            'template': '%s - ed808',
            'vmid': 'name'
          },
        ],
        links: [
          {rel: 'canonical', href: 'https://ed808.com/tags'}
        ]
      }
    }
}
</script>

<style lang="scss" scoped>
  @import "../assets/scss/vars.scss";

  .calendar-header {
    min-height: 50vh;
    position: relative;
    background-size: cover;
    background-position: center;
    overflow: hidden;
    background-image: url(/images/tag.jpg);

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
      margin: -60px 15px 0;
      padding: 15px 15px;
    }
    .tag-holder {
      display: flex;
      flex-wrap: wrap;
    }
  }

  .tag-filters {
    padding: 10px;
  }

  .pager-container {
    margin: 30px;
    text-align: center;
    button {
      float: none;
      background: transparent;
      box-shadow: none;
      color: rgba(0, 0, 0, 0.87) !important;
      &.active {
        background: #fff !important;
        border: 1px solid #9c27b0;
        box-shadow: none !important;
        color: #9c27b0 !important;
      }
    }
  }

  .sort-select {
    padding-left: 20px;
  }

  .md-size-30, .md-size-70 {
    @media screen and (max-width: 720px){
      min-width: 100%;
      max-width: 100%;
      flex: 0 1 100%;
      &.sort-select {
        padding-left: 0;
      }
    }
  }
</style>
