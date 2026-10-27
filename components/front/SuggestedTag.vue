<template>
  <div class="container" v-if="tags.length > 0">
    <div class="md-headline">
      <h5>Suggested Tags</h5>
      <div class="more-link">
        <nuxt-link tag="md-button" to="">See All</nuxt-link>
      </div>
    </div>
    <div class="md-layout md-gutter">
      <div
        v-for="tag in tags"
        :key="tag.tid"
        class="md-layout-item"
      >
        <tag-teaser
          tagTeaserType="suggestedTag"
          :tag_name="tag.name"
          :tid="tag.tid"
          :tag_description="tag.meta_description"
          :user_follow="tag.user_follow"
          :follow_counts="tag.follower_count"
        />
      </div>
    </div>
  </div>
</template>

<script>
  import axios from 'axios'
  import TagTeaser from "@/components/fields/TagTeaser"
  export default {
    name: 'SuggestedTag',
    components: {
      TagTeaser
    },
    data() {
      return {
        tags: [],
      }
    },
    mounted(){
      axios.defaults.withCredentials = true
      axios.defaults.crossDomain = true
      axios.get('https://temp.ed808.com/api/latin/tag?parameter[page]=0&parameter[limits]=1000')
        .then((data)=>{
          // console.log(data.data)
          let tags = []
          data.data.tags.forEach(function (tag) {
            if(tag.user_follow == false) {
              tags.push(tag)
            }
          })
          this.tags = this.shuffle(tags)
        })
    },
    methods: {
      shuffle(array) {
        let counter = array.length - 1;

        // While there are elements in the array
        while (counter > 0) {
          // Pick a random index
          let index = Math.floor(Math.random() * counter);

          // Decrease counter by 1
          counter--;

          // And swap the last element with it
          let temp = array[counter];
          array[counter] = array[index];
          array[index] = temp;
        }
        console.log(array.slice(0,6))
        return array.slice(0,6);
      }
    }
  }
</script>
