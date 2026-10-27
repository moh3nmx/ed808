<template>
  <div>
    <!--login with overlay-->
    <md-dialog class="login-dialog" :md-active.sync="$store.state.loginActive">
      <div @click="hideLoginCard($event)">
        <div class="login-card active">
          <login-card :pageComponent="false" />
        </div>
      </div>
    </md-dialog>

    <!--Main Navigation menu-->
    <main-nav
      :transparent="true"
      class="fixed"
    />

    <!--Main Contents-->
    <md-content class="top">
      <!--Page Header with background Image-->
      <div
        class="background-image"
        :style="`background-image: url(${$store.state.userBackground ? $store.state.userBackground : user.background_image})`"
      >
        <div
          v-if="sameUser"
          class="edit-background-image"
        >
          <input
            id="file1"
            ref="file1"
            type="file"
            @change="handleFileUpload('background_image')"
          />
          <md-icon class="md-size-2x">
            edit
          </md-icon>
        </div>
        <div
          v-if="updateFile.background_image"
          class="spinner-loading"
        >
          <md-progress-spinner
            :md-diameter="100"
            :md-stroke="5"
            md-mode="indeterminate"
          />
        </div>

        <nuxt-link class="md-settings" v-if="sameUser" :to="'/user/'+ routUid +'/settings/'">
          <md-icon>settings</md-icon>
          <span>Settings</span>
        </nuxt-link>

      </div>
      <!--User Avatar-->
      <div class="user-image">
        <div
          v-if="updateFile.picture"
          class="dim"
        ></div>

        <img
          v-if="$store.state.userImage || user.picture "
          :src="$store.state.userImage ? $store.state.userImage : user.picture"
          :alt="'image of ' + user.name" v-bind:title="'image of ' + user.name"
          class="img-raised"
        >
        <img
          v-else
          src="/images/avatar.png"
          :alt="'image of ' + user.name"
          :title="'image of ' + user.name"
          class="img-raised"
        >
        <div
          v-if="sameUser"
          class="edit-picture"
        >
          <input
            id="file2"
            ref="file2"
            type="file"
            @change="handleFileUpload('picture')"
          />
          <md-icon>photo_camera</md-icon>
        </div>
        <div
          v-if="updateFile.picture"

          class="spinner-loading"
        >
          <md-progress-spinner
            :md-stroke="3"
            md-mode="indeterminate"
          />
        </div>
      </div>
      <!--Container-->
      <div class="container-raised">
        <!--Profile Tabs-->
        <div class="profile-tabs">
          <md-bottom-bar md-type="fixed">
            <nuxt-link :to="'/user/'+ routUid">
              <md-bottom-bar-item
                id="t1"
                md-label="Profile"
                md-icon="account_circle"
              />
            </nuxt-link>
            <nuxt-link :to="'/user/'+ routUid +'/followers'">
              <md-bottom-bar-item
                id="t2"
                md-label="Followers"
                md-icon="people"
              />
            </nuxt-link>
            <div class="space"></div>
            <nuxt-link :to="'/user/'+ routUid +'/following'">
              <md-bottom-bar-item
                id="t3"
                md-label="Following"
                md-icon="people_outline"
              />
            </nuxt-link>
            <nuxt-link :to="'/user/'+ routUid +'/activity'">
              <md-bottom-bar-item
                id="t4"
                md-label="Activities"
                md-icon="access_time"
              />
            </nuxt-link>
          </md-bottom-bar>
        </div>

        <!-- Page Contents Goes Here  -->
        <nuxt />
      </div>
    </md-content>

    <footer class="footer footer-transparent footer-big userpanel-footer">
      <div class="container">
        <div class="content">
          <div class="md-layout">
            <div class="md-layout-item md-medium-size-50 md-large-size-50 md-small-size-100">
              <ul class="links-horizontal">
                <li>
                  <nuxt-link to="/aboutus">
                    About Us
                  </nuxt-link>
                </li>
                <li>
                  <nuxt-link to="/contactus">
                    Contact Us
                  </nuxt-link>
                </li>
                <li>
                  <nuxt-link to="/faq">
                    FAQ
                  </nuxt-link>
                </li>
                <li>
                  <nuxt-link to="/privacy">
                    Privacy Policy
                  </nuxt-link>
                </li>
              </ul>
            </div>
            <div class="copyright md-layout-item md-medium-size-50 md-large-size-50 md-small-size-100">
              Copyright © 2019 ED 808 .ltd All Rights Reserved.
            </div>
          </div>
        </div>
      </div>
    </footer>

    <!-- Side Menu For Mobile -->
    <side-menu/>
  </div>
