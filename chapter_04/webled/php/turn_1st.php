<?php
	# 1st part - Global defines & functions
	define("value_f", "/sys/class/gpio/gpio66/value");

	# 2nd part - Get the current led status
	$led_status = intval(file_get_contents(value_f));

	# 3rd part - Render the led status by HTML code
?>
<html>
  <head>
    <title>Turning a led on/off using PHP</title>
  </head>

  <body>
    <h1>Turning a led on/off using PHP</h1>

    Current led status is: <? echo $led_status ? "on" : "off" ?>
  </body>
</html>
