<?php

function contents_resource_index($parameter , $filter){
    /*pre processes*/
    $page = isset($parameter["page"]) && !empty($parameter["page"]) ? $parameter["page"] : 0;
    $limits = isset($parameter["limits"]) && !empty($parameter["limits"]) ? $parameter["limits"] : 30;
    $sort_base = isset($parameter["sort"]) && !empty($parameter["sort"]) ? $parameter["sort"] : "newest";
    $fields = !empty($parameter["query"]) ? explode("," , $parameter["query"]) : array();

    /*Get the exception nid*/
    $exception = db_select("field_data_field_latin_contents_types" , "event");
    $exception->fields("event" , array("entity_id"));
    $exception->condition("event.entity_type" , "node");
    $exception->condition("event.bundle" , "latin_contents");
    $exception->condition("event.field_latin_contents_types_tid" , array(4058 , 4599) , "IN");
    $exception = $exception->execute()->fetchCol();
    /*---------------------------------------------------------------------------------------------------*/

    /*this part is for filtering, searching on type, category, topic, tag*/
    if(!empty($filter)){
        /* this part is used for filtering
         * collect custom nids in $custom_contents
         * */
        $custom_contents = array();
        $is_filtered = false;
        if(isset($filter["search"])){
            $custom_contents = search_content_by_value($filter["search"]);
            $is_filtered = true;
        }
        if(isset($filter["type"])) {
            $tids = explode("," , $filter["type"]);
            $query = db_select("field_data_field_latin_contents_types" , "type");
            $query->fields("type" , array("entity_id"));
            $query->condition("type.field_latin_contents_types_tid" , $tids , "IN");
            $results = $query->execute()->fetchCol();
            $custom_contents = $is_filtered ? array_intersect($custom_contents , $results) : $results;
            $is_filtered = true;
        }
        if(isset($filter["category"])){
            $tids = explode("," , $filter["category"]);
            $query = db_select("field_data_field_latin_contents_categories" , "category");
            $query->fields("category" , array("entity_id"));
            $query->condition("category.field_latin_contents_categories_tid" , $tids , "IN");
            $results = $query->execute()->fetchCol();
            $custom_contents = $is_filtered ? array_intersect($custom_contents , $results) : $results;
            $is_filtered = true;
        }
        if(isset($filter["topic"])){
            $tids = explode("," , $filter["topic"]);
            $query = db_select("field_data_field_latin_contents_topics" , "topic");
            $query->fields("topic" , array("entity_id"));
            $query->condition("topic.field_latin_contents_topics_tid" , $tids , "IN");
            $results = $query->execute()->fetchCol();
            $custom_contents = $is_filtered ? array_intersect($custom_contents , $results) : $results;
            $is_filtered = true;
        }
        if(isset($filter["tag"])){
            $query = db_select("field_data_field_latin_tags" , "tags");
            $query->fields("tags" , array("entity_id"));
            $query->condition("tags.field_latin_tags_tid" , $filter["tag"]);
            $results = $query->execute()->fetchCol();
            $custom_contents = $is_filtered ? array_intersect($custom_contents , $results) : $results;
            $is_filtered = true;
        }
        if($is_filtered && empty($custom_contents)) return ["contents" => array() , "count" => 0 , "page" => $page];
    }
    /*---------------------------------------------------------------------------------------------------*/

    /*fetch content list data*/
    $query = db_select("node" , "node");
    $query->fields("node" , array("nid" , "title" , "uid" , "created" , "changed"));
    $query->condition("node.status" , 1);
    $query->condition("node.type" , "latin_contents");

    /*remove the exception nodes*/
    if(!empty($exception))$query->condition("node.nid" , $exception , "NOT IN");

    /*filter the response*/
    if(isset($custom_contents) && !empty($custom_contents)) $query->condition("node.nid" , $custom_contents, "IN");

    /*If this query is called form other places with custom nids, it is filtered appropriate based on custom node*/
    if(isset($parameter["custom_contents"]) && !empty($parameter["custom_contents"])) $query->condition("node.nid" , $parameter["custom_contents"] , "IN");

    /*choose stick or none stick if $parameter[stick] is set*/
    if(!empty($parameter["stick"]) && $parameter["stick"] == 1) $query->condition("node.sticky" , 1);
    elseif(!empty($parameter["stick"]) && $parameter["stick"] == -1) $query->condition("node.sticky" , 0);

    /*add body*/
    if(in_array("body" , $fields)){
        $query->leftJoin("field_data_body" , "body" , "node.nid = body.entity_id and body.entity_type = 'node' and body.bundle = 'latin_contents' ");
        $query->addField("body" , "body_value" , "body");
    }

    /*add description*/
    if(in_array("meta_description" , $fields)){
        $query->leftJoin("field_data_field_meta_description" , "meta_description" , "node.nid = meta_description.entity_id and meta_description.entity_type = 'node' and meta_description.bundle = 'latin_contents' ");
        $query->addField("meta_description" , "field_meta_description_value" , "meta_description");
    }

    /*add first picture*/
    if(in_array("picture" , $fields)){
        $query->leftJoin("field_data_field_image" , "field_image" , "node.nid = field_image.entity_id and field_image.entity_type = 'node' and field_image.bundle = 'latin_contents' and field_image.delta = '0' ");
        $query->leftJoin("file_managed" , "file" , "file.fid = field_image.field_image_fid");
        $query->addField("file" , "uri" , "picture");
    }

    /*add author full name*/
    if(in_array("author_name" , $fields)){
        $query->join("users" , "user" , "user.uid = node.uid");
        $query->leftJoin("field_data_field_full_name" , "name" , "user.uid = name.entity_id and name.entity_type = 'user' and name.bundle = 'user' ");
        $query->addField("name" , "field_full_name_value" , "author_name");
    }

    /*add author pic*/
    if(in_array("author_pic" , $fields)){
        $query->leftJoin("file_managed" , "user_pic" , "user_pic.fid = user.picture");
        $query->addField("user_pic" , "uri" , "author_pic");
    }

    /*add each content's type*/
    if(in_array("types" , $fields)){
        $query->leftJoin("field_data_field_latin_contents_types" , "types" , "node.nid = types.entity_id and types.delta = '0' ");
        $query->leftJoin("taxonomy_term_data", "type_data", "type_data.tid = types.field_latin_contents_types_tid");
        $query->addField("type_data" , "tid" , "types_tid");
        $query->addField("type_data" , "name" , "types_name");
    }

    /*add each content's category*/
    if(in_array("categories" , $fields)){
        $query->leftJoin("field_data_field_latin_contents_categories" , "categories" , "node.nid = categories.entity_id and categories.delta = '0' ");
        $query->leftJoin("taxonomy_term_data", "category_data", "category_data.tid = categories.field_latin_contents_categories_tid");
        $query->addField("category_data" , "tid" , "categories_tid");
        $query->addField("category_data" , "name" , "categories_name");
    }

    /*add each content's category*/
    if(in_array("topics" , $fields)){
        $query->leftJoin("field_data_field_latin_contents_topics" , "topics" , "node.nid = topics.entity_id and topics.delta = '0' ");
        $query->leftJoin("taxonomy_term_data", "topic_data", "topic_data.tid = topics.field_latin_contents_topics_tid");
        $query->addField("topic_data" , "tid" , "topics_tid");
        $query->addField("topic_data" , "name" , "topics_name");
    }

    /*add each content's clap*/
    if(in_array("clap" , $fields)){
        $clapping = db_select("clapping" , "clapping");
        $clapping->fields("clapping" , array("entity_id"));
        $clapping->groupBy("clapping.entity_id");
        $clapping->condition("clapping.entity_type" , "node");
        $clapping->condition("clapping.bundle" , "latin_contents");
        $clapping->addExpression("SUM(clapping.vote)" , "clap");

        $query->leftJoin($clapping , "clapping" , "node.nid = clapping.entity_id");
        $query->addField("clapping" , "clap" , "clap");
    }

    /*add each content's comment*/
    if(in_array("comment" , $fields)){
        $comment = db_select("comment" , "cm");
        $comment->fields("cm" , array("nid"));
        $comment->groupBy("cm.nid");
        $comment->addExpression("COUNT(cm.cid)" , "cm_counts");

        $query->leftJoin($comment , "node_comment" , "node.nid = node_comment.nid");
        $query->addField("node_comment" , "cm_counts" , "comment_count");
    }

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

        case "popular" :
            if(!isset($clapping)){
                $clapping = db_select("clapping" , "clapping");
                $clapping->fields("clapping" , array("entity_id"));
                $clapping->groupBy("clapping.entity_id");
                $clapping->condition("clapping.entity_type" , "node");
                $clapping->condition("clapping.bundle" , "latin_contents");
                $clapping->addExpression("SUM(clapping.vote)" , "clap");
                $query->leftJoin($clapping , "clapping" , "node.nid = clapping.entity_id");
            }
            $query->orderBy("clapping.clap" , "DESC")->orderBy("node.nid" , "DESC");
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
    /*---------------------------------------------------------------------------------------------------*/


    /*post process on contents*/
    foreach ($contents as $content){
        $content->created_timestamp = $content->created;
        $content->created = date("F j, Y" , $content->created);

        $content->changed_timestamp = $content->changed;
        $content->changed = date("F j, Y" , $content->changed);

        if(in_array("clap" , $fields) && empty($content->clap)) $content->clap = "0";

        if(in_array("comment" , $fields) && empty($content->comment_count)) $content->comment_count = "0";

        if(in_array("picture" , $fields))
            $content->picture = !empty($content->picture) ? image_style_url("300x200" , $content->picture) : null;

        if(in_array("author_name" , $fields))
            $content->author_pic = !empty($content->author_pic) ? image_style_url("60x60" , $content->author_pic) : null;

        if(!empty($content->types_tid) && !empty($content->types_name)){
            $content->types = array("tid" => $content->types_tid , "name" => $content->types_name);
            unset($content->types_tid);
            unset($content->types_name);
        }
        if(!empty($content->categories_tid) && !empty($content->categories_name)){
            $content->categories = array("tid" => $content->categories_tid , "name" => $content->categories_name);
            unset($content->categories_tid);
            unset($content->categories_name);
        }
        if(!empty($content->topics_tid) && !empty($content->topics_name)){
            $content->topics = array("tid" => $content->topics_tid , "name" => $content->topics_name);
            unset($content->topics_tid);
            unset($content->topics_name);
        }
    }
    /*---------------------------------------------------------------------------------------------------*/


    return ["contents" => $contents , "count" => (int) $count->countQuery()->execute()->fetchField() , "page" => $page];
}

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


