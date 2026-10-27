<?php

/*-----------------------------------operations----------------------------------------*/
function contents_resource_index($parameter){
    /*
     * this part is for filters
     * search and filters on type, category, topic, tag
     */
    $page = isset($parameter["page"]) && !empty($parameter["page"]) ? $parameter["page"] : 0;
    $limits = isset($parameter["limits"]) && !empty($parameter["limits"]) ? $parameter["limits"] : 30;
    $sort_base = isset($parameter["sort"]) && !empty($parameter["sort"]) ? $parameter["sort"] : "newest";

    /* this part is used for filtering
     * collect custom nids in $custom_contents
     * */
    $custom_contents = array();
    $filter = false;
    if(isset($parameter["search"])){
        $custom_contents = search_content_by_value($parameter["search"]);
        $filter = true;
    }
    if(isset($parameter["type"])) {
        $tids = explode("," , $parameter["type"]);
        $query = db_select("field_data_field_latin_contents_types" , "type");
        $query->fields("type" , array("entity_id"));
        $query->condition("type.field_latin_contents_types_tid" , $tids , "IN");
        $results = $query->execute()->fetchCol();
        $custom_contents = (!$filter)? $results : array_intersect($custom_contents , $results);
        $filter = true;
    }
    if(isset($parameter["category"])){
        $tids = explode("," , $parameter["category"]);
        $query = db_select("field_data_field_latin_contents_categories" , "category");
        $query->fields("category" , array("entity_id"));
        $query->condition("category.field_latin_contents_categories_tid" , $tids , "IN");
        $results = $query->execute()->fetchCol();
        $custom_contents = (!$filter)? $results : array_intersect($custom_contents , $results);
        $filter = true;
    }
    if(isset($parameter["topic"])){
        $tids = explode("," , $parameter["topic"]);
        $query = db_select("field_data_field_latin_contents_topics" , "topic");
        $query->fields("topic" , array("entity_id"));
        $query->condition("topic.field_latin_contents_topics_tid" , $tids , "IN");
        $results = $query->execute()->fetchCol();
        $custom_contents = (!$filter)? $results : array_intersect($custom_contents , $results);
        $filter = true;
    }
    if(isset($parameter["tag"])){
        $query = db_select("taxonomy_index" , "taxonomy_index");
        $query->fields("taxonomy_index" , array("nid"));
        $query->condition("taxonomy_index.tid" , $parameter["tag"]);
        $results = $query->execute()->fetchCol();
        $custom_contents = (!$filter)? $results : array_intersect($custom_contents , $results);
        $filter = true;
    }
    if($filter == true && empty($custom_contents)) return ["contents" => array() , "count" => 0 , "page" => $page];

    /*Get the Events nid*/
    $query = db_select("field_data_field_latin_contents_types" , "event");
    $query->fields("event" , array("entity_id"));
    $query->condition("event.entity_type" , "node");
    $query->condition("event.bundle" , "latin_contents");
    $query->condition("event.field_latin_contents_types_tid" , 4058);
    $events = $query->execute()->fetchCol();

    /*fetch content list data*/
    $query = db_select("node" , "node");
    $query->fields("node" , array("nid" , "title" , "uid" , "created" , "changed"));
    $query->condition("node.status" , 1);
    $query->condition("node.type" , "latin_contents");

    /*add body*/
    $query->leftJoin("field_data_body" , "body" , "node.nid = body.entity_id and body.entity_type = 'node' and body.bundle = 'latin_contents' ");
    $query->addField("body" , "body_value" , "body");

    /*add description*/
    $query->leftJoin("field_data_field_meta_description" , "meta_description" , "node.nid = meta_description.entity_id and meta_description.entity_type = 'node' and meta_description.bundle = 'latin_contents' ");
    $query->addField("meta_description" , "field_meta_description_value" , "meta_description");

    /*add first picture*/
    $query->leftJoin("field_data_field_image" , "field_image" , "node.nid = field_image.entity_id and field_image.entity_type = 'node' and field_image.bundle = 'latin_contents' and field_image.delta = '0' ");
    $query->leftJoin("file_managed" , "file" , "file.fid = field_image.field_image_fid");
    $query->addField("file" , "uri" , "picture");

    /*add author full name*/
    $query->join("users" , "user" , "user.uid = node.uid");
    $query->leftJoin("field_data_field_full_name" , "name" , "user.uid = name.entity_id and name.entity_type = 'user' and name.bundle = 'user' ");
    $query->addField("name" , "field_full_name_value" , "author_name");

    /*add author pic*/
    $query->leftJoin("file_managed" , "user_pic" , "user_pic.fid = user.picture");
    $query->addField("user_pic" , "uri" , "author_pic");

    /*except the events*/
    if(!empty($events))$query->condition("node.nid" , $events , "NOT IN");

    /*filter the response*/
    if($filter == true) $query->condition("node.nid" , $custom_contents, "IN");

    /*choose stick or none stick if $parameter[stick] is set*/
    if(!empty($parameter["stick"]) && $parameter["stick"] == 1) $query->condition("node.sticky" , 1);
    elseif(!empty($parameter["stick"]) && $parameter["stick"] == -1) $query->condition("node.sticky" , 0);

    /*If this query is called form other places with custom nids, it is filtered appropriate based on custom node*/
    if(isset($parameter["custom_contents"]) && !empty($parameter["custom_contents"])) $query->condition("node.nid" , $parameter["custom_contents"] , "IN");

    /*sort the response*/
    switch ($sort_base){
        case "newest" :
            $query->orderBy("node.nid" , "DESC");
            break;

        case "first" :
            $query->orderBy("node.nid" , "ASC");
            break;

        case "updated" :
            $query->orderBy("node.changed" , "DESC");
            break;
        default :
            $query->orderBy("node.nid" , "DESC");
            break;
    }

    /*calculating the counts of response*/
    $count = clone ($query);

    /*filter to limited data*/
    $query->range($page * $limits , $limits);

    $contents = $query->execute()->fetchAll();
    if(empty($contents)) return ["contents" => array() , "count" => 0];

    $nids = array();
    $contents_r = array();
    /*post process on contents*/
    foreach ($contents as $content){
        $content->created_timestamp = $content->created;
        $content->created = date("F j, Y" , $content->created);

        $content->changed_timestamp = $content->changed;
        $content->changed = date("F j, Y" , $content->changed);

        $content->picture = !empty($content->picture) ? image_style_url("300x200" , $content->picture) : null;
        $content->author_pic = !empty($content->author_pic) ? image_style_url("60x60" , $content->author_pic) : null;

        array_push($nids , $content->nid);
        $content->types = array();
        $content->categories = array();
        $content->topics = array();

        $contents_r[$content->nid] = $content;
    }

    /*add each content's type*/
    $query = db_select("field_data_field_latin_contents_types" , "types");
    $query->fields("types" , array("entity_id"));
    $query->join("taxonomy_term_data" , "term_data" , "term_data.tid = types.field_latin_contents_types_tid");
    $query->fields("term_data" , array("tid" , "name"));
    $query->condition("types.entity_id" , $nids , "IN");
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($contents_r[$result->entity_id]->types , array("tid" => $result->tid , "name" => $result->name));

    /*add each content's category*/
    $query = db_select("field_data_field_latin_contents_categories" , "categories");
    $query->fields("categories" , array("entity_id"));
    $query->join("taxonomy_term_data" , "term_data" , "term_data.tid = categories.field_latin_contents_categories_tid");
    $query->fields("term_data" , array("tid" , "name"));
    $query->condition("categories.entity_id" , $nids , "IN");
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($contents_r[$result->entity_id]->categories , array("tid" => $result->tid , "name" => $result->name));

    /*add each content's topic*/
    $query = db_select("field_data_field_latin_contents_topics" , "topics");
    $query->fields("topics" , array("entity_id"));
    $query->join("taxonomy_term_data" , "term_data" , "term_data.tid = topics.field_latin_contents_topics_tid");
    $query->fields("term_data" , array("tid" , "name"));
    $query->condition("topics.entity_id" , $nids , "IN");
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($contents_r[$result->entity_id]->topics , array("tid" => $result->tid , "name" => $result->name));

    $contents = array_values($contents_r);

    return ["contents" => $contents , "count" => (int) $count->countQuery()->execute()->fetchField() , "page" => $page];
}
function contents_resource_retrieve($nid , $parameter){
    global $user;
    $author = db_select("node")->fields("node" , array("uid"))->condition("node.nid" , $nid)->execute()->fetch();

    /*fetch content data*/
    $query = db_select("node" , "node");
    $query->fields("node" , array("nid" , "uid" , "title" , "status" , "created" , "changed"));
    $query->condition("node.nid" , $nid);
    if($user->uid != $author->uid) $query->condition("node.status" , 1);

    /*add body*/
    $query->leftJoin("field_data_body" , "body" , "body.entity_id = node.nid and body.entity_type = 'node' and body.bundle = 'latin_contents' ");
    $query->fields("body" , array("body_value"));

    /*add first image*/
    $query->leftJoin("field_data_field_image" , "field_image" , "field_image.entity_id = node.nid and field_image.entity_type = 'node' and field_image.delta = '0' ");
    $query->leftJoin("file_managed" , "image" , "image.fid = field_image.field_image_fid");
    $query->addField("image" , "uri" , "image");

    /*add video link*/
    $query->leftJoin("field_data_field_link" , "field_link" , "field_link.entity_id = node.nid and field_link.delta = '0' ");
    $query->addField("field_link" , "field_link_url" , "video_link");

    /*add event time*/
    $query->leftJoin("field_data_field_eventtime" , "eventtime" , "node.nid = eventtime.entity_id");
    $query->addField("eventtime" , "field_eventtime_value" , "event_time");

    /*add time and duration*/
    $query->leftJoin("field_data_field_event_time_and_duration" , "time_and_duration" , "node.nid = time_and_duration.entity_id");
    $query->addField("time_and_duration" , "field_event_time_and_duration_value" , "time_and_duration");

    /*add event place*/
    $query->leftJoin("field_data_field_event_place" , "event_place" , "node.nid = event_place.entity_id");
    $query->addField("event_place" , "field_event_place_value" , "event_place");

    /*add company*/
    $query->leftJoin("field_data_field_company" , "company" , "node.nid = company.entity_id");
    $query->addField("company" , "field_company_value" , "company");

    /*add registration link*/
    $query->leftJoin("field_data_field_event_registration_link" , "registration_link" , "node.nid = registration_link.entity_id");
    $query->addField("registration_link" , "field_event_registration_link_url" , "registration_link");

    /*add if webinar*/
    $query->leftJoin("field_data_field_ifwebinar" , "ifwebinar" , "node.nid = ifwebinar.entity_id");
    $query->addField("ifwebinar" , "field_ifwebinar_value" , "ifwebinar");

    /*add 808 suggestion*/
    $query->leftJoin("field_data_field_need_login_for_register" , "need_login" , "node.nid = need_login.entity_id");
    $query->addField("need_login" , "field_need_login_for_register_value" , "need_login");

    /*add 808 suggestion*/
    $query->leftJoin("field_data_field_event_808_suggestion" , "808_suggestion" , "node.nid = 808_suggestion.entity_id");
    $query->addField("808_suggestion" , "field_event_808_suggestion_value" , "808_suggestion");

    /*add meta description*/
    $query->leftJoin("field_data_field_meta_description" , "meta_description" , "node.nid = meta_description.entity_id and meta_description.bundle = 'latin_contents' ");
    $query->addField("meta_description" , "field_meta_description_value" , "meta_description");

    /*add if user clap this node or not*/
    $query->leftJoin("clapping" , "user_clap" , "user_clap.entity_id = node.nid and user_clap.entity_type = 'node' and user_clap.uid = '" . $user->uid . "'");
    $query->addField("user_clap" , "vote" , "user_clap");

    /*subquery calculate the sum of the claps for this content*/
    $subquery = db_select("clapping" , "clap");
    $subquery->fields("clap" , array("entity_id"));
    $subquery->addExpression("SUM(vote)" , "clap_point");
    $subquery->groupBy("entity_id");
    $subquery->condition("clap.entity_type" , "node");
    $subquery->condition("clap.bundle" , "latin_contents");
    $subquery->condition("clap.entity_id" , $nid);
    $query->leftJoin($subquery , "clap" , "node.nid = clap.entity_id");
    $query->addField("clap" , "clap_point" , "clap_point");

    $content = $query->execute()->fetch();
    if(empty($content)) return services_error("This content is not found!" , 404);

    /*post process, changes time format and add other information*/
    $content->created = date("F j, Y" , $content->created);
    $content->changed = date("F j, Y" , $content->changed);
    if(!empty($content->image)) $content->image = generate_file_address($content->image);
//    if(isset($content->event_time)){
//        $time = explode(" " , $content->event_time);
//        $date = explode("-" , $time[0]);
//        $times = explode(":" , $time[1]);
//        $timestamp = mktime($times[0], $times[1], $times[2], $date[1], $date[2], $date[0]);
//        $content->event_time = date("F j, Y" , $timestamp);
//    }
    $content->clap_point = empty($content->clap_point) ? 0 : (int) $content->clap_point;
    $content->user_clap = empty($content->user_clap) ? 0 : (int) $content->user_clap;
    $content->references =  array();
    $content->files =  array();
    $content->type =  array();
    $content->category =  array();
    $content->topic =  array();
    $content->tags =  array();

    /*add files*/
    $query = db_select("field_data_field_files" , "field_files");
    $query->join("file_managed" , "file" , "field_files.field_files_fid = file.fid");
    $query->addField("file" , "uri" , "file");
    $query->condition("field_files.entity_id" , $content->nid);
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($content->files , generate_file_address($result->file));

    /*add references*/
    $query = db_select("field_data_field_latin_content_references" , "ref");
    $query->addField("ref" , "field_latin_content_references_url" , "references_url");
    $query->condition("ref.entity_id" , $content->nid);
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($content->references , $result->references_url);

    /*add types*/
    $query = db_select("field_data_field_latin_contents_types" , "types");
    $query->join("taxonomy_term_data" , "taxonomy" , "types.field_latin_contents_types_tid = taxonomy.tid");
    $query->fields("taxonomy" , array("tid" , "name"));
    $query->condition("types.entity_id" , $content->nid);
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($content->type , array("tid" => $result->tid , "name" => $result->name));

    /*add categories*/
    $query = db_select("field_data_field_latin_contents_categories" , "categories");
    $query->join("taxonomy_term_data" , "taxonomy" , "categories.field_latin_contents_categories_tid = taxonomy.tid");
    $query->fields("taxonomy" , array("tid" , "name"));
    $query->condition("categories.entity_id" , $content->nid);
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($content->category , array("tid" => $result->tid , "name" => $result->name));

    /*add topics*/
    $query = db_select("field_data_field_latin_contents_topics" , "topics");
    $query->join("taxonomy_term_data" , "taxonomy" , "topics.field_latin_contents_topics_tid = taxonomy.tid");
    $query->fields("taxonomy" , array("tid" , "name"));
    $query->condition("topics.entity_id" , $content->nid);
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($content->topic , array("tid" => $result->tid , "name" => $result->name));

    /*add tags*/
    $query = db_select("field_data_field_latin_tags" , "tags");
    $query->join("taxonomy_term_data" , "taxonomy" , "tags.field_latin_tags_tid = taxonomy.tid");
    $query->fields("taxonomy" , array("tid" , "name"));
    $query->condition("tags.entity_id" , $content->nid);
    $results = $query->execute()->fetchAll();
    foreach ($results as $result) array_push($content->tags , array("tid" => $result->tid , "name" => $result->name));

    global $user;
    /*about author*/
    $query = db_select("users" , "user");
    $query->leftJoin("field_data_field_full_name" , "name" , "user.uid = name.entity_id");
    $query->leftJoin("field_data_field_about_me" , "about_me" , "user.uid = about_me.entity_id");
    $query->leftJoin("file_managed" , "file_managed" , "user.picture = file_managed.fid");
    $query->fields("user" , array("uid"));
    $query->addField("name" , "field_full_name_value" , "full_name");
    $query->addField("about_me" , "field_about_me_value" , "about_me");
    $query->addField("file_managed" , "uri" , "picture");
    $query->condition("user.uid" , $content->uid);
    $author = $query->execute()->fetch();
    if(!empty($author->picture)) $author->picture = image_style_url("60x60" , $author->picture);
    $author->user_follow = is_flagged($user->uid , $author->uid , "user");

    /*check user if user bookmark this content or not*/
    $content->user_bookmark = is_flagged($user->uid , $content->nid , "node");

    return ["content" => $content , "author" => $author];
}
function contents_resource_create($data){
    global $user;
    if(!$user->uid > 0) return services_error("You should login for this action!" , 403);

    $node = new stdClass();
    $node->title  = $data['title'];
    $node->type  = "latin_contents";
    node_object_prepare($node);
    $node->language = 'en';
    $node->uid  = $user->uid;
    $node->status = !empty($data["publish"]) && $data["publish"] == 1 ? 1 : 0;
    $node->comment = 2;
    $node->created = REQUEST_TIME;
    $node->body['und'][0]['value'] = $data['body'];
    $node->body['und'][0]['summary'] = '';
    $node->body['und'][0]['format'] = 'full_html';

    $node_wrapper= entity_metadata_wrapper('node',$node);

    $node_wrapper->field_meta_description->set($data["meta_description"]);

    $node_wrapper->save();
    if(isset($data["tags"])){
      foreach ($data["tags"] as $tag =>$value){
        $node->field_latin_tags["und"][]=array("tid"=>$value);
      }
    }
  if(isset($data["types"])){
    foreach ($data["types"] as $tag =>$value){
      $node->field_latin_contents_types["und"][]=array("tid"=>$value);
    }
  }
  if(isset($data["topics"])){
    foreach ($data["topics"] as $tag =>$value){
      $node->field_latin_contents_topics["und"][]=array("tid"=>$value);
    }
  }

    $field_images=[];

    if(isset($data['field_image']) && !empty($data['field_image'])){
      $c=0;
      foreach ($data["field_image"] as $value){
        $field_images["und"][$c]["display"]=1;
        $field_images["und"][$c]["fid"]=$value;
        $c++;
      }
      $node->field_image  = $field_images;
    }
    node_save($node);

//    module_invoke_all('field_storage_pre_update' , "node" , $node);

    $node = contents_resource_retrieve($node->nid , array());


    return $node;
}
function contents_resource_update($nid , $data){
    global $user;
    if(!$user->uid > 0) return services_error("You should login for this action!" , 403);
    $node = node_load($nid);
    if(!$node) return services_error("Content is not found!" , 404);
    if($user->uid != $node->uid && !in_array("administrator" , $user->roles)) return services_error("You can not do this action!" , 403);

    $update = false;
    if(isset($data['title']) && !empty($data['title']) && $node->title !== $data["title"]){
        $node->title  = $data['title'];
        $update = true;
    }
    if(strlen($data["publish"]) > 0 && $node->status !== $data["publish"]) {
        $node->status = $data["publish"];
        $update = true;
    }
    if(isset($data['body']) && !empty($data['body']) && $node->body['und'][0]['value'] !== $data["body"]) {
        $node->body['und'][0]['value'] = $data['body'];
        $update = true;
    }
    if(isset($data['field_image']) && !empty($data['field_image'])) $node->field_image  = $data['field_image'];

    if($update) {
        node_save($node);
        module_invoke_all('field_storage_pre_update' , "node" , $node);
    }

    return $node;
}
function contents_resource_delete($nid){
    global $user;
    if(!$user->uid > 0) return services_error("You should login for this action!" , 403);
    $node = db_select("node" , "node")->fields("node" , array())->condition("nid" , $nid)->execute()->fetch();
    if(empty($node)) return services_error("Content is not found!" , 404);
    if($user->uid != $node->uid && !in_array("administrator" , $user->roles)) return services_error("You can not do this action!" , 403);
    node_delete($nid);
    return true;
}

