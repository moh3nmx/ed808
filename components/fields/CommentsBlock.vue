<template>
  <div class="relative">

    <!--section title-->
    <h2 class="section-title">
      Comments:
    </h2>
    <!--end of section title-->

    <!--form and comments-->
    <div class="comment-list">
      <transition name="fade">
        <div class="loading-overlay" v-if="onSubmit"></div>
      </transition>

      <!--Comment form-->
      <div class="comment-card comment-form">
        <form>
          <md-card-header>
            <md-avatar>
              <img
                v-if="$store.state.user.picture"
                :src="$store.state.user.picture"
                alt="Avatar"
              >
              <img
                v-else
                src="/images/avatar.png"
                alt="Avatar"
              >
            </md-avatar>
            <div class="md-commentor" v-if="$store.state.user.full_name">
              {{$store.state.user.full_name}}
            </div>
            <div class="md-commentor" v-else>
              Guest User
            </div>
            <div class="md-right">
              <md-button
                :disabled="commentText == '' || onSubmit"
                class="md-dense md-primary"
                @click="submitComment(0)"
              >
                Submit
              </md-button>
            </div>
          </md-card-header>
          <!--<md-card-header v-else>-->
            <!--<div>You need to-->
              <!--<a @click="$store.commit('TOGGLE_LOGIN')">Login</a>-->
              <!--or-->
              <!--<nuxt-link to="/register">Register</nuxt-link>-->
              <!--for submiting your comment.</div>-->
          <!--</md-card-header>-->
          <md-card-content>
            <md-field>
              <label>Write your comment</label>
              <md-textarea
                v-model="commentText"
                :class="{'active': textareaActive}"
                :disabled="onSubmit"
                @focus="growField()"
                md-autogrow
              />
              <md-snackbar
                class="error"
                :md-active.sync="commentErr"
                :md-duration="snackbarDuration"
              >
                {{ commentErr }}
              </md-snackbar>
            </md-field>
          </md-card-content>
        </form>

      </div>
      <!--end of Comment form-->

      <!--loading box-->
      <div class="comment-card" v-if="commentLoading">
        <md-card-header>
          Loading Comments ...
        </md-card-header>
      </div>
      <!--end of loading box-->

      <!--Parent Comment-->
      <div v-for="(c,index) in commentList" :key="c.cid">
          <div>
            <div class="comment-card">
              <div class="md-layout">
                <div class="md-layout-item md-size-20">
                  <div class="user-details">
                    <md-avatar>
                      <img
                        v-if="c.author_pic"
                        :src="c.author_pic"
                        alt="Avatar"
                      >
                      <img
                        v-else
                        src="/images/avatar.png"
                        alt="Avatar"
                      >
                    </md-avatar>
                    <div class="md-title">
                      <nuxt-link :to="'/user/'+c.uid">
                        {{ c.author_name ? c.author_name : c.author_username }}
                      </nuxt-link>
                    </div>
                    <div class="md-date">
                      {{ c.created }}
                    </div>
                  </div>
                </div>
                <div class="md-layout-item">

                  <!-- comment body -->
                  <div class="md-comment-body">
                    <p v-if="onEditing != c.cid" v-html="c.body">

                    </p>
                    <md-field
                      v-if="onEditing == c.cid"
                      class="reply-box"
                      style="margin-bottom: 5px"
                    >
                      <label>Edit your comment</label>

                      <md-textarea
                        ref="reply"
                        v-model="editingCommentBody"
                        :disabled="onSubmit"
                        md-autogrow
                      />
                    </md-field>

                    <!-- reply box -->
                    <transition name="fade">
                      <md-field
                        v-if="onReply == c.cid"
                        class="reply-box"
                      >
                        <label>Write your reply</label>

                        <md-textarea
                          ref="reply"
                          v-model="replyText"
                          :disabled="onSubmit"
                          md-autogrow
                        />
                      </md-field>
                    </transition>
                    <!-- end of reply box -->

                    <!-- comment actions -->
                    <div class="comment-action">

                      <!--Submit Edit-->
                      <md-button
                        v-if="onEditing == c.cid && $store.getters.getUid"
                        class="md-icon-button md-primary md-dense"
                        @click="editComment(c.cid)"
                      >
                        <md-icon style="font-size: 20px!important;">
                          done
                        </md-icon>
                        <md-tooltip md-direction="bottom">
                          Submit your changes
                        </md-tooltip>
                      </md-button>

                      <!--Cancel Edit-->
                      <md-button
                        v-if="onEditing == c.cid && $store.getters.getUid"
                        class="md-icon-button md-primary md-dense"
                        @click="onEditing = false"
                      >
                        <md-icon style="font-size: 20px!important;">
                          close
                        </md-icon>
                        <md-tooltip md-direction="bottom">
                          Discard your comment
                        </md-tooltip>
                      </md-button>

                      <!--Send Reply-->
                      <md-button
                        v-if="onReply == c.cid && replyText != '' && $store.getters.getUid"
                        class="md-icon-button md-primary md-dense"
                        @click="submitComment(c.cid)"
                      >
                        <md-icon style="font-size: 20px!important;">
                          send
                        </md-icon>
                        <md-tooltip md-direction="bottom">
                          Send your reply
                        </md-tooltip>
                      </md-button>

                      <!--Cancel Reply-->
                      <md-button
                        v-if="onReply == c.cid && $store.getters.getUid "
                        class="md-icon-button md-primary md-dense"
                        @click="onReply = false"
                      >
                        <md-icon style="font-size: 20px!important;">
                          close
                        </md-icon>
                        <md-tooltip md-direction="bottom">
                          Discard your message
                        </md-tooltip>
                      </md-button>


                      <!--like Btn-->
                      <md-badge class="md-primary"  :md-content="c.likes" md-dense >
                        <md-button v-if="!c.user_like"
                          :disabled="c.uid == $store.getters.getUid "
                          class="md-icon-button md-danger md-dense md-like"
                          :class="{ 'active' : c.user_like }"
                          @click="likeComment(c.cid, false, index, false)"
                        >
                          <md-icon>favorite</md-icon>
                          <md-tooltip md-direction="bottom" v-if="$store.getters.getUid">
                           Like this comment
                          </md-tooltip>
                        </md-button>
                        <md-button
                          v-else
                          :disabled="c.uid == $store.getters.getUid "
                          class="md-icon-button md-danger md-dense md-like"
                          :class="{ 'active' : c.user_like }"
                          @click="likeComment(c.cid, true, index, false)"
                        >
                          <md-icon>favorite</md-icon>
                        </md-button>
                        <md-tooltip md-direction="bottom" v-if="!$store.getters.getUid">
                          Please login to like this comment
                        </md-tooltip>
                        <md-tooltip md-direction="bottom" v-if="c.uid == $store.getters.getUid">
                          This is your comment
                        </md-tooltip>
                        <md-tooltip md-direction="bottom" v-if="c.user_like">
                          Click to dislike this comment
                        </md-tooltip>
                      </md-badge>

                      <!--Reply Btn-->
                      <md-button
                        class="md-icon-button md-primary md-dense"
                        @click="showReplyBox(c.cid); onEditing = false"
                      >
                        <md-icon>reply</md-icon>
                        <md-tooltip v-if="$store.getters.getUid" md-direction="bottom">
                          Reply to this Comment
                        </md-tooltip>
                        <md-tooltip v-if="!$store.getters.getUid" md-direction="bottom">
                          Please login to reply this comment
                        </md-tooltip>
                      </md-button>


                      <!--More Functions Menu-->
                      <md-menu
                        md-size="small"
                        :md-offset-x="127"
                        :md-offset-y="-36"
                        v-if="$store.getters.getUid == c.uid"
                      >
                        <md-button
                          class="md-icon-button md-primary md-dense"
                          md-menu-trigger
                        >
                          <md-icon>more_vert</md-icon>
                          <md-tooltip md-direction="bottom"  v-if="$store.getters.getUid == c.uid">
                            More Functions
                          </md-tooltip>
                        </md-button>

                        <md-menu-content v-if="$store.getters.getUid == c.uid">
                          <md-menu-item @click="onEditing = c.cid; editingCommentBody = c.body; onReply = false">
                            <md-icon>edit</md-icon>Edit
                          </md-menu-item>
                          <md-menu-item @click="deleteComment(c.cid)">
                            <md-icon>clear</md-icon>Delete
                          </md-menu-item>
                        </md-menu-content>
                      </md-menu>
                    </div>
                    <!-- end of comment actions -->

                  </div>
                  <!-- end of comment body -->
                </div>
              </div>
            </div>
          </div>

          <!--Replies of parent-->
          <div
            v-for="(r,rIndex) in c.replies"
            class="reply-comment"
            :key="r.cid"
          >
            <div class="comment-card">
              <div class="md-layout">
                <div class="md-layout-item md-size-20">
                  <div class="user-details">
                    <!--<md-avatar>-->
                      <!--<img-->
                        <!--:src="r.author_pic"-->
                        <!--alt="Avatar"-->
                      <!--&gt;-->
                    <!--</md-avatar>-->
                    <div class="md-title">
                      <nuxt-link :to="'/user/'+r.uid">
                        {{ r.author_name ? r.author_name : r.author_username }}
                      </nuxt-link>
                    </div>
                    <div class="md-date">
                      {{ r.created }}
                    </div>
                  </div>
                </div>
                <div class="md-layout-item">
                  <!-- comment body reply-->
                  <div class="md-comment-body">
                    <p v-html="r.body" v-if="onEditing != r.cid"/>
                    <md-field
                      v-if="onEditing == r.cid"
                      class="reply-box"
                      style="margin-bottom: 5px"
                    >
                      <label>Edit your comment</label>

                      <md-textarea
                        ref="reply"
                        v-model="editingCommentBody"
                        :disabled="onSubmit"
                      />
                    </md-field>

                    <!-- comment actions reply -->
                    <div class="comment-action">

                      <!--Submit Edit for reply-->
                      <md-button
                        v-if="onEditing == r.cid"
                        class="md-icon-button md-primary md-dense"
                        @click="editComment(r.cid)"
                      >
                        <md-icon style="font-size: 20px!important;">
                          done
                        </md-icon>
                        <md-tooltip md-direction="bottom">
                          Submit your changes
                        </md-tooltip>
                      </md-button>

                      <!--Cancel Edit for reply-->
                      <md-button
                        v-if="onEditing == r.cid"
                        class="md-icon-button md-primary md-dense"
                        @click="onEditing = false"
                      >
                        <md-icon style="font-size: 20px!important;">
                          close
                        </md-icon>
                        <md-tooltip md-direction="bottom">
                          Discard changes
                        </md-tooltip>
                      </md-button>


                      <!--like Btn-->
                      <md-badge class="md-primary"
                        :md-content="r.likes"
                        md-dense
                      >
                        <md-button v-if="!r.user_like"
                          :disabled=" r.uid == $store.getters.getUid "
                          class="md-icon-button md-danger md-dense md-like"
                          :class="{ 'active' : r.user_like }"
                          @click="likeComment(r.cid, false, index, rIndex)"
                        >
                          <md-icon>favorite</md-icon>
                          <md-tooltip md-direction="bottom">
                            Like this comment
                          </md-tooltip>
                        </md-button>
                        <md-button v-else
                          :disabled=" r.uid == $store.getters.getUid "
                          class="md-icon-button md-danger md-dense md-like"
                          :class="{ 'active' : r.user_like }"
                          @click="likeComment(r.cid, true, index, rIndex)"
                        >
                          <md-icon>favorite</md-icon>
                        </md-button>
                        <md-tooltip md-direction="bottom" v-if="!$store.getters.getUid">
                          Please login to like this comment
                        </md-tooltip>
                        <md-tooltip md-direction="bottom" v-if="$store.getters.getUid && r.uid == $store.getters.getUid">
                          This is your comment
                        </md-tooltip>
                      </md-badge>

                      <!--More Functions Menu for reply-->
                      <md-menu
                        md-size="small"
                        :md-offset-x="127"
                        :md-offset-y="-36"
                        v-if="$store.getters.getUid == r.uid"
                      >

                        <md-button
                          class="md-icon-button md-primary md-dense"
                          md-menu-trigger
                        >
                          <md-icon>more_vert</md-icon>
                          <md-tooltip md-direction="bottom">
                            More Functions
                          </md-tooltip>
                        </md-button>
                        <md-menu-content v-if="$store.getters.getUid == r.uid">
                          <md-menu-item @click="onEditing = r.cid; editingCommentBody = r.body">
                            <md-icon>edit</md-icon>Edit
                          </md-menu-item>
                          <md-menu-item @click="deleteComment(r.cid)">
                            <md-icon>clear</md-icon>Delete
                          </md-menu-item>
                        </md-menu-content>
                      </md-menu>
                    </div>
                    <!-- end of comment actions reply -->
                  </div>
                  <!-- end of comment body reply -->
                </div>
              </div>
            </div>
          </div>
        </div>

    </div>
    <!--end of form and comments-->

  </div>

