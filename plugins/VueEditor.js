import Vue from 'vue'
import VueQuillEditor from 'vue-quill-editor'
import Quill from 'quill'


import { ImageUpload } from 'quill-image-upload'
Quill.register('modules/imageUpload', ImageUpload)

// require styles
import 'quill/dist/quill.core.css'
import 'quill/dist/quill.snow.css'
import 'quill/dist/quill.bubble.css'

Vue.use(VueQuillEditor, /* { default global options } */)
