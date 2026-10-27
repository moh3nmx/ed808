<template>
    <div class="with-background">
      <div class="login-card-page topic-selection">
        <div class="card card-signup card-topics">
          <p>Tell us what you're interested in:</p>
          <div class="card-content">
            <div class="interested-topics" v-if="topics.length > 0">
              <div
                v-for="(t, index) in topics"
                :key="index"
                class="topic"
                :class="t.selected ? 'topic-active' : ''"
                :style="`background-image: url(${t.picture})`"
                @click="addToInterests(t.tid, index, $event)"
              >
                <span class="inside-text">
                  {{ t.name }}
                </span>
                <span class="check-text" v-if="t.selected" style="bottom: calc(50% - 13px);">
                  <i class="mdi mdi-check"></i>
                </span>
              </div>
            </div>
          </div>
          <div class="footer text-center" style="float: left; width: 100%">
            <md-button
              form="registerForm"
              class="md-primary"
              @click="submitInterests()"
              :disabled="sending"
            >
              Done
            </md-button>
          </div>
        </div>
      </div>
    </div>
</template>

<script>
  import axios from 'axios'
  import { cookie } from '@/components/mixins/cookie.js'


  export default {
    name: "InterestFields",

    data(){
      return {
        topics: [],
        selectedTopics: [],
        sending: false
      }
    },
    mixins: [cookie],
    mounted(){

      this.getTopics()
    },
    methods: {
      submitInterests(){
        this.sending = true
        let topics = []

        this.topics.forEach((el)=>{
          if(el.selected)
            topics.push(el.tid)
        })
        console.log(topics)
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        axios.post('https://ed808.com:92/latin/user/interests',
          {
            topics : topics
          },
          {
            headers: {
              'Content-type': 'application/json',
              'X-CSRF-Token': this.getCookie("token")
            }
          }
        )
          .then(()=>{
            this.sending = false
            this.$router.push('/')
          })
      },
      getTopics(){
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        axios.get('https://ed808.com:92/latin/contents/list/topics',{},{
          headers: {
            'Content-type': 'application/json',
          }
        })
          .then((data)=>{
            data.data.forEach((el)=> {
              let obj = {
                tid: el.tid,
                name: el.name,
                picture: el.picture,
                selected: false,
                height: false
              }
              this.topics.push(obj)
            })
          })
      },
      addToInterests(topic, index, event){
        this.topics[index].selected = !this.topics[index].selected
      }
    }

  }
</script>


