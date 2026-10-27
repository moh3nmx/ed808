<template>
  <!-- user posts + Personal user info without edit -->
  <div ref="content">
    <div id="cv_canvas" v-html="testCanvas"></div>
    <div class="profile-brief profile-sections">
      <section>
        <h3 class="title">
          {{ user.full_name ? user.full_name : user.name }}
        </h3>
        <div v-if="!sameUser && user.name">
          <button
            v-if="!sameUser"
            class="md-follow"
            :class="user.user_follow ? 'active' : ''"
            @click="follow(user.user_follow, uid)"
          >
            {{ user.user_follow ? 'Following' : 'Follow' }}
          </button>
        </div>
        <h6 v-if="(user.job && editingEl != 'headline') || (sameUser && editingEl != 'headline')">
          <span v-if="user.job != null && user.job != ''">{{ user.job }}</span>
          <span v-else-if="sameUser" @click="editingEl='headline'" style="color: #999">To add your headline please click here.</span>
          <md-menu class="actions">
            <md-button
              class="md-icon-button md-dense"
              md-menu-trigger
              v-if="sameUser"
            >
              <md-icon>
                more_vert
              </md-icon>
            </md-button>
            <md-menu-content>
              <md-menu-item @click="onEditingFields('headline')">
                Edit
              </md-menu-item>
            </md-menu-content>
          </md-menu>
        </h6>
        <div v-if="sameUser && editingEl == 'headline'">
          <md-field>
            <label>Headline</label>
            <md-input v-model="user.job"></md-input>
          </md-field>
          <div class="loading-spinner" v-if="updateField">
            <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
          </div>
          <div class="md-layout md-alignment-center-space-between profile-edit-actions" style="    justify-content: space-between;">
            <div class="md-layout-item">
              <md-button
                :disabled="updateField"
                class="md-raised green"
                @click="completeEditingFields('headline')"
              >
                Save
              </md-button>
            </div>
            <div class="md-layout-item">
            <div class="md-layout-item">
              <md-button
                :disabled="updateField"
                @click="cancelEditingFields('headine')"
              >
                Cancel
              </md-button>
            </div>
          </div>
        </div>
        </div>
        <div class="user-social-links">
          <a
            v-if="user.hasOwnProperty('mail')"
            :href="'mailto:'+user.mail"
          >
            <md-button class="md-icon-button">
              <i class="mdi mdi-email"></i>
            </md-button>
            <md-tooltip>{{user.mail}}</md-tooltip>
          </a>

          <a v-for="link in user.socials" :key="link.sid" :href="link.link">
            <md-button class="md-icon-button">
              <i class="mdi" :class="`mdi-${link.name}`"></i>
            </md-button>
            <md-tooltip>{{ firstCapitalize(link.name) }}</md-tooltip>
          </a>
        </div>
      </section>

    </div>

    <!--Modal for editing education and experience fields and skills section-->
    <md-dialog :md-active.sync="showModal" :md-fullscreen="true" class="md-scrollbar">
      <md-dialog-title v-if="newItem == 'university'">Add new education</md-dialog-title>
      <md-dialog-title v-if="newItem == 'work'">Add new experience</md-dialog-title>
      <md-dialog-title v-if="newItem == 'skill'">Add new skill</md-dialog-title>
      <md-dialog-title v-if="newItem == 'hobby'">Add Hobbies &amp; Intrests</md-dialog-title>
      <md-dialog-title v-if="editingEl == 'university'">Edit education</md-dialog-title>
      <md-dialog-title v-if="editingEl == 'work'">Edit experience</md-dialog-title>
      <md-dialog-title v-if="editingEl == 'skill'">Edit Skills</md-dialog-title>

      <!--add new education-->
      <div v-if="newItem == 'university'" class="new-experience">
        <div class="loading-spinner" v-if="updateField">
          <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
        </div>
        <div>
          <md-field :class="getValidationClass('title')">
            <label for="title">Field of study</label>
            <md-input name="title" id="title" autocomplete="given-name" v-model="exp.title" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.title.required">Field of study is required</span>
          </md-field>
        </div>
        <div>
          <md-field :class="getValidationClass('location')">
            <label for="location">School</label>
            <md-input name="location" id="location" autocomplete="given-name" v-model="exp.location" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.location.required">School is required</span>
          </md-field>
        </div>
        <div class="md-layout md-gutter">
          <div class="md-layout-item">
            <md-field :class="getValidationClass('position')">
              <label for="position">Degree</label>
              <md-input name="position" id="position" autocomplete="given-name" v-model="exp.position" :disabled="loading" />
            </md-field>
          </div>
        </div>
        <div class="md-layout md-gutter">
          <div class="md-layout-item md-size-50">
            <md-field :class="getValidationClass('start_year')">
              <md-select v-model="exp.start_year" placeholder="Start Year" :disabled="loading">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field :class="getValidationClass('start_month')">
              <md-select v-model="exp.start_month" placeholder="Start Month" :disabled="loading">
                <md-option v-for="(n,index) in mnthArray" :value="index+1" :key="n">{{ n }}</md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div class="md-layout md-gutter" v-if="!exp.untilNow">
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select v-model="exp.end_year" placeholder="End Year">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select
                v-model="exp.end_month"
                placeholder="End Month"
              >
                <md-option
                  v-for="(n,index) in mnthArray"
                  :value="index+1"
                  :key="n"
                >
                  {{ n }}
                </md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div>
          <md-checkbox v-model="exp.untilNow" class="md-primary" :disabled="loading">I am currently studying.</md-checkbox>
        </div>
        <md-field>
          <label>Description</label>
          <md-textarea v-model="exp.description" :disabled="loading"></md-textarea>
        </md-field>
        <div class="footer">
          <md-button class="md-primary md-raised" @click="submitExp()">
            Add
          </md-button>
          <md-button @click="addNewExp('education')">
            Cancel
          </md-button>
        </div>
      </div>

      <!--add new job experience-->
      <div v-if="newItem == 'work'" class="new-experience">
        <div class="loading-spinner" v-if="updateField">
          <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
        </div>
        <div>
          <md-field :class="getValidationClass('title')">
            <label for="title">Job title</label>
            <md-input name="title" id="title" autocomplete="given-name" v-model="exp.title" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.title.required">Field of study is required</span>
          </md-field>
        </div>
        <div>
          <md-field :class="getValidationClass('location')">
            <label for="location">Company</label>
            <md-input name="location" id="location" autocomplete="given-name" v-model="exp.location" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.location.required">School is required</span>
          </md-field>
        </div>
        <div class="md-layout md-gutter">
          <div class="md-layout-item md-size-50">
            <md-field :class="getValidationClass('start_year')">
              <md-select v-model="exp.start_year" placeholder="Start Year" :disabled="loading">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field :class="getValidationClass('start_month')">
              <md-select v-model="exp.start_month" placeholder="Start Month" :disabled="loading">
                <md-option v-for="(n,index) in mnthArray" :value="index+1" :key="n">{{ n }}</md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div class="md-layout md-gutter" v-if="!exp.untilNow">
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select v-model="exp.end_year" placeholder="End Year">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select
                v-model="exp.end_month"
                placeholder="End Month"
              >
                <md-option
                  v-for="(n,index) in mnthArray"
                  :value="index+1"
                  :key="n"
                >
                  {{ n }}
                </md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div>
          <md-checkbox v-model="exp.untilNow" class="md-primary" :disabled="loading">I am currently working.</md-checkbox>
        </div>
        <md-field>
          <label>Description</label>
          <md-textarea v-model="exp.description" :disabled="loading"></md-textarea>
        </md-field>
        <div class="footer">
          <md-button class="md-primary md-raised" @click="submitExp()">
            Add
          </md-button>
          <md-button @click="addNewExp('work')">
            Cancel
          </md-button>
        </div>
      </div>

      <!--edit education-->
      <div v-if="editingEl == 'university'" class="new-experience">
        <div class="loading-spinner" v-if="updateField">
          <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
        </div>
        <div>
          <md-field :class="getValidationClass('title')">
            <label for="title">Field of study</label>
            <md-input name="title" id="title" autocomplete="given-name" v-model="exp.title" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.title.required">Field of study is required</span>
          </md-field>
        </div>
        <div>
          <md-field :class="getValidationClass('location')">
            <label for="location">School</label>
            <md-input name="location" id="location" autocomplete="given-name" v-model="exp.location" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.location.required">School is required</span>
          </md-field>
        </div>
        <div class="md-layout md-gutter">
          <div class="md-layout-item">
            <md-field :class="getValidationClass('position')">
              <label for="position">Degree</label>
              <md-input name="position" id="position" autocomplete="given-name" v-model="exp.position" :disabled="loading" />
            </md-field>
          </div>
        </div>
        <div class="md-layout md-gutter">
          <div class="md-layout-item md-size-50">
            <md-field :class="getValidationClass('start_year')">
              <md-select v-model="exp.start_year" placeholder="Start Year" :disabled="loading">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field :class="getValidationClass('start_month')">
              <md-select v-model="exp.start_month" placeholder="Start Month" :disabled="loading">
                <md-option v-for="(n,index) in mnthArray" :value="index+1" :key="n">{{ n }}</md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div class="md-layout md-gutter" v-if="!exp.untilNow">
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select v-model="exp.end_year" placeholder="End Year">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select
                v-model="exp.end_month"
                placeholder="End Month"
              >
                <md-option
                  v-for="(n,index) in mnthArray"
                  :value="index+1"
                  :key="n"
                >
                  {{ n }}
                </md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div>
          <md-checkbox v-model="exp.untilNow" class="md-primary" :disabled="loading">I am currently studying.</md-checkbox>
        </div>
        <md-field>
          <label>Description</label>
          <md-textarea v-model="exp.description" :disabled="loading"></md-textarea>
        </md-field>
        <div class="footer">
          <md-button :disabled="loading" class="md-primary md-raised" @click="submitExp(true)">
            Save
          </md-button>
          <md-button :disabled="loading" @click="openExpEdit('university')">
            Cancel
          </md-button>
        </div>
      </div>

      <!--edit job experience-->
      <div v-if="editingEl == 'work'" class="new-experience">
        <div class="loading-spinner" v-if="updateField">
          <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
        </div>
        <div>
          <md-field :class="getValidationClass('title')">
            <label for="title">Job title</label>
            <md-input name="title" id="title" autocomplete="given-name" v-model="exp.title" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.title.required">Field of study is required</span>
          </md-field>
        </div>
        <div>
          <md-field :class="getValidationClass('location')">
            <label for="location">Company</label>
            <md-input name="location" id="location" autocomplete="given-name" v-model="exp.location" :disabled="loading" />
            <span class="md-error" v-if="!$v.exp.location.required">School is required</span>
          </md-field>
        </div>
        <div class="md-layout md-gutter">
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select v-model="exp.start_year" placeholder="Start Year" :disabled="loading">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select v-model="exp.start_month" placeholder="Start Month" :disabled="loading">
                <md-option v-for="(n,index) in mnthArray" :value="index+1" :key="n">{{ n }}</md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div class="md-layout md-gutter" v-if="!exp.untilNow">
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select v-model="exp.end_year" placeholder="End Year">
                <md-option v-for="n in 50" :value="n+1969" :key="n">{{ n + 1969 }}</md-option>
              </md-select>
            </md-field>
          </div>
          <div class="md-layout-item md-size-50">
            <md-field>
              <md-select
                v-model="exp.end_month"
                placeholder="End Month"
              >
                <md-option
                  v-for="(n,index) in mnthArray"
                  :value="index+1"
                  :key="n"
                >
                  {{ n }}
                </md-option>
              </md-select>
            </md-field>
          </div>
        </div>
        <div>
          <md-checkbox v-model="exp.untilNow" class="md-primary" :disabled="loading">I am currently working.</md-checkbox>
        </div>
        <md-field>
          <label>Description</label>
          <md-textarea v-model="exp.description" :disabled="loading"></md-textarea>
        </md-field>
        <div class="footer">
          <md-button :disabled="loading" class="md-primary md-raised" @click="submitExp(true)">
            Save
          </md-button>
          <md-button :disabled="loading" @click="openExpEdit('work')">
            Cancel
          </md-button>
        </div>
      </div>

      <!--Modal for adding skill -->
      <div v-if="newItem == 'skill'" class="new-experience">
        <div class="loading-spinner" v-if="updateField">
          <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
        </div>
        <div>
          <md-field :class="getValidationClass('title',$v.newSkill)">
            <label for="title">Skill title</label>
            <md-input name="title" id="title" autocomplete="given-name" v-model="newSkill.title" :disabled="loading" />
            <span class="md-error" v-if="!$v.newSkill.title.required">Skill title is required</span>
          </md-field>
        </div>
        <div>
          <md-field :class="getValidationClass('type',$v.newSkill)">
            <md-select class="skill-type" v-model="newSkill.type" placeholder="Skill Type" :disabled="loading">
              <md-option value="Software">Software</md-option>
              <md-option value="General">General</md-option>
              <md-option value="Language">Language</md-option>
            </md-select>
            <span class="md-error" v-if="!$v.newSkill.type.required">Skill type is required</span>
          </md-field>
        </div>
        <div class="skill-level">
          <label for="" class="level">Skill level</label>
          <div class="rate">
            <input type="radio" id="star5" v-model="newSkill.rate" value="5" />
            <label for="star5" title="Expert"> </label>
            <input type="radio" id="star4"  v-model="newSkill.rate" value="4" />
            <label for="star4" title="Advanced"> </label>
            <input type="radio" id="star3"  v-model="newSkill.rate" value="3" />
            <label for="star3" title="Intermediate"> </label>
            <input type="radio" id="star2"  v-model="newSkill.rate" value="2" />
            <label for="star2" title="Novice"> </label>
            <input type="radio" id="star1"  v-model="newSkill.rate" value="1" />
            <label for="star1" title="Fundamental Awareness"> </label>
          </div>
        </div>
        <div class="footer">
          <md-button class="md-primary md-raised" @click="$v.newSkill.$touch()">
            Add
          </md-button>
          <md-button @click="addNewExp('skill')">
            Cancel
          </md-button>
        </div>
      </div>

      <!--Modal for adding hobbby -->
      <div v-if="newItem == 'hobby'" class="new-experience">
        <div class="loading-spinner" v-if="updateField">
          <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
        </div>

        <div>
          <ul class="data left clearfix rate">
            <li>
              <ul class="hb-list hobby">
                <li
                  class="hb-list li"
                  v-for="(interest, index) in selectableInterests"
                  :key="index"
                  @click="interest.selected=!interest.selected"
                  :class="{'active' : interest.selected}"
                  v-on:change="addHobbies(interest)"
                >
                  <span><md-icon>{{interest.icon}}</md-icon></span>
                  <md-tooltip md-direction="bottom">{{ interest.name }}</md-tooltip>
                </li>

              </ul>
            </li>
          </ul>
        </div>
        <div class="footer">
          <md-button class="md-primary md-raised" @click="submitExp()">
            Add
          </md-button>
          <md-button @click="addNewExp('hobby')">
            Cancel
          </md-button>
        </div>
      </div>

    </md-dialog>

    <!--Confirm modal for deleting items-->
    <md-dialog-confirm
      :md-active.sync="deletingItem"
      md-content="Are you sure you want to delete this item?"
      md-confirm-text="Delete"
      md-cancel-text="Cancel"
      @md-cancel="deletingItem = false"
      @md-confirm="deleteExp(deletingItem.jid, deletingItem.index, deletingItem.type)"
    />


    <div class="profile-sections">

      <!--Bio Section-->
      <section>
        <h3 class="section-title" v-if="(user.about != null && user.about != '') || sameUser">
          About Me
          <md-menu class="actions">
            <md-button
              class="md-icon-button md-dense"
              md-menu-trigger
              v-if="sameUser"
            >
              <md-icon>
                more_vert
              </md-icon>
            </md-button>
            <md-menu-content>
              <md-menu-item @click="onEditingFields('about')">Edit</md-menu-item>
              <md-menu-item @click="">Delete</md-menu-item>
            </md-menu-content>
          </md-menu>
        </h3>

        <div
          v-if="user.about && editingEl != 'about'"
          class="description text-center"
        >
          <p v-html="user.about" style="    white-space: pre-line;"></p>
        </div>

        <div
          v-if="sameUser && !user.about && editingEl != 'about'"
          class="description text-center"
        >
          <p @click="onEditingFields('about')">Click here to write your bio ...</p>
        </div>

        <div
          v-if="editingEl == 'about'"
          class="box-edit"
        >
          <md-field>
            <md-textarea
              v-model="user.about"
            />
            <div class="loading-spinner" v-if="updateField">
              <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
            </div>
          </md-field>
          <div class="md-layout md-alignment-center-space-between profile-edit-actions" style="    justify-content: space-between;">
            <div class="md-layout-item">
              <md-button
                :disabled="updateField"
                class="md-raised green"
                @click="completeEditingFields('about')"
              >
                Save
              </md-button>
            </div>
            <div class="md-layout-item">
              <md-button
                :disabled="updateField"
                @click="cancelEditingFields('about')"
              >
                Cancel
              </md-button>
            </div>
          </div>
        </div>


      </section>

      <!--Personal Info Section-->
      <section>
        <h3 class="section-title">
          Personal Info
          <md-menu class="actions">
            <md-button
              class="md-icon-button md-dense"
              md-menu-trigger
              v-if="sameUser"
            >
              <md-icon>
                more_vert
              </md-icon>
            </md-button>
            <md-menu-content>
              <md-menu-item @click="onEditingFields('personal')">Edit</md-menu-item>
              <md-menu-item @click="">Delete</md-menu-item>
            </md-menu-content>
          </md-menu>
        </h3>
        <div class="personal-info">
          <div class="loading-spinner" v-if="updateField">
            <md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>
          </div>
          <!--Listing-->
          <div class="listing mt-40">
            <div class="listing-inner">
              <div class="listing-event">
                <ul class="data left" v-if="editingEl == 'personal' && sameUser">
                  <li class="emph-1">Name</li>
                  <li class="emph-1">Username</li>
                  <li class="emph-1">Birthday</li>
                  <li class="emph-1">Country</li>
                  <li class="emph-1">Phone</li>
                  <li class="emph-1">Email</li>

                </ul>
                <ul class="data left" v-else>
                  <li class="emph-1">Name</li>
                  <li class="emph-1">Username</li>
                  <li class="emph-1" v-if="user.hasOwnProperty('birthday') && user.birthday != null">Birthday</li>
                  <li class="emph-1" v-if="user.hasOwnProperty('country') && user.country != null">Country</li>
                  <li class="emph-1" v-if="user.mobile && sameUser">Phone</li>
                  <li class="emph-1" v-if="user.mail && sameUser">Email</li>

                </ul>

                <ul class="data right" v-if="editingEl != 'personal'">
                  <li>{{ user.full_name }}</li>
                  <li>{{ user.name }}</li>
                  <li v-if="user.hasOwnProperty('birthday') && user.birthday != null">{{ timestampToDate }}</li>
                  <li v-if="user.hasOwnProperty('country')"> {{ getCountryByCode(user.country) }} </li>
                  <li v-if="user.mobile && sameUser">{{ getCountryByCode(user.country,true) }}  {{ user.mobile }}</li>
                  <li v-if="user.mail  && sameUser">{{ user.mail }}</li>
                </ul>

                <ul class="data right" v-if="sameUser && editingEl == 'personal'" >
                  <li>
                    <md-field>
                      <md-input v-model="user.full_name" placeholder="eg: John Doe"/>
                    </md-field>
                  </li>
                  <li>
                    <md-field>
                      <md-input v-model="user.name" disabled placeholder="eg: johndoe1980" required/>
                    </md-field>
                  </li>
                  <li>
                    <div class="md-layout date-picker">
                      <div class="md-layout-item md-size-33">
                        <md-field>
                          <md-select v-model="year" placeholder="Year">
                            <md-option v-for="n in 110" :value="n+1899" :key="n">{{ n + 1899 }}</md-option>
                          </md-select>
                        </md-field>
                      </div>
                      <div class="md-layout-item md-size-33">
                        <md-field>
                          <md-select v-model="month" placeholder="Month">
                            <md-option v-for="(n,index) in mnthArray" :value="index+1" :key="n">{{ n }}</md-option>
                          </md-select>
                        </md-field>
                      </div>
                      <div class="md-layout-item md-size-33">
                        <md-field>
                          <md-select v-model="day" placeholder="Day">
                            <md-option v-for="n in 31" :value="n" :key="n">{{ n }}</md-option>
                          </md-select>
                        </md-field>
                      </div>
                    </div>
                  </li>
                  <li>
                    <md-field>
                      <md-select v-model="user.country" placeholder="Country & Dial Code">
                        <md-option v-for="country in countries" :value="country.code" :key="country.code">{{ country.name }} ({{ country.dial_code }})</md-option>
                      </md-select>
                    </md-field>
                  </li>
                  <li>
                    <md-field class="phone-with-code">
                      <span class="code" style="padding-right: 5px">{{ getCountryByCode(user.country,true) }} </span>
                      <md-input v-model="user.mobile" placeholder="eg: 2558465885"/>
                    </md-field>
                  </li>
                  <li v-if="user.mail">
                    <md-field>
                      <md-input v-model="user.mail" placeholder="eg: johndoe1980@gmail.com" required/>
                    </md-field>
                  </li>
                </ul>
              </div>
            </div>
          </div>
          <!--End listing-->
          <div class="md-layout md-alignment-center-space-between profile-edit-actions" v-if="editingEl == 'personal'">
            <div class="md-layout-item">
              <md-button
                :disabled="updateField"
                class="md-raised green"
                @click="completeEditingFields('personal')"
              >
                Save
              </md-button>
            </div>
            <div class="md-layout-item">
              <md-button
                :disabled="updateField"
                @click="cancelEditingFields('personal')"
              >
                Cancel
              </md-button>
            </div>
          </div>
        </div>
      </section>

      <!--Experience Section-->
      <section v-if="(user.jobs.hasOwnProperty('work') && user.jobs.work.length > 0) || sameUser">
        <h3 class="section-title">
          Experience
          <div class="actions">
            <md-button
              class="md-icon-button md-dense"
              v-if="sameUser"
              @click="addNewExp('work')"
            >
              <md-icon>
                add
              </md-icon>
            </md-button>
          </div>
        </h3>
        <div class="personal-info">
          <!--Timeline-->
          <div class="timeline" v-if="user.jobs.hasOwnProperty('work') && user.jobs.work.length > 0">
            <div class="timeline-inner" v-for="(job,index) in user.jobs.work" :key="job.jid">
              <md-menu class="actions">
                <md-button
                  v-if="sameUser"
                  class="md-icon-button md-dense"
                  md-menu-trigger
                >
                  <md-icon>
                    more_vert
                  </md-icon>
                </md-button>
                <md-menu-content>
                  <md-menu-item @click="openExpEdit('work', index)">Edit</md-menu-item>
                  <md-menu-item @click="deleteConfirm(job.jid, index, 'work')">Delete</md-menu-item>
                </md-menu-content>
              </md-menu>
              <div class="name" :class="index%2 === 0 ? 'switched' : ''">
                <span class="date" v-if="job.start && job.start != '0'">{{ convertDate(job.start) }}
                  <em v-if="job.end == -1">Current</em>
                  <span v-else-if="job.end != 0">- {{ convertDate(job.end)}}</span>
                  <span v-if="job.end != '0'">({{ getTimestampMonth(job.start, job.end) }} {{ (getTimestampMonth(job.start, job.end)==1 ? 'Month' : 'Months') }})</span>
                </span>
                <h4> {{ job.title }} </h4>
                <p> at {{ job.location }} </p>
              </div>
              <div class="detail">
                <p v-if="job.description != null" v-html="job.description"></p>
                <p v-else-if="sameUser" style="color: #e2e2e2;" @click="openExpEdit('work',index)">Click here to add description.</p>
                <p v-else style="color: #e2e2e2;">No description.</p>
              </div>
            </div>
          </div>
          <div v-else-if="sameUser"  style="color: #999;font-size: 14px;" @click="addNewExp('work')">
            To add your work experiences please click here.
          </div>

          <!--End Timeline-->
        </div>
      </section>

      <!--Education Section-->
      <section v-if="(user.jobs.hasOwnProperty('university') && user.jobs.university.length > 0) || sameUser">
        <h3 class="section-title">
          Education
          <div class="actions">
            <md-button
              class="md-icon-button md-dense"
              v-if="sameUser"
              @click="addNewExp('university')"
            >
              <md-icon>
                add
              </md-icon>
            </md-button>
          </div>
        </h3>
        <div class="personal-info">
          <!--Timeline-->
          <div class="timeline" v-if="user.jobs.hasOwnProperty('university') && user.jobs.university.length > 0">
            <div class="timeline-inner" v-for="(job,index) in user.jobs.university" :key="job.jid">
              <md-menu class="actions">
                <md-button
                  class="md-icon-button md-dense"
                  md-menu-trigger
                  v-if="sameUser"
                >
                  <md-icon>
                    more_vert
                  </md-icon>
                </md-button>
                <md-menu-content>
                  <md-menu-item @click="openExpEdit('university',index)">Edit</md-menu-item>
                  <md-menu-item @click="deleteConfirm(job.jid,index,'university')">Delete</md-menu-item>
                </md-menu-content>
              </md-menu>
              <div class="name" :class="index%2 === 0 ? 'switched' : ''">
                <span class="date" v-if="job.start && job.start != '0'">{{ convertDate(job.start)}} <em v-if="job.end == -1">Current</em><span v-else-if="job.end != 0">- {{ convertDate(job.end) }}</span></span>
                <h4> {{ job.position }}, {{ job.title }} </h4>
                <p> at {{ job.location }} </p>
              </div>
              <div class="detail">
                <p v-if="job.description != null" v-html="job.description"></p>
                <p v-else-if="sameUser" style="color: #e2e2e2;"  @click="openExpEdit('university',index)">Click here to add description.</p>
                <p v-else style="color: #e2e2e2;">No description.</p>

              </div>
            </div>
          </div>
          <div v-else-if="sameUser" @click="addNewExp('university')" style="color: #999;font-size: 14px;">
            To add education please click here.
          </div>
          <!--End Timeline-->
        </div>
      </section>

      <!--@Todo: Implement Edit and Add New Skill and Hobby-->
      <!--Skills section -->
      <!--<section>-->
        <!--<h3 class="section-title">-->
          <!--Skills-->
          <!--<div class="actions">-->
            <!--<md-button-->
              <!--class="md-icon-button md-dense"-->
              <!--v-if="sameUser"-->
              <!--@click="addNewExp('skill')"-->
            <!--&gt;-->
              <!--<md-icon>-->
                <!--add-->
              <!--</md-icon>-->
            <!--</md-button>-->
          <!--</div>-->
        <!--</h3>-->
        <!--<div class="skill-info">-->
          <!--&lt;!&ndash;Timeline&ndash;&gt;-->
          <!--<div class="timeline">-->
            <!--<div class="listing-large-inner">-->
              <!--<div class="listing-event">-->
                <!--<div class="data left">-->
                  <!--<h5 class="skills-title">Professional Skills:</h5>-->
                  <!--<ul class="clearfix">-->
                    <!--<li v-for="skill in skills.professional">-->
                      <!--<h5>{{ skill.name }}</h5>-->
                      <!--<div class="rating">-->
                        <!--<span class="circle" :class="skill.rate >=1 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=2 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=3 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=4 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=5 ? 'active' : ''"></span>-->
                      <!--</div>-->
                    <!--</li>-->
                  <!--</ul>-->

                  <!--<h5 class="skills-title">General Skills:</h5>-->
                  <!--<ul class="clearfix">-->
                    <!--<li v-for="skill in skills.general">-->
                      <!--<h5>{{ skill.name }}</h5>-->
                      <!--<div class="rating">-->
                        <!--<span class="circle" :class="skill.rate >=1 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=2 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=3 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=4 ? 'active' : ''"></span>-->
                        <!--<span class="circle" :class="skill.rate >=5 ? 'active' : ''"></span>-->
                      <!--</div>-->
                    <!--</li>-->
                  <!--</ul>-->
                <!--</div>-->



                <!--<ul class="data right clearfix">-->
                  <!--<li>-->
                    <!--<h5>Assests</h5>-->
                    <!--<div-->
                      <!--v-if="user.about && editingEl != 'about'"-->
                      <!--class="description text-center"-->
                    <!--&gt;-->
                      <!--<p v-html="user.about" style="    white-space: pre-line;"></p>-->
                    <!--</div>-->

                    <!--<div-->
                      <!--v-if="sameUser && !user.about && editingEl != 'about'"-->
                      <!--class="description text-center"-->
                    <!--&gt;-->
                      <!--<p @click="onEditingFields('about')">Click here to write about yourself</p>-->
                    <!--</div>-->

                    <!--<div-->
                      <!--v-if="editingEl == 'about'"-->
                      <!--class="box-edit"-->
                    <!--&gt;-->
                      <!--<md-field>-->
                        <!--<md-textarea-->
                          <!--v-model="user.about"-->
                        <!--/>-->
                        <!--<div class="loading-spinner" v-if="updateField">-->
                          <!--<md-progress-spinner class="md-accent" :md-stroke="3" md-mode="indeterminate"></md-progress-spinner>-->
                        <!--</div>-->
                      <!--</md-field>-->
                      <!--<div class="md-layout md-alignment-center-space-between profile-edit-actions" style="    justify-content: space-between;">-->
                        <!--<div class="md-layout-item">-->
                          <!--<md-button-->
                            <!--:disabled="updateField"-->
                            <!--class="md-raised green"-->
                            <!--@click="completeEditingFields('about')"-->
                          <!--&gt;-->
                            <!--Save-->
                          <!--</md-button>-->
                        <!--</div>-->
                        <!--<div class="md-layout-item">-->
                          <!--<md-button-->
                            <!--:disabled="updateField"-->
                            <!--@click="cancelEditingFields('about')"-->
                          <!--&gt;-->
                            <!--Cancel-->
                          <!--</md-button>-->
                        <!--</div>-->
                      <!--</div>-->
                    <!--</div>-->
                    <!--&lt;!&ndash;<p class="emph-3">Responsible, Diligence, Labour, Rigor, Creative, Funny, Great Communicator, Flexible</p>&ndash;&gt;-->
                  <!--</li>-->


                  <!--<li v-if="skills.languages.length > 0">-->
                    <!--<h5>Languages</h5>-->
                    <!--<ul>-->
                      <!--<li-->
                        <!--v-for="(lang,index) in skills.languages"-->
                        <!--:key="index"-->
                        <!--class="emph-1"-->
                      <!--&gt;-->
                        <!--{{ lang.name }}-->
                        <!--<span class="emph-4">-->
                          <!--({{ languageLevels[lang.rate-1]}})-->
                        <!--</span>-->
                      <!--</li>-->
                    <!--</ul>-->
                  <!--</li>-->

                  <!--<li v-if="skills.interests.length > 0">-->
                    <!--<md-menu class="actions hobby">-->
                      <!--<md-button-->
                        <!--class="md-icon-button md-dense hobby"-->
                        <!--md-menu-trigger-->
                        <!--v-if="sameUser"-->
                      <!--&gt;-->
                        <!--<md-icon>-->
                          <!--more_vert-->
                        <!--</md-icon>-->
                      <!--</md-button>-->
                      <!--<md-menu-content>-->
                        <!--<md-menu-item @click="addNewExp('hobby')">Add</md-menu-item>-->
                        <!--<md-menu-item>Edit</md-menu-item>-->
                      <!--</md-menu-content>-->
                    <!--</md-menu>-->
                    <!--<h5>Hobbies &amp; Interests</h5>-->
                    <!--<ul class="hb-list">-->
                      <!--<li v-for="(hobby,index) in skills.interests" :key="index">-->
                        <!--<span><md-icon class="hob">{{ hobby.icon }}</md-icon></span>-->
                        <!--<h6>{{ hobby.name }}</h6>-->
                      <!--</li>-->

                    <!--</ul>-->

                  <!--</li>-->

                <!--</ul>-->
              <!--</div>-->
            <!--</div>-->
          <!--</div>-->
          <!--&lt;!&ndash;End Timeline&ndash;&gt;-->
        <!--</div>-->
      <!--</section>-->

      <!--@Todo: Implement Download Mechanism for CV-->
      <!--<md-button class="md-raised" @click="downloadWithCSS()">-->
        <!--Download-->
      <!--</md-button>-->
    </div>

  </div>