/*
 * search value in to the titles and bodies and tags
 * return array of nids
 */
function search_content_by_value($value){
    $query = db_select("node", "node");
    $query->fields('node', array('nid'));
    $query->condition('node.title', '%' . $value . '%', 'LIKE');
    $nid1 = $query->execute()->fetchCol();

    $query = db_select("field_data_body", "body");
    $query->addField('body', 'entity_id', 'nid');
    $query->condition('body.body_value', '%' . $value . '%', 'LIKE');
    $query->condition('body.entity_type' , 'node');
    $nid2 = $query->execute()->fetchCol();

    $query = db_select("taxonomy_term_data" , "term_data");
    $query->join("taxonomy_index" , "taxonomy_index" , "taxonomy_index.tid = term_data.tid");
    $query->fields("taxonomy_index" , array("nid"));
    $query->condition("term_data.name" , '%' . $value . '%', 'LIKE');
    $nid3 = $query->execute()->fetchCol();

    $nids = array_merge($nid1, $nid2);
    $nids = array_merge($nids , $nid3);

    return $nids;
}
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------relationships-----------------------------------*/
/**
 * Returns the all required filters
 */
function contents_resource_filters($parameter  , $url){
    $type = array();
    $category = array();
    $topic = array();

    /*
     * find the taxonomies for filters
     */
    $query = db_select("taxonomy_term_data" , "term_data");
    $query->fields("term_data" , array("tid" , "vid" , "name"));
    $query->condition("term_data.vid" , array(30 , 34 , 35) ,"IN");
    $query->condition("term_data.tid" , 4058 ,"<>");
    $results = $query->execute()->fetchAll();

    /*categorize each filter*/
    foreach($results as $result){
        if($result->vid == 30) array_push($type , array("tid" => $result->tid , "name" => $result->name));
        if($result->vid == 34) array_push($category , array("tid" => $result->tid , "name" => $result->name));
        if($result->vid == 35) array_push($topic , array("tid" => $result->tid , "name" => $result->name));
    }

    return array(
        array("name" => "type", "children" => $type),
        array("name" => "category", "children" => $category),
        array("name" => "topic", "children" => $topic)
    );
}

