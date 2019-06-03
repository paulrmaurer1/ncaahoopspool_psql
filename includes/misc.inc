<?php	
    $host="localhost";
    $user="postgres";
    $passwd="Cubbies1182!";
    $dbname="ncaahoopspool_db";
    $cxn=pg_connect("host=$host user=$user password=$passwd dbname=$dbname") or die ("Couldn't connect to database.");
?>