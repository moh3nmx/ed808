<?php

/*--------------------------operations part-----------------------------*/
function comment_resource_create($data){
    global $user;
    if($user->uid == 0) return services_error("You should log in first!" , 403);
    if(!isset($data["nid"]) || empty($data["nid"])) return false;

    $node = db_select("node" , "node")->fields("node" , array());
    if(!in_array("administrator" , $user->roles)) $node->condition("node.status" , 1);
    $node->condition("node.nid" , $data["nid"]);
    $node = $node->execute()->fetch();
    if(empty($node)) return false;

    $comment = new stdClass();
    $comment->nid = $node->nid;
    $comment->pid = (isset($data['pid']) && !empty($data['pid'])) ? $data['pid'] : 0;
    $comment->uid = $user->uid;
    $comment->name = $user->name;
    $comment->mail = $user->mail;
    $comment->node_type = "comment_node_" . $node->type;
    $comment->is_anonymous = 0;
    $comment->status = COMMENT_PUBLISHED;
    $comment->language = 'und';
    if(isset($data['field_files']) && !empty($data['field_files'])) $comment->field_files = $data['field_files'];
    $comment->comment_body[$comment->language][0]['value'] = $data['body'];
    $comment->subject = "subject";
    comment_submit($comment);
    comment_save($comment);

    return $comment;
}

function comment_resource_update($cid , $data){
    $comment = comment_load($cid);
    if(!$comment) return false;
    global $user;
    if($user->uid != $comment->uid && !in_array("administrator" , $user->roles)) return services_error("You do not have permission to this action!" , 403);

    $update = false;
    if(isset($data["body"]) && !empty($data["body"])){
        $comment->comment_body[$comment->language][0]['value'] = $data['body'];
        $comment->comment_body[$comment->language][0]['safe_value'] = $data['body'];
        $update = true;
    }
    if(isset($data['field_files']) && !empty($data['field_files'])) {
        $comment->field_files = $data['field_files'];
        $update = true;
    }
    if($update){
        comment_save($comment);
    }
    return $comment;
}

function comment_resource_delete($cid , $data){
    global $user;
    $comment = db_select("comment" , "comment")->fields("comment" , array())->condition("cid" , $cid)->execute()->fetch();
    if(empty($comment)) return false;
    if($user->uid != $comment->uid && !in_array("administrator" , $user->roles)) return services_error("You do not have permission to this action!" , 403);

    comment_delete($cid);
    return true;
}
/*----------------------------------------------------------------------*/