/**
 * Returns the topics list
 * It is used to be selected by user after registration in site
 */
function contents_resource_topics($parameters , $url){
    $query = db_select("taxonomy_term_data" , "term_data");
    $query->leftJoin("field_data_field_image" , " field_image" , "term_data.tid = field_image.entity_id and field_image.entity_type = 'taxonomy_term' ");
    $query->leftJoin("file_managed" , "image" , "image.fid = field_image.field_image_fid");
    $query->fields("term_data" , array("tid" , "name"));
    $query->addField("image" , "uri" , "picture");
    $query->condition("term_data.vid" , 35);
    $results = $query->execute()->fetchAll();

    foreach ($results as $result){
        if(!empty($result->picture)) $result->picture = image_style_url("300x300" , $result->picture);
    }

    return $results;
}

/**
 * It returns the Events
 * if $url == list then it returns the upcoming list for home page
 * if $url == all then it returns all of the events
 */
function content_resource_event_list($parameter , $url){
    /*if $url == list then return the upcoming events from today until the end*/
    if(strcmp($url , "list") == 0) {
        $query = db_select("node", "node");
        $query->fields("node", array("nid", "title" , "sticky"));
        $query->condition('node.status', 1);

        $query->join("field_data_field_latin_contents_types", "contents_types", "contents_types.entity_id = node.nid and contents_types.field_latin_contents_types_tid = '4058' ");

        $query->join("field_data_field_eventtime", "eventtime", "eventtime.entity_id = node.nid");
        $query->addField("eventtime", "field_eventtime_value", "eventtime");
        $query->condition('eventtime.field_eventtime_value', date("y-m-d h:m:s", time()), '>=');

        $query->leftJoin('field_data_field_company', 'company', 'company.entity_id = node.nid');
        $query->addField("company", "field_company_value", "company");

        $query->leftJoin('field_data_field_ifwebinar', 'ifwebinar', 'ifwebinar.entity_id = node.nid');
        $query->addField("ifwebinar", "field_ifwebinar_value", "webinar");

        $query->orderBy('eventtime.field_eventtime_value', 'ASC');

        $events = $query->execute()->fetchAll();

        return $events;
    }
    /* if $url == all then return the events that is in a month == calender
     * parameter[year] and parameter[month] is needed to find the events for specific time
     */
    elseif(strcmp($url , "all") == 0){
        /* First calculate the timestamp of the first and last days of the month that is requested to show
         * NOTE: endMonth is tricky because the next month would be next year
         */
        $year = (int)$parameter["year"]; $month = (int)$parameter["month"];
        $startMonth = mktime(0 , 0 , 0 , $month , 1 , $year);
        if($month < 12) $endMonth  = mktime(0 , 0 , 0 , $month+1 , 1 , $year);
        else $endMonth  = mktime(0 , 0 , 0 , 1 , 1 , $year+1);
        $endMonth = strtotime("-1 day" , $endMonth);

        /* Fetch the events in the month
         * NOTE: The order is Important for calculate the first day
         */
        $query = db_select("node", "node");
        $query->fields("node", array("nid", "title" , "sticky"));
        $query->condition('node.status', 1);

        $query->join("field_data_field_latin_contents_types", "contents_types", "contents_types.entity_id = node.nid and contents_types.field_latin_contents_types_tid = '4058' ");

        $query->join("field_data_field_eventtime", "eventtime", "eventtime.entity_id = node.nid");
        $query->addField("eventtime", "field_eventtime_value", "eventtime");
        $query->condition('eventtime.field_eventtime_value', date("y-m-d h:m:s", $startMonth), '>=');
        $query->condition('eventtime.field_eventtime_value', date("y-m-d h:m:s", $endMonth), '<=');
        $query->orderBy("eventtime.field_eventtime_value" , "ASC");

        $query->leftJoin("field_data_field_meta_description" , "meta_description" , "node.nid = meta_description.entity_id and meta_description.bundle = 'latin_contents' ");
        $query->addField("meta_description" , "field_meta_description_value" , "meta_description");

        $events = $query->execute()->fetchAll();
        if(empty($events)) return array();

        /* $times is an associative array that each key is equal the day of the month
         * We used $day and $tomorrow for putting each event to its own day
         */
        $start = $events[0]->eventtime;
        $start = explode(" " , $start);
        $start = explode("-" , $start[0]);
        $day = mktime(0 , 0 , 0 , $start[1] , $start[2] , $start[0]);
        $tomorrow = strtotime("+ 1 day" , $day);

        $times = array();
        foreach ($events as $event){
            $eTime = strtotime($event->eventtime);
            $continue = true;
            /* while the day of the event is not found we go next day
             * when the day is found we go to the next event
             * NOTE: because the events are ordered by time we wont missed any event
             */
            while ($continue) {
                if ($eTime >= $day && $eTime < $tomorrow) {
                    $date = date('Y-m-d',$day);
                    $date = explode("-" , $date);
                    $date = (int)$date[2];
                    if(!isset($times[$date])) $times[$date] = array();
                    array_push($times[$date], $event);
                    $continue = false;
                }
                else{
                    $day = $tomorrow;
                    $tomorrow = strtotime("+1 day" , $tomorrow);
                }
            }
        }
        return $times;
    }
}

