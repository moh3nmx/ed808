<template>
  <div class="editor-container">
    <div class="loading-spinner" v-if="loading">
      <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
    </div>
    <md-content class="md-elevation-3 card">
      <div class="card-head">
        <md-icon>edit</md-icon>
        <span class="item-text">Write your new story</span>
      </div>
      <div class="card-text">
        <div
          id="editorClick"
          class="default-text card-row"
          v-if="!editorActive && (viewMode == 'minimal')"
          @click="editorActive = true"
        >
          Write your new story and share it with the World. Just click here ...
        </div>
        <div v-if="mainPictureUploaded">
          <h3>Image Preview:</h3>
          <img :src="'https://ed808.com/' + mainPictureUploaded" alt="" width="80%">
        </div>

        <div id="editor" v-if="editorActive">
          <div class="md-layout md-gutter">
            <div class="md-layout-item md-size-70">
              <md-field class="title with-label">
                <md-icon>subject</md-icon>
                <label>Title</label>
                <md-input v-model="title" />
              </md-field>
            </div>
            <div class="md-layout-item md-size-30">
              <md-field>
                <label>Main Picture</label>
                <md-file ref="mainPicture" accept="image/*" @change="uploadStoryImage()" />
              </md-field>
            </div>
          </div>
          <quill-editor ref="quillEditor" v-model="body" @textChange="onTextChange" :options="customToolbar" />


          <div class="tag-selector">
            <vue-multiselect
              v-if="tags.length > 0"
              v-model="selectedTopics"
              :options="topics"
              :multiple="true"
              :close-on-select="false"
              :clear-on-select="false"
              :preserve-search="true"
              placeholder="Pick Some Topics"
              label="name"
              track-by="tid"
            />
          </div>

          <div class="tag-selector">
            <vue-multiselect
              v-if="tags.length > 0"
              v-model="selectedTags"
              :options="tags"
              :multiple="true"
              :close-on-select="false"
              :clear-on-select="false"
              :preserve-search="true"
              placeholder="Pick some tags"
              label="name"
              track-by="tid"
            />
          </div>
          <div class="buttons md-layout md-alignment-center-space-between">
            <div class="md-layout-item" style="flex:0;">
              <md-button class="md-raised green" @click="savePost()">
                Save
              </md-button>
            </div>
            <div class="md-layout-item" style="flex:0;">
              <md-button @click="cancelEditor()">
                Cancel
              </md-button>
            </div>
          </div>
        </div>

      </div>
    </md-content>
    <md-snackbar :md-active.sync="postSaved">
      Your story has been published successfully.
    </md-snackbar>
    <md-snackbar class="error" :md-active.sync="hasError">
      {{ errorText }}
    </md-snackbar>
  </div>

</template>

