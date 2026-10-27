<template>
  <div>

    <!--Confirm modal for deleting items-->
    <md-dialog-confirm
      :md-active.sync="deletingItem"
      md-content="Are you sure you want to delete this item?"
      md-confirm-text="Delete"
      md-cancel-text="Cancel"
      @md-cancel="deletingItem = false"
      @md-confirm="submitSocial(deletingItem, 'delete')"
    />

    <md-tabs>
      <md-tab
        id="account"
        md-label="Account Settings"
        md-icon="person_outline"
      >
        <section>
          <h4>External Links</h4>
          <div class="loading-spinner" v-if="sending">
            <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
          </div>
          <div
            class="md-layout md-gutter social-form"
            v-for="(link,index) in user.socials"
            :key="link.sid"
          >
            <div class="md-layout-item md-size-50 md-small-size-100" style="padding-left: 30px">
              <md-field
                class="input-with-icon"
              >
                <span class="icon-prefix mdi" :class="'mdi-' + user.socials[index].name"></span>
                <label>
                  Link
                </label>
                <md-input
                  v-model="user.socials[index].link"
                  type="text"
                  :disabled="sending"
                />
              </md-field>
            </div>
            <div class="md-layout-item md-size-30 md-small-size-50">
              <md-field>
                <label :for="'social-select-' + link.sid">Social Network</label>
                <md-select
                  v-model="user.socials[index].name"
                  :name="'social-select-' + link.sid"
                  :id="'social-select-' + link.sid"
                >
                  <md-option value="linkedin">
                    LinkedIn
                  </md-option>
                  <md-option value="instagram">
                    Instagram
                  </md-option>
                  <md-option value="facebook">
                    Facebook
                  </md-option>
                  <md-option value="link">
                    Personal Website
                  </md-option>
                  <md-option value="web">
                    Other
                  </md-option>
                </md-select>
              </md-field>
            </div>
            <div class="md-layout-item md-size-20 md-small-size-50" style="padding-top: 10px">
              <md-button class="md-icon-button" @click="submitSocial(index)">
                <md-icon>check</md-icon>
                <md-tooltip>Save Changes</md-tooltip>
              </md-button>
              <md-button class="md-icon-button" @click="deletingItem = index.toString()">
                <md-icon>delete</md-icon>
                <md-tooltip>Delete Item</md-tooltip>
              </md-button>
            </div>
          </div>

          <md-button class="md-icon-button" @click="addingNewSocail=true">
            <md-icon>add</md-icon>
            <md-tooltip>Add New Item</md-tooltip>
          </md-button>

          <!--new social-->
          <transition name="fade">
            <div v-if="addingNewSocail">
              <h4>New External Link</h4>
              <div
                class="md-layout md-gutter"
                style="padding-left: 30px"
              >
                <div class="md-layout-item md-size-50 md-small-size-100">
                  <md-field
                    class="input-with-icon"
                  >
                    <span class="icon-prefix mdi" :class="'mdi-' + newSocial.name"></span>
                    <label>
                      Link
                    </label>
                    <md-input
                      v-model="newSocial.link"
                      type="text"
                      :disabled="sending"
                    />
                  </md-field>
                </div>
                <div class="md-layout-item md-size-30 md-small-size-50">
                  <md-field>
                    <label>Social Network</label>
                    <md-select
                      v-model="newSocial.name"
                    >
                      <md-option value="linkedin">
                        LinkedIn
                      </md-option>
                      <md-option value="instagram">
                        Instagram
                      </md-option>
                      <md-option value="facebook">
                        Facebook
                      </md-option>
                      <md-option value="link">
                        Personal Website
                      </md-option>
                      <md-option value="web">
                        Other
                      </md-option>
                    </md-select>
                  </md-field>
                </div>
                <div class="md-layout-item md-size-20 md-small-size-50" style="padding-top: 10px">
                  <md-button class="md-icon-button" @click="submitSocial(null)">
                    <md-icon>check</md-icon>
                    <md-tooltip>Save Link</md-tooltip>
                  </md-button>
                  <md-button class="md-icon-button" @click="toggleNewSocial">
                    <md-icon>clear</md-icon>
                    <md-tooltip>Discard</md-tooltip>
                  </md-button>
                </div>
              </div>
            </div>
          </transition>

        </section>
      </md-tab>
      <md-tab
        id="security"
        md-label="Privacy & Security"
        md-icon="security"
      >
        <section>
          <h4>Change Password</h4>
          <md-progress-bar
            v-if="sending"
            md-mode="indeterminate"
          />
          <form
            id="password_change"
            novalidate
            @submit.prevent="formSubmit"
          >
            <!-- each field hast validation class that is set by vuelidate -->
            <md-field
              :class="getValidationClass('password')"
              class="input-with-icon"
            >
              <span class="icon-prefix mdi mdi-lock"></span>
              <label for="current_password">
                Current Password
              </label>
              <md-input
                id="current_password"
                v-model="form.current_password"
                type="password"
                name="password"
                autocomplete="new-password"
                :disabled="sending"
                required
              />
              <span
                v-if="!$v.form.current_password.required"
                class="md-error"
              >
                The Password is required
              </span>
              <span
                v-else-if="!$v.form.current_password.minLength"
                class="md-error"
              >
                Should be at least 6 character
              </span>
            </md-field>

            <!-- each field hast validation class that is set by vuelidate -->
            <md-field
              :class="getValidationClass('password')"
              class="input-with-icon"
            >
              <span class="icon-prefix mdi mdi-lock"></span>
              <label for="password">
                New Password
              </label>
              <md-input
                id="password"
                v-model="form.password"
                type="password"
                name="password"
                autocomplete="new-password"
                :disabled="sending"
                required
              />
              <span
                v-if="!$v.form.password.required"
                class="md-error"
              >
                The Password is required
              </span>
              <span
                v-else-if="!$v.form.password.minLength"
                class="md-error"
              >
                Should be at least 6 character
              </span>
            </md-field>
            <!-- each field hast validation class that is set by vuelidate -->
            <md-field
              :class="getValidationClass('rePassword')"
              class="input-with-icon"
            >
              <span class="icon-prefix mdi mdi-lock"></span>
              <label for="rePassword">
                Password Confirmation
              </label>
              <md-input
                id="rePassword"
                v-model="form.rePassword"
                type="password"
                name="rePassword"
                autocomplete="new-password"
                :disabled="sending"
                required
              />
              <span
                v-if="!$v.form.rePassword.required"
                class="md-error"
              >
                The Password Confirmation is required
              </span>
              <span
                v-else-if="!$v.form.rePassword.sameAsPassword"
                class="md-error"
              >
                Password is not the same
              </span>
            </md-field>

            <!-- Snackbar for showing success or error in registration -->
            <md-snackbar
              :md-active.sync="PasswordSaved"
            >
              Your password saved successfully!
            </md-snackbar>
            <md-snackbar
              class="error"
              :md-active.sync="showError"
            >
              {{ errors }}
            </md-snackbar>
          </form>
          <md-button
            class="md-raised md-primary "
            type="submit"
            :disabled="sending"
            form="password_change"
          >
            Save
          </md-button>
        </section>
      </md-tab>
    </md-tabs>
    <!--<span v-for="(role,index) in roles" :key="index">
      {{role}}
      <span v-if="(roles.length-1 != index)">,</span>
    </span>-->
  </div>
