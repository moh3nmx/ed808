<?php

/*
 * flag_resource_user_follow: called from user/targeted_actions/follow
 * flag_resource_content_bookmark: called from contents/targeted_actions/bookmark
 * flag_resource_tag_bookmark: called from tag/targeted_actions/bookmark
 *
 * each function prepare the parameter for flag the related entity
 * */
/*-------------------------------------------------*/
function flag_resource_user_follow($uid , $data){
    if($data["action"] === "follow") $action = 1;
    elseif($data["action"] === "unfollow") $action = 0;
    else return services_error("Unknown Action!" , 406);
    $parameter = array(
        "entity_id" => $uid,
        "entity_type" => "user",
        "table" => "users",
        "fid" => 3,
        "action" => $action,
        "entity" => "user"
    );
    return flag_function($parameter);
}
function flag_resource_content_bookmark($nid , $data){
    if($data["action"] === "bookmark") $action = 1;
    elseif($data["action"] === "unbookmark") $action = 0;
    else return services_error("Unknown Action!" , 406);
    $parameter = array(
        "entity_id" => $nid,
        "entity_type" => "node",
        "table" => "node",
        "fid" => 1,
        "action" => $action,
        "entity" => "content"
    );
    return flag_function($parameter);
}
function flag_resource_tag_follow($tid , $data){
    if($data["action"] === "follow") $action = 1;
    elseif($data["action"] === "unfollow") $action = 0;
    else return services_error("Unknown Action!" , 406);
    $parameter = array(
        "entity_id" => $tid,
        "entity_type" => "taxonomy_term",
        "table" => "taxonomy_term_data",
        "fid" => 2,
        "action" => $action,
        "entity" => "tag"
    );
    return flag_function($parameter);
}
/*--------------------------------------------------*/

/*----------------main function---------------------*/
/*
 * flag_function: makes functionality for flagging based on entity and action
 * */
function flag_function($data){
    /*check if the object entity is exist or not*/
    $entity_exist = entity_exist($data["table"] , $data["entity_id"]);
    if(!$entity_exist) return services_error("Unknown ". $data["entity"] ."!" , 404);

    global $user;
    if(!$user->uid > 0) return services_error("You should login for this action!" , 403);

    /*
     * check if this action is valid or not
     * if entity was flagged before it cant be flagged again
     * if entity was not flagged before it cant be un_flagged
     * */
    $status = flag_status($data["fid"] , $data["entity_type"] , $data["entity_id"] , $user->uid , $data["action"]);
    if(!$status) return services_error("Wrong action!" , 406);

    /*
     * if 1 insert the flag
     * if 0 delete the flag
     * */
    if($data["action"] === 1){
        db_insert("flagging")->fields(array(
            "fid" => $data["fid"],
            "entity_type" => $data["entity_type"],
            "entity_id" => $data["entity_id"],
            "uid" => $user->uid,
            "sid" => 0,
            "timestamp" => REQUEST_TIME,
        ))->execute();
        field_cache_clear();
        return true;
    }
    elseif ($data["action"] === 0){
        db_delete("flagging")
            ->condition("fid" , $data["fid"])
            ->condition("entity_type" , $data["entity_type"])
            ->condition("entity_id" , $data["entity_id"])
            ->condition("uid" , $user->uid)
            ->execute();
        field_cache_clear();
        return true;
    }
    else return false;
}
/*-------------------------------------------------------------------------------------*/

/*---------------------helper functions------------------------------------------------*/
/*see @flag_function first*/
function entity_exist($table , $entity_id){
    $query = db_select($table , $table);
    $query->fields($table , array());
    if($table === "users") $query->condition("uid" , $entity_id);
    elseif($table === "node"){
        $query->condition("nid" , $entity_id);
        $query->condition("status" , 1);
    }
    elseif($table === "taxonomy_term_data") $query->condition("tid" , $entity_id);
    $exist = $query->execute()->fetch();
    if(!empty($exist)) return true;
    else return false;
}
function flag_status($fid , $entity_type , $entity_id , $uid ,  $action){
    $query = db_select("flagging" , "flagging");
    $query->fields("flagging" , array("flagging_id"));
    $query->condition("flagging.fid" , $fid);
    $query->condition("flagging.entity_type" , $entity_type);
    $query->condition("flagging.entity_id" , $entity_id);
    $query->condition("flagging.uid" , $uid);
    $flag = $query->execute()->fetch();

    if($action == 1 && empty($flag)) return $fid;
    elseif($action == 0 && !empty($flag)) return $fid;
    else return false;
}
/*-------------------------------------------------------------------------------------*/