function intern($parameter){
    $page = isset($parameter["page"]) && !empty($parameter["page"]) ? $parameter["page"] : 0;
    if($page > 2) $page = 2;
    $limits = 10;
    $sort_base = isset($parameter["sort"]) && !empty($parameter["sort"]) ? $parameter["sort"] : "last";

    /*fetch content list data*/
    $query = db_select("node" , "node");
    $query->fields("node" , array("nid" , "title" , "created"));
    $query->condition("node.status" , 1);
    $query->condition("node.type" , "latin_contents");

    /*add first picture*/
    $query->leftJoin("field_data_field_image" , "field_image" , "node.nid = field_image.entity_id and field_image.entity_type = 'node' and field_image.bundle = 'latin_contents' and field_image.delta = '0' ");
    $query->leftJoin("file_managed" , "file" , "file.fid = field_image.field_image_fid");
    $query->addField("file" , "uri" , "picture");

    /*sort the response*/
    switch ($sort_base){
        case "last" :
            $query->orderBy("node.nid" , "DESC");
            break;

        case "first" :
            $query->orderBy("node.nid" , "ASC");
            break;

        default :
            $query->orderBy("node.nid" , "DESC");
            break;
    }

    $query->range($page * $limits , $limits);

    $contents = $query->execute()->fetchAll();

    /*post process on contents*/
    foreach ($contents as $content){
        $content->picture = !empty($content->picture) ? image_style_url("300x200" , $content->picture) : null;
    }

    return ["contents" => $contents , "count" => 30 , "page" => $page];
}