</template>

<script>

  import axios from "@/node_modules/axios";
  import { cookie } from '@/components/mixins/cookie.js'
  import { validationMixin } from 'vuelidate'
  import {
    required,
    email,
    minLength,
    sameAs
  } from 'vuelidate/lib/validators'
  import VueRecaptcha from 'vue-recaptcha'

  export default {
    name: 'Settings',
    layout: 'userpanel',
    components: {
      'vue-recaptcha': VueRecaptcha
    },
    scrollToTop: true,
    mixins: [validationMixin,cookie],
    data(){
      return{
        //for keeping last field data temporary, after editing it
        user_copy:{},
        errors:'',
        showError: false,
        roles:[],
        sending: false,
        PasswordSaved: false,
        uid:this.$route.params.uid,
        loading: true,
        updateField: false,
        user:{},
        sameUser:false,
        newSocial: {
          link: null,
          name: null
        },
        deletingItem: false,
        addingNewSocail : false,
        form: {
          current_password: null,
          password: null,
          rePassword: null,
        }
      }
    },
    //Vuelidate Validation Options & Configs
    validations: {
      form: {
        current_password: {
          required,
          minLength: minLength(6)
        },
        password: {
          required,
          minLength: minLength(6)
        },
        rePassword: {
          required,
          sameAsPassword: sameAs('password')
        }
      }
    },
    asyncData({isDev, router, store, env, params, query, req, res, redirect, error}) {
      if (store.state.user.uid != params.uid )
        redirect('/user/'+params.uid)
    },
    mounted(){
      this.isSameUser()
    },
    methods:{
      // Refresh Register Form
      clearForm () {
        this.$v.$reset()
        this.form.current_password = null
        this.form.password = null
        this.form.rePassword = null
        this.showError = false
        this.errors = ''
      },
      toggleNewSocial(){
        if(this.addingNewSocail)
          this.newSocial = { name: null, link: null }

        this.addingNewSocail = !this.addingNewSocail
      },
      // Main Function For Registration
      async savePassword() {
        this.sending = true
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        await axios.put('https://ed808.com:92/latin/user/'+ this.uid,
          {
            hash : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            current_password: this.form.current_password,
            password : this.form.password,
          },
          {
            headers: {
              'Content-type': 'application/json',
              'X-CSRF-Token' : this.getCookie("token")
            }
          })
          .then(() => {
            this.sending = false
            this.passwordSaved = true
          })
          .catch(e => {
            if(e.hasOwnProperty('response')){
              if(e.response.hasOwnProperty('data'))
                this.errors = e.response.data
              else{
                this.errors = e.response
              }
            }
            else{
              this.errors = e
            }
            this.showError = true
          })
        this.clearForm()
      },
      submitSocial(index,action){
        this.sending = true
        let data = {}
        console.log()
        if(index != null) {
          if(action == 'delete') {
            data = {
              "hash": "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "socials": {
                "sid": this.user.socials[index].sid,
                "delete": true
              }
            }
          }else {
            data = {
              "hash": "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "socials": this.user.socials[index]
            }
          }
        }
        else {

            data = {
              "hash": "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "socials": this.newSocial
            }

        }
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials  = true;
        axios.put('https://ed808.com:92/latin/user/'+ this.uid, data,
          {
            headers: {
              'Content-type': 'application/json',
              'X-CSRF-Token' : this.getCookie("token")
            }
          })
          .then((data) => {
            this.user = data.data
            this.newSocial = { name: null, link: null }
            this.sending = false
          })
      },
      // This Functions Runs Some Preprocessing Func Before Submit
      formSubmit() {
        // Validating Form
        this.$v.$touch()
        console.log(this.$v)
        // run recaptcha and let it to fire up 'saveUser' function with google token
        if (!this.$v.$invalid) {
          this.savePassword()
        }
      },
      isSameUser(){
        if(this.$store.getters.getUid){
          if(this.$store.getters.getUid == this.uid)
            this.sameUser = true
          //console.log(this.sameUser)
        }
        this.getProfile()
      },
      // Set Invalid Class For Invalid Inputs
      getValidationClass (fieldName) {
        const field = this.$v.form[fieldName]

        if (field) {
          return {
            'md-invalid': field.$invalid && field.$dirty
          }
        }
      },
      handleFileUpload(fieldName){

        //start loading for files
        this.updateFile[fieldName] = true

        if(fieldName == 'picture'){
          this.file[fieldName] = this.$refs.file2.files[0]
        }
        else if(fieldName == 'background_image'){
          this.file[fieldName] = this.$refs.file1.files[0]
        }
        else if(fieldName == 'cv'){
          this.file[fieldName] = this.$refs.file3.files[0]
        }

        this.submitFile(fieldName)
      },
      submitFile(fieldName){
        let formData = new FormData();
        formData.append('file', this.file[fieldName]);
        //needs validation here
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials  = true;
        axios.post('https://ed808.com:92/latin/file?parameter[hash]=020042f70a981fd6806ecf5e53f2267b377da9d9f981e15297d70c3f7c2a87d0',
          formData,
          {
            headers: {
              'Content-Type': 'multipart/form-data',
              'X-CSRF-Token' : this.getCookie("token")
            }
          }
        ).then((data) =>{
          this.afterUpload[fieldName] = data.data.file

          //@TODO: add some prettier notification here after uploading cv with success
          //stop loading for files
          this.updateFile['cv'] = false

          console.log(this.afterUpload[fieldName])
          console.log('New '+ fieldName + ' was upload SUCCESSfully')
          if((fieldName == 'picture') || (fieldName == 'background_image')){
            this.doneEditingThis(fieldName)
          }
        })
          .catch((e) =>{
            console.log('FAILURE!!' + e)
          })
      },
      cancleEditingThis(fieldName){
        if(fieldName == 'education'){
          this.user['university'] = this.user_copy['university']
          this.user['education_degree'] = this.user_copy['education_degree']
          this.user['education_field'] = this.user_copy['education_field']
        }
        else if(fieldName == 'personal'){
          this.user['full_name'] = this.user_copy['full_name']
          this.user['mail'] = this.user_copy['mail']
          this.user['mobile'] = this.user_copy['mobile']
        }
        else if(fieldName == 'work'){
          this.user['job'] = this.user_copy['job']
          this.user['skills'] = this.user_copy['skills']
        }
        else{
          this.user[fieldName] = this.user_copy[fieldName]
        }
        //switch from editing section to showing data
        this.editingEl = ''
      },
      doneEditingThis(fieldName){

        //start loading for fields except files
        this.updateField = true

        //create data with field that was changed
        var data = this.createData(fieldName)

        axios.defaults.crossDomain = true
        axios.defaults.withCredentials  = true
        axios.put('https://ed808.com:92/latin/user/'+ this.uid,
          data,
          {
            headers: {
              'Content-type': 'application/json',
              'X-CSRF-Token' : this.getCookie("token")
            }
          })
          .then((data) => {
            if((fieldName == 'picture') || (fieldName == 'background_image')){
              this.afterFileSaved(fieldName,data)
            }
            else{
              this.afterfieldsaved(fieldName,data)
            }
          })
          .catch(e => {
            if(e.hasOwnProperty('response')){
              if(e.response.hasOwnProperty('data'))
                this.errors = e.response.data
              else{
                this.errors = e.response
              }
            }
            else{
              this.errors = e
            }
            this.showError = true
          })
      },
      afterFileSaved(fieldName,data){

        this.user[fieldName] = data.data[fieldName]

        //stop loading for files
        this.updateFile[fieldName] = false

        //update navbar with new picture
        if(fieldName == 'picture'){
          this.$store.commit('CHANGE_USER_DATA', data.data[fieldName], '')
        }

      },
      afterfieldsaved(fieldName,data){

        //for cv uploading cv because the field is not binding with like others
        if((fieldName == 'work') && (Object.keys(this.afterUpload['cv']).length > 0)){
          this.user.cv = data.data.cv
          this.user.cvName = data.data.cvName
          this.user_copy['cv'] = data.data.cv
          this.user_copy['cvName'] = data.data.cvName
        }

        //stop loading for fields
        this.updateField = false

        //switch from editing section to showing data
        this.editingEl = ''

        //making change to copy object as well
        if(fieldName == 'education'){
          this.user_copy['university'] = this.user['university']
          this.user_copy['education_degree'] = this.user['education_degree']
          this.user_copy['education_field'] = this.user['education_field']
        }
        else if(fieldName == 'personal'){
          this.user_copy['full_name'] = this.user['full_name']
          this.user_copy['mail'] = this.user['mail']
          this.user_copy['mobile'] = this.user['mobile']

          //update navbar with new full_names but it's not working
          this.$store.commit('CHANGE_USER_DATA','', this.user['full_name'])
        }
        else if(fieldName == 'work'){
          this.user_copy['job'] = this.user['job']
          this.user_copy['skills'] = this.user['skills']
        }
        else{
          this.user_copy[fieldName] = this.user[fieldName]
        }

      },
      createData(fieldName){
        var data = {}
        if(fieldName == 'education'){
          data = {
            "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            "university" : this.user['university'],
            "education_degree" : this.user['education_degree'],
            "education_field" : this.user['education_field']
          }
        }
        else if(fieldName == 'personal'){
          data = {
            "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            "full_name" : this.user['full_name'],
            "mail" : this.user['mail'],
            "mobile" : this.user['mobile']
          }
        }
        else if(fieldName == 'work'){
          data = {
            "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            "job" : this.user['job'],
            "skills" : this.user['skills'],
          }
          //check if cv was uploaded
          if(Object.keys(this.afterUpload['cv']).length > 0){
            Object.assign(data,{"cv" : this.afterUpload['cv']})
            console.log(data)
          }
        }
        else if((fieldName == 'picture') || (fieldName == 'background_image')){
          data = {
            "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            [fieldName] : this.afterUpload[fieldName]
          }
        }
        else{
          data = {
            "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            [fieldName] : this.user[fieldName]
          }
        }
        return data
      },
      getProfile(){
        this.sending = true
        if(!this.sameUser) {
          this.$router.push('/user/'+this.uid)
        }
        else {
          axios.defaults.crossDomain = true;
          axios.defaults.withCredentials  = true;
          axios.get('https://ed808.com:92/latin/user/'+ this.uid,
            {
              headers: {
                'Content-type': 'application/json'
              }
            })
            .then((data) => {
              this.user = data.data
              this.user_copy = Object.assign({}, this.user)
              /*for(var field in this.user){
                this.user[field] = data.data[field]
              }
              /*for (var key in data.data.roles) {
                this.roles.push(data.data.roles[key])
              }*/
              this.roles = Object.values(data.data.roles)
              this.sending = false
            })
            .catch(e => {
              this.errors = e.response.data
            })
        }
      },
    }
  }
