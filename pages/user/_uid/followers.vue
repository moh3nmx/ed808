<template>
  <div>
    <div class="users">
      <div v-if="users.length == 0 && !loading">
        Nobody is following you!
      </div>
      <div v-if="loading">
        Please wait...
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
          :following.sync="user.user_follow"
        />
      </div>
    </div>
  </div>
</template>

<script>
  import axios from '@/node_modules/axios'
  import UserTeaser from "@/components/fields/UserTeaser";
  export default {
    name: "Followers",
    components: {UserTeaser},
    layout: 'userpanel',
    data() {
      return {
        users: [],
        loading: true
      }
    },
    mounted() {
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials  = true;
      axios.get(`https://ed808.com:92/latin/user/${this.$route.params.uid}/follower?parameter[sort]=time&parameter[sort_dir]=ASC&parameter[page]=0&parameter[limits]=10`)
        .then((data)=> {
          console.log(data.data)
          this.users = data.data.follower
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