function content_resource_services_list($parameter , $url){
    $query = db_select("node" , "node");
    $query->fields("node" , array("nid" , "title" , "uid" , "created" , "changed"));
    $query->condition("node.status" , 1);
    $query->condition("node.type" , "latin_contents");

    /*filter the Services*/
    $query->join("field_data_field_latin_contents_types" , "type" , "node.nid = type.entity_id and type.field_latin_contents_types_tid = '4599' ");

    /*add picture*/
    $query->leftJoin("field_data_field_image" , "field_image" , "node.nid = field_image.entity_id and field_image.entity_type = 'node' and field_image.bundle = 'latin_contents' and field_image.delta = '0' ");
    $query->leftJoin("file_managed" , "file" , "file.fid = field_image.field_image_fid");
    $query->addField("file" , "uri" , "picture");

    /*add author full name*/
    $query->join("users" , "user" , "user.uid = node.uid");
    $query->leftJoin("field_data_field_full_name" , "name" , "user.uid = name.entity_id and name.entity_type = 'user' and name.bundle = 'user' ");
    $query->addField("name" , "field_full_name_value" , "author_name");

    /*add author pic*/
    $query->leftJoin("file_managed" , "user_pic" , "user_pic.fid = user.picture");
    $query->addField("user_pic" , "uri" , "author_pic");

    $contents = $query->execute()->fetchAll();

    foreach ($contents as $content){
        $content->created_timestamp = $content->created;
        $content->created = date("F j, Y" , $content->created);

        $content->changed_timestamp = $content->changed;
        $content->changed = date("F j, Y" , $content->changed);

        $content->picture = !empty($content->picture) ? image_style_url("300x200" , $content->picture) : null;
        $content->author_pic = !empty($content->author_pic) ? image_style_url("60x60" , $content->author_pic) : null;
    }

    return $contents;
}

