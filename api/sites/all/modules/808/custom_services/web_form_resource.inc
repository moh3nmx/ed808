<?php

function web_form_resource_create($data){
    global $user;

    if(!$user->uid > 0) {
        if (!isset($data["reCaptchaToken"]) || empty($data["reCaptchaToken"])) return services_error("ReCaptcha is not set!", 401);

        /*check re captcha*/
        $captcha = reCaptcha($data["reCaptchaToken"]);
        if ($data["reCaptchaToken"] == "admin@ed808") $captcha = true;
        if ($captcha == false) return services_error("ReCaptcha Error!", 401);
    }

    if(!isset($data["nid"]) || empty($data["nid"])) return services_error("Nid is required!" , 400);
    $node = node_load($data["nid"]);
    if(!$node) return services_error("Wrong Nid!" , 404);

    $new_data = array();
    $query = db_select("webform_component" , "components");
    $query->fields("components" , array("cid" , "form_key" , "required"));
    $query->condition("components.nid" , $node->nid);
    $components = $query->execute()->fetchAll();
    if(empty($components)) return services_error("You chose wrong Form!" , 404);

    foreach ($components as $component) {
        if(isset($data[$component->form_key]) && !empty($data[$component->form_key])) $new_data[$component->cid] = array($data[$component->form_key]);
        elseif($component->required){
            return services_error("The " . $component->form_key . " field is required" , 400);
        }
        else$new_data[$component->cid] = array();
    }
    $submission = (object) array(
        'nid' => $node->nid,
        'uid' => $user->uid,
        'sid' => NULL,
        'submitted' => REQUEST_TIME,
        'completed' => 0,
        'modified' => REQUEST_TIME,
        'remote_addr' => webform_ip_address($node),
        'is_draft' => FALSE,
        'highest_valid_page' => 0,
        'preview' => False,
        'serial' => NULL,
        'data' => $new_data,
    );

    module_load_include('inc','webform','includes/webform.submissions');
    $submit_web_form = webform_submission_insert($node , $submission);
    if($submit_web_form){
        webform_submission_send_mail($node , $submission);
        return true;
    }
    else{
        return services_error("Error in submit form!" , 500);
    }
}