<script>
  import axios from "@/node_modules/axios"
  import { cookie } from '@/components/mixins/cookie.js'
  import VueMultiselect from "@/node_modules/vue-multiselect/src/Multiselect";

  export default {
    name: "AddPost",
    components: {
      VueMultiselect
    },
    mixins: [cookie],
    //Options are either 'wild' or 'minimal'
    props:['viewMode','uploadedFiles','tags'],
    data() {
      return {
        postSaved:false,
        quill: {},
        title:'',
        body:'',
        image:'',
        openEditor:false,
        loading: false,
        editorActive: false,
        content: '<h1>Some initial content</h1>',
        time:'',
        uploaded: this.uploadedFiles,
        hasError: false,
        errorText: null,
        mainPicture: '',
        selectedTags: [],
        selectedTopics: [],
        selectedType: ['3939'],
        topics: [],
        fid: null,
        bodyImageFid: [],
        mainPictureUploaded: false,
        customToolbar: {
          modules: {
            toolbar: [
              [{ 'header': '1'},{ 'header': '2'}],
              ['bold', 'italic', 'underline'],
              [{ 'list': 'ordered'}, { 'list': 'bullet' }],
              ['image'],
              ['blockquote'],
              ['clean'],
              [{ 'align': [] }],
            ],
            imageUpload: {
              // add custom headers, example { token: 'your-token'}
              customUploader: (file) => {
                this.loading = true
                const form = new FormData()
                form.append('image', file)
                axios.defaults.withCredentials = true
                axios({
                  url: 'https://ed808.com:92/latin/file/upload_image', // server url. If the url is empty then the base64 returns
                  method: 'POST',
                  data : form,
                  headers: {
                    "Content-Type": "multipart/form-data",
                    "X-CSRF-Token": this.getCookie("token")
                  }
                }).then((data)=> {
                  this.uploaded.push(data.data.filename)
                  this.$emit('update:uploadedFiles', this.uploaded)
                  this.bodyImageFid.push(data.data.fid)
                  this.loading = false
                  // console.log(data,this.uploadedFiles)
                  const selection = this.$refs.quillEditor.quill.getSelection();
                  const index = selection
                    ? selection.index
                    : this.$refs.quillEditor.quill.getLength();
                  this.$refs.quillEditor.quill.insertEmbed(index, 'image', 'https://ed808.com/' + data.data.fileUrl, 'user');
                }).catch(e => {
                  this.loading = false
                  if(e.hasOwnProperty('response')){
                    if(e.response.hasOwnProperty('data'))
                      this.errorText = e.response.data[0]
                    else {
                      this.errorText = e.response
                    }
                  }
                  else{
                    this.errorText = e
                  }
                  this.hasError = true
                })
              }, // add custom uploader

              // personalize successful callback and call next function to insert new url to the editor
              callbackOK: (serverResponse, next) => {

                console.log(serverResponse)
                next(serverResponse)
              },

              // personalize failed callback
              callbackKO: serverError => {
                alert(serverError);
              },

              // optional
              // add callback when a image have been chosen
              checkBeforeSend: (file, next) => {
                if(file.size <= 5*1024*1024) {
                  next(file); // go back to component and send to the server
                }else {
                  this.hasError = true
                  this.errorText = 'Image size should not be larger than 5 MB'
                }
              }
            }
          },
        }
      }
    },
    mounted(){
      this.getTopics()

    },
    methods:{
      getImgUrls(delta) {
        return delta.ops.filter(i => i.insert && i.insert.image).map(i => i.insert.image);
      },
      // TextChange Event for Editor
      // It Shows Src of Deleted Image From Editor
      onTextChange(arg){
        // if User Deletes Images From Editor
        if (arg.source === "user" && !this.postSaved) {
          let currentContents = arg.content;
          let diff = currentContents.diff(arg.oldDelta);

          let image = this.getImgUrls(diff)
          console.log('textchange')
          try {
            this.$emit('deleteUploaded',[
              image[0].split('/').pop()
            ])
          } catch (_error) {}
        }
      },

      // Create Story or Post In DB
      savePost(){
        this.postSaved = false
        // mini validator
        if (this.title != '' && this.body != '') {
          //start loading for fields
          this.loading = true
          let tags = []
          let topics = []

          if(this.selectedTags.length > 0) {
            this.selectedTags.forEach(function (tag) {
              tags.push(tag.tid.toString())
            })
          }
          if(this.selectedTopics.length > 0) {
            this.selectedTags.forEach(function (tag) {
              topics.push(tag.tid.toString())
            })
          }
          let arr = [this.fid, ...this.bodyImageFid]
          let images = []

          arr.forEach(function (el) {
            images.push({
              'fid': el,
              'display': '1'
            })
          })

          //create data with field that was changed
          let data = {
            "title": this.title,
            "body": this.body,
            "meta_description": this.body.replace(/<(?:.|\n)*?>/gm, ' ').slice(0,150),
            "tags" : tags,
            "field_image" : arr,
            "types": this.selectedType,
            "topics": topics,
          }

          axios.defaults.crossDomain = true
          axios.defaults.withCredentials  = true
          axios.post('https://ed808.com:92/latin/contents',
            data,
            {
              headers: {
                'Content-type': 'application/json',
                'X-CSRF-Token' : this.getCookie("token")
              }
            })
            .then((data) => {
              this.postSaved = true
              //stop loading for fields
              this.loading = false
              this.$emit('updatePosts')
              this.resetForm()
            })
            .catch(e => {
              this.loading = false
              if(e.hasOwnProperty('response')){
                if(e.response.hasOwnProperty('data'))
                  this.errorText = e.response.data[0]
                else{
                  this.errorText = e.response
                }
              }
              else{
                this.errorText = e
              }
              this.hasError = true
            })
        } else {
          this.hasError = true
          this.errorText = "Title and body shouldn't be empty"
        }

      },

      cancelEditor(){
        this.editorActive = false
        this.mainPicture = null
        this.body = null
        this.title = null
        this.mainPictureUploaded = false
        this.$emit('deleteUploaded',this.uploadedFiles)
      },
      // Clear Editor
      resetForm(){
        //stop loading for fields
        this.loading = false
        this.body = null
        this.title = null
        this.mainPicture = null
        this.mainPictureUploaded = false
        this.uploadedFiles = [];
      },

      // Upload Main Picture of Story
      uploadStoryImage(){
        let file = this.$refs.mainPicture.$refs.inputFile.files[0]
        // check if Main Image is assigned Before Delete the old one
        if (this.mainPictureUploaded) {
          this.$emit('deleteUploaded',[
            this.mainPictureUploaded.split('/').pop()
          ])
          this.mainPictureUploaded = false

        }
        // check size of image
        if(file && file.size <= 5*1024*1024) {
          this.loading = true
          const form = new FormData()
          form.append('image', file)
          axios.defaults.withCredentials = true
          axios({
            url: 'https://ed808.com:92/latin/file/upload_image', // server url. If the url is empty then the base64 returns
            method: 'POST',
            data : form,
            headers: {
              "Content-Type": "multipart/form-data",
              "X-CSRF-Token": this.getCookie("token")
            }
          }).then((data)=> {
            this.loading = false
            this.uploaded.push(data.data.filename)
            this.$emit('update:uploadedFiles', this.uploaded)
            this.fid = data.data.fid
            this.mainPictureUploaded = data.data.fileUrl
          }).catch(e=>{
            this.loading = false
            if(e.hasOwnProperty('response')){
              if(e.response.hasOwnProperty('data'))
                this.errorText = e.response.data[0]
              else{
                this.errorText = e.response
              }
            }
            else{
              this.errorText = e
            }
            this.hasError = true
          })
        }
        // show error for size of picture
        else if (file && file.size > 5*1024*1024) {
          this.$refs.mainPicture.clearField()
          this.hasError = true
          this.errorText = 'Image size should not be larger than 5 MB'
        }
      },
      getTopics(){
        axios.get('https://ed808.com:92/latin/contents/list/topics')
          .then(data=>{
            this.topics = data.data
          })
      }
    },

  }
