<template>
  <!-- following tag + following user -->
  <div>
    <div class="users">

      <div v-if="loading">
        Please wait...
      </div>
      <div v-if="users.length == 0 && !loading">
        You are not following any user!
      </div>
      <div
        v-if="users.length != 0 && !loading"
      >
        <UserTeaser
          v-for="user in users"
          :key="user.uid"
          teaserType="list-item"
          :uid="user.uid"
          :name="user.full_name"
          :picture="user.picture"
          :about_me="user.job"
          :following="user.user_follow"
        />
      </div>

    </div>
  </div>
</template>

<script>
  import axios from '@/node_modules/axios'
  import UserTeaser from "@/components/fields/UserTeaser";
  export default {
    name: "Following",
    components: {UserTeaser},
    layout: 'userpanel',
    data() {
      return {
        users: [],
        loading: true
      }
    },
    asyncData({params, app}){

    },
    mounted() {
      console.log(this.$route.params.uid)
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials  = true;
      axios.get(`https://temp.ed808.com/api/latin/user/${this.$route.params.uid}/following?parameter[sort]=time&parameter[sort_dir]=ASC&parameter[page]=0&parameter[limits]=10`,
        {},
        {
          headers: {
            'Cache-Control': 'no-cache',
            'cache-control': 'no-cache',

          }
        })
        .then((data)=> {
          console.log(data.data)
          this.users = data.data.following
          setTimeout(()=>{this.loading = false},500)
        })
    }
  }
</script>

<style lang="scss" scoped>
  @import '@/assets/scss/vars.scss';
  .users {
    margin: auto;
    width: 60%;
    @media #{$x960} {
      width: 100%;
    }
  }
</style>
