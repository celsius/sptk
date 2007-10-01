<?php
/* 
 * generate 'bitmap text' from text
 *
 * $Id: mailto.php,v 1.1 2007/10/01 03:09:14 s_sako Exp $
 */
header("Content-type: image/png");

$im = imagecreatetruecolor(250, 32);
$white = imagecolorallocate($im, 255, 255, 255);
$black = imagecolorallocate($im, 0, 0, 0);
imagefilledrectangle($im, 0, 0, 250, 32, $white);
$text = "dsp-cmnd@sp.nitech.ac.jp";
$font = 'Vera.ttf';
imageantialias($im,True);
imagecolortransparent($im, $white);
imagettftext($im, 11, 0, 10, 20, $black, $font, $text);
imagepng($im);
imagedestroy($im);
?> 
