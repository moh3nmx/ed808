<?php
/*
 * clap a content
 * limit 10 point for each content per user
 * */
function vote_resource_clap_content($nid , $data){
    global $user;
    if(!$user->uid > 0) return services_error("You should login for this action!" , 403);

    $node = db_select("node" , "node")->fields("node" , array())->condition("nid" , $nid)->condition("status" , 1)->execute()->fetch();
    if(empty($node)) return services_error("Unknown content!" , 404);

    /*fetch the last point for this content and this user*/
    $vote = db_select("clapping" , "clapping")->fields("clapping" , array("vote"))
        ->condition("entity_type" , "node")
        ->condition("bundle" , $node->type)
        ->condition("entity_id" , $nid)
        ->condition("uid" , $user->uid)
        ->execute()->fetch();
    $vote = empty($vote) ? 0 : $vote->vote;

    if($vote >= 10) return true; //if the user vote to this content more than 10 it is not saved
    $vote++;

    return vote("node" , $node->type , $nid , $user->uid , $vote);
}
/*
 * like a comment or take it back
 * for taking back sender should send {$data['action'] = 'dislike'}
 * otherwise it is considered like action
 * */
function vote_resource_like_comment($cid , $data){
    global $user;
    if(!$user->uid > 0) return services_error("You should login for this action!" , 403);

    $comment = db_select("comment" , "comment")->fields("comment" , array())->condition("cid" , $cid)->condition("status" , 1)->execute()->fetch();
    if(empty($comment)) return services_error("Unknown comment!" , 404);

    $vote = db_select("clapping" , "clapping")->fields("clapping" , array("vote"))
        ->condition("entity_type" , "comment")
        ->condition("bundle" , "comment")
        ->condition("entity_id" , $cid)
        ->condition("uid" , $user->uid)
        ->execute()->fetch();

    if(isset($data["action"]) && $data["action"] === "dislike"){
        if(empty($vote) || (!empty($vote) && $vote->vote == 0)) return true;
        $vote = 0;
    }
    else{
        if (!empty($vote) && $vote->vote == 1) return true;
        $vote = 1;
    }

    return vote("comment" , "comment" , $cid , $user->uid , $vote);
}

/*Insert or Update 'clapping' table record with inputs*/
function vote($entity_type , $bundle , $entity_id , $uid , $vote){
    $up = db_merge("clapping")
        ->key(array(
            'entity_type' => $entity_type,
            'bundle' => $bundle,
            'entity_id' => $entity_id,
            'uid' => $uid
        ))->fields(array(
            'vote' => $vote,
            'date' => REQUEST_TIME
        ))
        ->execute();
    if($up) return true;
    else return false;
}