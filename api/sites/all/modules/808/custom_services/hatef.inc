<?php


function upload_file_1(){

  global $user;
  $new_hash = hash_hmac("sha256" ,time() , "@ed899" , false);

  $fileName="uid-$user->uid-".substr($new_hash,0,10).'-'.$_FILES['image']['name'];
  $uploadfile = "public://".$fileName;


  $file = new stdClass();
  $file->uid      = $user->uid;
  $file->status   = 0;
  $file->filename = trim($fileName, '.');
  $file->uri      = $uploadfile;
  $file->filemime = file_get_mimetype($file->filename);
  $file->filesize = $_FILES['image']['size'];



  try{
     move_uploaded_file($_FILES['image']['tmp_name'], $uploadfile);
     $fileContent= file_save($file);
     $fileContent->fileUrl="api/sites/default/files/".$fileName;
    return $fileContent;
  }catch (Exception $e){
    return $e->getMessage();
  }
}


function delete_uploaded_file_1($data){

  $data= json_decode(file_get_contents("php://input"));



  foreach ($data->names as $name){

    $query = db_select("file_managed", "f");
    $query->fields("f", array("fid", "filename", "uid", "uri", "filemime", "filesize"));
    $query->condition("f.filename", $name);


    $res = $query->execute();
    $row = $res->fetchAssoc();

    $file = new stdClass();
    $file->uid = $row["uid"];
    $file->status = 0;
    $file->filename = trim($row["filename"], '.');
    $file->uri = $row["uri"];
    $file->filemime = file_get_mimetype($row["filename"]);
    $file->filesize = $row["filename"];
    $file->fid = $row["fid"];


    $deleted = db_delete('file_managed')
      ->condition('fid', $file->fid)
      ->execute();
    db_delete('file_usage')
      ->condition('fid', $file->fid)
      ->execute();
    if ($deleted) {

      $results[$name]= unlink($file->uri);

    }
  }
  return $results;

}

function add_tag_1(){

}
