<?php
        # Check the PHP's MySQL support
        if (!function_exists("mysql_connect"))
                die("MySQL support missing!");

        # Get connect with the MySQL server.
        # Don't forget to replace the following string "ROOTPASS" with
        # the database's root password you choose during the installation!!!
        $connect = mysql_connect("localhost", "root", "ROOTPASS")
                or die("Unable to Connect");
        mysql_select_db("information_schema")
                or die("Could not open the db");

        # Do a simple query
        $showtablequery = "SHOW TABLES FROM information_schema";
        $query_result = mysql_query($showtablequery);

        # Let's display the result!
        while($row = mysql_fetch_array($query_result))
                echo $row[0] . " ";
?>