</template>

<script>
import axios from "@/node_modules/axios"
import { cookie } from '@/components/mixins/cookie.js'

export default {
  name: "Comment",
  mixins: [cookie],
  props: {
    nid: String
  },
  data() {
    return {
      commentText:'',
      replyText:'',
      commentList: [],
      onReply: {},
      textareaActive: false,
      onSubmit: false,
      commentErr: false,
      snackbarDuration: 5000,
      commentLoading: true,
      onEditing: false,
      editingCommentBody: ''
    };
  },
  mounted() {
    this.getComments();
  },
  methods: {
    growField(){
      if(!this.$store.getters.getUid){
        this.$store.commit('TOGGLE_LOGIN')
      }else {
        this.textareaActive = true
      }
    },
    // cid = comment id
    // dislike = if wants to dislike = true  else = false
    // index = index of comment
    // rindex = index of reply
    likeComment(cid, dislike, index, rindex){
      if(!this.$store.getters.getUid){
        this.$store.commit('TOGGLE_LOGIN')
      }else {
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        axios
          .post("https://ed808.com:92/latin/comments/" + cid + "/like", {

            'action': dislike ? 'dislike' : ''

          }, {
            headers: {
              'Content-Type': 'application/json',
              'X-CSRF-Token': this.getCookie('token')
            }
          })
          .then(response => {
            this.commentErr = false;
            /* this is for likes counter and btn color */
            if (rindex !== false) {
              this.commentList[index].replies[rindex].user_like = dislike ? false : true;
              dislike ? this.commentList[index].replies[rindex].likes--
                : this.commentList[index].replies[rindex].likes++;
            } else {
              this.commentList[index].user_like = dislike ? false : true;
              dislike ? this.commentList[index].likes--
                : this.commentList[index].likes++;
            }
          })
          .catch(err => {
            console.log(err);
            if (err.response.statusText) {
              this.commentErr = err.response.statusText;
            } else {
              this.commentErr = 'Error While Sending Request';
            }
          });
      }
    },
    submitComment(pid){
      this.onSubmit = true;
      console.log(this.commentText);
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      axios
        .post("https://ed808.com:92/latin/comments" ,{
          "nid" : this.nid,
          "pid" : pid,
          "body" : pid != 0 ? this.replyText.replace(/\n/g, "<br />") : this.commentText.replace(/\n/g, "<br />")
        }, {
          headers: {
            'Content-Type': 'application/json',
            'X-CSRF-Token' : this.getCookie('token')
          }
        })
        .then(response => {
          this.replyText = '';
          this.commentText = '';
          this.textareaActive = false;
          this.commentErr = false;
          this.getComments();
          this.onReply = null;
          this.onSubmit = false;
        })
        .catch(err => {
          console.log(err.response);
          if(err.response.statusText){
            this.commentErr = err.response.statusText;
          }else {
            this.commentErr = 'Error While Sending Request';
          }
          this.onSubmit = false;
        });
    },
    editComment(cid){
      this.onSubmit = true;
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      axios
        .put("https://ed808.com:92/latin/comments/"+cid ,{
          "body": this.editingCommentBody
        }, {
          headers: {
            'Content-Type': 'application/json',
            'X-CSRF-Token' : this.getCookie('token')
          }
        })
        .then(response => {
          this.commentErr = false;
          this.onEditing = false;
          this.onSubmit = false;
          this.editingCommentBody = false
          this.getComments();
        })
        .catch(err => {
          console.log(err);
          if(err.response.statusText){
            this.commentErr = err.response.statusText;
          }else {
            this.commentErr = 'Error While Sending Request';
          }
          this.onEditing = false;
          this.onSubmit = false;
        });
    },
    deleteComment(cid){
      this.onSubmit = true;
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      axios
        .delete("https://ed808.com:92/latin/comments/"+cid , {
          headers: {
            'Content-Type': 'application/json',
            'X-CSRF-Token' : this.getCookie('token')
          }
        })
        .then(response => {
          this.commentErr = false;
          this.onSubmit = false;
          this.getComments();
        })
        .catch(err => {
          console.log(err);
          if(err.response.statusText){
            this.commentErr = err.response.statusText;
          }else {
            this.commentErr = 'Error While Sending Request';
          }
          this.onSubmit = false;
        });
    },
    showReplyBox(commentID) {
      if(!this.$store.getters.getUid){
        this.$store.commit('TOGGLE_LOGIN')
      }else{
        this.commentText = '';
        this.replyText = '';
        this.onReply = commentID;
      }
      // this.$nextTick(() => {
      //   this.$refs.reply.focus()
      // })
    },
    getComments() {
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials = true;
      axios
        .get("https://ed808.com:92/latin/contents/"+ this.nid +"/comments")
        .then(response => {
          this.commentErr = false;
          this.commentLoading = false;
          this.commentList = response.data;
        })
        .catch(err => {
          console.log(err);
          if(err.response.statusText){
            this.commentErr = err.response.statusText;
          }else {
            this.commentErr = 'Error While Sending Request';
          }

        });
    }
  }
};
</script>