/**
 * Returns the comments of a node
 */
function content_resource_comments($parameter , $nid){
    global $user;

    $likes = db_select("clapping" , "likes");
    $likes->fields("likes" , array("entity_id"));
    $likes->addExpression("SUM(vote)" , "likes");
    $likes->groupBy("entity_id");
    $likes->condition("likes.entity_type" , "comment");
    $likes->condition("likes.bundle" , "comment");

    /*query fetch the just the parent comments*/
    $query = db_select("comment" , "comment");
    $query->join("field_data_comment_body" , "body" , "body.entity_id = comment.cid");
    $query->join("users" , "user" , "user.uid = comment.uid");
    $query->leftJoin("field_data_field_full_name" , "full_name" , "full_name.entity_id = user.uid and full_name.entity_type = 'user' ");
    $query->leftJoin("file_managed" , "user_pic" , "user_pic.fid = user.picture");
    $query->leftJoin("clapping" , "user_like" , "comment.cid = user_like.entity_id and user_like.entity_type = 'comment' and user_like.uid = ' " . $user->uid . " ' ");
    $query->leftJoin($likes , "likes" , "comment.cid = likes.entity_id");
    $query->fields("comment" , array("cid" , "pid" , "uid" , "created" , "changed"));
    $query->addField("body" , "comment_body_value" , "body");
    $query->addField("user" , "name" , "author_username");
    $query->addField("full_name" , "field_full_name_value" , "author_name");
    $query->addField("user_pic" , "uri" , "author_pic");
    $query->addField("user_like" , "vote" , "user_like");
    $query->addField("likes" , "likes" , "likes");
    $query->condition("comment.nid" , $nid);
    $query->condition("comment.pid" , 0);
    $query->orderBy("comment.cid" , "ASC");
    $query->range(isset($parameter["page"])? $parameter["page"]*30 : 0 , 30);

    /*query fetch the just the children of the parent comments*/
    $subquery = db_select("comment" , "comment");
    $subquery->join($query , "parent" , "parent.cid = comment.pid");
    $subquery->join("field_data_comment_body" , "body" , "body.entity_id = comment.cid");
    $subquery->join("users" , "user" , "user.uid = comment.uid");
    $subquery->leftJoin("field_data_field_full_name" , "full_name" , "full_name.entity_id = user.uid and full_name.entity_type = 'user' ");
    $subquery->leftJoin("file_managed" , "user_pic" , "user_pic.fid = user.picture");
    $subquery->leftJoin("clapping" , "user_like" , "comment.cid = user_like.entity_id and user_like.entity_type = 'comment' and user_like.uid = ' " . $user->uid . " ' ");
    $subquery->leftJoin($likes , "likes" , "comment.cid = likes.entity_id");
    $subquery->fields("comment" , array("cid" , "pid" , "uid" , "created" , "changed"));
    $subquery->addField("body" , "comment_body_value" , "body");
    $subquery->addField("user" , "name" , "author_username");
    $subquery->addField("full_name" , "field_full_name_value" , "author_name");
    $subquery->addField("user_pic" , "uri" , "author_pic");
    $subquery->addField("user_like" , "vote" , "user_like");
    $subquery->addField("likes" , "likes" , "likes");
    $subquery->orderBy("comment.cid" , "ASC");

    $comments = $query->execute()->fetchAll();
    $children = $subquery->execute()->fetchAll();

    $associate_comments = array();
    foreach ($comments as $comment){
        $comment->replies = array();
        $comment->user_like = empty($comment->user_like) ? false : true;
        $comment->likes = empty($comment->likes) ? 0 : (int) $comment->likes;
        $comment->created = date("F j, Y" , $comment->created);
        $comment->changed = date("F j, Y" , $comment->changed);
        if(!empty($comment->author_pic)) $comment->author_pic = image_style_url("200x200", $comment->author_pic);
        $associate_comments[$comment->cid] = $comment;
    }
    foreach ($children as $child){
        if(isset($associate_comments[$child->pid])) {
            $child->user_like = empty($child->user_like) ? false : true;
            $child->likes = empty($child->likes) ? 0 : (int) $child->likes;
            $child->created = date("F j, Y" , $child->created);
            $child->changed = date("F j, Y" , $child->changed);
            if(!empty($child->author_pic)) $child->author_pic = image_style_url("200x200", $child->author_pic);
            array_push($associate_comments[$child->pid]->replies, $child);
        }
    }
    $comments = array_values($associate_comments);

    return $comments;
}