</template>

<script>
import axios from "@/node_modules/axios";
import { cookie } from "../components/mixins/cookie.js";
import MainNav from "../components/elements/MainNav";
import SideMenu from "../components/elements/SideMenu";
import LoginCard from "../components/fields/LoginCard";

export default {
  name: "userpanel",
  components: { MainNav, LoginCard, SideMenu },
  scrollToTop: true,
  mixins: [cookie],
  head() {
    return {
      title: this.user.full_name
    };
  },
  data() {
    return {
      //for keeping last field data temporary, after editing it
      user_copy: {},
      following: false,
      errors: "",
      roles: [],
      uid: this.$route.params.uid,
      loading: true,
      updateField: false,
      user: {},
      editingEl: "",
      sameUser: false,
      updateFile: {
        picture: false,
        background_image: false,
        cv: false
      },
      file: {
        picture: {},
        background_image: {},
        cv: {}
      },
      afterUpload: {
        picture: {},
        background_image: {},
        cv: {}
      }
    };
  },
  mounted() {
    this.isSameUser();
    this.getProfile();
    this.loading = false;
    this.uid = this.$route.params.uid;
    this.$router.afterEach((to, from) => {
      this.getProfile();
      this.isSameUser();
    });
    // this.$store.commit('SET_PROFILE', this.user.picture, this.user.background_image)
  },
  methods: {
    isSameUser() {
      if (this.$store.getters.getUid) {
        this.sameUser = this.$store.getters.getUid == this.$route.params.uid;
      }
    },
    // Set Type of Uploading File
    handleFileUpload(fieldName) {
      //start loading for files
      this.updateFile[fieldName] = true;

      if (fieldName == "picture") {
        this.file[fieldName] = this.$refs.file2.files[0];
      } else if (fieldName == "background_image") {
        this.file[fieldName] = this.$refs.file1.files[0];
        console.log(this.$refs.file1.files[0])
      } else if (fieldName == "cv") {
        this.file[fieldName] = this.$refs.file3.files[0];
      }

      this.submitFile(fieldName);
    },

    // Upload File To server
    submitFile(fieldName) {
      let formData = new FormData();
      formData.append("file", this.file[fieldName]);
      //needs validation here
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      axios
        .post(
          "https://ed808.com:92/latin/file?parameter[hash]=020042f70a981fd6806ecf5e53f2267b377da9d9f981e15297d70c3f7c2a87d0",
          formData,
          {
            headers: {
              "Content-Type": "multipart/form-data",
              "X-CSRF-Token": this.getCookie("token")
            }
          }
        )
        .then(data => {
          this.afterUpload[fieldName] = data.data.file;

          //@TODO: add some prettier notification here after uploading cv with success
          //stop loading for files
          this.updateFile["cv"] = false;
          if (fieldName == "picture" || fieldName == "background_image") {
            this.CompleteEditingThis(fieldName);
          }
        })
        .catch(e => {
          console.log("FAILURE!!" + e);
        });
    },
    onEditingFields(fieldName) {
      this.editingEl = fieldName;
      //stop loading for fields because after calling CompleteEditingThis for save file it get true
      this.updateField = false;
    },
    cancelEditingFields(fieldName) {
      if (fieldName == "education") {
        this.user["university"] = this.user_copy["university"];
        this.user["education_degree"] = this.user_copy["education_degree"];
        this.user["education_field"] = this.user_copy["education_field"];
      } else if (fieldName == "personal") {
        this.user["full_name"] = this.user_copy["full_name"];
        this.user["mail"] = this.user_copy["mail"];
        this.user["mobile"] = this.user_copy["mobile"];
      } else if (fieldName == "work") {
        this.user["job"] = this.user_copy["job"];
        this.user["skills"] = this.user_copy["skills"];
      } else {
        this.user[fieldName] = this.user_copy[fieldName];
      }
      //switch from editing section to showing data
      this.editingEl = "";
    },
    CompleteEditingThis(fieldName) {
      //start loading for fields except files
      this.updateField = true;

      //create data with field that was changed
      var data = this.createData(fieldName);

      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      axios
        .put(
          "https://ed808.com:92/latin/user/" + this.$route.params.uid,
          data,
          {
            headers: {
              "Content-type": "application/json",
              "X-CSRF-Token": this.getCookie("token")
            }
          }
        )
        .then(data => {
          if (fieldName == "picture" || fieldName == "background_image") {
            this.afterFileSaved(fieldName, data);
          } else {
            this.afterfieldsaved(fieldName, data);
          }
        })
        .catch(e => {
          if (e.hasOwnProperty("response")) {
            if (e.response.hasOwnProperty("data"))
              this.errors = e.response.data;
            else {
              this.errors = e.response;
            }
          } else {
            this.errors = e;
          }
          this.showError = true;
        });
    },
    afterFileSaved(fieldName, data) {
      this.user[fieldName] = data.data[fieldName];

      //stop loading for files
      this.updateFile[fieldName] = false;

      //update navbar with new picture
      if (fieldName == "picture") {
        this.$store.commit("CHANGE_USER_DATA", data.data[fieldName], "");
      }
    },
    afterfieldsaved(fieldName, data) {
      //for cv uploading cv because the field is not binding with like others
      if (
        fieldName == "work" &&
        Object.keys(this.afterUpload["cv"]).length > 0
      ) {
        this.user.cv = data.data.cv;
        this.user.cvName = data.data.cvName;
        this.user_copy["cv"] = data.data.cv;
        this.user_copy["cvName"] = data.data.cvName;
      }

      //stop loading for fields
      this.updateField = false;

      //switch from editing section to showing data
      this.editingEl = "";

      //making change to copy object as well
      if (fieldName == "education") {
        this.user_copy["university"] = this.user["university"];
        this.user_copy["education_degree"] = this.user["education_degree"];
        this.user_copy["education_field"] = this.user["education_field"];
      } else if (fieldName == "personal") {
        this.user_copy["full_name"] = this.user["full_name"];
        this.user_copy["mail"] = this.user["mail"];
        this.user_copy["mobile"] = this.user["mobile"];

        //update navbar with new full_names but it's not working
        this.$store.commit("CHANGE_USER_DATA", "", this.user["full_name"]);
      } else if (fieldName == "work") {
        this.user_copy["job"] = this.user["job"];
        this.user_copy["skills"] = this.user["skills"];
      } else {
        this.user_copy[fieldName] = this.user[fieldName];
      }
    },

    // Create Data For Profile Section
    createData(fieldName) {
      var data = {};
      if (fieldName == "education") {
        data = {
          hash:
            "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
          university: this.user["university"],
          education_degree: this.user["education_degree"],
          education_field: this.user["education_field"]
        };
      } else if (fieldName == "personal") {
        data = {
          hash:
            "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
          full_name: this.user["full_name"],
          mail: this.user["mail"],
          mobile: this.user["mobile"]
        };
      } else if (fieldName == "work") {
        data = {
          hash:
            "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
          job: this.user["job"],
          skills: this.user["skills"]
        };
        //check if cv was uploaded
        if (Object.keys(this.afterUpload["cv"]).length > 0) {
          Object.assign(data, { cv: this.afterUpload["cv"] });
        }
      } else if (fieldName == "picture" || fieldName == "background_image") {
        data = {
          hash:
            "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
          [fieldName]: this.afterUpload[fieldName]
        };
      } else {
        data = {
          hash:
            "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
          [fieldName]: this.user[fieldName]
        };
      }
      return data;
    },

    // Get Profile Full Data
    async getProfile() {
      try {
        let { data } = await axios.get(
          "https://ed808.com:92/latin/user/" + this.$route.params.uid,
          {
            headers: {
              "Content-type": "application/json",
              Cookie: cookie
            }
          }
        );
        this.user = data;
      } catch (e) {
        console.log(e);
      }
    }
  },
  computed: {
    routUid: function() {
      return this.$route.params.uid;
    }
  }
};
</script>

