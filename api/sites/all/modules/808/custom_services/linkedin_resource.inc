<?php

function linkedin_resource_login($data){
    /* If token is fetched correctly
     * sending request to fetch Email
     */
    $curl = curl_init();
    curl_setopt_array($curl, array(
//        CURLOPT_URL => "https://api.linkedin.com/v1/people/~:(id,public-profile-url,first-name,last-name,email-address,location,summary,num-connections,formatted-name,headline,industry,specialties,positions,picture-url,picture-urls::(original))?format=json",
        CURLOPT_URL => "https://api.linkedin.com/v1/people/~:(email-address)?format=json",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 30,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "GET",
        CURLOPT_HTTPHEADER => array(
            "authorization: Bearer " . $data["access_token"],
            "cache-control: no-cache",
            "connection: Keep-Alive"
        ),
    ));
    $data = curl_exec($curl);
    $err = curl_error($curl);
    curl_close($curl);
    if($err){
        return services_error("Error in connecting to linkedin!" , 500);
    }
    $email = json_decode($data, true);
    $email = $email["emailAddress"];

    /*if user has already logged in, just redirect */
    global $user;
    if($user->uid > 0){
        $login_data = new stdClass();
        $login_data->uid = $user->uid;
        $login_data->token = drupal_get_token('services');
    }

    /*get the uid with this email for making the user login*/
    $uid = db_select("users" , "user")->fields("user" , array("uid"))->condition("mail" , $email)->execute()->fetch();
    if(empty($uid)){
        return services_error("You do not have account with this Email, Please register first." , 404);
    }

    /*login user*/
    $user = user_load($uid->uid);
    user_login_finalize();

    db_merge("custom_linkedIn_token")
        ->key(array(
            "uid" => $user->uid
        ))
        ->fields(array(
            "token" => $data["access_token"],
            "location" => "login",
            "expire" => REQUEST_TIME + $data["expires_in"]
        ))->execute();

    $login_data = new stdClass();
    $login_data->uid = $user->uid;
    $login_data->token = drupal_get_token('services');

    return $login_data;
}

/**
 * It is called form LinkedIn Call Back
 * If everything is ok, It is backed with "code"
 * If there is an error, It is backed wih "error"
 */