</template>

<script>
  // import jsPDF from "jspdf"
  import axios from "@/node_modules/axios"
  import addPost from "@/components/user/addPost"
  import { cookie } from "@/components/mixins/cookie.js";
  import { validationMixin } from 'vuelidate'
  import {
    required,
    email,
    minLength,
    maxLength
  } from 'vuelidate/lib/validators'
  export default {
    name: "User",
    layout:'userpanel',
    mixins: [cookie, validationMixin],
    scrollToTop: true,
    head(){
      return {
        title: this.user.full_name
      }
    },
    data() {
      return {
        testCanvas : null,
        uid: this.$route.params.uid,
        user: {},
        social: null,
        posts: [],
        brief: {},
        month: null,
        year: null,
        day: null,
        newItem: '',
        newSkill: {
          type: '',
          name: '',
          rate: 1
        },
        skills : {
          interests: [
            {
              name: 'Music',
              icon: 'music_note',
            },
            {
              name: 'Gaming',
              icon: 'games',
            },
          ],
          languages: [
            {
              name: 'English',
              rate: 3
            },
            {
              name: 'Persian',
              rate: 5
            }
          ],
          general: [
            {
              name: 'Microsoft Office',
              rate: 5
            },
            {
              name: 'Adobe Photoshop',
              rate: 3
            }
          ],
          professional: [
            {
              name: 'Web Development',
              rate: 5
            },
            {
              name: 'VueJS',
              rate: 4
            }
          ]
        },
        selectableInterests: [
          {
            name: 'Music',
            icon: 'music_note',
            selected : false
          },
          {
            name: 'Gaming',
            icon: 'games',
            selected: false
          },
          {
            name: 'Traveling',
            icon: 'flights',
            selected: false
          },
          {
            name: 'Reading',
            icon: 'book',
            selected: false
          },
          {
            name: 'Photography',
            icon: 'photo_camera',
            selected: false
          },
          {
            name: 'Sports',
            icon: 'rowing',
            selected: false
          },
          {
            name: 'Blogging',
            icon: 'create',
            selected: false
          },
          {
            name: 'Volunteer work',
            icon: 'group_work',
            selected: false
          },
          {
            name: 'Arts',
            icon: 'palette',
            selected: false
          },
          {
            name: 'Job related activities',
            icon: 'work_outline',
            selected: false
          }
        ],
        languageLevels: ['Beginner','Intermediate','Proficient','Fluent','Native'],
        showModal: false,
        phone_code: '',
        phone_code_2: '',
        countries: [],
        mnthArray: ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul","Aug", "Sep", "Oct", "Nov", "Dec"],
        sameUser: this.$route.params.uid == this.$store.getters.getUid ? true : false ,
        number_of_post:'',
        page: 0,
        count: 0,
        posts_copy: {},
        spinner_loading: true,
        user_copy:{},
        errors:'',
        showError:false,
        roles:[],
        loading: true,
        updateField: false,
        editingEl:'',
        deletingItem: false,
        updateFile: {
          picture:false,
          background_image:false,
          cv:false
        },
        file:{
          picture:{},
          background_image:{},
          cv:{}
        },
        afterUpload:{
          picture:{},
          background_image:{},
          cv:{}
        },
        exp: {
          title: null,
          position: null,
          location: null,
          start_month: null,
          start_year: null,
          end_month: null,
          end_year: null,
          start: null,
          end: null,
          untilNow: false,
          description: null,
        }
      }
    },
    validations: {
      exp: {
        title: {
          required,
        },
        location: {
          required
        },
      },
      // newSkill: {
      //   title: {
      //     required,
      //   },
      //   type: {
      //     required
      //   },
      // }
    },
    async asyncData({params, req, query, error}){
      let cookie = '';
      if ( req && req.headers && req.headers.cookie) {
        cookie = req.headers.cookie
      }
      axios.defaults.crossDomain = true;
      axios.defaults.withCredentials  = true;
      try {
        let {data} = await axios.get('https://ed808.com:92/latin/user/'+ params.uid,
          {
            headers: {
              'Content-type': 'application/json',
              'Cookie' : cookie
            }
          })
        let current_datetime = new Date(data.birthday*1000)
        return {
          user: data,
          user_copy: Object.assign({}, data),
          roles : Object.values(data.roles),
          loading: false,
          year: data.birthday != null ? current_datetime.getFullYear() : null,
          month: data.birthday != null ?  current_datetime.getMonth()+1: null,
          day: data.birthday != null ? current_datetime.getDate(): null
        }
      }
      catch (e) {
          error({ statusCode: e.response.status, message: e.response.statusText })
      }

    },
    computed: {
      timestampToDate: function(){
        let current_datetime = new Date(this.user.birthday*1000)
        return this.mnthArray[current_datetime.getMonth()] + ' ' + current_datetime.getDate() + ", " + current_datetime.getFullYear()
      }
    },
    mounted(){
      this.isSameUser()
      this.getCountries()
      /*
      * @todo: add getProfile here and showing it aside posts
      */
    },
    methods:{
      selectInterest(index) {
        this.interests[index] = !this.interests[index]
      },
      addHobbies(object) {
        if (object.selected == true) {

        }
      },
      download() {
        // if (process.browser) {
        //   const jsPDF = require('jspdf');
        //   require('html2canvas')
        //   // require('jspdf-autotable');
        //   const doc = new jsPDF();
        //   const contentHtml = this.$refs.content.innerHTML;
        //   doc.fromHTML(contentHtml, 15, 15, {
        //     width: 170
        //   });
        //   doc.save("sample.pdf");
        // }

      },

      // downloadWithCSS() {
      //   if (process.browser) {
      //     const jsPDF = require('jspdf');
      //     const html2canvas = require('html2canvas')
      //     const doc = new jsPDF();

      //     /** WITH CSS */
      //     let canvasElement = document.createElement('canvas');
      //     let test = null
      //     // console.log(canvasElement)
      //     // html2canvas(this.$refs.content, {
      //     //   canvas: canvasElement
      //     // }).then(function (canvas) {
      //     //   const img = canvas.toDataURL("image/png");
      //     //   console.log(canvas)
      //     //   document.getElementById('cv_canvas').appendChild(canvas)
      //     //   doc.addImage(img, 'JPEG', 20, 20);
      //     //   doc.save("sample.pdf");
      //     // });

      //     this.testCanvas = test
      //   }
      // },
      // Calculate Difference of two Timestamps and returns in month format
      getTimestampMonth(start, end) {
        if (end == '-1') {
          end = (new Date().getTime())/1000
        }
        return parseInt(((end - start)/2592000))
      },

      // Capitalize First Character of String
      firstCapitalize(string){
        return string.charAt(0).toUpperCase() + string.slice(1);
      },

      // Open Modal For adding Experience or Education
      addNewExp(type){
        this.editingEl = ''
        this.clearExpForm()
        if(this.showModal) {
          this.showModal = false
          this.newItem = ''
        }
        else
        {
          this.showModal = true
          this.newItem = type
        }
      },

      // Open Modal For Editing Experience or Education or Skill or Hobby
      openExpEdit(type, index){
        this.newItem = ''
        if(this.showModal) {
            this.showModal = false
            this.editingEl = ''
            this.clearExpForm()
        }else {
          if (type == 'university' || type == 'work' || type =='skill' || type=='hobby')
          {
            this.showModal = true;
            this.editingEl = type;
            this.exp = {
              title: this.user.jobs[type][index].title,
              position: this.user.jobs[type][index].position,
              location: this.user.jobs[type][index].location,
              start_month: (new Date(this.user.jobs[type][index].start * 1000)).getMonth() + 1,
              start_year: (new Date(this.user.jobs[type][index].start * 1000)).getFullYear(),
              end_month: this.user.jobs[type][index].end === '-1' ? null :(new Date(this.user.jobs[type][index].end * 1000)).getMonth() + 1,
              end_year: this.user.jobs[type][index].end === '-1' ? null : (new Date(this.user.jobs[type][index].end * 1000)).getFullYear(),
              untilNow: this.user.jobs[type][index].end === '-1',
              description: this.user.jobs[type][index].description,
              jid: this.user.jobs[type][index].jid,
            }
          }
        }
      },

      // Submit New or Edited Experience and Education
      submitExp(edit){
        this.$v.$touch()

        // run recaptcha and let it to fire up 'saveUser' function with google token
        if (!this.$v.$invalid) {
          this.updateField = true
          let end = ''
          let start = ''
          if(this.exp.untilNow){
            end = '-1'
          }else{
            if(this.exp.end_month != null && this.exp.end_year != null) {
              end = Math.floor(new Date(this.exp.end_year + '-' + this.exp.end_month + '-' + '01').getTime() / 1000)
            } else {
              end = '0'
            }
          }
          if(this.exp.start_month != null && this.exp.start_year != null) {
            console.log(this.exp.start_month + ' ' + this.exp.start_year)
            start = Math.floor(new Date(this.exp.start_year + '-' + this.exp.start_month + '-' + '01').getTime() / 1000)
          } else {
            start = '0'
          }
          let data = {}
          if (edit) {
            data = {
              "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "jobs" : {
                "type" : this.editingEl,
                "title" : this.exp.title,
                "position" : this.exp.position,
                "location" : this.exp.location,
                "jid": this.exp.jid,
                "start" : start,
                "end": end,
                "description" : this.exp.description
              }
            }
          } else {
            data = {
              "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "jobs" : {
                "type" : this.newItem,
                "title" : this.exp.title,
                "position" : this.exp.position,
                "location" : this.exp.location,
                "start" : start,
                "end": end,
                "description" : this.exp.description
              }
            }
          }
          axios.defaults.crossDomain = true;
          axios.defaults.withCredentials = true;
          axios
            .put(
              "https://ed808.com:92/latin/user/" + this.uid, data, {
                headers: {
                  "Content-Type": "application/json",
                  "X-CSRF-Token": this.getCookie("token"),
                  'Cache-Control': 'no-cache',
                }
              })
            .then((data) => {
              this.user = data.data
              this.addNewExp(this.newItem)
              this.clearExpForm()
              this.updateField = false
            })
            .catch(err => {
              console.log(err);
            });
        }
      },

      // Delete Confirmation Modal
      deleteConfirm(jid,index,type){
        this.deletingItem = {
          'jid': jid,
          'index': index,
          'type': type
        }
      },

      // Delete Experience or Education
      deleteExp(jid,index,type){
        axios.defaults.crossDomain = true;
        axios.defaults.withCredentials = true;
        axios
          .put(
            "https://ed808.com:92/latin/user/" + this.uid,
            {
              "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
              "jobs" : {
                "jid": jid,
                "delete": true
              }
            }
            , {
              headers: {
                "Content-Type": "application/json",
                "X-CSRF-Token": this.getCookie("token"),
                'Cache-Control': 'no-cache',
              }
            })
          .then(() => {
            this.user.jobs[type].splice(index, 1);
            this.updateField = false
          })
          .catch(err => {
            console.log(err);
          });
      },

      // Clear Experience and Education Form
      clearExpForm(){
        this.exp =
          {
            title: null,
            position: null,
            location: null,
            start_month: null,
            start_year: null,
            end_month: null,
            end_year: null,
            start: null,
            end: null,
            untilNow: false,
            description: null,
          }
      },

      // Getting Validation Classes for Inputs
      getValidationClass (fieldName,form=this.$v.exp) {
        const field = form[fieldName]

        if (field) {
          return {
            'md-invalid': field.$invalid && field.$dirty
          }
        }
      },

      // Convert Timestamp to Month and Year
      convertDate(timestamp) {
        let date = new Date(timestamp * 1000)
        const months = ["Jan", "Feb", "Mar","Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
        return months[date.getMonth()] + ' ' + date.getFullYear()
      },

      // Handling Files for Uploading
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

      // Save Files In DB
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
            this.completeEditingFields(fieldName)
          }
        })
          .catch((e) =>{
            console.log('FAILURE!!' + e)
          })
      },

      // Switch To Editing Mode For Bio, Headline and Personal Info
      onEditingFields(fieldName){
        this.editingEl = fieldName
        //stop loading for fields because after calling completeEditingFields for save file it get true
        this.updateField = false
      },

      // Exit Editing Mode For Bio, Headline and Personal Info and Discard Input Data
      cancelEditingFields(fieldName){
        if(fieldName == 'university'){
          this.user['university'] = this.user_copy['university']
          this.user['education_degree'] = this.user_copy['education_degree']
          this.user['education_field'] = this.user_copy['education_field']
        }
        else if(fieldName == 'personal'){
          this.user['full_name'] = this.user_copy['full_name']
          this.user['mail'] = this.user_copy['mail']
          this.user['mobile'] = this.user_copy['mobile']
          this.user['country'] = this.user_copy['country']
          this.user['birthday'] = this.user_copy['birthday']
        }
        else if(fieldName == 'headline'){
          this.user['job'] = this.user_copy['job']
        }
        else{
          this.user[fieldName] = this.user_copy[fieldName]
        }
        //switch from editing section to showing data
        this.editingEl = ''
      },

      // Submit And Save Data in DB
      completeEditingFields(fieldName){

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
            this.user = data.data
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

      // Handling Uploaded File and Update Old One
      afterFileSaved(fieldName,data){

        this.user[fieldName] = data.data[fieldName]

        //stop loading for files
        this.updateFile[fieldName] = false

        //update navbar with new picture
        if(fieldName == 'picture'){
          this.$store.commit('CHANGE_USER_DATA', data.data[fieldName], '')
        }

      },

      // Handling Updated Data
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
          this.user_copy['country'] = this.user['country']
          this.user_copy['birthday'] = this.user['birthday']

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

      // Create Form Data For Sending
      createData(fieldName){
        let data = {}
        if(fieldName == 'personal'){
          this.user.birthday = Math.floor(new Date(this.year + '-' + this.month + '-' + this.day).getTime() / 1000)
          data = {
            "hash": "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            "full_name": this.user['full_name'],
            "mail": this.user['mail'],
            "mobile": this.user['mobile'],
            "country": this.user['country'],
            "birthday": Math.floor(new Date(this.year + '-' + this.month + '-' + this.day).getTime() / 1000)
          }
        }
        else if((fieldName == 'picture') || (fieldName == 'background_image')){
          data = {
            "hash" : "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            [fieldName] : this.afterUpload[fieldName]
          }
        }else if(fieldName == 'headline') {
          data = {
            "hash": "50e185c2e0c2bc30215338db776022c92ecbc441fd933688c6bf4f274c863c60",
            "job": this.user['job'],
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

      // Follow Funtion
      follow(following, uid) {
        if(!this.$store.getters.getUid){
          console.log('not logged in')
          console.log(this.$store)
          this.$store.commit('TOGGLE_LOGIN')
        }else {
          axios.defaults.crossDomain = true;
          axios.defaults.withCredentials = true;
          axios
            .post(
              "https://ed808.com:92/latin/user/" + uid + "/follow",
              {
                action: following ? "unfollow" : "follow"
              },
              {
                headers: {
                  "Content-Type": "application/json",
                  "X-CSRF-Token": this.getCookie("token"),
                  'Cache-Control': 'no-cache',
                }
              }
            )
            .then(() => {
              console.log("sent");
              this.user.user_follow = !this.user_follow;
            })
            .catch(err => {
              console.log(err);
            });
        }
      },

      // To Check User who Opens Profile Page
      isSameUser(){
        if(this.$store.getters.getUid){
          if(this.$store.getters.getUid == this.uid)
            this.sameUser = true
        }
      },

      // get List of Countries with codes
      getCountries(){
        axios.get('/countries.json')
          .then((data)=>{
            this.countries = data.data
          })
      },

      // get Country name and DialCode
      getCountryByCode(code,wantDial=false) {
        for (var i = 0; i < this.countries.length; i++){
          // look for the entry with a matching `code` value
          if (this.countries[i].code == code){
            if(wantDial)
              return this.countries[i].dial_code
            else
              return this.countries[i].name
          }
        }
      },
    },
  }
</script>

<style lang="scss" scoped>
  @import '@/assets/scss/vars.scss';
  #canvas_handler {
    position:absolute;
    left: -10000px;
  }
  .profile-sections {
    section {
      width: 60%;
      margin: 100px auto;
      position: relative;
      .profile-edit-actions {
        margin-top: 15px;
        .md-button {
          margin: 0;
        }
      }
      @media #{$x1024} {
        width: 80%;
        margin: 80px auto;
      }
      @media #{$x600} {
        width: 100%;
        margin: 60px auto;
      }
      .actions {
        position: absolute;
        left: 0;
        @media #{$x600} {
          left: auto;
          right: 0;
        }
        .md-button {
          margin: 0;
        }
      }
      .description {
        p {
          line-height: 32px;
          @media #{$x600} {
            font-size: 12px;
          }
        }
        .md-button.edit {
          opacity: 0.5;
          display: none;
          position: absolute;
          left: 0;
          transition: 0.5s;
        }
        &:hover {
          .md-button.edit {
            display: block;
            &:hover {
              opacity: 1;
            }
          }
        }
      }
      .personal-info {
        position: relative;
        .md-layout.date-picker {
          justify-content: space-between;
          .md-layout-item {
            width: 31%;
            min-width: 31%;
            max-width: 31%;
          }
        }
        .listing {
          padding: 40px 0;
          position: relative;
          &:after {
              background-color: #eaeaea;
              content: " ";
              height: 100%;
              left: 50%;
              position: absolute;
              top: 0;
              width: 1px;
              z-index: 0;
          }
          .listing-event {
            overflow: hidden;
            .data.left {
                list-style-type: none;
                float: left;
                padding-right: 28px;
                position: relative;
                text-align: right;
                font-family: Roboto Slab,Times New Roman,serif;
                width: 50%;
                li {
                  line-height: 32px;
                  @media #{$x600} {
                    font-size: 12px;
                  }
                }
            }
            .data.right {
                list-style-type: none;
                color: #999;
                font-family: Roboto Slab,Times New Roman,serif;
                padding-left: 38px;
                float: right;
                position: relative;
                text-align: left;
                width: 50%;
                li {
                  line-height: 32px;
                  @media #{$x600} {
                    font-size: 12px;
                  }
                }
                .md-field {
                  width: 100%;
                  min-height: 27px;
                  margin: 0;
                  padding-top: 0;
                  display: flex;
                  position: relative;
                  font-family: inherit;
                }
            }
          }

        }
        .listing-inner:after {
            background: #673AB7;
            bottom: 0px;
            content: " ";
            height: 12px;
            left: calc(50% - 6px);
            position: absolute;
            width: 12px;
            z-index: 1;
            border-radius: 50%;
        }
        .listing-inner:before {
            background: #2a2a2a;
            top: 0px;
            content: " ";
            height: 12px;
            left: calc(50% - 6px);
            position: absolute;
            width: 12px;
            z-index: 1;
            border-radius: 50%;

        }
        .timeline {
          padding: 20px 0 20px 0;
          position: relative;
          overflow: hidden;
        }


        .timeline:after {
          background-color: #eaeaea;
          content: " ";
          height: 100%;
          left: 50%;
          position: absolute;
          top: 0;
          width: 1px;
          z-index: 0;
        }


        .timeline .timeline-inner {
          margin-bottom: 40px;
          position: relative;
          z-index: 2;
          display: flex;
          flex-wrap: wrap;
        }


        .timeline .timeline-inner:last-child {
          margin-bottom: 0;
        }

        .timeline .timeline-inner .name {
          font-size: 18px;
          padding-right: 28px;
          position: relative;
          text-align: right;
          width: 50%;
          font-family: 'roboto slab';
          @media #{$x600} {
            font-size: 14px;
            width: 100%;
            padding-left: 28px;
            padding-right: 0;
            text-align: left;
          }
          p {
            margin-top: 0;
            font-size: 12px;
          }
        }

        .timeline .timeline-inner .name:before {
          background: #673AB7;
          border: 4px solid #fff;
          border-radius: 50%;
          content: " ";
          height: 20px;
          position: absolute;
          right: -10px;
          top: 1px;
          width: 20px;
          -webkit-background-clip: padding-box;
          -moz-background-clip:padding;
          background-clip:padding-box;
          @media #{$x600} {
            right: auto;
            left: 0;
            top: 4px;
          }
        }

        .timeline .timeline-inner .name.switched:before{
          border-radius: 50%;
          background: #2a2a2a;
          -webkit-background-clip: padding-box;
          -moz-background-clip:padding;
          background-clip:padding-box;

        }

        .timeline .timeline-inner .detail {
          padding-left: 28px;
          position: relative;
          text-align:left;
          width:50%;
          @media #{$x600} {
            width: 100%;
          }
          p {
            margin-top: 0;
            min-height: 100px;
            @media #{$x600} {
              font-size: 13px;
            }
          }

        }

        .timeline .timeline-inner .name h4 {
          margin-bottom: 0;
          font-size: 15px;
          font-weight: 700;
        }


        .timeline .timeline-inner .name span.date {
          color:#999999;
          font-size: 13.5px;
          font-weight: 400;
        }

        .timeline .timeline-inner .name span.date em {
          background:#e2e2e2;
          border-radius: 3px;
          -moz-border-radius:3px;
          -webkit-border-radius :3px;
          -ms-border-radius:3px;
          -o-border-radius: 3px;
          color: #673AB7;
          font-size: 12.5px;
          font-weight: 400;
          margin-left: 10px;
          padding: 2px 7px;
          position: relative;
          top: -1px;
          position:relative;
        }


        .timeline .timeline-inner .name span.date em:before {
          border-color: transparent #e2e2e2;
          border-style: solid;
          border-width: 6px 5px 6px 0;
          content: "";
          display: block;
          left: -4px;
          position: absolute;
          top: 4px;
          width: 0;
          z-index: 0;
        }

        .listing-large {
          padding: 60px 0 40px 0;
          position: relative;
        }


        .listing-large:after {
          background-color: #eaeaea;
          content: " ";
          height: 100%;
          left: 49.9%;
          position: absolute;
          top: 0;
          width: 1px;
          z-index: 0;
        }

        .listing-large .listing-large-inner {
          margin-bottom: 40px;
          position: relative;
          z-index: 2;
        }

        .listing-large-inner:before {
          background: #2a2a2a;
          top: -80px;
          content: " ";
          height: 12px;
          left: 49.1%;
          position: absolute;
          width: 12px;
          z-index: 0;
        }

        .listing-large-inner:after {
          background: #ffd42d;
          bottom: -100px;
          content: " ";
          height: 12px;
          left: 49.1%;
          position: absolute;
          width: 12px;
          z-index: 0;
        }

        .listing-large .listing-event .data.left {
          float: left;
          font-size: 18px;
          font-weight: 300;
          padding-right: 5px;
          position: relative;
          text-align: right;
          width: 46%;
        }

        .listing-large .listing-event .data.right {
          margin-left: 41%;
          padding-left: 92px;
          position: relative;
          text-align:left;
          width:57%;
        }

        .listing-large .listing-event .data.right li {
          margin-bottom: 30px;
        }

        .listing-large .listing-event .data.right li p{
          font-size:14.5px;
        }


        .listing-large .listing-event .data.right li:last-child {
          margin-bottom: 0;
        }

        .listing-large .listing-event .data.right li ul li {
          margin-bottom: 0;
        }



        .listing-large .listing-event .data.right li:nth-child(2) ul li {
          font-weight: 400;
          font-size:15px;
        }


        .listing-large .listing-event .data.right li:nth-child(2) ul li span {
          font-weight: 300;
          font-size:14px;
          margin-left:2px;
        }

        .listing-large .listing-event .data.right h5 {
          font-size: 15px;
          font-weight: 700;
          margin-bottom:20px;
        }



        ul.hb-list li {
          display: inline-block;
          margin-right: 10px;
          text-align: center;
        }


        ul.hb-list li span {
          border: 1px solid #ffd42d;
          color: #ffd42d;
          display: block;
          height: 45px;
          width: 45px;
        }


        ul.hb-list li span i{
          font-size: 16px;
          line-height: 42px;
          display:block;
        }


        ul.hb-list li h6 {
          color: #999999;
          margin-top: 10px;
          font-style:italic;
          font-size:14.5px;
        }


        .listing-large .listing-event .data.left li {
          clear:both;
          line-height: 28px;
        }


        .listing-large .listing-event .data.left h5 {
          display: block;
          float: right;
          margin-left:15px;
          font-size: 15px;
          font-weight: 400;

        }


        .rating {
          float: right;
        }

        .rating span {
          background: #ffd42d ;
          display: inline-block;
          margin: 0 3px;
          height: 6px;
          width: 6px;
          position: relative;
          top: -3px;
        }
        .rating span.transparent {
          background:#eaeaea ;
        }


        .listing-large  a {
          font-size: 14px;
          font-weight: 700;
          position: absolute;
          bottom: -25px;
        }


        .listing-large  a:hover {
          color:#ffd42d;
        }

        .listing-large  a.btn-1 {
          left:26.3%;
        }


        .listing-large  a.btn-2 {
          right:26.3%;
        }

        .services{
          position:relative;
          padding: 60px 40px;
        }

        .services:before {
          background: #2a2a2a;
          top: 40px;
          content: " ";
          height: 12px;
          left: 49.1%;
          position: absolute;
          width: 12px;
          z-index: 0;
        }

        .services:after {
          background: #ffd42d;
          bottom: 40px;
          content: " ";
          height: 12px;
          left: 49.1%;
          position: absolute;
          width: 12px;
          z-index: 0;
        }

        .services .block-service  .col-sm-6:nth-child(-n+2) {
          border-bottom: 1px solid #eaeaea;
        }


        .services .block-service  .col-sm-6:nth-child(2n+1) {
          border-right: 1px solid #eaeaea;
        }


        .services .block-service  .col-sm-6:nth-child(-n+3) {
          border-bottom: 1px solid #eaeaea;
        }
        .services .block-service  .col-sm-6:nth-child(3n+1) {
          border-bottom: 1px solid #eaeaea;
        }

        .services .block-service .col-sm-6 {
          padding: 50px 0;
        }

        .block-service .ico {
          color: #ffd42d;
          font-size: 30px;
        }

        .block-service .det h5 {
          font-size: 15px;
          font-weight: 700;
        }

        .block-service .det p {
          padding:0  40px;
        }

        .block-quote {
          background:#f2f3f4;
          padding:50px;
          position:relative;
          color:#777777;
          font-size: 15px;
        }

        .block-quote:after {
          content: "";
          border-top: 15px solid #f2f3f4;
          border-left: 15px solid rgba(0, 0, 0, 0);
          border-right: 15px solid rgba(0, 0, 0, 0);
          bottom: -12px;
          display: inline-block;
          left: 50%;
          position: absolute;
          transform: translate3d(-50%, 0px, 0px);
          -ms-transform: translate3d(-50%, 0px, 0px);
          -webkit-transform: translate3d(-50%, 0px, 0px);
          -ms-transform:translate(-50%, 0px);
          -o-transform:translate(-50%, 0px);
          z-index: 2;
        }

        .block-quote .block-profile {
          font-size: 16px;
          font-weight: 700;
        }


        .block-quote .block-profile span {
          color: #777777;
          font-size: 13.5px;
          font-weight: 400;
          display:block;
          font-style:italic;
        }

        .block-img img {
          margin-bottom: 15px;
          margin-top: 50px;
          max-height: 100px;
          text-align: center;
        }
      }
      h3.section-title {
        position: relative;
        font-size: 1.3rem;
        font-family: Roboto Slab,Times New Roman,serif;
        margin-bottom: 50px;
        @media #{$x600} {
          font-size: 1.1rem;
        }
        &:after {
          content:'';
          display:block;
          margin: 10px auto;
          width: 20px;
          border-bottom: 4px solid #673AB7;
        }
      }
      .md-layout.md-alignment-center-space-between .md-layout-item {
        flex: 0;
      }
      .md-field {
        width: 100%;
        min-height: 48px;
        margin: 4px 0 4px;
        padding-top: 16px;
        display: flex;
        position: relative;
        font-family: inherit;
      }
    }
  }
  .profile-brief{
    margin-top: -67px ;
  }
  h3.title{
    margin-top: 30px;
    margin-bottom: 25px;
    min-height: 32px;
    font-weight: 700;
    font-family: Roboto Slab,Times New Roman,serif;
  }
  h6 {
    margin-top: 10px;
    margin-bottom: 10px;
  }
  // .description {
  //   margin: 15px auto 0 !important;
  //   max-width: 80%;
  // }
  .h6, h6 {
    font-size: .75rem;
    text-transform: uppercase;
    font-weight: 500;
  }
  .h3, h3 {
    font-size: 1.5625rem;
    line-height: 1.4em;
    @media #{$x600} {
      font-size: 1.3rem;
    }
  }
  .md-card {
    max-width: 800px;
    text-align: left;
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
    -webkit-box-orient: vertical;
    -webkit-box-direction: normal;
    -ms-flex-direction: column;
    flex-direction: column;
    font-size: .875rem;
    position: relative;
    width: 100%;
    margin: 30px 0;
    overflow: unset;
    -webkit-box-shadow: 0 2px 2px 0 rgba(0,0,0,.14),0 3px 1px -2px rgba(0,0,0,.2),0 1px 5px 0 rgba(0,0,0,.12);
    box-shadow: 0 2px 2px 0 rgba(0,0,0,.14),0 3px 1px -2px rgba(0,0,0,.2),0 1px 5px 0 rgba(0,0,0,.12);
    border-radius: 6px;
    color: rgba(0,0,0,.87);
    background: #fff
  }
  .text-info {
    color: #00bcd4!important;
  }
  .posts{
    min-height: 100px;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
  }
  .card-title, .card-title a, .card-title a:focus, .card-title a:hover, .comment .comment-heading, .comment .comment-heading a, .comment .comment-heading a:focus, .comment .comment-heading a:hover, .footer-big h4, .footer-big h4 a, .footer-big h4 a:focus, .footer-big h4 a:hover, .footer-big h5, .footer-big h5 a, .footer-big h5 a:focus, .footer-big h5 a:hover, .footer-brand, .footer-brand a, .footer-brand a:focus, .footer-brand a:hover, .info-title, .info-title a, .info-title a:focus, .info-title a:hover, .title, .title a, .title a:focus, .title a:hover {
    color: #3c4858!important;
    text-decoration: none;
  }
  .card-title, .comment .comment-heading, .footer-big h4, .footer-big h5, .footer-brand, .info-title, .title {
    font-family: Roboto Slab,Times New Roman,serif;
  }
  h4{
    font-size: 18px;
    margin:10px 0;
    font-weight: normal;
  }
  h6 {
    /*font-size: .75rem;*/
    font-size: 12px;
    text-transform: uppercase;
    font-weight: 500;
    margin:10px 0;
  }
  .card-description, .description, .footer-big p {
    color: #999;
    font-size: 14px;
    margin: 0 0 10px;
  }
  .md-card.row-space .header {
    padding: 15px 20px 0
  }

  .md-card .card-title {
    margin-top: .625rem
  }

  .md-card .card-category-social .fa,.md-card .card-category-social .fab,.md-card .card-category-social .far,.md-card .card-category-social .fas,.md-card .card-category-social .md-icon {
    font-size: 24px;
    position: relative;
    margin-top: -4px;
    top: 2px;
    margin-right: 5px
  }

  .md-card .card-category-social .md-icon {
    margin-top: -7px;
    color: inherit
  }

  .md-card .card-title:last-child {
    margin-bottom: 0
  }

  .md-card[class*=bg-] .author a,.md-card[class*=bg-] .card-category:not([class*=text-]),.md-card[class*=bg-] .card-description,.md-card[class*=bg-] .card-title small,.md-card[class*=bg-] .stats,.md-card[class*=bg-] .stats .md-icon:not(.md-icon-button),.md-card [class*=md-card-header-] .author a,.md-card [class*=md-card-header-] .card-category,.md-card [class*=md-card-header-] .card-description,.md-card [class*=md-card-header-] .stats {
    color: hsla(0,0%,100%,.8)!important
  }

  .md-card[class*=bg-] .card-title,.md-card[class*=bg-] .card-title a,.md-card[class*=bg-] .icon:not([class*=icon-]) i {
    color: #fff!important
  }

  .md-card[class*=bg-] .icon:not([class*=icon-]) i,.md-card [class*=md-card-header-] .icon i {
    border-color: hsla(0,0%,100%,.25)!important
  }

  .md-card.md-card-form-horizontal .md-field {
    margin: 0;
    padding-top: 0
  }

  .md-card.md-card-form-horizontal .md-card-content {
    padding-left: 15px;
    padding-right: 15px
  }

  .md-card.md-card-form-horizontal .md-button {
    margin: 0
  }

  .md-card .title {
    margin-top: 0;
    margin-bottom: 5px
  }

  .md-card .card-image {
    height: 60%;
    position: relative;
    overflow: hidden;
    margin-left: 15px;
    margin-right: 15px;
    margin-top: -30px;
    border-radius: 6px
  }

  .md-card .card-image img {
    width: 100%;
    height: 200px;
    border-radius: 6px;
    pointer-events: none
  }

  .md-card .card-image .card-title {
    position: absolute;
    bottom: 15px;
    left: 15px;
    color: #fff;
    font-size: 1.125rem;
    text-shadow: 0 2px 5px rgba(33,33,33,.5)
  }

  .md-card .category:not([class*=text-]) {
    color: #999
  }

  .md-card .md-card-content {
    padding: .9375rem 1.875rem;
    line-height: inherit
  }

  .md-card .md-card-content .category {
    margin: 0
  }

  .md-card .md-card-header {
    margin: -40px 20px 0;
    border-radius: 3px;
    padding: 0;
    background-color: #999
  }

  .md-card .md-card-header .card-title,.md-card .md-card-header .title {
    color: #fff!important
  }

  .md-card .md-card-header .category {
    margin: 0;
    color: hsla(0,0%,100%,.62)
  }

  .md-card .md-card-header a {
    color: #fff!important
  }

  .md-card .md-card-header.card-chart {
    padding: 0;
    min-height: 160px
  }

  .md-card .md-card-header.card-chart+.content h4 {
    margin-top: 0
  }

  .md-card .md-card-header .ct-label {
    color: hsla(0,0%,100%,.7)
  }

  .md-card .md-card-header .ct-grid {
    stroke: hsla(0,0%,100%,.2)
  }

  .md-card .md-card-header .ct-series-a .ct-bar,.md-card .md-card-header .ct-series-a .ct-line,.md-card .md-card-header .ct-series-a .ct-point,.md-card .md-card-header .ct-series-a .ct-slice-donut {
    stroke: hsla(0,0%,100%,.8)
  }

  .md-card .md-card-header .ct-series-a .ct-area,.md-card .md-card-header .ct-series-a .ct-slice-pie {
    fill: hsla(0,0%,100%,.4)
  }

  .md-card .md-card-header-image {
    position: relative;
    padding: 0;
    z-index: 1;
    margin-left: 15px;
    margin-right: 15px;
    margin-top: -30px;
    border-radius: 6px
  }

  .md-card .md-card-header-image a {
    display: block
  }

  .md-card .md-card-header-image img {
    width: 100%;
    border-radius: 6px;
    pointer-events: none
  }

  .md-card .md-card-header-image:not([shadow-off=true]) img {
    -webkit-box-shadow: 0 5px 15px -8px rgba(0,0,0,.24),0 8px 10px -5px rgba(0,0,0,.2);
    box-shadow: 0 5px 15px -8px rgba(0,0,0,.24),0 8px 10px -5px rgba(0,0,0,.2)
  }

  .md-card .md-card-header-image .card-title {
    position: absolute;
    bottom: 15px;
    left: 15px;
    color: #fff!important;
    font-size: 1.125rem;
    text-shadow: 0 2px 5px rgba(33,33,33,.5)
  }

  .md-card .md-card-header-image .colored-shadow {
    -webkit-transform: scale(.94);
    transform: scale(.94);
    top: 12px;
    -webkit-filter: blur(12px);
    filter: blur(12px);
    position: absolute;
    width: 100%;
    height: 100%;
    background-size: cover;
    z-index: -1;
    -webkit-transition: opacity .45s;
    transition: opacity .45s;
    opacity: 0
  }

  .md-card .md-card-header-image[no-colored-shadow=true] {
    -webkit-box-shadow: none;
    box-shadow: none
  }

  .md-card .md-card-header-image[no-colored-shadow=true] .colored-shadow {
    display: none!important
  }

  .md-card .md-card-header-image:not([shadow-off=true])[shadow-normal=true] {
    -webkit-box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2);
    box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2)
  }

  .md-card .md-card-header-image:not([shadow-off=true])[shadow-normal=true] .colored-shadow {
    display: none!important
  }

  .md-card .chart-title {
    position: absolute;
    top: 25px;
    width: 100%;
    text-align: center
  }

  .md-card .chart-title h3 {
    margin: 0;
    color: #fff
  }

  .md-card .chart-title h6 {
    margin: 0;
    color: hsla(0,0%,100%,.4)
  }

  .md-card .md-card-actions {
    padding: 0 1.875rem .9375rem
  }

  .md-card .md-card-actions .content {
    display: block
  }

  .md-card .md-card-actions .author {
    color: #999
  }

  .md-card .md-card-actions .stats {
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
    -webkit-box-align: center;
    -ms-flex-align: center;
    align-items: center;
    color: #999;
    margin-right: 3px;
    margin-left: 3px
  }

  .md-card .md-card-actions .stats .md-icon.md-theme-default.md-icon-font {
    position: relative;
    top: -1px;
    font-size: 18px!important;
    color: #999
  }

  .md-card .md-card-actions h6 {
    color: #999
  }

  .md-card img {
    width: 100%;
    height: auto
  }

  .md-card .category .md-icon {
    position: relative;
    top: 6px;
    line-height: 0
  }

  .md-card .category-social .fa {
    font-size: 24px;
    position: relative;
    margin-top: -4px;
    top: 2px;
    margin-right: 5px
  }

  .md-card .author .avatar {
    width: 30px;
    height: 30px;
    overflow: hidden;
    border-radius: 50%;
    margin-right: 5px
  }

  .md-card .author a {
    color: #3c4858;
    text-decoration: none
  }

  .md-card .author a .ripple-container {
    display: none
  }

  .md-card .table {
    margin-bottom: 0
  }

  .md-card .table tr:first-child td {
    border-top: none
  }

  .md-card.bg-primary,.md-card .md-card-header-primary,.md-card.md-card-rotate.bg-primary .back,.md-card.md-card-rotate.bg-primary .front,.md-card .md-tabs.md-primary .md-tabs-navigation,.md-card [data-background-color=purple] {
    background: linear-gradient(60deg,#ab47bc,#7b1fa2);
    -webkit-box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2);
    box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2);
    background-repeat: no-repeat
  }

  .md-card.bg-info,.md-card .md-card-header-info,.md-card.md-card-rotate.bg-info .back,.md-card.md-card-rotate.bg-info .front,.md-card .md-tabs.md-info .md-tabs-navigation,.md-card [data-background-color=blue] {
    background: linear-gradient(60deg,#26c6da,#0097a7);
    -webkit-box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2);
    box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2);
    background-repeat: no-repeat
  }

  .md-card.bg-success,.md-card .md-card-header-green,.md-card.md-card-rotate.bg-success .back,.md-card.md-card-rotate.bg-success .front,.md-card .md-tabs.md-success .md-tabs-navigation,.md-card [data-background-color=green] {
    background: linear-gradient(60deg,#66bb6a,#388e3c);
    -webkit-box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2);
    box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2);
    background-repeat: no-repeat
  }

  .md-card.bg-warning,.md-card .md-card-header-orange,.md-card.md-card-rotate.bg-warning .back,.md-card.md-card-rotate.bg-warning .front,.md-card .md-tabs.md-warning .md-tabs-navigation,.md-card [data-background-color=orange] {
    background: linear-gradient(60deg,#ffa726,#f57c00);
    -webkit-box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2);
    box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2);
    background-repeat: no-repeat
  }

  .md-card.bg-danger,.md-card .md-card-header-red,.md-card.md-card-rotate.bg-danger .back,.md-card.md-card-rotate.bg-danger .front,.md-card .md-tabs.md-danger .md-tabs-navigation,.md-card [data-background-color=red] {
    background: linear-gradient(60deg,#ef5350,#d32f2f);
    -webkit-box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2);
    box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2);
    background-repeat: no-repeat
  }

  .md-card.bg-rose,.md-card .md-card-header-rose,.md-card.md-card-rotate.bg-rose .back,.md-card.md-card-rotate.bg-rose .front,.md-card .md-tabs.md-rose .md-tabs-navigation,.md-card [data-background-color=rose] {
    background: linear-gradient(60deg,#ec407a,#c2185b);
    -webkit-box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2);
    box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2);
    background-repeat: no-repeat
  }

  .md-card [data-background-color],.md-card [data-background-color] a {
    color: #fff
  }

  .md-card-stats .title {
    margin: 0
  }

  .md-card-stats .title small {
    color: #999;
    font-size: 65%;
    line-height: 1;
    font-weight: 400
  }

  .md-card-stats .md-card-header {
    float: left;
    text-align: center
  }

  .md-card-stats .md-card-header i {
    font-size: 36px!important;
    line-height: 56px;
    width: 56px;
    height: 56px;
    color: #fff!important
  }

  .md-card-stats .md-card-content {
    text-align: right;
    padding-top: 10px
  }

  .md-card-blog {
    margin-top: 45px;
  }

  .card-nav-tabs .header-raised {
    margin-top: -30px;
  }

  .card-nav-tabs .nav-tabs {
    background: transparent;
    padding: 0
  }

  .card-nav-tabs .nav-tabs-title {
    float: left;
    padding: 10px 10px 10px 0;
    line-height: 24px
  }

  .md-card-plain {
    background: transparent!important;
    -webkit-box-shadow: none!important;
    box-shadow: none!important
  }

  .md-card-plain .md-card-header {
    background-color: transparent!important;
    margin-left: 0;
    margin-right: 0
  }

  .md-card-plain .md-card-actions,.md-card-plain .md-card-content {
    padding-left: 5px;
    padding-right: 5px
  }

  .md-card-plain .card-image {
    margin: 0;
    border-radius: 3px
  }

  .md-card-plain .card-image img {
    border-radius: 3px
  }

  .md-card-horizontal .md-card-header-image {
    margin-top: 0
  }

  .md-card-profile,.md-card-testimonial {
    margin-top: 30px
  }

  .md-card-profile:not(.md-card-horizontal),.md-card-testimonial:not(.md-card-horizontal) {
    text-align: center
  }

  .md-card-profile .btn-just-icon.btn-raised,.md-card-testimonial .btn-just-icon.btn-raised {
    margin-left: 6px;
    margin-right: 6px
  }

  .md-card-profile .md-card-avatar,.md-card-testimonial .md-card-avatar {
    max-width: 130px;
    max-height: 130px;
    margin: -50px auto 0;
    border-radius: 50%;
    overflow: hidden;
    -webkit-box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2);
    box-shadow: 0 16px 38px -12px rgba(0,0,0,.56),0 4px 25px 0 rgba(0,0,0,.12),0 8px 10px -5px rgba(0,0,0,.2)
  }

  .md-card-profile .md-card-avatar+.md-card-content,.md-card-testimonial .md-card-avatar+.md-card-content {
    margin-top: 15px
  }

  .md-card-profile.flex-direction-row .md-card-avatar,.md-card-profile.flex-direction-row .md-card-content,.md-card-profile.md-card-plain .card-avatar,.md-card-testimonial.flex-direction-row .md-card-avatar,.md-card-testimonial.flex-direction-row .md-card-content,.md-card-testimonial.md-card-plain .card-avatar {
    margin-top: 0
  }

  .md-card-profile.flex-direction-row .md-card-content,.md-card-testimonial.flex-direction-row .md-card-content {
    text-align: left;
    margin-left: 15px
  }

  .md-card-testimonial .md-card-header {
    background-color: #fff;
    margin-top: 0
  }

  .md-card-testimonial .card-description {
    font-style: italic;
    margin-bottom: 35px
  }

  .md-card-testimonial .md-card-actions {
    display: block
  }

  .md-card-testimonial .md-card-actions .md-card-avatar {
    margin-top: 10px;
    margin-bottom: -60px
  }

  .md-card-testimonial .md-card-avatar {
    max-width: 100px;
    max-height: 100px
  }

  .md-card-testimonial .icon {
    margin-top: 30px
  }

  .md-card-testimonial .icon.icon-black i {
    color: rgba(0,0,0,.87)
  }

  .md-card-testimonial .icon .md-icon {
    font-size: 40px!important
  }

  .md-card-contact .md-card-header {
    padding: 1rem 15px
  }

  .md-card-tabs {
    margin-top: 0
  }

  .md-card-tabs .md-list {
    -webkit-box-orient: vertical;
    -webkit-box-direction: normal;
    -ms-flex-flow: initial;
    flex-flow: initial;
    padding: 0 15px
  }

  .md-card-tabs.page-subcategories>.md-card-header .title {
    margin-top: 30px!important;
    margin-bottom: 25px!important;
    min-height: 32px
  }

  .md-card-tabs .tab-content {
    padding: 20px 0 9px
  }

  .md-card-tabs .md-list-item+.md-list-item {
    margin-left: 5px
  }

  .md-card-tabs .md-list .md-list-item .md-list-item-button {
    line-height: 24px;
    text-transform: uppercase;
    font-size: 12px;
    font-weight: 500;
    min-width: 100px;
    color: #3c4858!important;
    -webkit-transition: all .3s;
    transition: all .3s;
    border-radius: 30px;
    padding: 0
  }

  .md-card-tabs .md-list .md-list-item .md-list-item-button .md-icon {
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex;
    width: auto;
    height: auto;
    margin: 0;
    font-size: 30px!important;
    padding: 15px 0;
    color: #555
  }

  .md-card-tabs .md-list .md-list-item .md-list-item-button .md-list-item-content {
    min-height: auto;
    padding: 10px 15px;
    -webkit-box-pack: center;
    -ms-flex-pack: center;
    justify-content: center;
    display: -webkit-box;
    display: -ms-flexbox;
    display: flex
  }

  .md-card-tabs .md-list .md-list-item .md-list-item-button:focus,.md-card-tabs .md-list .md-list-item .md-list-item-button:hover {
    background-color: hsla(0,0%,78%,.2)!important
  }

  .md-card-tabs .md-list .md-list-item:not(.active) .md-list-item-button .md-list-item-content {
    color: #555
  }

  .md-card-tabs .active.md-list-item .md-list-item-button {
    color: #fff!important
  }

  .md-card-tabs .active.md-list-item .md-list-item-button,.md-card-tabs .active.md-list-item .md-list-item-button:focus,.md-card-tabs .active.md-list-item .md-list-item-button:hover {
    background-color: #999!important;
    -webkit-box-shadow: 0 12px 20px -10px hsla(0,0%,60%,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px hsla(0,0%,60%,.2);
    box-shadow: 0 12px 20px -10px hsla(0,0%,60%,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px hsla(0,0%,60%,.2)
  }

  .md-card-tabs .active.md-list-item .md-list-item-button,.md-card-tabs .active.md-list-item .md-list-item-button .md-list-item-content .md-icon {
    color: #fff!important
  }

  .md-card-tabs .active.md-list-item.md-rose .md-list-item-button,.md-card-tabs .active.md-list-item.md-rose .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-rose .md-list-item-button:hover {
    background-color: #e91e63!important;
    -webkit-box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2);
    box-shadow: 0 12px 20px -10px rgba(233,30,99,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(233,30,99,.2)
  }

  .md-card-tabs .active.md-list-item.md-primary .md-list-item-button,.md-card-tabs .active.md-list-item.md-primary .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-primary .md-list-item-button:hover {
    background-color: #9c27b0!important;
    -webkit-box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2);
    box-shadow: 0 12px 20px -10px rgba(156,39,176,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(156,39,176,.2)
  }

  .md-card-tabs .active.md-list-item.md-warning .md-list-item-button,.md-card-tabs .active.md-list-item.md-warning .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-warning .md-list-item-button:hover {
    background-color: #ff9800!important;
    -webkit-box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2);
    box-shadow: 0 12px 20px -10px rgba(255,152,0,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(255,152,0,.2)
  }

  .md-card-tabs .active.md-list-item.md-info .md-list-item-button,.md-card-tabs .active.md-list-item.md-info .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-info .md-list-item-button:hover {
    background-color: #00bcd4!important;
    -webkit-box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2);
    box-shadow: 0 12px 20px -10px rgba(0,188,212,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(0,188,212,.2)
  }

  .md-card-tabs .active.md-list-item.md-success .md-list-item-button,.md-card-tabs .active.md-list-item.md-success .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-success .md-list-item-button:hover {
    background-color: #4caf50!important;
    -webkit-box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2);
    box-shadow: 0 12px 20px -10px rgba(76,175,80,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(76,175,80,.2)
  }

  .md-card-tabs .active.md-list-item.md-danger .md-list-item-button,.md-card-tabs .active.md-list-item.md-danger .md-list-item-button:focus,.md-card-tabs .active.md-list-item.md-danger .md-list-item-button:hover {
    background-color: #f44336!important;
    -webkit-box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2);
    box-shadow: 0 12px 20px -10px rgba(244,67,54,.28),0 4px 20px 0 rgba(0,0,0,.12),0 7px 8px -5px rgba(244,67,54,.2)
  }

  .md-card-tabs.md-card-plain .md-card-header,.md-card-tabs.md-card-plain .md-list {
    background-color: transparent!important
  }

  .md-card-tabs.md-card-plain .title {
    -webkit-box-pack: center;
    -ms-flex-pack: center;
    justify-content: center
  }

  button {
    transition: 0.5s;
    &.md-follow {
      background-color: transparent;
      color: #2196F3;
      border: 1px solid #2196F3;
      padding: 10px;
      border-radius: 30px;
      margin: 0;
      min-width: 120px;
      position: relative;
      top: -3px;
      &.active {
        background-color: #2196F3;
        color: white;
      }
    }
    &:hover {
      opacity: 0.7;
      cursor: pointer;
    }
  }

  // full profile css's:
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
  .full-profile {
    max-width: 1130px;
    margin: 50px auto;
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
  .md-dialog {
    &.md-scrollbar {
      overflow-y: scroll;
    }
    .new-experience {
      padding: 24px;
      padding-top: 0;
    }
    .footer {
      text-align: center  ;
    }
    .hobby {
      max-width: 334px;
    }
  }
  .skill-level {
    min-height: 48px;
    padding-top: 16px;
    margin: 4px 0 24px;
    *{
      margin: 0;
      padding: 0;
    }
    .rate {
      float: right;
      height: 25px;
      padding: 0 10px;
      label {
        margin: 5px;
      }
    }
    .rate:not(:checked) > input {
      position:absolute;
      top:-9999px;
    }
    .rate:not(:checked) > label {
      width: 12px;
      height: 12px;
      border-radius: 50%;
      float:right;
      overflow:hidden;
      white-space:nowrap;
      cursor:pointer;
      font-size:30px;
      background-color: #eaeaea !important;
    }
    /*.rate:not(:checked) > label:before {*/
      /*content: '';*/

    /*}*/
    .rate > input:checked ~ label {
      background-color: #673AB7 !important;
    }
    .rate:not(:checked) > label:hover,
    .rate:not(:checked) > label:hover ~ label {
      background-color: #673AB7 !important;
    }
    .rate > input:checked + label:hover,
    .rate > input:checked + label:hover ~ label,
    .rate > input:checked ~ label:hover,
    .rate > input:checked ~ label:hover ~ label,
    .rate > label:hover ~ input:checked ~ label {
      background-color: #512e91 !important;
    }

    /* Modified from: https://github.com/mukulkant/Star-rating-using-pure-css */
  }
  .level {
    float: left;
  }
  .data.left.clearfix.rate {
    display: unset;
  }
  .hb-list.hobby {
    display: block;
    margin: 10px 0;
    max-width: 275px;
  }
  ul.hb-list.hobby li {
    margin: 5px;
    display: inline-block;
  }
  .skill-info {
    position: relative;
  }
  .skill-type {
    min-width: 320px;
  }
  .timeline .listing-large-inner {
    margin-bottom: 40px;
    overflow: hidden;
    position: relative;
    z-index: 2;
    @media #{$x600} {
      padding-right: 10px;
    }
  }
  .listing-large-inner:before {
    background: #2a2a2a;
    top: 0 !important;
    right: calc(50% - 6px);
    border-radius: 50%;
    content: " ";
    height: 12px;
    position: absolute;
    width: 12px;
    z-index: 0;
    @media #{$x600} {
      right: auto;
      left: 6px;
    }
  }
  .listing-large-inner:after {
    background: #673AB7 !important;
    bottom: 0 !important;
    content: " ";
    height: 12px;
    border-radius: 50%;
    left: calc(50% - 6px);
    position: absolute;
    width: 12px;
    z-index: 0;
    @media #{$x600} {
      right: auto;
      left: 6px;
    }
  }
  .timeline:after {
    background-color: #eaeaea;
    content: " ";
    height: 100%;
    left: 50%;
    position: absolute;
    top: 0;
    width: 1px;
    z-index: 0;
    @media #{$x600} {
      left: 10px !important;
    }
  }
  .timeline .listing-event .data.left {
    float: left;
    font-size: 18px;
    font-weight: 300;
    padding-right: 28px;
    position: relative;
    text-align: right;
    width: 50%;
    @media #{$x600} {
      padding-left: 28px;
      padding-right: 0;
      width: 100%;
    }
  }
  .timeline .listing-event .data.right {
    float: right;
    padding-left: 28px;
    position: relative;
    text-align: left;
    width: 50%;
    @media #{$x600} {
      width: 100%;
    }
  }
  .timeline .listing-event .data.right li {
    margin-bottom: 30px;
  }
  ul {
    list-style: none;
    padding: 0;
  }
  .clearfix:before {
    display: table;
    content: " ";
  }
  .clearfix:after {
    clear: both;
  }

  .timeline .listing-event .data.left li {
    clear: both;
    line-height: 32px;
    min-height: 32px;
  }
  .timeline .listing-event .data.left h5 {
    display: block;
    float: left !important;
    margin: 0 !important;
    font-size: 15px;
    font-weight: 400;
    font-family: "Roboto Slab" !important;
    &.skills-title {
      float: none;
      text-align: left;
      font-weight: 700;
    }
  }
  .timeline .listing-event .data.right h5 {
    font-size: 15px;
    font-weight: 700;
    margin-bottom: 10px;
    margin-top: 0;
  }
  .timeline .listing-event .data.right li p {
    text-align: justify;
    line-height: 24px;
    margin-top: -5px;
    font-size: 13px;
    @media #{$x600} {
      font-size: 11px;
    }
  }
  .emph-3 {
    font-style: italic;
    color: #999999;
  }
  .emph-4 {
    color: #999999;
  }
  li.emph-1 {
    display: block;
    width: 100%;
  }
  .timeline .listing-event .data.right li:nth-child(2) ul li {
    font-weight: 400;
    font-size: 15px;
  }
  .timeline .listing-event .data.right li:nth-child(2) ul li span {
    font-weight: 300 !important;
    font-size: 14px;
    margin-left: 2px;
  }
  .timeline .listing-event .data.right li ul li {
    margin-bottom: 0;
    display: inline-block;
  }
  ul.hb-list li span {
    border: 1px solid #673AB7 !important;
    color: #673AB7 !important;
    display: block;
    height: 45px;
    width: 45px;
    border-radius: 50%;
  }
  .hb-list.li {
    border-radius: 50%;
    &:hover{
      background-color: #673AB7;
      cursor: pointer;
      transition-duration: 0.5s;
      i {
        color: #ffffff !important;
      }
    }
    &.active {
      border-radius: 50%;
      background-color: #673AB7;
      i {
        color: #ffffff !important;
      }
    }
  }
  ul.hb-list li span i {
    font-size: 16px;
    line-height: 42px;
    display: block;
    color: #673AB7 !important;
  }
  ul.hb-list li h6 {
    color: #999999;
    margin-top: 10px;
    font-style: italic;
    font-size: 10.5px !important;
  }
  ul.hb-list li h6, .timeline .listing-event .data.right li:nth-child(2) ul li, h5, p,
  .timeline .listing-event .data.right li:nth-child(2) ul li span {
    font-family: "Roboto Slab" !important;
  }
  .rating {
    float: right;
  }
  .rating .circle {
    background: #eaeaea !important;
    display: inline-block;
    margin: 0 3px;
    height: 12px !important;
    width: 12px !important;
    border-radius: 50%;
    position: relative;
    cursor: pointer;
    top: -3px;
    &.active {
      background: #673AB7 !important;
    }
  }
  .md-dialog fieldset {
    display: block;
    border: none;
    margin: .6em 0 .2em;
    padding: 0;
  }
  .md-dialog fieldset input {
    display: none;
  }
  .md-dialog fieldset label {
    float: right;
  }
  .md-dialog fieldset label {
    position: relative;
    display: inline-block;
    padding: 0!important;
    margin: 0!important;
    width: 2em!important;
    height: 1.2em;
    text-align: center;
  }
  .md-dialog fieldset input:checked~label:before, .md-dialog fieldset input:hover~label:before {
    color: #414856!important;
  }
  .md-dialog fieldset input~label:before {
    font-size: 1.78em;
    color: #c4c4c4!important;
    position: relative;
  }
  form label[for] {
    cursor: pointer;
  }
  .actions.hobby {
    position: relative !important;
    float: right !important;
  }
</style>