<style lang="scss">
@import "@/assets/scss/vars.scss";
.img-raised {
  -webkit-box-shadow: 0 5px 15px -8px rgba(0, 0, 0, 0.24),
    0 8px 10px -5px rgba(0, 0, 0, 0.2);
  box-shadow: 0 5px 15px -8px rgba(0, 0, 0, 0.24),
    0 8px 10px -5px rgba(0, 0, 0, 0.2);
}
.userpanel-footer * {
  color: #3e3947 !important;
}

.loading {
  position: absolute;
  width: 100%;
  height: 5px;
  left: 0;
  bottom: 0;
  background: #fff;
}
.edit_cv {
  position: relative;
  width: 155px;
  border-bottom: 1px solid #d9d9d9;
  input {
    height: 40px;
    overflow: hidden;
    width: 100%;
    opacity: 0;
    z-index: 3;
    position: relative;
    cursor: pointer;
  }
  i {
    position: absolute;
    left: 0;
    top: 14px;
  }
}
.top {
  .dim {
    position: absolute;
    top: 0;
    bottom: 0;
    width: 100%;
    right: 0;
    left: 0;
    background-color: #868686c4;
  }
  position: relative;
  .spinner-loading {
    position: absolute;
    top: calc(50% - 35px);
    right: calc(50% - 30px);
  }
  .background-image {
    width: 100%;
    height: 380px;
    overflow: hidden;
    background-color: #c4b6c2;
    position: relative;
    background-size: cover;
    background-position: center;
    @media #{$x600} {
      height: 220px;
    }
    &:before {
      position: absolute;
      z-index: 1;
      width: 100%;
      height: 100%;
      display: block;
      left: 0;
      top: 0;
      content: "";
      background: rgba(0, 0, 0, 0.5);
      //background: rgba(0,0,0,.2);
    }
    &:hover {
      .edit-background-image {
        opacity: 1;
      }
    }
    .spinner-loading {
      top: calc(50% - 50px);
    }
    .edit-background-image {
      position: absolute;
      transition: 0.5s cubic-bezier(0.25, 0.8, 0.5, 1);
      width: 60px;
      right: 33px;
      top: 141px;
      background-color: rgba(210, 210, 210, 0.35);
      opacity: 0;
      height: 60px;
      border-radius: 50%;
      [type="file"] {
        height: 100%;
        overflow: hidden;
        width: 100%;
        opacity: 0;
        z-index: 3;
        position: relative;
        cursor: pointer;
      }
      [type="file"] + i {
        position: absolute;
        top: 0;
        right: 0;
        left: 0;
        bottom: 0;
        z-index: 0;
        width: 30px;
        height: 30px;
        max-width: 30px;
        font-size: 30px !important;
        color: #ffffff;
      }
    }
  }
  .user-image {
    position: absolute;
    top: 221px;
    left: calc(50% - 80px);
    z-index: 4;
    overflow: hidden;
    min-width: 160px;
    padding-bottom: 0px;
    border-radius: 50%;
    box-shadow: 0 5px 15px -8px rgba(0, 0, 0, 0.24),
      0 8px 10px -5px rgba(0, 0, 0, 0.2);
    @media #{$x720} {
      top: 268px;
      left: calc(50% - 50px);
      z-index: 4;
      overflow: hidden;
      min-width: 100px;
    }
    @media (max-width: 540px) {
      top: 75px;
      left: calc(50% - 50px);
      z-index: 4;
      overflow: hidden;
      min-width: 100px;
    }
    img {
      width: 160px;
      height: 160px;
      min-width: 160px;
      @media #{$x720} {
        width: 100px;
        height: 100px;
        min-width: 100px;
      }

      border-radius: 50%;
      //border: 3px solid white;
    }
    &:hover {
      .edit-picture {
        opacity: 1;
        bottom: 0;
        top: 50%;
      }
    }
    .edit-picture {
      position: absolute;
      transition: 0.5s cubic-bezier(0.25, 0.8, 0.5, 1);
      width: 100%;
      height: 50%;
      right: 0;
      left: 0;
      background-color: #ffffff82;
      opacity: 0;
      top: 100%;
      [type="file"] {
        height: 100%;
        overflow: hidden;
        width: 100%;
        opacity: 0;
        z-index: 3;
        position: relative;
        cursor: pointer;
      }
      [type="file"] + i {
        font-weight: bold;
        position: absolute;
        top: 0;
        right: 0;
        left: 0;
        bottom: 0;
        z-index: 0;
      }
    }
  }
  .profile-tabs {
    position: absolute;
    top: 10px;
    left: calc(50% - 250px);
    max-width: 500px;
    width: calc(100% - 20px);
    @media (max-width: 540px) {
      left: 10px !important;
      right: 10px;
    }
    .md-bottom-bar {
      max-width: 500px;
      width: 100%;
      margin: auto;
      box-shadow: none;
      > .md-ripple {
        a {
          @media (max-width: 540px) {
            display: block;
            width: 25%;
          }
        }
      }
      div.space {
        width: 160px;
        @media #{$x720} {
          width: 110px;
        }
        @media (max-width: 540px) {
          width: 0px;
        }
      }
      .md-ripple-wave {
        display: none;
      }
    }
    .md-bottom-bar .md-bottom-bar-item {
      min-width: 85px !important;
      max-width: 100% !important;
      flex: 1 1 32px;
      transition: 0.3s cubic-bezier(0.4, 0, 0.2, 1);
      transition-property: padding, min-width, max-width, flex, color;
      will-change: padding, min-width, max-width, flex, color;
      @media #{$x720} {
        min-width: 97.5px !important;
      }
      @media (max-width: 540px) {
        min-width: 100% !important;
      }
    }
    .md-bottom-bar .md-bottom-bar-item.md-active {
      min-width: 85px !important;
      max-width: 100%;
      flex: 1 1 32px;
      @media #{$x720} {
        min-width: 97.5px !important;
      }
      @media (max-width: 540px) {
        min-width: 100% !important;
      }
    }
    .md-bottom-bar.md-theme-default {
      background: transparent !important;
      color: #673ab7;
      .md-bottom-bar-item {
        color: #9a9a9a !important;
      }
      .md-icon {
        color: #9a9a9a !important;
      }
      .md-active {
        color: #9a9a9a;
        .md-bottom-bar-label {
          transform: translate3D(0, 0px, 0) !important;
        }
      }
    }
    .md-bottom-bar {
      .nuxt-link-exact-active.nuxt-link-active {
        border-radius: 6px;
        background: #ede7f6 !important;
        .md-bottom-bar-label {
          transform: translate3D(0, 1px, 0) !important;
          color: #673ab7;
        }
        .md-icon {
          font-size: 30px !important;
          color: #673ab7 !important;
        }
      }
    }
  }
}
footer {
  padding: 0.9375rem 0;
  text-align: center;
  display: -webkit-box;
  display: -ms-flexbox;
  display: flex;
  position: static;
  bottom: 0;
  left: 0;
  width: 100%;
  &.footer-transparent {
    .links-horizontal {
      @media screen and (max-width: 992px) {
        text-align: center;
      }
      li {
        margin-right: 0.9375rem;
        color: #fff;
      }
      &:first-child a {
        padding-left: 0;
      }
      &:last-child a {
        padding-right: 0;
      }
    }
    .copyright {
      color: #fff;
      font-size: 12px;
      text-align: right;
      @media screen and (max-width: 992px) {
        text-align: center;
      }
    }
  }
  ul {
    margin: 0;
    padding: 0;
    list-style: none;

    li {
      display: inline-block;

      a {
        color: inherit !important;
        padding: 0.9375rem;
        font-weight: 500;
        font-size: 12px;
        text-transform: uppercase;
        border-radius: 3px;
        position: relative;
        display: block;
        text-decoration: none;

        &:hover {
          text-decoration: none;
        }
      }

      .btn {
        margin: 0;
      }
    }

    &.links-horizontal {
      li a {
        padding: 0.9375rem;
      }
      &:first-child a {
        padding-left: 0;
      }
      &:last-child a {
        padding-right: 0;
      }
    }
    &.links-vertical {
      li {
        display: block;
        margin-left: -5px;
        margin-right: -5px;
        a {
          padding: 5px;
        }
      }
    }
  }
  a:not(.md-button):hover {
    text-decoration: none !important;
    color: #9c27b0 !important;
  }
  .social-buttons {
    .btn,
    a {
      margin-top: 5px;
      margin-bottom: 5px;
      padding: 0;
    }
  }
  .footer-brand {
    padding: 15px;
    font-size: 18px;
    margin-left: -15px;
    color: #3c4858 !important;
    font-weight: bold;
    &:focus,
    &:hover {
      color: #3c4858 !important;
    }
  }
  .copyright {
    padding: 15px 0;
    .md-icon {
      font-size: 18px !important;
      color: inherit !important;
    }
  }
  nav ul li a {
    margin-left: -15px;
  }
  .container,
  .container-fluid {
    padding-right: 15px;
    padding-left: 15px;
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
    -webkit-box-pack: justify;
    -ms-flex-pack: justify;
    justify-content: space-between;
    width: 100%;
    margin-left: auto;
    margin-right: auto;
    position: relative;
  }
  .md-layout {
    margin-right: -15px;
    margin-left: -15px;
  }
  .md-layout-item {
    padding-right: 15px;
    padding-left: 15px;
  }
  .md-field {
    padding-top: 13px;
    margin: -5px 0 10px;
    min-height: auto;
    .md-input {
      height: 36px;
      padding: 7px 0;
      font-size: 14px !important;
      -webkit-text-fill-color: #3c4858 !important;
      -webkit-appearance: none;
      -moz-appearance: none;
      appearance: none;
    }
  }
}