/**
 * It finds the relative nodes for a node
 * It is using  fetch_content_relatives() function
 */
function content_resource_relatives($parameter , $nid){
    $node = db_select("node" , "node")->fields("node" , array())->condition("nid" , $nid)->condition("status" , 1)->execute()->fetch();
    if(empty($node)) return services_error("This content is not found!" , 404);

    /*get the most relative 20 nids for $nid for each $parameter['page']*/
    $relatives_priority =  fetch_content_relatives($nid);

    /*get the nids*/
    $nids = array_keys($relatives_priority);

    /*fetch the data*/
    $query = db_select("node" , "node");
    $query->leftJoin("field_data_field_image" , "field_image" , "node.nid = field_image.entity_id and field_image.entity_type = 'node' and field_image.bundle = 'latin_contents' and field_image.delta = '0' ");
    $query->leftJoin("file_managed" , "file" , "file.fid = field_image.field_image_fid");
    $query->fields("node" , array("nid" , "title" , "uid" , "created" , "changed"));
    $query->addField("file" , "uri" , "picture");
    $query->condition("node.nid" , $nids , "IN");
    $query->orderBy("node.nid" , "DESC");
    $relatives = $query->execute()->fetchAll();

    /*fetch the types related to this nids*/
    $query = db_select("taxonomy_term_data" , "term");
    $query->join("field_data_field_latin_contents_types" , "type" , "term.tid = type.field_latin_contents_types_tid and type.bundle = 'latin_contents' ");
    $query->fields("term" , array("tid" , "name"));
    $query->fields("type" , array("entity_id"));
    $query->condition("type.entity_id" , $nids , "IN");
    $types = $query->execute()->fetchAll();

    /*
     * this part is for sort the result base on priority
     * and change the picture address and the time format
     */
    $sorted = array();
    foreach ($relatives as $relative){
        $relative->created = date("F j, Y" , $relative->created);
        $relative->changed = date("F j, Y" , $relative->changed);
        $relative->picture = (!empty($relative->picture)) ? image_style_url("300x200" , $relative->picture) : "";
        $relative->types = array();
        $sorted[$relative->nid] = $relative;
    }

    /*add each type to its node*/
    foreach ($types as $type) if(isset($sorted[$type->entity_id])) array_push($sorted[$type->entity_id]->types , array("tid" => $type->tid , "name" => $type->name));

    /*sort the nodes based on priorities*/
    foreach($relatives_priority as $key => $value) $relatives_priority[$key] = $sorted[$key];
    $relatives = array_values($relatives_priority);

    return $relatives;
}

