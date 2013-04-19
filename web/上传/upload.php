<?php
move_uploaded_file($_FILES["datafile"]["tmp_name"], "./upload/" . $_FILES["datafile"]["name"]);
echo $_POST["path"] ."/" .  $_FILES["datafile"]["name"]
?>