.footer-transparent {
  background-color: transparent;
}

.footer-black {
  background-color: #232323;
  background: radial-gradient(ellipse at center, #585858 0, #232323 100%);
  background-size: 550% 450%;
  hr {
    border-color: hsla(0, 0%, 100%, 0.2);
  }
  a {
    opacity: 0.86;
    &:focus,
    &:hover {
      opacity: 1;
    }
  }
  .copyright,
  .footer-brand,
  .footer-brand:focus,
  .footer-brand:hover,
  a,
  h4,
  h5,
  i {
    color: #fff !important;
  }
}

.footer-big {
  padding: 1.875rem 0;
  .gallery-feed img {
    width: 20%;
    margin-right: 5%;
    margin-bottom: 5%;
    float: left;
  }
  .content {
    text-align: left;
  }
  .social-feed i {
    font-size: 20px;
    padding-right: 10px;
  }
  .feed-line {
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
    p {
      padding: 0 20px 0 5px;
    }
  }
  h4 {
    font-weight: bold;
  }
  h5,
  .h5 {
    color: #3c4858 !important;
    text-decoration: none;
    font-size: 1.0625rem;
    line-height: 1.55em;
    margin-bottom: 15px;
    font-weight: bold;
    font-family: roboto slab;
  }
  p {
    color: #999;
  }
}

hr {
  margin-top: 1rem;
  margin-bottom: 1rem;
  border: 0;
  width: 100%;
  border-top: 1px solid rgba(0, 0, 0, 0.1);
}

.footer-big,
.page-header,
.section {
  .container,
  .container-fluid {
    -webkit-box-orient: vertical;
    -webkit-box-direction: normal;
    -ms-flex-direction: column;
    flex-direction: column;
  }
}
.social-buttons a {
  &.md-telegram .md-button-content {
    @include before(F501, "", 24px, #0188cc);
  }
  &.md-facebook .md-button-content {
    @include before(f20c, "", 24px, #4a66a0);
  }
  &.md-instagram .md-button-content {
    @include before(f2fe, "", 24px, #e91e63);
  }
  &.md-linkedin .md-button-content {
    @include before(f33b, "", 24px, #0976b4);
  }
  &.md-youtube .md-button-content {
    @include before(f5c3, "", 24px, #e52d27);
  }
}
.md-tabs.md-theme-default .md-tabs-navigation {
  display: flex;
  justify-content: center;
}
.container-raised {
  padding-top: 150px;
  @media #{$x600} {
    padding-top: 120px;
  }
}
.md-settings {
  display: block;
  width: 50px;
  position: absolute;
  bottom: 10px;
  font-size: 10px;
  z-index: 1;
  right: 60px;
  padding: 5px 10px 50px 10px;
  background: #eeeeee;
  transition: all .5s !important;
  border-radius: 10px 10px 0 0;
  @media #{$x600} {
    right: 30px;
  }
  &:hover {
    transform: translateY(-15px)
  }
  span {
    position: absolute;
    left: 0;
    right: 0;
    top: 23px;
    color: rgba(0, 0, 0, 0.54);
    display: block;
    text-align: center;
  }
}

</style>