/**
 * find the most relative nodes for the $node_id based on the priority
 * node_topics has 4, node_categories has 3, node_tag has 2, node_types has 1
 * if the relatives did not find before it is computed and save in database
 * if there is record in database and is inserted before 3 days ago it is updated again
 * else the result fetch form database and split 20 rows based on $page and return the relative nids
 */
function fetch_content_relatives($node_id , $refresh = false){

    /*fetch the $node_id in db*/
    $relatives = db_select("relative_contents" , "relative")->fields("relative" , array())
            ->condition("entity_id" , $node_id)->condition("entity_type" , "node")->execute()->fetch();

    /*find the timestamp for 1 days before*/
    $time = strtotime('-1 day' , REQUEST_TIME);

    /*if there is no record or old record update compute again else fetch data from db*/
    if(empty($relatives) || $relatives->date < $time || $refresh){
        $priority = array();

        /*check relatives on topics with priority 4*/
        $query = db_select("field_data_field_latin_contents_topics" , "node_topics");
        $query->join("field_data_field_latin_contents_topics" , "relative_topics" , "relative_topics.field_latin_contents_topics_tid = node_topics.field_latin_contents_topics_tid and relative_topics.entity_type = 'node' and relative_topics.bundle = 'latin_contents' ");
        $query->join("node" , "node" , "node.nid = relative_topics.entity_id and node.status = '1' ");
        $query->fields("node" , array("nid"));
        $query->condition("node_topics.entity_id" , $node_id);
        $query->condition("relative_topics.entity_id" , $node_id , "<>");
        $nodes = $query->execute()->fetchCol();
        foreach($nodes as $nid) $priority[$nid] = 4;

        /*check relatives on categories with priority 3*/
        $query = db_select("field_data_field_latin_contents_categories" , "node_categories");
        $query->join("field_data_field_latin_contents_categories" , "relative_categories" , "relative_categories.field_latin_contents_categories_tid = node_categories.field_latin_contents_categories_tid and relative_categories.entity_type = 'node' and relative_categories.bundle = 'latin_contents' ");
        $query->join("node" , "node" , "node.nid = relative_categories.entity_id and node.status = '1' ");
        $query->fields("node" , array("nid"));
        $query->condition("node_categories.entity_id" , $node_id);
        $query->condition("relative_categories.entity_id" , $node_id , "<>");
        $nodes = $query->execute()->fetchCol();
        foreach($nodes as $nid) if(isset($priority[$nid])) $priority[$nid] += 3; else $priority[$nid] = 3;

        /*check relatives on tags with priority 2*/
        $query = db_select("field_data_field_latin_tags" , "node_tag");
        $query->join("field_data_field_latin_tags" , "relative_tag" , "node_tag.field_latin_tags_tid = relative_tag.field_latin_tags_tid and relative_tag.entity_type = 'node' and relative_tag.bundle = 'latin_contents' ");
        $query->join("node" , "node" , "node.nid = relative_tag.entity_id and node.status = '1' ");
        $query->fields("node" , array("nid"));
        $query->condition("node_tag.entity_id" , $node_id);
        $query->condition("relative_tag.entity_id" , $node_id , "<>");
        $nodes = $query->execute()->fetchCol();
        foreach($nodes as $nid) if(isset($priority[$nid])) $priority[$nid] += 2; else $priority[$nid] = 2;

        /*check relatives on types with priority 1*/
        $query = db_select("field_data_field_latin_contents_types" , "node_types");
        $query->join("field_data_field_latin_contents_types" , "relative_types" , "relative_types.field_latin_contents_types_tid = node_types.field_latin_contents_types_tid and relative_types.entity_type = 'node' and relative_types.bundle = 'latin_contents' ");
        $query->join("node" , "node" , "node.nid = relative_types.entity_id and node.status = '1' ");
        $query->fields("node" , array("nid"));
        $query->condition("node_types.entity_id" , $node_id);
        $query->condition("relative_types.entity_id" , $node_id , "<>");
        $nodes = $query->execute()->fetchCol();
        foreach($nodes as $nid) if(isset($priority[$nid])) $priority[$nid] += 1; else $priority[$nid] = 1;

        arsort($priority);

        /* choosing random n-randoms nids from top priority
         * in $last array categorize the nids based on the priority
         * the nids that have same priority are put in the same array
         */
        $n = 3;
        $max = 10;
        $last = array();
        $count = 0;
        foreach ($priority as $key=>$value){
            if($value == $max){
                $last[$max][$key] = $value;
                $count++;
            }
            elseif($count < $n){
                $max = $value;
                $last[$max][$key] = $value;
                $count++;
            }
        }
        /* in this part we check if the high priority size less than n
         * all the array will push
         * else we choose the random nids based on our capacity
         * */
        $priority = array();
        foreach ($last as $key=>$value){
            $capacity = $n - count($priority);
            if(($capacity - count($last[$key])) >= 0){
                foreach ($value as $k=>$v) $priority[$k] = $v;
            }
            elseif($capacity > 0){
                $random_keys = array_rand($value,$capacity);
                if(is_array($random_keys)) {
                    foreach ($random_keys as $k) $priority[$k] = $value[$k];
                }
                else{
                    $priority[$random_keys] = $value[$random_keys];
                }
            }
        }

        $nids = $priority;
        $result = serialize($priority);

        db_merge("relative_contents")->key(array(
            "entity_id" => $node_id,
            "entity_type" => "node",
            "bundle" => "latin_contents"
        ))->fields(array(
            "contents_id" => $result,
            "date" => REQUEST_TIME
        ))->execute();
    }
    else{
        $nids = unserialize($relatives->contents_id);
        $nodes = db_select("node" , "node")->fields("node" , array("nid"));
        $nodes->condition("node.nid" , array_keys($nids) , "IN")->condition("node.status" , 1);
        $nodes = $nodes->execute()->fetchCol();
        /*if a related node has became unpublished we need to fetch them again*/
        if(count($nodes) < count(array_keys($nids))) return fetch_content_relatives($node_id , true);
    }

    return $nids;
}