<style lang='scss' scoped>
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

.comment-list {
  @include main-center-content();
  position: relative;
  a:hover {
    cursor: pointer;
  }
  .comment-card {
    padding: 10px 0;
    padding-bottom: 5px;
    margin-top: 20px;
    border-radius: 3px;
    border: 1px solid #d9d9d9;
    box-shadow: 0 0 4px 0 rgba(0, 0, 0, 0.09);
    .md-layout {
      width: 100%;
    }
    @media screen and (max-width: 720px) {
      .md-layout-item.md-size-20 {
        min-width: 100%;
        max-width: 100%;
        flex: 0 1 100%;
      }
      .user-details{
        position: relative;
        .md-avatar {
          display: inline-block;
          position: absolute;
          left: 0;
          top: 8px;
          min-height: 32px;
          width: 40px;
          height: 40px;
          margin: auto;
        }
        .md-title {
          text-align: left;
          margin-left: 50px;
        }
        .md-date {
          text-align: left;
          margin-left: 50px;
        }
      }
    }
  }
  .comment-form {
    padding: 0;
    .md-card-header {
      position: relative;
      margin-bottom: 0;
      border-bottom: 1px solid #d9d9d9;
      .md-commentor {
        line-height: 40px;
        display: inline-flex;
      }
      .md-right {
        position: absolute;
        right: 16px;
        top: 16px;
        padding: 4px;
        display: inline-block;
      }
    }
    .md-card-content {
      padding: 16px;
      .md-field.md-theme-default.md-has-textarea:not(.md-autogrow):after {
        border-color: #d9d9d9;
      }
      .md-field {
        margin-bottom: 5px;
        .md-textarea {
          min-height: 50px;
          transition: 1s;
          -webkit-transition: 1s;
        }
        .md-textarea.active {
          height: auto;
          min-height: 100px;
          transition: 1s;
          -webkit-transition: 1s;
        }
      }
    }
  }
  .user-details {
    text-align: center;
    align-content: center;
    padding: 5px 5px;
    .md-title {
      font-size: 12px;
    }

    .md-avatar {
      display: block;
      width: 48px;
      height: 48px;

      margin: 5px auto;
    }
    .md-date {
      font-size: 10px;
    }
  }
  .md-comment-body {
    padding: 10px 10px;
    p {
    }
    .comment-action {
      text-align: right;
      .md-like {
        .md-icon {
          font-size: 18px!important;
        }
      }
      .md-badge.md-theme-default {
        color: #f44336;
        background-color: #f5f5f5 !important;
      }
    }
  }
  .reply-comment {
    margin-left: 30px;
    .comment-card {
      background: #f9f9f9;
      margin-top: 2px;
      p{
        margin: 0;
        margin-bottom: 10px;
      }
    }
  }
  .md-red {
    color: #ff5252;
  }
}
.md-list-item-content > .md-icon:first-child {
  margin-right: 0px;
}
.md-list-item-content > .md-icon:last-child {
  margin-left: 0px;
}
.md-button.md-theme-default.md-primary {
  .md-icon-font {
    color: rgba(0, 0, 0, 0.54);
    color: rgba(0, 0, 0, 0.54);
    color: var(--md-theme-default-icon-on-background, rgba(0, 0, 0, 0.54));
  }
}

