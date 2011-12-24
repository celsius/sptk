<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"  
  "http://www.w3.org/TR/html4/loose.dtd">  
<html lang="en">
<head>
<title>Speech Singal Processing Toolkit (SPTK)</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"> 
<link rel="stylesheet" type="text/css" href="default.css"> 
<link rel="shortcut icon" href="image/favicon.ico">
</head>
<body>

<center><img class="image" src="image/logo.png"></center>

<div class=head>
 Speech Signal Processing Toolkit (SPTK)<br>
 Version 3.5<br>
 December 25, 2011
</div>
<h2>README</h2>
<hr>

<?php
$readme = "/home/groups/s/sp/sp-tk/SPTK/README";
if( file_exists( $readme)){
   $body = file( $readme);
   echo '<pre>';
   foreach( $body as $line)
      echo htmlentities( $line);
   echo '</pre>';

   echo '<hr><address>Last modified ';
   echo date( "M d, Y", filemtime( $readme));
   echo '</address>';
}
?>

</body>
</html>
