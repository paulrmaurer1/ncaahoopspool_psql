<?php
	$dbopts = parse_url(getenv('DATABASE_URL'));
	$host=$dbopts["host"];
	$user=$dbopts["user"];
	$passwd=$dbopts["pass"];
	$dbname=ltrim($dbopts["path"],'/');
	$cxn=pg_connect("host=$host user=$user password=$passwd dbname=$dbname") or die ("Couldn't connect to database.");
?>