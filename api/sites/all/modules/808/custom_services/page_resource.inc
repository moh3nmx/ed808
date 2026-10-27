<?php
/*-----------------retrieve part--------------------*/
function page_resource_retrieve($nid , $parameter){
    $query = db_select("node" , "node");
    $query->leftJoin("field_data_body" , "body" , "body.entity_id = node.nid and body.entity_type = 'node' and body.bundle = 'page' ");
    $query->leftJoin("field_data_field_image" , "field_image" , "field_image.entity_id = node.nid and field_image.entity_type = 'node' and field_image.delta = '0' ");
    $query->leftJoin("file_managed" , "image" , "image.fid = field_image.field_image_fid");
    $query->fields("node" , array("nid" , "uid" , "title" , "created" , "changed"));
    $query->fields("body" , array("body_value"));
    $query->addField("image" , "uri" , "image");
    $query->condition("node.nid" , $nid);
    $query->condition("node.type" , "page");
    $page = $query->execute()->fetch();

    $page->created = format_date($page->created);
    $page->changed = format_date($page->changed);
    if(!empty($page->image)) $page->image = generate_file_address($page->image);

    return $page;
}
/*-----------------------------------------------*/