</script>

<style lang="scss" scoped>
.editor-container {
  position: relative;
  width: 100%;
  padding: 10px;
  .card {
    position: relative;
    display: inline-block;
    border-radius: 6px;
    width: 100%;
    margin: 0 27px 27px 0;
    transition: .3s cubic-bezier(.25, .8, .5, 1);

    &:hover .card-head .edit {
      opacity: 1;
    }

    .card-head {
      border-bottom: 1px solid #eee;
      min-height: 48px;
      padding: 4px 16px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      transition: padding .4s cubic-bezier(.25, .8, .25, 1);

      .md-icon:first-child {
        margin-right: 14px;
        margin-top: 7px;
      }

      .item-text {
        flex: 1;
        display: flex;
        flex-direction: column;
        align-items: flex-start;
        overflow: hidden;
        line-height: 1.25em;
        white-space: nowrap;
      }

      .edit {
        opacity: 0;
        transition: .3s cubic-bezier(.25, .8, .5, 1);
        margin: 0 -12px 0 0;

        .md-icon {
          margin: 0px !important;
        }
      }
    }

    .card-text {
      margin: 10px 0;
      font-size: 15px;
      padding: .9375rem 1.875rem;

      .card-row {
      }

      .default-text {
        color: #BDBDBD;
      }

      .label {
        color: #888;
        font-size: 13px;
      }

      .card-edit {

        .md-field {
          margin-bottom: 9px;
        }

        .md-field.md-has-textarea {
          textarea {
            font-size: 16px !important;
          }

          &:not(.with-label) {
            padding: 7px 1px;
          }

          margin: 0;
        }
      }
    }
  }
  #editor{
    transition: all 0.9s ease-in-out;
    /*overflow: hidden;*/
    .ql-editor{
      min-height: 250px;
    }
    > * {
      //opacity: 0;
      //:0px;
      transition: all 0.9s ease-in-out;
    }
    .title{
      input{
        width: 100%;
      }
    }
    .quillWrapper{
      text-align: left;
      margin: 30px auto 0px auto;
    }
    .buttons{
      margin-top: 10px;
    }
  }
  #editorClick{
    text-align: left;
  }
  .md-card{
    box-shadow: 0px -1px 5px 0 rgba(0,0,0,.12), 0 2px 2px 0 rgba(0,0,0,.14), 0 3px 1px -2px rgba(0,0,0,.2), 0 1px 5px 0 rgba(0,0,0,.12);
  }
  button{
    margin: 6px 0 0 0;
    &.green{
      background-color: #4caf50 !important;
      color: white !important;
    }
  }
  .card.minimal {
    min-width: 799px;
    width: 60%;
    margin-top: 28px;
    box-shadow: 0 3px 3px -2px rgba(0, 0, 0, 0.2), 0 3px 4px 0 rgba(0, 0, 0, 0.14), 0 1px 8px 0 rgba(0, 0, 0, 0.12);
    margin-right: 8px;

  }
}
</style>