function linked_in_register(){
    /*get user authentication code from linkedin*/
    $code =  isset($_GET["code"]) && !empty($_GET["code"]) ? $_GET["code"] : "";
    $error = isset($_GET["error"]) && !empty($_GET["error"]) ? $_GET["error"] : "";

    if(($error === "user_cancelled_login" || $error === "user_cancelled_authorize")){
        drupal_set_message(t('Login with Linkedin is not completed'), 'status');
        drupal_goto("/user/login");
    }
    if(empty($code)){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user/login");
    }

    $curl = curl_init();
    curl_setopt_array($curl, array(
        CURLOPT_URL => "https://www.linkedin.com/oauth/v2/accessToken",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 30,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "POST",
        CURLOPT_POSTFIELDS => "grant_type=authorization_code&code=" . $code . "&redirect_uri=https://civil808.com/linkedin/register/finalize&client_id=77xpw5r1ppmuby&client_secret=7iDuWEHv7yTOCgqx",
        CURLOPT_HTTPHEADER => array(
            "content-type: application/x-www-form-urlencoded"
        ),
    ));
    $response = curl_exec($curl);
    $err = curl_error($curl);
    curl_close($curl);
    if($err){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user/login");
    }

    $response = json_decode($response, true);
    if(isset($response["error"]) && !empty($response["error"])){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user/login");
    }

    $curl = curl_init();
    curl_setopt_array($curl, array(
//        CURLOPT_URL => "https://api.linkedin.com/v1/people/~:(id,public-profile-url,first-name,last-name,email-address,location,summary,num-connections,formatted-name,headline,industry,specialties,positions,picture-url,picture-urls::(original))?format=json",
        CURLOPT_URL => "https://api.linkedin.com/v1/people/~:(first-name,last-name,formatted-name,email-address)?format=json",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 30,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "GET",
        CURLOPT_HTTPHEADER => array(
            "authorization: Bearer " . $response["access_token"],
            "cache-control: no-cache",
            "connection: Keep-Alive"
        ),
    ));
    $data = curl_exec($curl);
    $err = curl_error($curl);
    curl_close($curl);
    if($err){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user/login");
    }
    $profile = json_decode($data, true);
    $full_name = $profile["formatted-name"];
    $email = $profile["emailAddress"];
    $username = explode("@" , $email);
    $username = $username[0];

    global $user;
    if($user->uid > 0) {
        drupal_goto("/user/" . $user->uid);
        die();
    }

    /*get the uid with this email to check if the user had account before or not*/
    $uid = db_select("users" , "user")->fields("user" , array("uid"))->condition("mail" , $email)->execute()->fetch();
    /*if the email has existed just login the user*/
    if(!empty($uid)){
        $user = user_load($uid->uid);
        user_login_finalize();
        db_merge("custom_linkedIn_token")
            ->key(array(
                "uid" => $user->uid
            ))
            ->fields(array(
                "token" => $response["access_token"],
                "location" => "login",
                "expire" => REQUEST_TIME + $response["expires_in"]
            ))->execute();

        drupal_goto("/user/" . $user->uid);
        die();
    }
    /* else we generate username by email
     * we use do while for preventing to generate repetitive username
     * */
    do {
        $check_username = db_select("users", "user")->fields("user", array("uid"))->condition("name", $username)->execute()->fetch();
        if(!empty($check_username)) $username .= '1';
    }while(!empty($check_username));

    $pass = user_password();
    $new_user = array(
        'name' => $username,
        'password' => $pass,
        'mail' => $email,
        'status' => 1,
        'init' => $email,
        'roles' => array(
            DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        ),
    );
    $user = user_save('', $new_user);
    $profile = profile2_create(array('type' => 'main', 'uid' => $user->uid));
    $profile->field_full_name['und'][0]['value'] = $full_name;
    profile2_save($profile);
    /*login user*/
    user_login_finalize();

    db_merge("custom_linkedIn_token")
        ->key(array(
            "uid" => $user->uid
        ))
        ->fields(array(
            "token" => $response["access_token"],
            "location" => "login",
            "expire" => REQUEST_TIME + $response["expires_in"]
        ))->execute();

    drupal_set_message(t('Note: choose a secret Password or You can not login again!'), 'warning');
    drupal_goto("user/" . $user->uid . "/edit#edit-account");
    die();
}

/*connect the profile to linkedin*/
function linked_in_connect(){
    /*get user authentication code from linkedin*/
    $code =  isset($_GET["code"]) && !empty($_GET["code"]) ? $_GET["code"] : "";
    $error = isset($_GET["error"]) && !empty($_GET["error"]) ? $_GET["error"] : "";
    if(($error === "user_cancelled_login" || $error === "user_cancelled_authorize")){
        drupal_set_message(t('Connect to Linkedin is not completed'), 'status');
        drupal_goto("/user");
        die();
    }
    if(empty($code)){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user");
        die();
    }

    /*sending a request to get access token*/
    $curl = curl_init();
    curl_setopt_array($curl, array(
        CURLOPT_URL => "https://www.linkedin.com/oauth/v2/accessToken",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 30,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "POST",
        CURLOPT_POSTFIELDS => "grant_type=authorization_code&code=" . $code . "&redirect_uri=https://civil808.com/linkedin/connect/finalize&client_id=77xpw5r1ppmuby&client_secret=7iDuWEHv7yTOCgqx",
        CURLOPT_HTTPHEADER => array(
            "content-type: application/x-www-form-urlencoded"
        ),
    ));
    $response = curl_exec($curl);
    $err = curl_error($curl);
    curl_close($curl);
    if($err){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user");
        die();
    }
    $response = json_decode($response, true);
    if(isset($response["error"]) && !empty($response["error"])){
        drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
        drupal_goto("/user");
        die();
    }

    /*update or insert token for user*/
    global $user;
    db_merge("custom_linkedIn_token")
        ->key(array(
            "uid" => $user->uid
        ))
        ->fields(array(
            "token" => $response["access_token"],
            "location" => "login",
            "expire" => REQUEST_TIME + $response["expires_in"]
        ))->execute();

    drupal_goto("/user/" . $user->uid . "/linkedin");
    die();
}
/* access function for connecting to linkedin
 * if user who is logged in is not the use-page return false
 * else return true
 */
