<template>
  <div>
    <div class="left-rail-container">
      <div class="profile-rail-card">
        <div class="user-info">
          <div class="profile-cover" style="background-image: url(https://ed808.com:92/sites/default/files/styles/1360x500_blur/public/field/image/user_174_68fb4c02.jpg?itok=huvPMkt7);">
          </div>
          <nuxt-link to="" target="_blank">
            <md-avatar class="md-large-tag">
              <img src="https://ed808.com:92/sites/default/files/styles/200x200/public/pictures/picture-174-1557748677.jpg?itok=_vFPtXF5" alt="user_image" />
            </md-avatar>
            <div class="colored-shadow"></div>
          </nuxt-link>
          <nuxt-link to="">
            <span class="username">Poorya Khajouie</span>
          </nuxt-link>
          <p class="headline-info">Front-end Developer</p>
        </div>
        <div class="profile-details">
          <div class="info">
            <nuxt-link to="">
              <div>
                <p>Followers</p>
              </div>
            </nuxt-link>
            <nuxt-link to="">
              <div>
                <p>Followings</p>
              </div>
            </nuxt-link>
            <nuxt-link to="">
              <div>
                <p>Stories shared</p>
              </div>
            </nuxt-link>
          </div>
        </div>
      </div>
    </div>

    <!--middle part-->
    <div class="newfeed-editor" style="float:left">
      <addPost viewMode="minimal" :tags="[]"></addPost>
    </div>

    <!--right side -->
    <div class="right-rail-container">
      <div class="extra-rail-card">
        <div class="tag-suggestion">
          <div class="suggestion-title">
            <h2>Add to your feed</h2>
          </div>
          <ul class="suggestion-list">
            <li class="suggestion-member">
              <nuxt-link to="" target="_blank">
                <md-avatar class="md-large-tag suggestion">
                  <img src="https://ed808.com:92/sites/default/files/styles/200x200/public/pictures/picture-174-1557748677.jpg?itok=_vFPtXF5" alt="user_image" />
                </md-avatar>
                <div class="colored-shadow"></div>
              </nuxt-link>
              <div class="suggestion-desc">
                <p>Template user, tag & etc</p>
              </div>
              <a
                class="btn btn-info btn-round purple"
              >
                Follow
              </a>
            </li>
            <li class="suggestion-member">
              <nuxt-link to="" target="_blank">
                <md-avatar class="md-large-tag suggestion">
                  <img src="https://ed808.com:92/sites/default/files/styles/200x200/public/pictures/picture-174-1557748677.jpg?itok=_vFPtXF5" alt="user_image" />
                </md-avatar>
                <div class="colored-shadow"></div>
              </nuxt-link>
              <div class="suggestion-desc">
                <p>Template user, tag & etc</p>
              </div>
              <a
                class="btn btn-info btn-round purple"
              >
                Follow
              </a>
            </li>
            <li class="suggestion-member">
              <nuxt-link to="" target="_blank">
                <md-avatar class="md-large-tag suggestion">
                  <img src="https://ed808.com:92/sites/default/files/styles/200x200/public/pictures/picture-174-1557748677.jpg?itok=_vFPtXF5" alt="user_image" />
                </md-avatar>
                <div class="colored-shadow"></div>
              </nuxt-link>
              <div class="suggestion-desc">
                <p>Template user, tag & etc</p>
              </div>
              <a
                class="btn btn-info btn-round"
              >
                Follow
              </a>
            </li>
          </ul>
        </div>
        <div class="user-suggestion">

        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import addPost from "@/components/user/addPost"
  export default {
    name:'NewsFeed',
    components:{
      addPost
    },
    data() {
      return {

      }
    },
    head(){
      return {
        title: 'News Feed'
      }
    }
  }
</script>