</script>

<style scoped lang="scss">
  @import '@/assets/scss/vars.scss';
  .md-tab {
    min-height: 400px;
  }
  .loading {
    position: absolute;
    width: 100%;
    height: 5px;
    left: 0;
    bottom: 0;
    background: #fff;
  }

  .edit_cv{
    position: relative;
    width: 155px;
    border-bottom: 1px solid #d9d9d9;
    input{
      height: 40px;
      overflow: hidden;
      width: 100%;
      opacity: 0;
      z-index: 3;
      position: relative;
      cursor: pointer;
    }
    i{
      position: absolute;
      left: 0;
      top: 14px;
    }
  }
  .bottom {
    max-width: 1130px;
    margin: 27px auto;
    padding: 5px;
    text-align: left;
    display: flex;
    flex-wrap: wrap;

    .min-col {
      width: 28%;
      margin-right: 2%;
      @media screen and (max-width: 992px) {
        width: 38%;
        margin-right: 2%;
      }
      @media screen and (max-width: 600px) {
        width: 100%;
        margin-right: 0;
      }
    }

    .max-col {
      max-width: 70%;
      @media screen and (max-width: 992px) {
        width: 60%;
      }
      @media screen and (max-width: 600px) {
        width: 100%;
      }

      .box {
        min-width: 100%;
      }
    }
  }
  .box {
    position: relative;
    display: inline-block;
    width: 100%;
    margin: 0 27px 27px 0;
    transition: .3s cubic-bezier(.25, .8, .5, 1);

    &:hover .box-head .edit {
      opacity: 1;
    }

    .box-head {
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

    .box-text {
      margin: 10px 0;
      font-size: 15px;

      .box-row {
        padding: 5px 25px;
      }

      .default-text {
        color: #BDBDBD;
      }

      .label {
        color: #888;
        font-size: 13px;
      }

      .box-edit {
        padding: 5px 25px;

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

        .md-layout.md-alignment-center-space-between .md-layout-item {
          flex: 0;

          button {
            margin: 6px 0 0 0;

            &.green {
              background-color: #4caf50;
              color: white !important;
            }
          }
        }
      }
    }
  }
  section{
    width: 60%;
    margin: auto;
    position: relative;
    @media #{$x1024} {
      width: 80%;
    }
    @media #{$x600} {
      width: 95%;
    }
    .social-form {
      @media #{$x600} {
        margin-top: 20px;
      }
      .md-layout-item.md-size-30 {
        @media #{$x600} {
          padding-left: 30px;
        }
      }
    }
    form#password_change {
      padding-left: 30px;
      .md-field {
        position: relative;
        label {
        }
        .mdi {
          position: absolute;
          left: -35px;
          top: 21px;
          font-size: 24px;
          color: #757575;
        }
      }
    }
  }
  .md-tab {
    .md-field {
      .mdi {
        position: absolute;
        left: -35px;
        top: 21px;
        font-size: 24px;
        color: #757575;
      }
    }
  }
</style>