function connect_access($uid){
    global $user;
    if($user->uid == $uid)
        return true;
    else
        return false;
}

/* if user has new Version of Linked in,
 * and is not expired it return tpl page on this module
 * else it returns previous_data() function
 * */
function linked_in_profile($uid){
    $token = db_select("custom_linkedIn_token" , "token")
        ->fields("token" , array("token" , "expire"))
        ->condition("uid" , $uid)
        ->condition("expire" , REQUEST_TIME , ">")
        ->execute()->fetch();

    if(!empty($token)){
        global $user;
        if($user->uid == $uid &&
            $token->expire - REQUEST_TIME <= 86400
        ){
            return linkedin_get_token_request("connect");
        }

        $curl = curl_init();
        curl_setopt_array($curl, array(
            CURLOPT_URL => "https://api.linkedin.com/v1/people/~:(id,public-profile-url,first-name,last-name,email-address,location,summary,num-connections,formatted-name,headline,industry,specialties,positions,picture-url,picture-urls::(original))?format=json",
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_ENCODING => "",
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 30,
            CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
            CURLOPT_CUSTOMREQUEST => "GET",
            CURLOPT_HTTPHEADER => array(
                "authorization: Bearer " . $token->token,
                "cache-control: no-cache",
                "connection: Keep-Alive"
            ),
        ));
        $data = curl_exec($curl);
        $err = curl_error($curl);
        curl_close($curl);
        if ($err) {
            drupal_set_message(t('There was a problem with the configuration of LinkedIn on this website. Please try again later.'), 'error');
            drupal_goto("/user/login");
        }
        $data = json_decode($data, true);

        $result = array();
        $result["full_name"] = $data["formattedName"];
        $result["headline"] = $data["headline"];
        $result["location"] = $data["location"]["name"];
        $result["industry"] = $data["industry"];
        $result["summary"] = $data["summary"];
        $result["connections"] = $data["numConnections"];
        $result["profile"] = $data["publicProfileUrl"];
        $result["picture"] = !empty($data["pictureUrls"]["values"]) ? $data["pictureUrls"]["values"][0] : null;
        $result["positions"] = $data["positions"]["values"];

        return theme('custom_linkedIn_user_page', array('profile' => $result));
    }
    else{
        return previous_data($uid);
    }
}

/*check if user has profile LinkedIn or not*/
function has_linkedin_account($uid){
    /*check with new version*/
    $token = db_select("custom_linkedIn_token" , "token")
        ->fields("token" , array("token"))
        ->condition("uid" , $uid)
        ->condition("expire" , REQUEST_TIME , ">")
        ->execute()->fetch();

    if(!empty($token)) return true;

    /*check for previous version*/
    $user = user_load($uid);
    if (isset($user->linkedin) && !empty($user->linkedin)) return true;

    /*if user does not have any linkedin connection*/
    return false;
}

/* if user who is logged in is not same with user-page return false
 * else if user does not have token, return 0
 * else return expire time
 * it is used in tpl files
 * */
function linkedin_token_expire(){
    global $user;
    if(arg(0) == 'user' && arg(1) == $user->uid){
        $token = db_select("custom_linkedIn_token" , "token")
            ->fields("token" , array("expire"))
            ->condition("uid" , $user->uid)
            ->execute()->fetch();
        if(empty($token)) return 0;
        return format_date($token->expire ,'short');
    }
    return false;
}