<style lang="scss">
  .left-rail-container {
    /*width: calc((576px * .20833) + 96px);*/
    margin: 10px 8px 8px 15px !important;
    float: left;
    border-radius: 2px;
    /*box-shadow: 0 0 0 1px rgba(0,0,0,.15), 0 2px 3px rgba(0,0,0,.2);*/
  }
  .profile-rail-card {
    border-radius: 6px;
    overflow: hidden;
    padding: 0;
    text-align: center;
    width: 240px;
    box-shadow: 0 0 0 1px rgba(0,0,0,.15);
    background-color: #fff;
    background-clip: padding-box;
    transition: box-shadow 83ms;
    font-family: "Roboto Slab";
  }
  .user-info {
    border-bottom: 1px solid rgba(0,0,0,.15);
    padding: 12px 12px 16px;
  }
  .profile-cover {
    background-position: center;
    background-size: cover;
    display: block;
    height: 54px;
    margin: -12px -12px 0;
  }
  .md-large-tag {
    border: 2px solid #fff;
    box-shadow: inset 0 1.5px 3px 0 rgba(0,0,0,.15), 0 1.5px 3px 0 rgba(0,0,0,.15);
    margin: -38px auto 12px;
    background-color: #fff;
    width: 64px;
    height: 64px;
  }
  .username {
    font-size: 14px;
    line-height: 1.5;
    font-weight: 600;
    color: rgba(0,0,0,.9);
    margin: 0;
    padding: 0;
  }
  .headline-info {
    margin-top: 4px!important;
    font-size: 12px;
    line-height: 1.33333;
    font-weight: 400;
    color: rgba(0,0,0,.6);
  }
  .info {
    font-size: 12px;
    line-height: 1.33333;
    text-align: left !important;
    font-weight: 600;
    color: rgba(0,0,0,.6);
    padding-bottom: 10px;
    &:visited {
      color: rgba(0,0,0,.6) !important;
    }
    p {
      margin: 8px 0 0 10px !important;
    }
  }
  .right-rail-container {
    float: left;
    margin-right: -1px;
    width: 30%;
    box-sizing: border-box;
    margin-left: calc((576px * 0) - 24px + 49px);
    box-shadow: 0 0 0 1px rgba(0,0,0,.15);
    background-color: #fff;
    border-radius: 6px;
    margin-top: 10px;
    margin-bottom: 15px;
  }
  .tag-suggestion {
    margin: 16px 0 !important;
  }
  .suggestion-list {
    padding: 10px;
    margin: 0;
  }
  .suggestion-member {
    display: flex;
    align-items: center;
    margin-top: 4px;
    position: relative;
  }
  .md-large-tag.suggestion {
    width: 48px;
    height: 48px;
    margin: 0;
  }
  .suggestion-title {
    h2 {
      font-size: 16px;
      font-weight: 400;
      text-align: left;
      padding-left: 15px;
      margin: 0;
      font-family: "Roboto Slab";
    }
  }
  .suggestion-desc {
    p {
      font-size: 14px;
      font-weight: 500;
      text-align: left;
      margin: 0;
      padding: 10px;
      font-family: "Roboto Slab";
    }
  }
  .btn {
    display: inline-block;
    text-align: center;
    white-space: nowrap;
    vertical-align: middle;
    user-select: none;
    border: 1px solid transparent;
    padding: .46875rem 1rem;
    font-size: 1rem;
    line-height: 1.5;
    border-radius: .25rem;
    transition: color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out;
  }
  .btn {
    position: relative;
    padding: 8px 12px;
    margin: .3125rem 1px;
    font-size: .75rem;
    font-weight: 400;
    line-height: 1.428571;
    text-decoration: none;
    text-transform: uppercase;
    letter-spacing: 0;
    cursor: pointer;
    background-color: transparent;
    border-radius: .2rem;
    outline: 0;
    transition: box-shadow .2s cubic-bezier(.4,0,1,1),background-color .2s cubic-bezier(.4,0,.2,1);
    will-change: box-shadow,transform;
  }
  .btn.btn-round {
    border-radius: 6px;
    .follow-counts {
      display: inline-block;
      margin-left: 10px;
    }
  }
  .btn:not(:disabled):not(.disabled) {
    cursor: pointer;
  }
  .btn-info {
    color: #2196F3 !important;
    background-color: #fff !important;
    border-color: #2196F3 !important;
    box-shadow: none;
    transition-duration: 0.5s;
  }
  .btn-info.active {
    color: #fff !important;
    background-color: #2196F3 !important;
    border-color: #2196F3 !important;
    box-shadow: none;
  }
  .btn.btn-info:active, .btn.btn-info:focus, .btn.btn-info:hover {
    box-shadow: 0 14px 26px -12px rgba(33,150,243,.42), 0 4px 23px 0 rgba(0,0,0,.12), 0 8px 10px -5px rgba(33,150,243,.2);
  }

  .btn.btn-info.focus, .btn.btn-info:focus, .btn.btn-info:hover {
    color: #fff !important;
    background-color: #2196F3 !important;
    border-color: #2196F3 !important;
  }
</style>