/**
 * This function return the contents for homepage
 * It responses based on user`s interests
 */
function content_resource_user_interest($parameter , $url){
    global $user;
    if($user->uid == 0) return array();

    /*for now we just have user`s topic interests*/
    $query = db_select("user_temp_interests" , "interests");
    $query->join("field_data_field_latin_contents_topics" , "topic" , "interests.entity_id = topic.field_latin_contents_topics_tid and topic.entity_type = 'node' and topic.bundle = 'latin_contents' ");
    $query->join("node" , "node" , "node.nid = topic.entity_id and node.status = '1' ");
    $query->fields("node" , array("nid"));
    $query->condition("interests.uid" , $user->uid);
    $query->condition("interests.action" , "interest");
    $query->condition("interests.entity_type" , "taxonomy_term");
    $query->condition("interests.bundle" , "latin_content_topic");
    $query->condition("interests.value" , "1");
    $query->orderBy("node.nid" , "DESC");
    $nids = $query->execute()->fetchCol();
    if(empty($nids)) return $nids;
    $parameter["custom_contents"] = $nids;
    return contents_resource_index($parameter);
}
/*-------------------------------------------------------------------------------------*/
/*--------------------------- Actions ----------------------------------*/

/**
 * @param $parameter Array
 * @return stdClass|string|void
 * User can upload image using this callback api, it requires CSRF token for authentication and image as input.
 * It checks the file type against common image types and if it does not conform to image types, it'll return 204 error code.
 * It also verifies that the file is sent otherwise will return response code of 403.
 */
function contents_resource_upload_image($parameter){
    global $user;
    $new_hash = hash_hmac("sha256" ,time() , "@ed899" , false);

    $fileName="uid-$user->uid-".substr($new_hash,0,10).'-'.$_FILES['image']['name'];
    $uploadfile = "public://".$fileName;


    $supported_extensions= array("gif","jpg","jpeg","png");
    $file = new stdClass();
    $file->uid      = $user->uid;
    $file->status   = 0;
    $file->filename = trim($fileName, '.');
    $file->uri      = $uploadfile;
    $file->filemime = file_get_mimetype($file->filename);
    $file->filesize = $_FILES['image']['size'];
    $fileInKB= $file->filesize/1024;
    if($fileInKB>5120){
      header("HTTP/1.1 400 Image is too big");
      return "Image size should not be larger than 5 MB";
    }
    if(!$_FILES["image"]) {
      header("HTTP/1.1 204 No image is selected");
      return "No image is selected";
    }
    $ext= strtolower(pathinfo($_FILES["image"]["name"],PATHINFO_EXTENSION));



      if(in_array($ext,$supported_extensions)){
        move_uploaded_file($_FILES['image']['tmp_name'], $uploadfile);
        $fileContent= file_save($file);
        $fileContent->fileUrl="api/sites/default/files/".$fileName;
        return $fileContent;
      }
      else{
        header("HTTP/1.1 415 Only images are allowed");
        return "Only images are allowed";
      }


}