.md-button.md-theme-default.md-danger {
  &:hover, &.active {
    .md-icon-font {
      color: #f44336;
      color: var(--md-theme-default-danger-on-background, #f44336);
    }
  }
  &.active {
    span {
      color: #f44336;
      color: var(--md-theme-default-danger-on-background, #f44336);
    }
  }
}

.md-button.md-theme-default.md-primary:hover {
  .md-icon-font {
    color: #92278f;
    color: #92278f;
    color: var(--md-theme-default-primary-on-background, #92278f);
  }
}

.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.5s;
}
.fade-enter, .fade-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
.md-field.reply-box {
  margin-bottom: 10px;
}
.md-field.md-theme-default.md-has-textarea:not(.md-autogrow):after {
  border-color: #d9d9d9;
}
.md-field.reply-box.md-has-textarea:not(.md-autogrow) .md-textarea {
  min-height: 50px;
}
.md-field.md-has-textarea:after, .md-field.md-has-textarea:before {
  height: auto;
  pointer-events: none;
  top: 0;
  bottom: 0;
  -webkit-transform: none;
  transform: none;
  background: none!important;
  border: 1px solid transparent;
  border-radius: 3px;
}
.relative {
  position: relative;
}
.loading-overlay {
  position: absolute;
  background-color: rgba(169,169,169,0.3);
  left: 15px;
  top: 0;
  bottom: 0;
  right: 15px;
  z-index: 10;
}

</style>
